#pragma once
#include "DataUtility.h"
#include "NewVertex.h"
#include "NewFacet.h"
#include <vector>
using std::vector;

class NewFacet;
class NewVertex;//不能相互包含的问题，所以需要这样做。

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

	vector<NewFacet*> facets;//假设一条边最多属于三个面。

	


};