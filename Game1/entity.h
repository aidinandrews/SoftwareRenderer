#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "vec3.h"
#include "triangle.h"
#include "fstream"
#include "renderer.h"
#include "renderer2.h"

class Entity {
public:;
	struct VertexIndex {
		// 0 = vertex index. 1 = texture index. 2 = normal index.
		int data[3];

		VertexIndex() {
			data[0] = 0;
			data[1] = 0;
			data[2] = 0;
		}
		int vertex() {
			return data[0];
		}
		int texture() {
			return data[1];
		}
		int normal() {
			return data[2];
		}
	};
	struct FaceData {
		VertexIndex points[3];

		int vertex(int v) {
			return points[v].vertex();
		}
		int texture(int v) {
			return points[v].texture();
		}
		int normal(int v) {
			return points[v].normal();
		}
	};
public:
	const char* m_path;
	Uint32BMP* p_textureMap;
	int m_numVertices = 0;
	int m_numTexturePoints = 0;
	int m_numNormals = 0;	
	int m_numFaces = 0;

	point3* m_v;
	point2* m_vt;
	vec3* m_vn;
	FaceData* m_f;

	point3 m_center;
	point3 m_delta;
	vec3 m_rotate;
	float m_scale;

	Renderer* p_renderer;
	Renderer2* p_renderer2;
	bool m_backFaceCulling = true;
	bool m_textureGood = false;
public:
	Entity();
	Entity(const char* path, Renderer* renderer);
	~Entity();
	void init(const char* path, Renderer2* renderer2);

	void readInfoSimple();
	void readInfoBlender();
	void readInfo();

	void update();
	void updateNormals();
	void render();
	void render2();
	void drawShadow();
	void move(point3 distnace);
	void teleport(point3 location);
	void scale(float amount);
	void rotate(point3 amount);
	point3 getCenter();
};

