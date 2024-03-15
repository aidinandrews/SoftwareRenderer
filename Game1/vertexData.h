#pragma once
#include "vec3.h"
#include "vec2.h"
#include "swap.h"


struct VertexData {
	static unsigned short int mapIndex, screenIndex, textureIndex, normalIndex, camDistIndex, shadowMapIndex, lightDistIndex;
	static unsigned short int numData;
	float data[14];

	VertexData() {
		memset(data, 0, sizeof(data));
	}

	point3 getMap() { return point3(data[mapIndex], data[mapIndex + 1], data[mapIndex + 2]); }
	float getMapX() { return data[mapIndex]; }
	float getMapY() { return data[mapIndex + 1]; }
	float getMapZ() { return data[mapIndex + 2]; }

	point2 getScreen() { return point2(data[screenIndex], data[screenIndex + 1]); }
	float getScreen(int XorY) { return data[screenIndex + XorY]; }
	float getScreenX() { return data[screenIndex]; }
	float getScreenY() { return data[screenIndex + 1]; }

	point2 getTexture() { return point2(data[textureIndex], data[textureIndex + 1]); }
	float getTextureX() { return data[textureIndex]; }
	float getTextureY() { return data[textureIndex + 1]; }

	point3 getNormal() { return point3(data[normalIndex], data[normalIndex + 1], data[normalIndex + 2]); }
	float getNormalX() { return data[normalIndex]; }
	float getNormalY() { return data[normalIndex + 1]; }
	float getNormalZ() { return data[normalIndex + 2]; }

	point2 getShadowMap() { return point2(data[shadowMapIndex], data[shadowMapIndex + 1]); }
	float getShadowMapX() { return data[shadowMapIndex]; }
	float getShadowMapY() { return data[shadowMapIndex + 1]; }

	float getCamDist() { return data[camDistIndex]; }

	float getLightDist() { return data[lightDistIndex]; }

	void setMap(point3& newMap) {
		data[mapIndex] = newMap.x();
		data[mapIndex + 1] = newMap.y();
		data[mapIndex + 2] = newMap.z();
	}
	void setScreen(point2 newScreen) {
		data[screenIndex] = newScreen.x();

		data[screenIndex + 1] = newScreen.y();
	}
	void setScreen(int XorY, float value) {
		data[screenIndex + XorY] = value;
	}
	void setScreenX(float value) { data[screenIndex] = value; }
	void setScreenY(float value) { data[screenIndex + 1] = value; }

	void setTexture(point2& newTexture) {
		data[textureIndex] = newTexture.x();
		data[textureIndex + 1] = newTexture.y();
	}
	void setNormal(point3& newNormal) {
		data[normalIndex] = newNormal.x();
		data[normalIndex + 1] = newNormal.y();
		data[normalIndex + 2] = newNormal.z();
	}
	void setShadowMap(point2& newShadowMap) {
		data[shadowMapIndex] = newShadowMap.x();
		data[shadowMapIndex + 1] = newShadowMap.y();
	}
	void setCamDist(float& newCamDist) {
		data[camDistIndex] = newCamDist;
	}
	void setLightDist(float& newLightDist) {
		data[lightDistIndex] = newLightDist;
	}

	/*VertexData& operator=(const VertexData& v) {
		for (int i = 0; i < numData; i++) {
			data[i] = v.data[i];
		}
		return *this;
	}*/
	VertexData& operator+=(const VertexData& v) {
		for (int i = 0; i < numData; i++) {
			data[i] += v.data[i];
		}
		return *this;
	}
	VertexData& operator-=(const VertexData& v) {
		for (int i = 0; i < numData; i++) {
			data[i] -= v.data[i];
		}
		return *this;
	}
	VertexData& operator*=(const VertexData& v) {
		for (int i = 0; i < numData; i++) {
			data[i] *= v.data[i];
		}
		return *this;
	}
	VertexData& operator/=(const VertexData& v) {
		for (int i = 0; i < numData; i++) {
			data[i] /= v.data[i];
		}
		return *this;
	}
};

// Allow for the struct to support drawing triangles with varying levels of information.

void SET_VERTEX_DATA(int vertexDataType);
void SET_VERTEX_DATA_SHADOW();
void SET_VERTEX_DATA_ALL_DATA();
void SET_VERTEX_DATA_2D_FLAT_COLOR();
void SET_VERTEX_DATA_2D_TEXTURED();
void SET_VERTEX_DATA_3D_FLAT_COLOR();
void SET_VERTEX_DATA_3D_TEXTURED();
void SET_VERTEX_DATA_3D_FLAT_COLOR_FLAT_SHADING();
void SET_VERTEX_DATA_3D_TEXTURED_FLAT_SHADING();
void SET_VERTEX_DATA_3D_FLAT_COLOR_PHONG_SHADING();
void SET_VERTEX_DATA_3D_TEXTURED_PHONG_SHADING();

// VertexData Utility Functions

inline std::ostream& operator<<(std::ostream& out, VertexData& v) {
	return out <<
		"map: " << v.getMap() << "\n" <<
		"screen: " << v.getScreen() << "\n" <<
		"texture: " << v.getTexture() << "\n" <<
		"normal: " << v.getNormal() << "\n" <<
		"lightDist: " << v.getLightDist() << "\n" <<
		"camDist: " << v.getCamDist();
}

inline VertexData operator+(const VertexData& A, const VertexData& B) {
	VertexData answer;
	for (int i = 0; i < VertexData::numData; i++) {
		answer.data[i] = A.data[i] + B.data[i];
	}
	return answer;
}
inline VertexData operator-(const VertexData& A, const VertexData& B) {
	VertexData answer;
	for (int i = 0; i < VertexData::numData; i++) {
		answer.data[i] = A.data[i] - B.data[i];
	}
	return answer;
}
inline VertexData operator*(const VertexData& A, const VertexData& B) {
	VertexData answer;
	for (int i = 0; i < VertexData::numData; i++) {
		answer.data[i] = A.data[i] * B.data[i];
	}
	return answer;
}
inline VertexData operator*(const VertexData& A, const float& B) {
	VertexData answer;
	for (int i = 0; i < VertexData::numData; i++) {
		answer.data[i] = A.data[i] * B;
	}
	return answer;
}
inline VertexData operator*(const float& B, const VertexData& A) {
	return A * B;
}
inline VertexData operator/(const VertexData& A, const float& B) {
	return A * (1 / B);
}

inline void swap(VertexData &A, VertexData &B) {
	VertexData temp = A; A = B; B = temp;
}