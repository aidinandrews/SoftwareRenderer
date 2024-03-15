#include "entity.h"

Entity::Entity() {
	p_renderer = nullptr;
}

void Entity::init(const char* path, Renderer2* renderer2) {
	m_path = path;
	p_renderer2 = renderer2;
	m_scale = 1;
	m_v = nullptr;
	m_vt = nullptr;
	m_vn = nullptr;
	m_f = nullptr;
	readInfo();
}

Entity::Entity(const char* path, Renderer* renderer) {
	m_path = path;
	p_renderer = renderer;
	m_scale = 1;
	m_v = nullptr;
	m_vt = nullptr;
	m_vn = nullptr;
	m_f = nullptr;
	readInfo();
}

Entity::~Entity() {
	delete[] m_v;
	delete[] m_vt;
	delete[] m_vn;
	delete[] m_f;
}

void Entity::readInfoSimple() {
	char currentChar;
	std::ifstream objFile(m_path);

	if (!objFile.is_open()) {
		std::cout << "cannot open obj file!\n";
		return;
	}
	while (objFile.good()) { // Repeats until the file has been read in its entirety.
		objFile >> currentChar;
		if (currentChar == 'v') {
			m_numVertices++;
		}
		if (currentChar == 'f') {
			m_numFaces++;
		}
	}
	objFile.clear(); // Set flags to 0.
	objFile.seekg(0); // Return to the beginning of the file.
	//m_vertexes = new point3[m_numVertices];
	//m_unalteredPoints = new point3[m_numVertices];
	//m_faces = new FaceData[m_numFaces];
	//m_normals = new vec3[m_numFaces];
	int pointIndex[3];
	point3 point;
	int i = 0, j = 0;

	while (objFile.good()) {
		objFile >> currentChar;
		if (currentChar == 'v' && i < m_numVertices) {
			objFile >> point.x() >> point.y() >> point.z();
			//m_unalteredPoints[i] = point;
			//m_vertexes[i] = point;
			i++;
			m_center += point;
		}
		if (currentChar == 'f' && j < m_numFaces) {
			objFile >> pointIndex[0] >> pointIndex[1] >> pointIndex[2];
			//m_faces[j].points[0].data[0] = pointIndex[0];
			//m_faces[j].points[1].data[0] = pointIndex[1];
			//m_faces[j].points[2].data[0] = pointIndex[2];
			//m_normals[j] = getNormal(m_vertexes[pointIndex[0]], m_vertexes[pointIndex[1]], m_vertexes[pointIndex[2]]);
			j++;
		}
	}
	m_center /= float(m_numVertices);
}

point3 stringToPoint3(std::string& string, int offset) {
	int index = 0;
	std::string value;
	point3 point;

	for (int i = offset; i < string.length(); i++) {
		if (string.at(i) != ' ') {
			value += string.at(i);
		}
		else {
			point[index] = std::stof(value);
			index++;
			value.clear();
		}
	}
	point.z() = std::stof(value);
	return point;
}
point2 stringToPoint2(std::string& string, int offset) {
	int index = 0;
	std::string value;
	point2 point;

	for (int i = offset; i < string.length(); i++) {
		if (string.at(i) != ' ') {
			value += string.at(i);
		}
		else {
			point[index] = std::stof(value);
			index++;
			value.clear();
		}
	}
	point.y() = std::stof(value);
	return point;
}

void Entity::readInfo() {

	std::ifstream objFile(m_path);

	if (!objFile.is_open()) {
		std::cout << "cannot open obj file!\n";
		return;
	}

	std::vector<point3> v, vn;
	std::vector<point2> vt;
	std::vector<FaceData> f;

	std::string currentLine;

	while (objFile.good()) {

		std::getline(objFile, currentLine);

		if (currentLine.length() < 3) {
			continue;
		}

		if (currentLine.at(0) == 'v' && currentLine.at(1) == ' ') {
			point3 point = stringToPoint3(currentLine, 2);
			v.push_back(point);
			m_center += point;
			continue;
		}
		if (currentLine.at(0) == 'v' && currentLine.at(1) == 't') {
			vt.push_back(stringToPoint2(currentLine, 3));
			continue;
		}
		if (currentLine.at(0) == 'v' && currentLine.at(1) == 'n') {
			vn.push_back(unitVector(stringToPoint3(currentLine, 3)));
			continue;

		}
		if (currentLine.at(0) == 'f' && currentLine.at(1) == ' ') {
			std::vector<VertexIndex> vertices;
			VertexIndex vertex;
			std::string value;
			int valueType = 0;

			for (int i = 2; i < currentLine.length(); i++) {
				if (currentLine.at(i) == ' ') {
					vertex.data[valueType] = stoi(value) - 1; // .obj files start at 1 not 0.
					value.clear();
					vertices.push_back(vertex);
					valueType = 0;
				}
				else if (currentLine.at(i) == '/') {
					vertex.data[valueType] = stoi(value) - 1;
					valueType++;
					value.clear();
				}
				else {
					value += currentLine.at(i);
				}
			}
			if (valueType < 3 && !value.empty()) {
				vertex.data[valueType] = stoi(value) - 1; // .obj files start at 1 not 0.
				vertices.push_back(vertex);
			}
			for (int i = 0; i < vertices.size() - 2; i++) {
				FaceData fd;
				fd.points[0] = vertices[0];
				fd.points[1] = vertices[i + 1];
				fd.points[2] = vertices[i + 2];
				f.push_back(fd);
			}
			continue;
		}
	}
	m_center /= float(v.size());

	m_v = new point3[v.size()];
	m_numVertices = int(v.size());
	for (int i = 0; i < v.size(); i++) {
		m_v[i] = v[i];
	}
	m_vt = new point2[vt.size()];
	m_numTexturePoints = int(vt.size());
	for (int i = 0; i < vt.size(); i++) {
		m_vt[i] = vt[i];
	}
	m_vn = new vec3[vn.size()];
	m_numNormals = int(vn.size());
	for (int i = 0; i < vn.size(); i++) {
		m_vn[i] = vn[i];
	}
	m_f = new FaceData[f.size()];
	m_numFaces = int(f.size());
	for (int i = 0; i < f.size(); i++) {
		m_f[i] = f[i];
	}

	/*for (int i = 0; i < v.size(); i++) {
		std::cout << "v " << v[i] << "\n";
	}
	for (int i = 0; i < vt.size(); i++) {
		std::cout << "vt " << vt[i] << "\n";
	}
	for (int i = 0; i < vn.size(); i++) {
		std::cout << "vn " << vn[i] << "\n";
	}
	for (int i = 0; i < m_numFaces; i++) {
		std::cout
			<< m_f[i].vertex(0) + 1 << ","
			<< m_f[i].texture(0) + 1 << ","
			<< m_f[i].normal(0) + 1 << " "
			<< m_f[i].vertex(1) + 1 << ","
			<< m_f[i].texture(1) + 1 << ","
			<< m_f[i].normal(1) + 1 << " "
			<< m_f[i].vertex(2) + 1 << ","
			<< m_f[i].texture(2) + 1 << ","
			<< m_f[i].normal(2) + 1 << "\n";
	}*/
}

void Entity::update() {

}

void Entity::drawShadow() {

	VertexData vertexData[3];
	point2* shadowMapPoints = new point2[m_numVertices];
	Plane lightPlane(point3(0, 0, 5000), vec3(0, 0, 1));
	float* lightDists = new float[m_numVertices];
	p_renderer2->defineTriangleType(Renderer2::TRIANGLE_TYPE_SHADOW);

	Camera light = *p_renderer2->getCamera();
	light.m_pitchStore =  float(M_PI / 2);
	light.m_pitchAdj = 0;
	light.update();

	for (int i = 0; i < m_numVertices; i++) {

		lightDists[i] = pointToPlaneDist(m_v[i], light.m_plane);

		shadowMapPoints[i] = p_renderer2->project(m_v[i], light);
		shadowMapPoints[i].x() = floor(shadowMapPoints[i].x());
		shadowMapPoints[i].y() = floor(shadowMapPoints[i].y());
	}
	for (int face = 0; face < m_numFaces; face++) {
		for (int point = 0; point < 3; point++) {

			vertexData[point].setScreen(shadowMapPoints[m_f[face].vertex(point)]);
			vertexData[point].setLightDist(lightDists[m_f[face].vertex(point)]);
		}
		vec3 normal = unitVector((m_vn[m_f[face].normal(0)] + m_vn[m_f[face].normal(1)] + m_vn[m_f[face].normal(2)]) / 3);
		if (dot(normal, point3(0,0,1)) > 0) {
			continue;
		}
		p_renderer2->renderTriangleShadow(vertexData[0], vertexData[1], vertexData[2], &normal);
	}
	delete[] shadowMapPoints;
	delete[] lightDists;
}

void Entity::render() {

	VertexData vertexData[3];
	point2 defaultTxtr;

	Triangle t;
	if (m_textureGood == false) {
		for (int i = 0; i < m_numFaces; i++) {
			t._mapPoints[0] = m_v[m_f[i].vertex(0)];
			t._mapPoints[1] = m_v[m_f[i].vertex(1)];
			t._mapPoints[2] = m_v[m_f[i].vertex(2)];

			t.p_texture = p_renderer->m_defaultTexture;

			t._normal[0] = m_vn[m_f[i].normal(0)];
			t._normal[1] = m_vn[m_f[i].normal(1)];
			t._normal[2] = m_vn[m_f[i].normal(2)];

			t._backFaceCulling = m_backFaceCulling;
			p_renderer->renderTriangle(&t);
		}
		return;
	}
	float textureWidth = float(p_renderer->m_defaultTexture->getWidth());
	float textureHeight = float(p_renderer->m_defaultTexture->getHeight());
	point2 textureSize(textureWidth, textureHeight);
	for (int i = 0; i < m_numFaces; i++) {
		t._mapPoints[0] = m_v[m_f[i].vertex(0)];
		t._mapPoints[1] = m_v[m_f[i].vertex(1)];
		t._mapPoints[2] = m_v[m_f[i].vertex(2)];

		t.p_texture = p_renderer->m_defaultTexture;
		t._texturePos[0] = m_vt[m_f[i].texture(0)] * textureSize;
		t._texturePos[1] = m_vt[m_f[i].texture(1)] * textureSize;
		t._texturePos[2] = m_vt[m_f[i].texture(2)] * textureSize;

		t._normal[0] = m_vn[m_f[i].normal(0)];
		t._normal[1] = m_vn[m_f[i].normal(1)];
		t._normal[2] = m_vn[m_f[i].normal(2)];

		t._backFaceCulling = m_backFaceCulling;
		p_renderer->renderTriangle(&t);
	}
}

void Entity::render2() {

	VertexData vertexData[3];
	point2 defaultTxtr;
	point2* screenPoints = new point2[m_numVertices];
	float* camDists = new float[m_numVertices];
	p_renderer2->defineTriangleType(Renderer2::TRIANGLE_TYPE_3D_TEXTURED_PHONG_SHADING);

	point2* shadowMapPoints = new point2[m_numVertices];
	Camera light = *p_renderer2->getCamera();
	Plane lightPlane(point3(0, 0, 5000), vec3(0, 0, 1));
	float* lightDists = new float[m_numVertices];
	light.m_pitchStore = float(M_PI / 2);
	light.m_pitchAdj = 0;
	light.update();

	for (int i = 0; i < m_numVertices; i++) {
		screenPoints[i] = p_renderer2->project(m_v[i], *p_renderer->getCamera());
		screenPoints[i].x() = floor(screenPoints[i].x());
		screenPoints[i].y() = floor(screenPoints[i].y());

		shadowMapPoints[i] = p_renderer2->project(m_v[i], light);
		shadowMapPoints[i].x() = floor(shadowMapPoints[i].x());
		shadowMapPoints[i].y() = floor(shadowMapPoints[i].y());

		lightDists[i] = pointToPlaneDist(m_v[i], light.m_plane);

		camDists[i] = getLineDist3d(m_v[i], p_renderer2->getCamera()->m_map);
	}
	for (int face = 0; face < m_numFaces; face++) {
		for (int point = 0; point < 3; point++) {

			int pointTarget = m_f[face].vertex(point);
			vertexData[point].setScreen(screenPoints[pointTarget]);
			vertexData[point].setShadowMap(shadowMapPoints[pointTarget]);
			point2 txtr;
			if (m_textureGood) {
				txtr = m_vt[m_f[face].texture(point)] * p_renderer2->p_defaultTexture->getSize();
			}
			else {
				txtr = point2(0, 0);
			}
			vertexData[point].setTexture(txtr);
			vertexData[point].setNormal(m_vn[m_f[face].normal(point)]);
			vertexData[point].setCamDist(camDists[pointTarget]);
			vertexData[point].setLightDist(lightDists[pointTarget]);
		}
		p_renderer2->p_texture = p_textureMap;
		Uint32 color = NULL;

		p_renderer2->renderTriangle(vertexData[0], vertexData[1], vertexData[2], color, NULL, NULL);
	}
	delete[] screenPoints;
	delete[] shadowMapPoints;
	delete[] camDists;
	delete[] lightDists;
}


void Entity::move(point3 distance) {
	//m_delta += distance;
	m_center += distance;
	for (int i = 0; i < m_numVertices; i++) {
		m_v[i] += distance;
	}
}

void Entity::teleport(point3 location) {

	for (int i = 0; i < m_numVertices; i++) {
		m_v[i] -= m_center;
		m_v[i] += location;
	}
	m_center = location;
}

void Entity::scale(float amount) {
	m_scale *= amount;
	for (int i = 0; i < m_numVertices; i++) {
		m_v[i] -= m_center;
		m_v[i] *= amount;
		m_v[i] += m_center;
	}
}

void Entity::rotate(point3 amount) {
	m_rotate += amount;
	for (int i = 0; i < m_numVertices; i++) {
		m_v[i].rotate(amount.x(), amount.y(), amount.z(), m_center);
	}
	for (int i = 0; i < m_numNormals; i++) {
		m_vn[i].rotate(amount.x(), amount.y(), amount.z(), point3(0, 0, 0));
		m_vn[i] = unitVector(m_vn[i]);
	}
}

point3 Entity::getCenter() {
	return m_center;
}