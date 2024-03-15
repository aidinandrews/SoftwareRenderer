#pragma once
#include "vec3.h"
#include "vec2.h"
#include "plane.h"
#include "screen.h"

class Camera {
public:
	point3 m_map, m_delta, m_deltaAdj, m_deltaStore, m_normal;
	float m_yaw, m_yawAdj, m_yawStore;
	float m_pitch, m_pitchAdj, m_pitchStore;
	float m_zoom, m_zoomAdj;
	float m_camDist;
	Plane m_plane;
	Screen* p_screen;
public:
	Camera();
	~Camera();
	void adjPos(int x, int y, int z);
	void adjYaw(float adjustment);
	void adjPitch(float adjustment);
	void adjZoom(float adjustment);
	void update();
};

