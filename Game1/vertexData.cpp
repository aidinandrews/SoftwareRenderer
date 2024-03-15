#include "vertexData.h"

// Define the static members
unsigned short int VertexData::mapIndex = 0;
unsigned short int VertexData::screenIndex = 0;
unsigned short int VertexData::textureIndex = 0;
unsigned short int VertexData::normalIndex = 0;
unsigned short int VertexData::shadowMapIndex = 0;
unsigned short int VertexData::camDistIndex = 0;
unsigned short int VertexData::lightDistIndex = 0;
unsigned short int VertexData::numData = 0;

void SET_VERTEX_DATA(int vertexDataType) {
	switch (vertexDataType) {
	case -1:
		SET_VERTEX_DATA_SHADOW();
		break;
	case 0:
		SET_VERTEX_DATA_2D_FLAT_COLOR();
		break;
	case 1:
		SET_VERTEX_DATA_2D_TEXTURED();
		break;
	case 2:
		SET_VERTEX_DATA_3D_FLAT_COLOR();
		break;
	case 3:
		SET_VERTEX_DATA_3D_TEXTURED();
		break;
	case 4:
		SET_VERTEX_DATA_3D_FLAT_COLOR_FLAT_SHADING();
		break;
	case 5:
		SET_VERTEX_DATA_3D_TEXTURED_FLAT_SHADING();
		break;
	case 6:
		SET_VERTEX_DATA_3D_FLAT_COLOR_PHONG_SHADING();
		break;
	case 7:
		SET_VERTEX_DATA_3D_TEXTURED_PHONG_SHADING();
		break;
	default:
		SET_VERTEX_DATA_ALL_DATA();
	}
}

void SET_VERTEX_DATA_ALL_DATA() {
	VertexData::numData = 11;
	VertexData::mapIndex = 0;
	VertexData::screenIndex = 3;
	VertexData::textureIndex = 5;
	VertexData::normalIndex = 7;
	VertexData::shadowMapIndex = 10;
	VertexData::camDistIndex = 12;
	VertexData::lightDistIndex = 13;
}
void SET_VERTEX_DATA_2D_FLAT_COLOR() {
	VertexData::numData = 2;
	VertexData::screenIndex = 0;

	VertexData::mapIndex = 8;
	VertexData::textureIndex = 8;
	VertexData::normalIndex = 8;
	VertexData::shadowMapIndex = 8;
	VertexData::camDistIndex = 8;
	VertexData::lightDistIndex = 8;
}
void SET_VERTEX_DATA_2D_TEXTURED() {
	VertexData::numData = 4;
	VertexData::screenIndex = 0;
	VertexData::textureIndex = 2;

	VertexData::mapIndex = 8;
	VertexData::normalIndex = 8;
	VertexData::shadowMapIndex = 8;
	VertexData::camDistIndex = 8;
	VertexData::lightDistIndex = 8;
}
void SET_VERTEX_DATA_3D_FLAT_COLOR() {
	VertexData::numData = 3;
	VertexData::screenIndex = 0;
	VertexData::camDistIndex = 2;

	VertexData::mapIndex = 8;
	VertexData::textureIndex = 8;
	VertexData::normalIndex = 8;
	VertexData::shadowMapIndex = 8;
	VertexData::lightDistIndex = 8;
}
void SET_VERTEX_DATA_3D_TEXTURED() {
	VertexData::numData = 5;
	VertexData::screenIndex = 0;
	VertexData::textureIndex = 2;
	VertexData::camDistIndex = 4;

	VertexData::mapIndex = 8;
	VertexData::normalIndex = 8;
	VertexData::shadowMapIndex = 8;
	VertexData::lightDistIndex = 8;
}

void SET_VERTEX_DATA_3D_FLAT_COLOR_FLAT_SHADING() {
	VertexData::numData = 3;
	VertexData::screenIndex = 0;
	VertexData::camDistIndex = 2;

	VertexData::mapIndex = 8;
	VertexData::textureIndex = 8;
	VertexData::normalIndex = 8;
	VertexData::shadowMapIndex = 8;
	VertexData::lightDistIndex = 8;
}

void SET_VERTEX_DATA_3D_TEXTURED_FLAT_SHADING() {
	VertexData::numData = 5;
	VertexData::screenIndex = 0;
	VertexData::textureIndex = 2;
	VertexData::camDistIndex = 4;

	VertexData::mapIndex = 8;
	VertexData::normalIndex = 8;
	VertexData::shadowMapIndex = 8;
	VertexData::lightDistIndex = 8;
}

void SET_VERTEX_DATA_3D_FLAT_COLOR_PHONG_SHADING() {
	VertexData::numData = 6;
	VertexData::screenIndex = 0;
	VertexData::normalIndex = 2;
	VertexData::camDistIndex = 5;

	VertexData::mapIndex = 8;
	VertexData::textureIndex = 8;
	VertexData::shadowMapIndex = 8;
	VertexData::lightDistIndex = 8;
}
void SET_VERTEX_DATA_3D_TEXTURED_PHONG_SHADING() {
	VertexData::numData = 11;
	VertexData::screenIndex = 0;
	VertexData::textureIndex = 2;
	VertexData::normalIndex = 4;
	VertexData::camDistIndex = 7; 	
	VertexData::shadowMapIndex = 8;
	VertexData::lightDistIndex = 10;
	
	VertexData::mapIndex = 11;	
}
void SET_VERTEX_DATA_SHADOW() {
	VertexData::numData = 3;
	VertexData::lightDistIndex = 2;
	VertexData::screenIndex = 0;

	VertexData::shadowMapIndex = 8;
	VertexData::mapIndex = 8;
	VertexData::textureIndex = 8;
	VertexData::normalIndex = 8;
	VertexData::camDistIndex = 8;
}