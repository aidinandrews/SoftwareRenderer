#include "renderer2.h"

point2 Renderer2::project(point3 mapPosition, Camera& camera) {
	point2 screenPosition = point2(mapPosition.x(), mapPosition.y());
	//translate
	screenPosition += camera.m_delta;
	//set (0, 0) to center of screen
	point2 screenCenter(screenWidth * 0.5f, screenHeight * 0.5f);
	screenPosition -= screenCenter;
	//rotate
	float newX = (screenPosition.x() * cos(camera.m_yaw) - screenPosition.y() * sin(camera.m_yaw));
	float newY = (screenPosition.y() * cos(camera.m_yaw) + screenPosition.x() * sin(camera.m_yaw));
	screenPosition = point2(newX, newY);
	//pitch
	screenPosition.y() *= sin(camera.m_pitch);
	//zoom
	screenPosition *= camera.m_zoom;
	//uncenter screen coordinates
	screenPosition += screenCenter;
	//adjust for z value
	float z = mapPosition.e[2];
	screenPosition.y() -= sin(camera.m_pitch - float(M_PI) / 2) * z * camera.m_zoom;
	return screenPosition;
}

void Renderer2::defineTriangleType(const int& triangleType) {
	if (triangleType > -2 && triangleType < 8) {
		m_triangleType = triangleType;
		SET_VERTEX_DATA(triangleType);
	}
	else {
		std::cout << "Renderer: Invalid triange type identifier!\n";
	}
}

void Renderer2::renderTriangle(VertexData& A, VertexData& B, VertexData& C, Uint32& color, Uint32BMP* texture, vec3* normal) {

	if (color != NULL) { m_color = color; }
	if (texture != nullptr) { p_texture = texture; }
	if (normal != NULL) {
		m_triangleNormal = *normal;
	}
	else {
		m_triangleNormal = unitVector((A.getNormal() + B.getNormal() + C.getNormal()) / 3);
		if (dot(m_triangleNormal, p_camera->m_normal) < -0.01f) {
			if (m_backFaceCulling) {
				return;
			}
		}
	}
	initTriangle(A, B, C);
}

void Renderer2::renderTriangleShadow(VertexData& A, VertexData& B, VertexData& C, vec3* normal) {
	initTriangle(A, B, C);
}

void Renderer2::initTriangle(VertexData& A, VertexData& B, VertexData& C) {
	m_vertices[0] = A;
	m_vertices[1] = B;
	m_vertices[2] = C;
	m_numPoints = 3;

	//std::cout << m_vertices[0].getLightDist() << std::endl;
	//std::cout << A.getScreen() << std::endl;

	// Check if the triangle is off screen, or if it is partially off screen.
	// If it is partially off screen, isOnScreen() will crop it and update the VertexData list.

	if (!isOnScreen()) {
		return;
	}

	for (int i = 0; i < m_numPoints - 2; i++) {
		createTriangleSides(m_vertices[0], m_vertices[i + 1], m_vertices[i + 2]);
	}
}

bool Renderer2::isOnScreen() {

	float boundingBox[4];
	int left = 0, right = 1, top = 2, bottom = 3;

	boundingBox[left] = m_vertices[0].getScreenX();
	boundingBox[right] = m_vertices[1].getScreenX();
	if (boundingBox[left] > boundingBox[right]) {
		swap(boundingBox[left], boundingBox[right]);
	}
	if (boundingBox[left] > m_vertices[2].getScreenX()) {
		boundingBox[left] = m_vertices[2].getScreenX();
	}
	if (boundingBox[right] < m_vertices[2].getScreenX()) {
		boundingBox[right] = m_vertices[2].getScreenX();
	}
	if (m_vertices[0].getScreenY() > m_vertices[1].getScreenY()) {
		swap(m_vertices[0], m_vertices[1]);
	}
	if (m_vertices[0].getScreenY() > m_vertices[2].getScreenY()) {
		swap(m_vertices[0], m_vertices[2]);
	}
	if (m_vertices[1].getScreenY() > m_vertices[2].getScreenY()) {
		swap(m_vertices[1], m_vertices[2]);
	}
	boundingBox[top] = m_vertices[0].getScreenY();
	boundingBox[bottom] = m_vertices[2].getScreenY();

	// Don't draw the triangle if all the points are outside the camera's view.
	if (boundingBox[left] > screenWidth ||
		boundingBox[right] < 0 ||
		boundingBox[top] > screenHeight ||
		boundingBox[bottom] < 0) {
		return false;
	}
	// Check if the triangle is completely inside the screen.
	if (boundingBox[left] > 0 &&
		boundingBox[right] < screenWidth &&
		boundingBox[top] > 0 &&
		boundingBox[bottom] < screenHeight) {
		m_numPoints = 3;
		return true;
	}
	else {
		suthHodgeCrop();
		return true;
	}
}

void Renderer2::suthHodgeCrop() {
	int currentPointCount = 3;
	int storedPointCount = 0;
	VertexData* currentPoints = new VertexData[7]{ m_vertices[0], m_vertices[1], m_vertices[2] };
	VertexData* storedPoints = new VertexData[7];
	int XorY[] = { 1, 0, 1, 0, 1 };
	float borderValues[] = { 0, screenWidth, screenHeight, 0 };
	int flip[] = { -1, 1, 1, -1 };

	for (int i = 0; i < 4; i++) { // Checks top -> left -> bottom -> right.
		for (int pointIndex = 0; pointIndex < currentPointCount; pointIndex++) {
			VertexData* pt1 = &currentPoints[pointIndex];
			VertexData* pt2 = &currentPoints[(pointIndex + 1) % currentPointCount];
			float pt1Value = (pt1->getScreen()[XorY[i]]);
			float pt2Value = (pt2->getScreen()[XorY[i]]);

			bool pt1Visible = pt1Value * flip[i] < borderValues[i];
			bool pt2Visible = pt2Value * flip[i] < borderValues[i];

			if (pt1Visible) {
				storedPoints[storedPointCount] = currentPoints[pointIndex];
				storedPointCount++;
			}
			bool lineSegmentIntersection = pt1Visible != pt2Visible;
			if (lineSegmentIntersection) {
				// Interpolate the data that lies on the intersection of the line segment and the screen border.
				// y = (y2 – y1) / (x2 – x1) * (x – x1) + y1
				VertexData intersection = (*pt2 - *pt1) / (pt2Value - pt1Value) * (borderValues[i] - pt1Value) + *pt1;

				intersection.setScreen(XorY[i], borderValues[i]);
				intersection.setScreen(XorY[i + 1], floor(intersection.getScreen(XorY[i + 1])));

				storedPoints[storedPointCount] = intersection;
				storedPointCount++;
			}
		}
		// Swap the stored and current values to prepare to compare the new points to the next border side.
		currentPointCount = storedPointCount;
		storedPointCount = 0;
		VertexData* temp = currentPoints;
		currentPoints = storedPoints;
		storedPoints = temp;
	}
	m_numPoints = currentPointCount;
	for (int i = 0; i < currentPointCount; i++) {
		m_vertices[i] = currentPoints[i];
	}
}

void Renderer2::createTriangleSides(VertexData A, VertexData B, VertexData C) {
	// Wire frame		
	if (state[SDL_SCANCODE_P]) {
		drawLine(*p_screen, A.getScreen(), B.getScreen(), 0xFFFFFFFF);
		drawLine(*p_screen, A.getScreen(), C.getScreen(), 0xFFFFFFFF);
		drawLine(*p_screen, B.getScreen(), C.getScreen(), 0xFFFFFFFF);
		return;
	}
	A.setScreenX(floor(A.getScreenX()));
	A.setScreenY(floor(A.getScreenY()));
	B.setScreenX(floor(B.getScreenX()));
	B.setScreenY(floor(B.getScreenY()));
	C.setScreenX(floor(C.getScreenX()));
	C.setScreenY(floor(C.getScreenY()));
	// Sort points top to bottom
	if (A.getScreenY() > B.getScreenY()) { swap(A, B); }
	if (A.getScreenY() > C.getScreenY()) { swap(A, C); }
	if (B.getScreenY() > C.getScreenY()) { swap(B, C); }
	// Group the points into lines.
	m_AC.createSide(A, C);
	// Draw the flat bottom half triangle.
	bool thread2Used = false;
	bool thread3Used = false;
	if (A.getScreenY() != B.getScreenY()) {
		thread2Used = true;
		m_AB.createSide(A, B);
		m_ySteps = m_AB.steps;
		// Check that the left side is actually on the left.
		if (m_AC.offset.getScreenX() < m_AB.offset.getScreenX()) {
			p_leftSide = &m_AC;
			p_rightSide = &m_AB;
		}
		else {
			p_leftSide = &m_AB;
			p_rightSide = &m_AC;
		}
		drawHalfTriangle(p_leftSide, p_rightSide, m_ySteps, p_screen);
	}
	if (B.getScreenY() != C.getScreenY()) {
		thread3Used = true;
		m_BC.createSide(B, C);
		m_ySteps = m_BC.steps;

		if (m_AC.offset.getScreenX() > m_BC.offset.getScreenX()) {
			p_leftSide = &m_AC;
			p_rightSide = &m_BC;
		}
		else {
			p_leftSide = &m_BC;
			p_rightSide = &m_AC;
		}
		drawHalfTriangle(p_leftSide, p_rightSide, m_ySteps, p_screen);
	}
}

void Renderer2::drawHalfTriangle(LineSegment* leftSide, LineSegment* rightSide, int steps, Screen* screenptr) {
	float colorDimAmount = 1;
	LineSegment scanline;
	vec3 lightDirection(0, 0, 1);
	Uint32 color = 0xFF0000FF;


	switch (m_triangleType) {
	case TRIANGLE_TYPE_SHADOW:
		while (steps > 0) {
			scanline.createScanline(leftSide->current, rightSide->current);
			int x = int(scanline.current.getScreenX());
			int y = int(scanline.current.getScreenY());
			int yPos = screenWidth * y;

			while (scanline.steps > 0) {
				x = int(scanline.current.getScreenX());
				//check if the pixel is visible
				if (scanline.current.getLightDist() < screenptr->getShadowMap(x, y)) {
					screenptr->setShadowMap(x, y, scanline.current.getLightDist());
				}
				scanline.offsetData();
			}
			leftSide->offsetData();
			rightSide->offsetData();
			steps--;
		}
		break;

	case TRIANGLE_TYPE_2D_FLAT_COLOR_NO_SHADING:
		while (steps > 0) {
			m_scanline.createScanline(leftSide->current, rightSide->current);
			int x = int(m_scanline.current.getScreenX());
			int y = int(m_scanline.current.getScreenY());
			int yPos = screenWidth * y;

			while (m_scanline.steps > 0) {
				x = int(m_scanline.current.getScreenX());

				p_screen->m_pixelBuffer[x + yPos] = m_color;

				m_scanline.offsetData();
			}
			leftSide->offsetData();
			rightSide->offsetData();
			steps--;
		}
		break;

	case TRIANGLE_TYPE_2D_TEXTURED_NO_SHADING:
		while (steps > 0) {
			m_scanline.createScanline(leftSide->current, rightSide->current);
			int x = int(m_scanline.current.getScreenX());
			int y = int(m_scanline.current.getScreenY());
			int yPos = screenWidth * y;

			while (m_scanline.steps > 0) {
				x = int(m_scanline.current.getScreenX());
				int tx = int(m_scanline.current.getTextureX());
				int ty = int(m_scanline.current.getTextureY());
				p_screen->m_pixelBuffer[x + yPos] = p_texture->getPixel(tx, ty);
				m_scanline.offsetData();
			}
			leftSide->offsetData();
			rightSide->offsetData();
			steps--;
		}
		break;

	case TRIANGLE_TYPE_3D_FLAT_COLOR_NO_SHADING:
		while (steps > 0) {
			m_scanline.createScanline(leftSide->current, rightSide->current);
			int x = int(m_scanline.current.getScreenX());
			int y = int(m_scanline.current.getScreenY());
			int yPos = screenWidth * y;

			while (m_scanline.steps > 0) {
				x = int(m_scanline.current.getScreenX());

				//check if the pixel is visible
				if (m_scanline.current.getCamDist() < p_screen->getZBuffer(x, y)) {
					p_screen->m_pixelBuffer[x + yPos] = m_color;
					p_screen->m_zBuffer[x + yPos] = m_scanline.current.getCamDist();
				}
				m_scanline.offsetData();
			}
			leftSide->offsetData();
			rightSide->offsetData();
			steps--;
		}
		break;

	case TRIANGLE_TYPE_3D_TEXTURED_NO_SHADING:
		while (steps > 0) {
			m_scanline.createScanline(leftSide->current, rightSide->current);
			int x = int(m_scanline.current.getScreenX());
			int y = int(m_scanline.current.getScreenY());
			int yPos = screenWidth * y;

			while (m_scanline.steps > 0) {
				x = int(m_scanline.current.getScreenX());
				int tx = int(m_scanline.current.getTextureX());
				int ty = int(m_scanline.current.getTextureY());

				if (m_scanline.current.getCamDist() < p_screen->getZBuffer(x, y)) {
					m_color = p_texture->getPixel(tx, ty);
					p_screen->m_pixelBuffer[x + yPos] = m_color;
					p_screen->m_zBuffer[x + yPos] = m_scanline.current.getCamDist();
				}
				m_scanline.offsetData();
			}
			leftSide->offsetData();
			rightSide->offsetData();
			steps--;
		}
		break;

	case TRIANGLE_TYPE_3D_FLAT_COLOR_FLAT_SHADING:

		colorDimAmount = float(sin(dot(m_lightDirection, m_triangleNormal) * (M_PI / 2)));
		colorDimAmount = (colorDimAmount + 0.25f) * (4.0f / 5.0f);
		if (colorDimAmount < 0.25f) { colorDimAmount = 0.25f; }

		while (steps > 0) {
			m_scanline.createScanline(leftSide->current, rightSide->current);
			int x = int(m_scanline.current.getScreenX());
			int y = int(m_scanline.current.getScreenY());
			int yPos = screenWidth * y;

			while (m_scanline.steps > 0) {
				x = int(m_scanline.current.getScreenX());
				if (m_scanline.current.getCamDist() < p_screen->getZBuffer(x, y)) {
					m_color = darkenColor(m_color, colorDimAmount);
					p_screen->m_pixelBuffer[x + yPos] = m_color;
					p_screen->m_zBuffer[x + yPos] = m_scanline.current.getCamDist();
				}
				m_scanline.offsetData();
			}
			leftSide->offsetData();
			rightSide->offsetData();
			steps--;
		}
		break;

	case TRIANGLE_TYPE_3D_TEXTURED_FLAT_SHADING:

		colorDimAmount = float(sin(dot(m_lightDirection, m_triangleNormal) * (M_PI / 2)));
		colorDimAmount = (colorDimAmount + 0.25f) * (4.0f / 5.0f);
		if (colorDimAmount < 0.25f) { colorDimAmount = 0.25f; }

		while (steps > 0) {
			m_scanline.createScanline(leftSide->current, rightSide->current);
			int x = int(m_scanline.current.getScreenX());
			int y = int(m_scanline.current.getScreenY());
			int yPos = screenWidth * y;

			while (m_scanline.steps > 0) {
				x = int(m_scanline.current.getScreenX());
				int tx = int(m_scanline.current.getTextureX());
				int ty = int(m_scanline.current.getTextureY());

				//check if the pixel is visible
				if (m_scanline.current.getCamDist() < p_screen->getZBuffer(x, y)) {

					m_color = p_texture->getPixel(tx, ty);
					m_color = darkenColor(m_color, colorDimAmount);
					p_screen->m_pixelBuffer[x + yPos] = m_color;
					p_screen->m_zBuffer[x + yPos] = m_scanline.current.getCamDist();
				}
				m_scanline.offsetData();
			}
			leftSide->offsetData();
			rightSide->offsetData();
			steps--;
		}
		break;

	case TRIANGLE_TYPE_3D_FLAT_COLOR_PHONG_SHADING:
		while (steps > 0) {
			m_scanline.createScanline(leftSide->current, rightSide->current);
			int x = int(m_scanline.current.getScreenX());
			int y = int(m_scanline.current.getScreenY());
			int yPos = screenWidth * y;

			while (m_scanline.steps > 0) {
				x = int(m_scanline.current.getScreenX());
				//check if the pixel is visible
				if (m_scanline.current.getCamDist() < p_screen->getZBuffer(x, y)) {

					colorDimAmount = float(sin(dot(m_lightDirection, m_scanline.current.getNormal()) * (M_PI / 2)));
					colorDimAmount = (colorDimAmount + 0.25f) * (4.0f / 5.0f);
					if (colorDimAmount < 0.25f) { colorDimAmount = 0.25f; }

					m_color = darkenColor(m_color, colorDimAmount);
					p_screen->m_pixelBuffer[x + yPos] = m_color;
					p_screen->m_zBuffer[x + yPos] = m_scanline.current.getCamDist();
				}
				m_scanline.offsetData();
			}
			leftSide->offsetData();
			rightSide->offsetData();
			steps--;
		}
		break;

	case TRIANGLE_TYPE_3D_TEXTURED_PHONG_SHADING:

		for (; steps > 0; steps--) {
			scanline.createScanline(leftSide->current, rightSide->current);
			int x = int(scanline.current.getScreenX());
			int y = int(scanline.current.getScreenY());
			int yPos = screenWidth * y;

			for (; scanline.steps > 0; scanline.offsetData()) {
				x = int(scanline.current.getScreenX());
				int tx = int(scanline.current.getTextureX());
				int ty = int(scanline.current.getTextureY());

				//check if the pixel is visible
				if (scanline.current.getCamDist() < p_screen->getZBuffer(x, y)) {

					color = p_texture->getPixel(tx, ty);

					int sX = scanline.current.getShadowMapX();
					int sY = scanline.current.getShadowMapY();

					/*colorDimAmount = float(sin(dot(lightDirection, scanline.current.getNormal()) * (M_PI / 2)));
					if (colorDimAmount < 0) {
						colorDimAmount = 0;
					}
					color = darkenColor(color, colorDimAmount);*/

					screenptr->m_pixelBuffer[x + y * screenWidth] = color;
					screenptr->m_zBuffer[x + yPos] = scanline.current.getCamDist();
				}

			}
			leftSide->offsetData();
			rightSide->offsetData();
		}
		break;
	}
}

struct HalfTriangleInput {
	LineSegment leftSide, rightSide, scanline;
	int ySteps;
	Uint32BMP* texturePointer;
};

//void drawHalfTriangle(HalfTriangleInput input) {
//
//	while (input.ySteps > 0) {
//		input.scanline.createScanline(input.leftSide.current, input.rightSide.current);
//		int screenY = int(input.scanline.current.getScreenY());
//		int screenPosition = screenWidth * screenY + int(input.scanline.current.getScreenX());
//
//		while (input.scanline.steps > 0) {
//			int screenX = int(input.scanline.current.getScreenX());
//			int textureX = int(input.scanline.current.getTextureX());
//			int textureY = int(input.scanline.current.getTextureY());
//
//			//check if the pixel is visible
//			if (input.scanline.current.getCamDist() < p_screen->m_zBuffer[screenPosition]) {
//
//				Uint32 pixelColor = input.texturePointer->getPixel(textureX, textureY);
//
//				float normalAlignment = dot(m_lightDirection, input.scanline.current.getNormal());
//				float colorDimAmount;
//				if (normalAlignment > 0) { colorDimAmount = normalAlignment * 0.75f + 0.25f; }
//				else { colorDimAmount = 0.25f; }
//				pixelColor = darkenColor(pixelColor, colorDimAmount);
//
//				p_screen->m_pixelBuffer[screenPosition] = pixelColor;
//				p_screen->m_zBuffer[screenPosition] = input.scanline.current.getCamDist();
//			}
//			input.scanline.offsetData();
//		}
//		input.leftSide.offsetData();
//		input.rightSide.offsetData();
//		input.ySteps -= m_threadCount;
//	}
//}
