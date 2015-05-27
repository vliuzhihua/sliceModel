#pragma once

#include "DataUtility.h"
#include "NewEdge.h"
#include <vector>
using std::vector;

class NewIntersection;

class NewFacet{
public:
	
	//这个面中边的集合。
	vector<NewEdge*> edges;
	//这个面中的顶点集合。
	vector<NewVertex*> vtxs;
	//这个面中的交点集合
	vector<NewIntersection*> intsects;
	//这个面的法向量。
	Tuple3d normal;

	int visited;

	NewFacet(){
		visited = 0;
	}
	

};