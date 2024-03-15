#include "line.h"

void drawLine(Screen& screen, point2 A, point2 B, Uint32 color) {

	float x, y, dx, dy, step;

	dx = abs(B.x() - A.x());
	dy = abs(B.y() - A.y());

	if (dx >= dy) { step = dx; }
	else { step = dy; }

	dx = (B.x() - A.x());
	dy = (B.y() - A.y());

	dx = dx / step;
	dy = dy / step;

	x = A.x();
	y = A.y();

	int i = 1;
	while (i <= step) {
		screen.setPixel(int(x), int(y), color);
		x = x + dx;
		y = y + dy;
		i = i + 1;
	}
}

float getLineDist2d(point2 A, point2 B) {

	return sqrt((B.x() - A.x()) * (B.x() - A.x()) + (B.y() - A.y()) * (B.y() - A.y()));
}

float getLineDist3d(point3 A, point3 B) {

	return sqrt((B.x() - A.x()) * (B.x() - A.x()) + (B.y() - A.y()) * (B.y() - A.y()) + (B.z() - A.z()) * (B.z() - A.z()));
}

void LineSegment::offsetData() {
	current = current + offset;
	steps--;
}
void LineSegment::createSide(VertexData A, VertexData B) {
	if (A.getScreenY() > B.getScreenY()) { swap(A, B); }
	current = A;
	steps = int(B.getScreenY() - A.getScreenY());

	offset = (B - A) / float(steps);
	offset.setScreenY(1);
}
void LineSegment::createScanline(VertexData leftPoint, VertexData rightPoint) {
	steps = int(rightPoint.getScreenX()) - int(leftPoint.getScreenX());
	current = leftPoint;
	current.setScreenX(floor(current.getScreenX()));

	offset = (rightPoint - leftPoint) / float(steps);
	offset.setScreen(point2(1, 0));
}