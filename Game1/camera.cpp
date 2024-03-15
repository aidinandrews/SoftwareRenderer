#include "camera.h"

Camera::Camera() 
	: m_yaw(0), m_pitchStore(float(M_PI/2)), m_zoom(1), m_zoomAdj(-3)
{}
Camera::~Camera() {}

void Camera::adjPos(int x, int y, int z) {
	m_deltaStore += point3(float(x), float(y), float(z));
}
void Camera::adjYaw(float adjustment) {
	m_yawStore += adjustment;
}
void Camera::adjPitch(float adjustment) {
	m_pitchStore += adjustment;
}
void Camera::adjZoom(float adjustment) {
	m_zoomAdj += adjustment;
}

void Camera::update() {
	
	m_yaw = m_yawStore + m_yawAdj;
	m_pitch = m_pitchStore + m_pitchAdj;	
	m_delta = m_deltaStore + m_deltaAdj;	
	m_camDist = 5000 * (1 / m_zoom);
	m_zoom = float(pow(1.25, m_zoomAdj));

	// Camera normal
	float x = -sin(m_yaw) * cos(m_pitch);
	float y = -cos(m_pitch) * cos(m_yaw);
	float z = sin(m_pitch);
	m_normal = vec3(x, y, z);
	
	// Camera position
	point3 floorPosition = m_delta + p_screen->screenCenter;
	m_map = (m_normal * 5000) + floorPosition;
	m_plane.setPlane(m_normal, m_map);
}