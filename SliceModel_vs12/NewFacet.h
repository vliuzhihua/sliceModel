#pragma once

#include "DataUtility.h"
#include "NewEdge.h"
#include <vector>
using std::vector;

class NewIntersection;

class NewFacet{
public:
	
	//������бߵļ��ϡ�
	vector<NewEdge*> edges;
	//������еĶ��㼯�ϡ�
	vector<NewVertex*> vtxs;
	//������еĽ��㼯��
	vector<NewIntersection*> intsects;
	//�����ķ�������
	Tuple3d normal;

	int visited;

	NewFacet(){
		visited = 0;
	}
	

};