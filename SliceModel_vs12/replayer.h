#include <iostream>
#include "StateRecover.h"
#include <vector>
#include "LayerOperator.h"
#include "NewSolid.h"
#include "ObjOperation.h"
#include "toolskit.h"
#include <string>
using std::vector;


class Replayer{
private:
	vector<StateRecover> srVec;
	int curShowState;

	int szOfFacets;
	double* faces;
	Layer curLayer;

public:
	void LoadFrom(Manager manager){
		srVec = manager.srVec;
	}
	Replayer(){
		faces == NULL;
		curShowState = -1;
	}
	Replayer(string path){
		curShowState = -1;
		faces == NULL;
		LoadStateRecover(path);
	}
	void LoadStateRecover(string path){
		ifstream in(path);
		StateRecover sr;
		while(in >> sr){
			srVec.push_back(sr);	
		}

		in.close();
	}

	void ShowState(int idx, int loopIdx, bool solidShowOrNot = true){

		if(idx < 0 || idx >= srVec.size()){
			return ;
		}

		


		NewSolid s;
		

		if(idx != curShowState){

			

			loadObj(srVec[idx].objFilePath.c_str(), s, srVec[idx].degree);

			curLayer = ToolsKit::CalLayer(s, srVec[idx].np);
			ToolsKit::transforNewSolid(s, faces, szOfFacets);


			//显示本次展示的信息
			srVec[idx].displayInfo();
			cout << "---------------" << endl;
			cout << curLayer.sz << endl;
			
			
			for(int i = 0; i < curLayer.sz; i++){
				cout << i << " :" << curLayer.loops[i].sz << endl;
			}
			cout << "---------------" << endl;


			cout << "calculate again:" << endl;
			Point3d rp[4];
			int a, b;

			if(curLayer.IsIntersect(rp, a, b)){
				cout << "true" << endl;
				cout << a << endl;
				cout << b << endl;

				glPointSize(6.0);
				glBegin(GL_POINTS);

				for(int i = 0; i < 4; i++){
					glVertex3d(rp[i].m_data[0], rp[i].m_data[1], rp[i].m_data[2]);
				}

				glEnd();

				//绘制线条。
				glBegin(GL_LINES);

				for(int i = 0; i < 4; i++){
					glVertex3d(rp[i].m_data[0], rp[i].m_data[1], rp[i].m_data[2]);
				}

				glEnd();
				
			}else{
				
			}
			
			

			
			curShowState = idx;


		}

		if(solidShowOrNot){
			ToolsKit::DrawSolid(faces, szOfFacets);
		}
	
		cout << "loopIdx : " << loopIdx << endl;
		if(loopIdx >= 0 && loopIdx < curLayer.sz){
			curLayer.Draw(loopIdx);
		}
		
		
		//srVec[idx].ShowError();


	}
	int size(){
		return srVec.size();
	}
	
};