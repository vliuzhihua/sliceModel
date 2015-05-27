#pragma warning(disable:4996)
#include <fstream>
#include<iostream>
#include "NewSolid.h"

using namespace std;


typedef Tuple<3, int> Index3d;

vector<Point3d> vtxs;
vector<Index3d> indexs;

//该函数之前生成的off文件有错误，因为off文件点的范围必须在(-1,1)之间。
bool Obj2Off(char objPath[], char offPath[]){

    ifstream in(objPath, ios::in);
	ofstream out(offPath, ios::out | ios::trunc);
	const int SZ = 1024; 
	char buf[SZ];
	char *index;

	double max;

	//开始扫描输入的obj文件。
	while (in.getline(buf, SZ)){
		index = buf;
		NewFacet facet;
		int ic;
		char* tmp;

		if ((tmp = strstr(index, "v ")) != NULL){
			index = strstr(index, "v ");
			index += strlen("v ");
			double nx, ny, nz;
			sscanf(index, "%lf %lf %lf", &nx, &ny, &nz);

			static bool first = true;
			if(first){
				max = abs(nx);
				first = false;
				
			}else{
				if(abs(nx) > max){
					max = abs(nx);
				}
			}
			if(abs(ny) > max){
					max = abs(ny);
				}
				if(abs(nz) > max){
					max = abs(nz);
				}
			


			vtxs.push_back(Point3d(nx, ny, nz));
		}

		if ((tmp = strstr(index, "f ")) != NULL){
			index = strstr(index, "f ");
			index += strlen("f ");
			int idx1, idx2, idx3;
			sscanf(index, "%d %d %d", &idx1, &idx2, &idx3);

			idx1 -= 1;
			idx2 -= 1;
			idx3 -= 1;

			indexs.push_back(Index3d(idx1, idx2, idx3));
		}
	}
	//写入off文件。

	cout << "max: " << max << endl;
	if (out.is_open()) {
      

       out << "OFF" << endl;
	   out << vtxs.size() << " " << indexs.size() << " " << 0 << endl;
	   int vtxsz = vtxs.size();
	   for(int i = 0; i < vtxsz; i++){
		   out << vtxs.at(i).m_data[0] / max << " " << vtxs.at(i).m_data[1] / max << " " << vtxs.at(i).m_data[2] / max << endl;
	   }

	   int indexsz = indexs.size();
	   for(int i = 0; i < indexsz; i++){
		   out << "3 " << indexs.at(i).m_data[0] << " " << indexs.at(i).m_data[1] << " " << indexs.at(i).m_data[2] << endl;
	   }

       out.close();
     }

	return true;
}