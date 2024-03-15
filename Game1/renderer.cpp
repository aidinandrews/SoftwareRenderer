#include "renderer.h"

Renderer::Renderer()
	: p_screen(nullptr), p_camera(nullptr), p_texture(nullptr), p_triangle(nullptr),
	m_color(0xFFFFFFFF), m_defaultTexture(new Uint32BMP("bitmaps\\wave.bmp")) {
}

Renderer::~Renderer() {
	delete m_defaultTexture;
}

void Renderer::init(Screen* screen, Camera* camera) {
	p_screen = screen;
	p_camera = camera;
}

point2 Renderer::project(const point3& mapPosition) {
	point2 screenPosition;
	screenPosition = mapPosition;
	//translate
	screenPosition += p_camera->m_delta;
	//set (0, 0) to center of screen
	point2 screenCenter(screenWidth * 0.5f, screenHeight * 0.5f);
	screenPosition -= screenCenter;
	//rotate
	float newX = (screenPosition.x() * cos(p_camera->m_yaw) - screenPosition.y() * sin(p_camera->m_yaw));
	float newY = (screenPosition.y() * cos(p_camera->m_yaw) + screenPosition.x() * sin(p_camera->m_yaw));
	screenPosition = point2(newX, newY);
	//pitch
	screenPosition.y() *= sin(p_camera->m_pitch);
	//zoom
	screenPosition *= p_camera->m_zoom;
	//uncenter screen coordinates
	screenPosition += screenCenter;
	//adjust for z value
	float z = mapPosition.e[2];
	screenPosition.y() -= sin(p_camera->m_pitch - float(M_PI) / 2) * z * p_camera->m_zoom;
	return screenPosition;
}

void Renderer::LineSegment::offsetData() {
	current = current + offset;
	steps--;
}
void Renderer::LineSegment::createSide(TrianglePointData A, TrianglePointData B) {
	if (A.screen.y() > B.screen.y()) { swap(A, B); }
	current = A;
	steps = int(B.screen.y() - A.screen.y());

	offset = (B - A) / float(steps);
	offset.screen.y() = 1;
}
void Renderer::LineSegment::createScanline(TrianglePointData leftPoint, TrianglePointData rightPoint) {
	steps = int(rightPoint.screen.x()) - int(leftPoint.screen.x());
	//std::cout << int(rightPoint.data[screenX]) - int(leftPoint.data[screenX]) << std::endl;
	current = leftPoint;
	current.screen.x() = floor(current.screen.x());

	offset = (rightPoint - leftPoint) / float(steps);
	offset.screen = point2(1, 0);
}

void Renderer::renderTriangle(Triangle* triangle) {
	p_triangle = triangle;
	p_texture = triangle->p_texture;
	triangle->_color = m_color;
	// Discard triangles facing away from the camera.
	awayFacing = false;
	// This check should be for 0, but it culls triangles it shouldnt due to floating point errors probably if set to 0.
	vec3 avgNormal = unitVector((triangle->_normal[0] + triangle->_normal[1] + triangle->_normal[2]) / 3);
	if (dot(avgNormal, p_camera->m_normal) < 0) {
		awayFacing = true;
		if (triangle->_backFaceCulling) {
			return;
		}
	}
	for (int i = 0; i < 3; i++) {
		m_points[i].map = triangle->_mapPoints[i];
		m_points[i].screen = project(m_points[i].map);
		m_points[i].texture = triangle->_texturePos[i];
		m_points[i].normal = triangle->_normal[i];
	}
	// Create a bounding box.
	float boundingBox[4];
	int left = 0, right = 1, top = 2, bottom = 3;

	boundingBox[left] = m_points[0].screen.x();
	boundingBox[right] = m_points[1].screen.x();
	if (boundingBox[left] > boundingBox[right]) {
		swap(boundingBox[left], boundingBox[right]);
	}
	if (boundingBox[left] > m_points[2].screen.x()) {
		boundingBox[left] = m_points[2].screen.x();
	}
	if (boundingBox[right] < m_points[2].screen.x()) {
		boundingBox[right] = m_points[2].screen.x();
	}
	if (m_points[0].screen.y() > m_points[1].screen.y()) {
		swap(m_points[0], m_points[1]);
	}
	if (m_points[0].screen.y() > m_points[2].screen.y()) {
		swap(m_points[0], m_points[2]);
	}
	if (m_points[1].screen.y() > m_points[2].screen.y()) {
		swap(m_points[1], m_points[2]);
	}
	// Discard flat triangles
	if (m_points[0].screen.y() == m_points[2].screen.y()) {
		return;
	}
	boundingBox[top] = m_points[0].screen.y();
	boundingBox[bottom] = m_points[2].screen.y();

	// Don't draw the triangle if all the points are outside the camera's view.
	if (boundingBox[left] > screenWidth ||
		boundingBox[right] < 0 ||
		boundingBox[top] > screenHeight ||
		boundingBox[bottom] < 0) {
		return;
	}
	// Calculate the rest of the data for each point.
	for (int i = 0; i < 3; i++) {
		m_points[i].camDist = pointToPlaneDist(m_points[i].map, p_camera->m_plane);
		// Snap vertex positions to exact pixels
		m_points[i].screen.x() = floor(m_points[i].screen.x());
		m_points[i].screen.y() = floor(m_points[i].screen.y());
	}
	// Check if the triangle is completely inside the screen.
	if (boundingBox[left] > 0 &&
		boundingBox[right] < screenWidth &&
		boundingBox[top] > 0 &&
		boundingBox[bottom] < screenHeight) {
		// Wire frame
		if (state[SDL_SCANCODE_P]) {
			point2 A = m_points[0].screen;
			point2 B = m_points[1].screen;
			point2 C = m_points[2].screen;
			drawLine(*p_screen, A, B, 0xFFFFFFFF);
			drawLine(*p_screen, A, C, 0xFFFFFFFF);
			drawLine(*p_screen, B, C, 0xFFFFFFFF);
			return;
		}
		drawTriangle(m_points[0], m_points[1], m_points[2]);
		return;
	}
	else {
		suthHodgeCrop();
	}
}

void Renderer::renderTriangle(TrianglePointData& A, TrianglePointData& B, TrianglePointData& C, bool backFaceCulling) {
	p_texture = m_defaultTexture;
	m_color = 0xFFFFFFFF;
	// Discard triangles facing away from the camera.
	// This check should be for 0, but it culls triangles it shouldnt due to floating point errors probably if set to 0.
	if (backFaceCulling) {
		vec3 avgNormal = unitVector((A.normal + B.normal + C.normal) / 3);
		if (dot(avgNormal, p_camera->m_normal) < 0) {
			return;
		}	
	}
	m_points[0] = A;
	m_points[1] = B;
	m_points[2] = C;

	// Create a bounding box.
	float boundingBox[4];
	int left = 0, right = 1, top = 2, bottom = 3;

	boundingBox[left] = m_points[0].screen.x();
	boundingBox[right] = m_points[1].screen.x();
	if (boundingBox[left] > boundingBox[right]) {
		swap(boundingBox[left], boundingBox[right]);
	}
	if (boundingBox[left] > m_points[2].screen.x()) {
		boundingBox[left] = m_points[2].screen.x();
	}
	if (boundingBox[right] < m_points[2].screen.x()) {
		boundingBox[right] = m_points[2].screen.x();
	}
	if (m_points[0].screen.y() > m_points[1].screen.y()) {
		swap(m_points[0], m_points[1]);
	}
	if (m_points[0].screen.y() > m_points[2].screen.y()) {
		swap(m_points[0], m_points[2]);
	}
	if (m_points[1].screen.y() > m_points[2].screen.y()) {
		swap(m_points[1], m_points[2]);
	}
	// Discard flat triangles
	if (m_points[0].screen.y() == m_points[2].screen.y()) {
		return;
	}
	boundingBox[top] = m_points[0].screen.y();
	boundingBox[bottom] = m_points[2].screen.y();

	// Don't draw the triangle if all the points are outside the camera's view.
	if (boundingBox[left] > screenWidth ||
		boundingBox[right] < 0 ||
		boundingBox[top] > screenHeight ||
		boundingBox[bottom] < 0) {
		return;
	}
	for (int i = 0; i < 3; i++) {
		m_points[i].screen.x() = floor(m_points[i].screen.x());
		m_points[i].screen.y() = floor(m_points[i].screen.y());
	}
	// Check if the triangle is completely inside the screen.
	if (boundingBox[left] > 0 &&
		boundingBox[right] < screenWidth &&
		boundingBox[top] > 0 &&
		boundingBox[bottom] < screenHeight) {
		// Wire frame
		if (state[SDL_SCANCODE_P]) {
			point2 A = m_points[0].screen;
			point2 B = m_points[1].screen;
			point2 C = m_points[2].screen;
			drawLine(*p_screen, A, B, 0xFFFFFFFF);
			drawLine(*p_screen, A, C, 0xFFFFFFFF);
			drawLine(*p_screen, B, C, 0xFFFFFFFF);
			return;
		}
		drawTriangle(m_points[0], m_points[1], m_points[2]);
		return;
	}
	else {
		suthHodgeCrop();
	}
}

void Renderer::drawTriangle(TrianglePointData A, TrianglePointData B, TrianglePointData C) {
	if (A.screen.y() > B.screen.y()) {
		swap(A, B);
	}
	if (A.screen.y() > C.screen.y()) {
		swap(A, C);
	}
	if (B.screen.y() > C.screen.y()) {
		swap(B, C);
	}
	// Group the points into lines.
	LineSegment AB, AC, BC;
	AC.createSide(A, C);
	LineSegment* leftSide;
	LineSegment* rightSide;
	int ySteps;
	// Draw the flat bottom half triangle.
	if (A.screen.y() != B.screen.y()) {
		AB.createSide(A, B);
		ySteps = AB.steps;
		// Check that the left side is actually on the left.
		if (AC.offset.screen.x() < AB.offset.screen.x()) {
			leftSide = &AC;
			rightSide = &AB;
		}
		else {
			leftSide = &AB;
			rightSide = &AC;
		}
		drawHalfTriangle(leftSide, rightSide, ySteps);
	}
	// Draw the flat top half triangle.
	if (B.screen.y() != C.screen.y()) {
		BC.createSide(B, C);
		ySteps = BC.steps;

		if (AC.offset.screen.x() > BC.offset.screen.x()) {
			leftSide = &AC;
			rightSide = &BC;
		}
		else {
			leftSide = &BC;
			rightSide = &AC;
		}
		drawHalfTriangle(leftSide, rightSide, ySteps);
	}
}
void Renderer::drawHalfTriangle(LineSegment* leftSide, LineSegment* rightSide, int steps) {
	LineSegment scanline;

	vec3 lightDirection(0, 0, 1);

	while (steps > 0) {
		scanline.createScanline(leftSide->current, rightSide->current);
		int x = int(scanline.current.screen.x());
		int y = int(scanline.current.screen.y());
		int yPos = screenWidth * y;

		while (scanline.steps > 0) {
			x = int(scanline.current.screen.x());
			int tx = int(scanline.current.texture.x());
			int ty = int(scanline.current.texture.y());

			//check if the pixel is visible
			if (scanline.current.camDist < p_screen->getZBuffer(x, y)) {

				float colorDimAmount = float(sin(dot(lightDirection, scanline.current.normal) * (M_PI / 2)));
				colorDimAmount = (colorDimAmount + 0.25f) * (4.0f / 5.0f);
				if (colorDimAmount < 0.25f) { colorDimAmount = 0.25f; }

				m_color = p_texture->getPixel(tx, ty);
				m_color = darkenColor(m_color, colorDimAmount);
				p_screen->m_pixelBuffer[x + yPos] = m_color;
				p_screen->m_zBuffer[x + yPos] = scanline.current.camDist;
			}
			scanline.offsetData();
		}
		leftSide->offsetData();
		rightSide->offsetData();
		steps--;
	}
}

void Renderer::renderLine(point2 A, point2 B, int width) {

}
void Renderer::renderLine(point3 A, point3 B, int width) {

}

void Renderer::LineSegment::offsetShadowData() {
	current = addShadows(current, offset);
	steps--;
}
void Renderer::LineSegment::createShadowSide(TrianglePointData A, TrianglePointData B) {
	if (A.shadow.y() > B.shadow.y()) { swap(A, B); }
	current = A;
	steps = int(B.shadow.y() - A.shadow.y());

	offset = divideShadows(subtractShadows(B, A), float(steps));
	offset.shadow.y() = 1;
}
void Renderer::LineSegment::createShadowScanline(TrianglePointData leftPoint, TrianglePointData rightPoint) {
	steps = int(rightPoint.shadow.x()) - int(leftPoint.shadow.x());
	current = leftPoint;
	current.shadow.x() = floor(current.shadow.x());

	offset = divideShadows(subtractShadows(rightPoint, leftPoint), float(steps));
	offset.shadow = point2(1, 0);
}

void Renderer::renderShadowMap(TrianglePointData A, TrianglePointData B, TrianglePointData C) {

}
void Renderer::drawShadowTriangle(TrianglePointData A, TrianglePointData B, TrianglePointData C) {

}
void Renderer::drawHalfShadowTriangle(LineSegment* leftSide, LineSegment* rightSide, int steps) {

}

void Renderer::suthHodgeCrop() {

	int currentPointCount = 3;
	int storedPointCount = 0;
	TrianglePointData currentPoints[7] = { m_points[0], m_points[1], m_points[2] };
	TrianglePointData storedPoints[7];
	int XorY[] = { 1, 0, 1, 0, 1 };
	float borderValues[] = { 0, screenWidth, screenHeight, 0 };
	int flip[] = { -1, 1, 1, -1 };

	TrianglePointData* pt1, * pt2, intersection;
	float pt1Value, pt2Value;
	bool pt1Visible, pt2Visible, lineSegmentIntersection;
	for (int i = 0; i < 4; i++) { // Checks top -> left -> bottom -> right.
		for (int pointIndex = 0; pointIndex < currentPointCount; pointIndex++) {
			pt1 = &currentPoints[pointIndex];
			pt2 = &currentPoints[(pointIndex + 1) % currentPointCount];
			pt1Value = (pt1->screen[XorY[i]]);
			pt2Value = (pt2->screen[XorY[i]]);

			pt1Visible = pt1Value * flip[i] < borderValues[i];
			pt2Visible = pt2Value * flip[i] < borderValues[i];

			if (pt1Visible) {
				storedPoints[storedPointCount] = currentPoints[pointIndex];
				storedPointCount++;
			}
			lineSegmentIntersection = pt1Visible != pt2Visible;
			if (lineSegmentIntersection) {
				// Interpolate the data that lies on the intersection of the line segment and the screen border.
				// y = (y2 – y1) / (x2 – x1) * (x – x1) + y1
				intersection = (*pt2 - *pt1) / (pt2Value - pt1Value) * (borderValues[i] - pt1Value) + *pt1;
				intersection.screen[XorY[i]] = borderValues[i];
				intersection.screen[XorY[i + 1]] = floor(intersection.screen[XorY[i + 1]]);
				storedPoints[storedPointCount] = intersection;
				storedPointCount++;
			}
		}
		// Swap the stored and current values to prepare to compare the new points to the next border side.
		currentPointCount = storedPointCount;
		storedPointCount = 0;
		for (int j = 0; j < currentPointCount; j++) {
			currentPoints[j] = storedPoints[j];
		}
	}
	for (int i = 0; i < currentPointCount - 2; i++) {
		//wire frame
		if (state[SDL_SCANCODE_P]) {
			point2 A = currentPoints[0].screen;
			point2 B = currentPoints[i + 1].screen;
			point2 C = currentPoints[i + 2].screen;
			drawLine(*p_screen, A, B, 0xFFFFFFFF);
			drawLine(*p_screen, A, C, 0xFFFFFFFF);
			drawLine(*p_screen, B, C, 0xFFFFFFFF);
			continue;
		}
		drawTriangle(currentPoints[0], currentPoints[i + 1], currentPoints[i + 2]);
	}
}

void blur(Screen& screen, int x, int y) {
	Uint32 color = 0;
	int red = 0, green = 0, blue = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			color = screen.getPixel(x + j, y + i);
			red += getRed(color);
			green += getGreen(color);
			blue += getBlue(color);

		}
	}
	red /= 9;
	green /= 9;
	blue /= 9;
	color = makeColor(red, green, blue, 255);
	screen.setPixel(x, y, color);
}

//void Renderer::antiAlias() {
//	Uint32 color = 0;
//	int red = 0;
//	int green = 0;
//	int blue = 0;
//	float numColors = 0;
//	for (int y = 0; y < screenHeight - 1; y++) {
//		for (int x = 0; x < screenWidth - 1; x++) {
//
//			int pos = x + y * screenWidth;
//
//			float minGap = 15;
//			bool horizontal = abs(p_screen->m_zBuffer[pos] - p_screen->m_zBuffer[pos + 1]) > minGap;
//			bool vertical = abs(p_screen->m_zBuffer[pos] - p_screen->m_zBuffer[pos + screenWidth]) > minGap;
//
//			if (horizontal) {
//				p_screen->m_pixelBuffer[pos] |= (1UL << 7);
//				//p_screen->setPixel(x, y, 0xFFFFFFFF);
//			}
//			else {
//				p_screen->m_pixelBuffer[pos] &= ~(1UL << 7);
//			}
//
//			if (vertical) {
//				p_screen->m_pixelBuffer[pos] |= (1UL << 6);
//				//p_screen->setPixel(x, y, 0xFF0000FF);
//			}
//			else {
//				p_screen->m_pixelBuffer[pos] &= ~(1UL << 6);
//			}
//			/*if (horizontal && vertical) {
//				p_screen->setPixel(x, y, 0xFF00FFFF);
//			}*/
//		}
//	}
//#define vert(x, y) (bool(p_screen->getPixel(x, y) & (1UL << 6)))
//#define hori(x, y) (bool(p_screen->getPixel(x, y) & (1UL << 7)))
//
//	// Key:	  1 |___| 3	   5: __|	6: |__   7:  ___   4:         
//	//		  2 |   | 5	     |			  |	    |   |     |___|    
//
//	for (int y = 1; y < screenHeight - 1; y++) {
//		for (int x = 1; x < screenWidth - 1; x++) {
//
//			int pos = x + y * screenWidth;
//			if (vert(x, y)) {
//				int steps = 1;
//				int shapeType = 0;
//
//				// Left side of shape.  See Key.
//				if (hori(x - 1, y + 1)) {
//					shapeType += 2;
//				}
//				if (hori(x - 1, y)) {
//					shapeType += 1;
//				}
//
//				// Find the length of the horizontal line.
//				while (vert(x + steps, y) == true) {
//					//p_screen->setPixel(x + steps, y, 0x00FFFFFF);
//					steps++;
//				} steps--;
//
//				// Right side of shape.  See Key.
//				if (hori(x + steps, y)) {
//					shapeType += 3;
//				}
//				if (hori(x + steps, y + 1)) {
//					shapeType += 5;
//				}
//
//				int leftSteps = steps / 2;
//				int rightSteps = steps - leftSteps;
//				float leftBlendAmount = 0, rightBlendAmount = 0;
//				float leftBlendAdj = 0, rightBlendAdj = 0;
//				int leftSideAdj = 0, rightSideAdj = 0;
//				int up = 0, down = 1;
//				Uint32 color = 0, leftColor = 0, rightColor = 0;
//				switch (shapeType) {
//				case 4:
//					break;
//					leftSideAdj = down;
//					rightSideAdj = down;
//
//					leftBlendAdj = 0;
//					leftBlendAmount = 0;
//					rightBlendAdj = 0;
//					rightBlendAmount = 0;
//
//					color = 0xFF00FFFF;
//					break;
//				case 7:
//					break;
//					leftSideAdj = up;
//					rightSideAdj = up;
//
//					leftBlendAdj = 0;
//					leftBlendAmount = 0;
//					rightBlendAdj = 0;
//					rightBlendAmount = 0;
//					color = 0x00FF00FF;
//					break;
//				case 5:
//					leftSideAdj = down;
//					rightSideAdj = up;
//
//					leftBlendAdj = -1 / float((leftSteps) * 2);
//					leftBlendAmount = 0.5f;
//					rightBlendAdj = -1 / float((rightSteps + 1) * 2);
//					rightBlendAmount = 1 + (rightBlendAdj);
//
//					color = 0xFF000FF;
//					break;
//				case 6:
//					break;
//					leftSideAdj = up;
//					rightSideAdj = down;
//
//					leftBlendAdj = 0;
//					leftBlendAmount = 0;
//					rightBlendAdj = 0;
//					rightBlendAmount = 0;
//
//					color = 0xFFFFFFFF;
//					break;
//				default:
//					p_screen->m_pixelBuffer2[pos] = p_screen->m_pixelBuffer[pos];
//					continue;
//				}
//
//				for (int i = 0; i < leftSteps; i++) {
//					Uint32 pixel = p_screen->getPixel(x + i, y);
//					Uint32 blendWith = p_screen->getPixel(x + i, y + 1);
//					Uint32 blended = blendColors(pixel, blendWith, leftBlendAmount);
//					leftBlendAmount += leftBlendAdj;
//					p_screen->m_pixelBuffer2[pos + i + (screenWidth * leftSideAdj)] = blended;
//					//p_screen->m_pixelBuffer2[pos + i + (screenWidth * leftSideAdj)] = 0xFF0000FF;
//				}
//				x += leftSteps;
//				pos += leftSteps;
//				for (int i = 0; i < rightSteps + 1; i++) {
//					Uint32 pixel = p_screen->getPixel(x + i, y);
//					Uint32 blendWith = p_screen->getPixel(x + i, y + 1);
//					Uint32 blended = blendColors(pixel, blendWith, rightBlendAmount);
//					rightBlendAmount += rightBlendAdj;
//					p_screen->m_pixelBuffer2[pos + i + (screenWidth * rightSideAdj)] = blended;
//					//p_screen->m_pixelBuffer2[pos + i + (screenWidth * rightSideAdj)] = 0xFFFFFFFF;
//				}
//				x += rightSteps;
//			}
//			else {
//				if (pos < screenWidth * screenHeight) {
//					//p_screen->m_pixelBuffer2[pos] = p_screen->m_pixelBuffer[pos];
//				}
//			}
//		}
//	}
//	for (int y = 1; y < screenHeight - 1; y++) {
//		for (int x = 1; x < screenWidth - 1; x++) {
//			int pos = x + y * screenWidth;
//			if (p_screen->m_pixelBuffer2[pos] != 0xFFFF00FF) {
//				p_screen->m_pixelBuffer[pos] = p_screen->m_pixelBuffer2[pos];
//			}
//		}
//	}
//}