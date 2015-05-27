#pragma warning(disable:4996)
#include <fstream>
#include<iostream>
#include "ObjOperation.h"
#include <map>
#include<string>
using namespace std;

//degree是一个旋转角度。
void loadObj(const char path[], NewSolid &s, double degree){
	//初始化ect
	//mapNE.find(
	//mapNE.insert(pair<long, NewEdge*>());
	ifstream in;
	in.open(path, ios::in);
	const int SZ = 1024; 
	char buf[SZ];
	char *index;

	//根据degree计算出绕某轴的旋转矩阵。
	double radianDeg = degree / 180 * 3.1415926;
	double rot[] = {cos(radianDeg), 0, -sin(radianDeg), 0, 1, 0, sin(radianDeg), 0, cos(radianDeg)};
	Matrix<3, 3, double> rotMat(rot);


	std::map<std::string, NewEdge*> mapNE;
	//AcEdge* ect = NULL;//edge cull table

	
	vector<NewVertex*> vtxAddrs;
	vector<NewEdge*> edgeAddrs;
	//int vtxCount = 0;

	while (in.getline(buf, SZ)){
		index = buf;
	

		if (index[0] == 'v'){
			index = strstr(index, "v ");
			index += strlen("v ");
			double nx, ny, nz;
			sscanf(index, "%lf %lf %lf", &nx, &ny, &nz);

			Tuple3d p = rotMat * Tuple3d(nx, ny, nz);

			//NewVertex* nv = new NewVertex(nx, ny, nz);
			NewVertex* nv = new NewVertex(p.m_data[0], p.m_data[1], p.m_data[2]);

			vtxAddrs.push_back(nv);
		}

		if (index[0] == 'f'){
			index = strstr(index, "f ");
			index += strlen("f ");
			int idx[3];
			sscanf(index, "%d %d %d", &idx[0], &idx[1], &idx[2]);

			for(int i = 0; i < 3; i++){
				idx[i]--;
			}


			//填充NewFacet的内部参数。
			//这一部分之前用到的ect会导致使用大的模型的时候会发生出错。即使用数组构建的图不够用了。
			//修改后如下:
			NewFacet* nf = new NewFacet();

			for(int i = 0; i < 3; i++){

				nf->vtxs.push_back(vtxAddrs[idx[i]]);

				char num[30];
				int max = idx[i] > idx[(i + 1) % 3] ? idx[i] : idx[(i + 1) % 3];
				int min = idx[i] < idx[(i + 1) % 3] ? idx[i] : idx[(i + 1) % 3];
				sprintf_s(num, "%d %d", max, min);
				string markStr = num;
				
				
				
				map<string, NewEdge*>::iterator iter;

				iter = mapNE.find(markStr);
				if(iter != mapNE.end())
				{
					nf->edges.push_back(iter->second);

					//添加
					iter->second->facets.push_back(nf);
					
				}
				else
				{
					NewEdge* ne = new NewEdge();

					edgeAddrs.push_back(ne);

					ne->facets.push_back(nf);
					ne->vtxs[0] = (NewVertex*)vtxAddrs[idx[i]];
					ne->vtxs[1] = (NewVertex*)vtxAddrs[idx[(i + 1) % 3]];
					
					mapNE.insert(std::pair<string, NewEdge*>(markStr, ne));
					nf->edges.push_back(ne);

				}
			}

			//计算法向量。
			Tuple3d v1 = vtxAddrs.at(idx[0])->pos - vtxAddrs.at(idx[1])->pos;
			Tuple3d v2 = vtxAddrs.at(idx[1])->pos - vtxAddrs.at(idx[2])->pos;
			Tuple3d normal = v1.cross(v2);
			nf->normal = normal.normalize();


			s.facets.push_back(nf);

		}
	}
	s.edges = edgeAddrs;
	s.vtxs = vtxAddrs;
}
