#pragma once
#include "DataUtility.h"
#include "NewVertex.h"
#include "NewFacet.h"
#include <vector>
using std::vector;

class NewFacet;
class NewVertex;//�����໥���������⣬������Ҫ��������

class NewEdge{

public:
	NewEdge(){
		vtxs[0] = NULL;
		vtxs[1] = NULL;
	}

	union{
		struct {
			NewVertex *vtxs[2];
		};
		struct {
			NewVertex *a, *b;
		};
	};

	vector<NewFacet*> facets;//����һ����������������档

	


};