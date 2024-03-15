#include "plant.h"

Plant::Plant(Renderer* renderer) {
	p_renderer = renderer;
	m_numBranches = -1;
	m_branchSplits = 2;
	Branch baseBranch;
	baseBranch.defineBranch(point3(0, 0, 0), point3(0, 0, 200), 20);
	m_branches.push_back(baseBranch);
}
Plant::~Plant() {

}

void Plant::render() {
	Screen* screen = p_renderer->getScreen();
	for (int i = 0; i < m_branches.size(); i++) {
		point2 base = p_renderer->project(m_branches[i].base);
		point2 tip = p_renderer->project(m_branches[i].tip);

		point2 adj = unitVector(tip - base) * (m_branches[m_branches[i].parentIndex].width) * p_renderer->getCamera()->m_zoom;
		adj.rotate(float(M_PI / 2), point2(0, 0));

		TrianglePointData top, bottom, A, B, C, D;
		point3 base3 = m_branches[i].base, tip3 = m_branches[i].tip;
		top.screen = tip;
		top.camDist = getLineDist3d(tip3, p_renderer->getCamera()->m_map);
		top.normal = point3(0, 0, 1);
		A = top;
		A.screen -= adj;
		B = top;
		B.screen += adj;
		bottom.screen = base;
		bottom.camDist = getLineDist3d(base3, p_renderer->getCamera()->m_map);
		bottom.normal = point3(0, 0, 1);
		C = bottom;
		C.screen -= adj;
		D = bottom;
		D.screen += adj;

		p_renderer->renderTriangle(A, B, C, false);
		p_renderer->renderTriangle(B, D, C, false);
	}
}
void Plant::grow() {
}
void Plant::addBranch() {
	Branch newBranch;
	m_numBranches++;
	newBranch.parentIndex = m_numBranches / m_branchSplits;
	Branch parentBranch = m_branches[newBranch.parentIndex];
	
	newBranch.base = parentBranch.tip;
	//newBranch.direction = parentBranch.direction;
	//newBranch.direction.rotate(M_PI/4,0, 0, point3(0,0,0));
	float x = float(rand() % 100 - 50) / 50;
	float y = float(rand() % 100 - 50) / 50;
	float z = float(rand() % 100) / 100 + 1;
	vec3 direction = point3(x, y, z);
	//std::cout << direction << "\n";
	direction = unitVector(direction);
	//direction.rotate(x, y, z, point3(0, 0, 0));
	newBranch.direction = direction;

	newBranch.width = parentBranch.width * 0.7f;
	newBranch.tip = newBranch.base + newBranch.direction * 0.9f * parentBranch.length;
	newBranch.length = getLineDist3d(newBranch.base, newBranch.tip);
	m_branches.push_back(newBranch);
}