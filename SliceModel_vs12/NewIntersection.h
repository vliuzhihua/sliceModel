#pragma once
#include "DataUtility.h"
#include "NewVertex.h"
#include "NewFacet.h"
#include "NewEdge.h"


class NewIntersection{
public:
	Point3d pos;
	NewVertex* vtx;
	NewEdge* edge;

	vector<NewFacet*> facets;//假定最多4个，哈哈。
	
	int type;//标记这个交点的类型的。
	int visited;//记录是否访问过了
	static enum{VERTEX_TYPE, CROSS_TYPE};  


	//NewIntersection* next;
	//NewIntersection* pre;

	//NewIntersection* orderedNext;
	//NewIntersection* orderedPre;


	NewIntersection(){
		//重要
		type = NewIntersection::CROSS_TYPE;
		vtx = NULL;
		edge = NULL;
		//next = this;
		//pre = this;
		//orderedNext = this;
		//orderedPre = this;

		//visited 这个值非常的重要，用来标记这个intersection是否
		visited = 0;
	}


};