#include "DataUtility.h"
#include "NewSolid.h"
#include "SliceModel.h"
#include "NewIntersection.h"
#include "LayerOperator.h"
#include "ObjOperation.h"
#include "StateRecover.h"
#include <string>
#include <time.h>
#include "toolskit.h"
using std::string;

class Manager{
public:
	vector<Layer> reLines;
	vector<NewPlane> npVec;
	
	int szOfFacets;
	double* faces;
	string path;
	string logInfoPath;

	static enum{SCAN_MODEL, REPLAY_MODEL};
	int curIdx;
	int curModel;//当前使用的模式.
	
	vector<StateRecover> srVec;

	Manager(string path){
		this->path = path;
		faces == NULL;

		time_t t = time(0); 
		tm* tmtm = localtime(&t);
		char str[200];

		sprintf(str, "record-%04d%02d%02d-%02d-%02d-%02d.txt", tmtm->tm_year + 1900, tmtm->tm_mon + 1, tmtm->tm_mday, tmtm->tm_hour , tmtm->tm_min, tmtm->tm_sec);
				
		logInfoPath = str; 
	}


	

	void CheckIntersectionAllDegree(){
		//这里可以有两种方式进行旋转求交。
		//第一种是使用旋转模型的进行旋转后求交。
		double degree = 0;
		NewSolid s;

		loadObj(path.c_str(), s, 0);
		CalNewPlaneArray(s);


	


		for(; degree <= 10; degree += 0.2){
			cout << "degree: " << degree << endl;
			LoadAndCal(degree, false);
		}
		//另一种是使用旋转切平面的方式进行求交。
	}




	//计算所有的新的切面。
	void CalNewPlaneArray(NewSolid &sss, double a = 1, double b = 1, double c = 0){
		NewPlane np;
		Tuple3d t3d(a, b, c);
		np.abc = t3d;
		np.D = 0;
		
		Box box = sss.GetBox();
		double dmin, dmax;
		CalBoxD(box, t3d, dmin, dmax);

		int cc = 10;
		double step = (dmax - dmin) / cc;

		reLines.clear();
		npVec.clear();

		/*
		for(double dd = box.s.m_data[1]; dd <= box.b.m_data[1]; dd += step){
			np.D = dd;
			npVec.push_back(np);
		}
		*/

		//static double degree = -10;
	
		//degree += 10;
		//double radianDeg = degree / 180 * 3.1415926;
		for(double d = dmin; d <= dmax; d += step){
			//double rot[] = {cos(radianDeg), 0, -sin(radianDeg), 0, 1, 0, sin(radianDeg), 0, cos(radianDeg)};
			//Matrix<3, 3, double> rotMat(rot);
			//Tuple3d ttt = rotMat * t3d;
			np.D = d;
			//np.abc = ttt;
			npVec.push_back(np);
		}
	}

	//显示所有的layer
	void DrawAllSliceLine(bool solidShowOrNot = true){
	
		//光照相关
		/*glShadeModel(GL_SMOOTH);
		GLfloat gAmbient[] = {0.6, 0,6, 0,6, 1.0};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gAmbient);
		GLfloat lightAmbient[] = {1.0, 1.0, 1.0, 1.0};
		GLfloat lightDiffuse[]   = {1.0, 1.0, 1.0, 1.0};
		GLfloat lightSpecular[] = {0.5, 0.5, 0.5, 1.0};
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);*/

		if(solidShowOrNot){
			ToolsKit::DrawSolid(faces, szOfFacets);
		}

		for(int j = 0; j < reLines.size(); j++){
			for(int k = 0; k < reLines[j].sz; k++){
				glPointSize(3.0);
				glColor3i(100, 0, 0);
		
				reLines[j].loops[k].Draw();
			}
		}	
	}

	

	//void LoadAndCal(double degree, bool loadFacetOrNot = true){
	void LoadAndCal(double degree, bool loadFacetOrNot = true){
		
		NewSolid s;//有度数的旋转.
		loadObj(path.c_str(), s, degree);

		if(loadFacetOrNot){
			ToolsKit::transforNewSolid(s, faces, szOfFacets);
		}

		//计算切割线。

		reLines.clear();
		//CalNewPlaneArray(s);

		for(int i = 0; i < npVec.size(); i++){
		
			Layer cp = ToolsKit::CalLayer(s, npVec[i]);
			reLines.push_back(cp);
			//检查是否是切割后的这一层是否没有自交现象。
			Point3d rePos[4];
			int a, b;
			if(cp.IsIntersect(rePos, a, b) == true){

				StateRecover sr(npVec[i], StateRecover::ROTATE_MODEL, degree, this->path, rePos, a, b);

				ofstream out(logInfoPath, ios::app);
				out << sr;
				out.close();

				srVec.push_back(sr);

				
				cout << "plane is: " << npVec[i].abc.m_data[0] << " " << 
					npVec[i].abc.m_data[1] << " " << npVec[i].abc.m_data[2] << " " << npVec[i].D << endl;
				cout << "###self intersection happend !" << endl;
				
			}else{

				//无关紧要的无用信息不输出了
				//cout << "###no self intersection !" <<endl;
			}
			cout << "layer sz: " << cp.sz << endl;
		}
	}
private:
	
	

	void CalBoxD(Box box, Tuple3d normal, double &dmin, double &dmax){
		//总计8种情况,都统计一下.
		double tmp = box.b.dot(normal);
		dmin = tmp;
		dmax = dmin;

		tmp = Tuple3d(box.b.m_data[0], box.b.m_data[1], box.s.m_data[2]).dot(normal);
		dmin = (dmin > tmp ? tmp : dmin);
		dmax = (dmax > tmp ? dmax : tmp);

		tmp = Tuple3d(box.b.m_data[0], box.s.m_data[1], box.b.m_data[2]).dot(normal);
		dmin = (dmin > tmp ? tmp : dmin);
		dmax = (dmax > tmp ? dmax : tmp);

		tmp = Tuple3d(box.b.m_data[0], box.s.m_data[1], box.s.m_data[2]).dot(normal);
		dmin = (dmin > tmp ? tmp : dmin);
		dmax = (dmax > tmp ? dmax : tmp);

		tmp = Tuple3d(box.s.m_data[0], box.b.m_data[1], box.b.m_data[2]).dot(normal);
		dmin = (dmin > tmp ? tmp : dmin);
		dmax = (dmax > tmp ? dmax : tmp);

		tmp = Tuple3d(box.s.m_data[0], box.b.m_data[1], box.s.m_data[2]).dot(normal);
		dmin = (dmin > tmp ? tmp : dmin);
		dmax = (dmax > tmp ? dmax : tmp);

		tmp = Tuple3d(box.s.m_data[0], box.s.m_data[1], box.b.m_data[2]).dot(normal);
		dmin = (dmin > tmp ? tmp : dmin);
		dmax = (dmax > tmp ? dmax : tmp);

		tmp = Tuple3d(box.s.m_data[0], box.s.m_data[1], box.s.m_data[2]).dot(normal);
		dmin = (dmin > tmp ? tmp : dmin);
		dmax = (dmax > tmp ? dmax : tmp);

	}



};