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

	vector<NewFacet*> facets;//�ٶ����4����������
	
	int type;//��������������͵ġ�
	int visited;//��¼�Ƿ���ʹ���
	static enum{VERTEX_TYPE, CROSS_TYPE};  


	//NewIntersection* next;
	//NewIntersection* pre;

	//NewIntersection* orderedNext;
	//NewIntersection* orderedPre;


	NewIntersection(){
		//��Ҫ
		type = NewIntersection::CROSS_TYPE;
		vtx = NULL;
		edge = NULL;
		//next = this;
		//pre = this;
		//orderedNext = this;
		//orderedPre = this;

		//visited ���ֵ�ǳ�����Ҫ������������intersection�Ƿ�
		visited = 0;
	}


};