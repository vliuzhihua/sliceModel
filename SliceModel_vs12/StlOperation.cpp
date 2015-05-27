#pragma warning(disable:4996)
#include <fstream>
#include<iostream>
#include "NewSolid.h"


bool loadStl(char path[], NewSolid &s){
	ifstream in;
	in.open(path, ios::in);
	const int SZ = 1024; 
	char buf[SZ];
	char *index;

	while (in.getline(buf, SZ)){
		index = buf;
		NewFacet* facet = new NewFacet();
		int ic;
		char* tmp;
		if ((tmp = strstr(index, "facet")) != NULL){
			ic = 0;

		}

		if ((tmp = strstr(index, "normal ")) != NULL){
			index = tmp;
			index += strlen("normal ");
			double nx, ny, nz;
			sscanf(index, "%lf%lf%lf", &nx, &ny, &nz);
			facet->normal = Tuple3d(nx, ny, nz);
		}
		if ((tmp = strstr(index, "vertex ")) != NULL){
			index = tmp;
			index += strlen("normal ");
			double x, y, z;
			sscanf(index, "%lf%lf%lf", &x, &y, &z);
			
			if (ic <= 2){
				facet->vtxs[ic] = new NewVertex(x, y, z);
				ic++;
			}
			
		}
		if ((tmp = strstr(index, "endfacet")) != NULL){
			s.facets.push_back(facet);
		}
	}

	//填充边的信息。因为之前只填充了点的信息。
	vector<NewFacet*> facets = s.facets;
	for(int i = 0; i < facets.size(); i++){
		NewEdge* ne = new NewEdge();
		//ne->facet
		//facets[i]->edge[0] = 
	}

	return true;
}