#pragma once
#include <vector>
#include "vec3.h"
#include "line.h"
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 

class Plant {
private:
	struct Branch {
		int generation; // The number of branches from the ground this branch is.
		int parentIndex = 0; // Used as the base of the branch.
		vec3 direction;
		float length = 0;
		point3 base;
		point3 tip;
		float width = 0;

		Branch() {};
		void defineBranch(point3 base, point3 tip, float width) {
			this->base = base;
			this->tip = tip;
			point3 branch = tip - base;
			length = getLineDist3d(base, tip);
			direction = unitVector(branch);
			this->width = width;
		}
	};

	std::vector<Branch> m_branches;
	Renderer* p_renderer;
	int m_branchSplits;
	int m_numBranches;

public:
	Plant(Renderer* renderer);
	~Plant();

	void render();
	void teleport(point3 location) {
		point3 adj = location - m_branches[0].base;
		for (int i = 0; i < m_branches.size(); i++) {
			m_branches[i].base += location;
			m_branches[i].tip += location;
		}
	}
	void grow();
	void addBranch();
};

