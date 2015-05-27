#pragma once
#include "DataUtility.h"
#include "NewVertex.h"
#include "NewFacet.h"
#include <vector>

class Box{
public:
	Point3d s, b;


	//这个判断有问题。
	bool IsIntersect(Box box){
		Box tmpBox = *this + box;
		for(int i = 0; i < 3; i++){
			if(tmpBox.GetLength(i) <= (this->GetLength(i) + box.GetLength(i))){
				return true;
			}
		}
		return false;
		
	}

	inline double GetLength(int i){
		if(i >= 0 && i <= 2){
			return b.m_data[i] - s.m_data[i];
		}else{
			return DBL_MAX;
		}
		
	}
	Box operator + (const Box &box){
		Box reBox;

		for(int j = 0; j < 3; j++){
			if(box.s.m_data[j] > s.m_data[j]){
				reBox.s.m_data[j] = s.m_data[j];
			}else{
				reBox.s.m_data[j] = box.s.m_data[j];
			}
			if(box.b.m_data[j] < b.m_data[j]){
				reBox.b.m_data[j] = b.m_data[j];
			}else{
				reBox.b.m_data[j] = box.b.m_data[j];
			}
		}
		return reBox;
	}

	Box operator += (const Box &box){
		*this = *this + box;
		return *this;
	}

	friend ostream& operator << (ostream& os, const Box& b){
		os << b.s.m_data[0] << " " <<  b.s.m_data[1] << " " << b.s.m_data[2] << endl;

		os << b.b.m_data[0] << " " <<  b.b.m_data[1] << " " << b.b.m_data[2] << endl;

		return os;
	}
};

class NewSolid{
public:
	vector<NewFacet*> facets;

	vector<NewVertex*> vtxs;
	vector<NewEdge*> edges;
	
	NewSolid(){
		
	}
	~NewSolid(){
		int sz = facets.size();
		for(int i = 0; i < sz; i++){
			delete facets[i];
		}

		sz = vtxs.size();
		for(int i = 0; i < sz; i++){
			delete vtxs[i];
		}

		sz = edges.size();
		for(int i = 0; i < sz; i++){
			delete edges[i];
		}

	}
	Box GetBox(){
		Box box;
		vector<NewVertex*> nvVec = this->vtxs;
		if(nvVec.size() >= 1){
			box.s = Point3d(nvVec.at(0)->pos);
			box.b = Point3d(nvVec.at(0)->pos);
		}

		for(int i = 0; i < nvVec.size(); i++){
			for(int j = 0; j < 3; j++){
				if(box.s.m_data[j] > nvVec.at(i)->pos.m_data[j]){
					box.s.m_data[j] = nvVec.at(i)->pos.m_data[j];
				}
				if(box.b.m_data[j] < nvVec.at(i)->pos.m_data[j]){
					box.b.m_data[j] = nvVec.at(i)->pos.m_data[j];
				}
			}
		}
		return box;
	}
	
};