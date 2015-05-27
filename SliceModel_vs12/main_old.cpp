#include "GL/glew.h"
#include "GL/glut.h"
#include <iostream>
#include "SceneDisplay.h"
#include "StlOperation.h"
#include "ObjOperation.h"
#include "OffOperation.h"
#include "AboutSlice.h"

#include "Solid.h"
using namespace std;



SceneDisplay sceneDis(100, 100, 900, 900);

#define ALL_MODE 1
#define SLICE_MODE 2

Solid s;

const int szOfFaces = 500000;
double faces[szOfFaces][12];

const int szOfEdges = 500000;
double edges[szOfEdges][6];
double tmpEdges[200000][6];
int ssss = 0;


//倒角操作示例的使用处。
void AllDisplay(){

	static bool first = true;
	if(first){
		int sz = s.facets.size();
		if(sz > szOfFaces){
			cout << sz << endl;
			cout << "number of face is too many" << endl;
			return ;
		}else{
			
			//简单转换便于快速显示。
			vector<Facet> tv = s.facets;
			for (int i = 0; i < sz; i++){
				faces[i][0] = tv.at(i).point[0].m_data[0];
				faces[i][1] = tv.at(i).point[0].m_data[1];
				faces[i][2] = tv.at(i).point[0].m_data[2];
				faces[i][3] = tv.at(i).point[1].m_data[0];
				faces[i][4] = tv.at(i).point[1].m_data[1];
				faces[i][5] = tv.at(i).point[1].m_data[2];
				faces[i][6] = tv.at(i).point[2].m_data[0];
				faces[i][7] = tv.at(i).point[2].m_data[1];
				faces[i][8] = tv.at(i).point[2].m_data[2];
				faces[i][9] = tv.at(i).normal.m_data[0];
				faces[i][10] = tv.at(i).normal.m_data[1];
				faces[i][11] = tv.at(i).normal.m_data[2];

				first = true;
			}
		}
	}


	glBegin(GL_TRIANGLES);

	int sz = s.facets.size();
	for (int i = 0; i < sz; i++){
		glNormal3d(faces[i][9], faces[i][10], faces[i][11]);
		glVertex3d(faces[i][0], faces[i][1], faces[i][2]);
		
		glNormal3d(faces[i][9], faces[i][10], faces[i][11]);
		glVertex3d(faces[i][3], faces[i][4], faces[i][5]);
		
		glNormal3d(faces[i][9], faces[i][10], faces[i][11]);
		glVertex3d(faces[i][6], faces[i][7], faces[i][8]);
	}
	glEnd();
	static int szz = 0;
	static bool fff = true;
	if(fff){
		int tmp = 0;
		
		for(double k = -0.7; k <= 0.7; k += 10){
			slice(s, tmpEdges, tmp, k);
			
			for(int i = 0; i < tmp; i++){
				edges[szz][0] = tmpEdges[i][0];
				edges[szz][1] = tmpEdges[i][1];
				edges[szz][2] = tmpEdges[i][2];
				edges[szz][3] = tmpEdges[i][3];
				edges[szz][4] = tmpEdges[i][4];
				edges[szz][5] = tmpEdges[i][5];
				szz++;
			}
		}
		fff = false;
	}

	ssss = szz;

	glBegin(GL_LINES);
	for(int i = 0; i < ssss; i++){
		glVertex3d(edges[i][0], edges[i][1], edges[i][2]);

		glVertex3d(edges[i][3], edges[i][4], edges[i][5]);
	}
	glEnd();
}
//扫成操作示例的使用处。
void SliceDisplay(){




	glBegin(GL_LINES);
	for(int i = 0; i < ssss; i++){
		glVertex3d(edges[i][0], edges[i][1], edges[i][2]);

		glVertex3d(edges[i][3], edges[i][4], edges[i][5]);
	}
	glEnd();
}

void myKey(unsigned char key, int x, int y){
	switch (key){
	
	}
}
//对右键的菜单的处理函数。
void processMenu(int bt){
	switch (bt){
	case ALL_MODE:
		sceneDis.setUsrDisplayFunc(AllDisplay);
		sceneDis.update();
		printf("AllDisplay..................\n");
		break;
	case SLICE_MODE:
		sceneDis.setUsrDisplayFunc(SliceDisplay);
		sceneDis.update();
		printf("SliceDisplay..................\n");
		break;
	}
}
//喜闻乐见的main函数。
int main(){
	//loadStl("ttt.stl", s);
	//loadStl("yyy.stl", s);
	//loadStl("aa1.stl", s);
	//loadStl("robot.stl", s);
	//loadObj("headRes.obj", s);
	//loadObj("Buddha_error.obj", s);
	loadObj("teapot.obj", s);
	//Obj2Off("headRes.obj", "result.off");
	//Obj2Off("teapot.obj", "teapot.off");
	cout << "convert complete!" << endl;

	int fillMenu = glutCreateMenu(processMenu);
	glutAddMenuEntry("都显示", ALL_MODE);
	glutAddMenuEntry("只显示切割线", SLICE_MODE);
	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	//sceneDis.setXYZSpan(-200, 200, -200, 200, 0.1, 3000);
	sceneDis.setXYZSpan(-100, 100, -100, 100, 0.1, 3000);

	sceneDis.setEyePos(0, 0, 20);
	//sceneDis.setWindowsPos(100, 100, 900, 900);

	sceneDis.setUsrDisplayFunc(AllDisplay);
	//开启使用shader的功能
	sceneDis.setShaderState(1);
	//sceneDis.setUsrKeyboardFunc(myKey);
	//sceneDis.startTimer();
	sceneDis.start();
}