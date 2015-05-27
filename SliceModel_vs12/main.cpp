#include "GL/glew.h"
#include "GL/glut.h"
#include <iostream>
#include "SceneDisplay.h"
#include "StlOperation.h"
#include "ObjOperation.h"
#include "OffOperation.h"
#include "AboutSlice.h"


#include "NewSolid.h"
#include "SliceModel.h"
#include <map>
#include "LayerOperator.h"
#include "manager.h"
#include "replayer.h"
using namespace std;


#define MAX4(a, b, c, d) ((a > b ? a : b) > (c > d ? c : d) ? (a > b ? a : b) : (c > d ? c : d))

static double degree = 0;
//char objFilePath[] = "D:/Documents/Downloads/3D tm Print/ModelsRepo/bunny.obj";
char objFilePath[] = "D:/Documents/Downloads/3D tm Print/ModelsRepo/HeadRes.obj";
//char objFilePath[] = "D:/Documents/Downloads/3D tm Print/ModelsRepo/222.obj";


SceneDisplay sceneDis(100, 100, 900, 900);
Manager manager(objFilePath);
Replayer replayer;
//Replayer replayer("record-20150527-10-07-44.txt");

#define ALL_MODE 1
#define SLICE_MODE 2

static int replay = 0;

int iii = 0;
int loopIdx = 0;

void AllDisplay(){

	//manager.DrawSolid();
	//manager.DrawAllSliceLine(true);
	replayer.ShowState(iii, loopIdx, true);
	
}

//扫成操作示例的使用处。
void SliceDisplay(){ 
	//manager.DrawAllSliceLine(false);
	replayer.ShowState(iii, loopIdx, false);
}

void myKey(unsigned char key, int x, int y){
	switch (key){
		case 'a':
			degree += 10;
			//manager.LoadAndCal(degree);
			//CalNewPlaneArray();
			glutPostRedisplay();
			break;
		case 'd':
			iii--;
			if(iii < 0){
				iii = 0;
			}
			glutPostRedisplay();
			break;

		case 'f':
			iii++;
			if(iii >= replayer.size()){
				iii = replayer.size() - 1;
			}
			glutPostRedisplay();
			break;
		case 'c':
			loopIdx--;
			if(loopIdx < 0){
				loopIdx = 0;
			}
			glutPostRedisplay();
			break;

		case 'v':
			loopIdx++;
			
			glutPostRedisplay();
			break;

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

#include "imath/imath.h"

//喜闻乐见的main函数。
int main(int argc,char *argv[]){


	/*Layer testS;
	testS.ss = new Loop[2];

	testS.sz = 2;

	testS.ss[0].init(4);
	testS.ss[0].data[0] = Point3d(1, 0, 0);
	testS.ss[0].data[1] = Point3d(1, 1, 0);
	testS.ss[0].data[2] = Point3d(0, 1, 0);
	testS.ss[0].data[3] = Point3d(0, 0, 0);

	testS.ss[1].init(4);
	testS.ss[1].data[0] = Point3d(-1, 0, 0);
	testS.ss[1].data[1] = Point3d(-1, -1, 0);
	testS.ss[1].data[2] = Point3d(0, -1, 0);
	testS.ss[1].data[3] = Point3d(-0.5, -0.5, 0);

	if(testS.IsIntersect() == true){
		cout << "test intersection function:" << "true" << endl;
	}else{
		cout << "test intersection function:" << "false" << endl;
	}*/
	
	//replay = argv[1][0] - '0';


	int fillMenu = glutCreateMenu(processMenu);
	glutAddMenuEntry("显示整个模型", ALL_MODE);
	glutAddMenuEntry("只显示切割线", SLICE_MODE);
	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	NewSolid ss;
	loadObj(objFilePath, ss, 0);
	

	Box box = ss.GetBox();
	//manager.LoadAndCal(0);


	manager.CheckIntersectionAllDegree();//进行本次程序运行的错误检测。
	replayer.LoadFrom(manager);//显示本次测试的出现错误的部分。
	
	//sceneDis.setXYZSpan(box.s.m_data[0] * 3, box.b.m_data[0] * 3, box.s.m_data[1] * 3, box.b.m_data[1] * 3, 0.1, 3000);
	
	double absMax = MAX4(abs(box.s.m_data[0]), abs(box.b.m_data[0]), abs(box.s.m_data[1]), abs(box.b.m_data[1]));


	sceneDis.setXYZSpan(-absMax, absMax, -absMax, absMax, -30000, 30000);
	sceneDis.setEyePos(0, 0, 20);
	//sceneDis.setWindowsPos(100, 100, 900, 900);

	sceneDis.setUsrDisplayFunc(AllDisplay);
	//开启使用shader的功能
	sceneDis.setShaderState(1);
	sceneDis.setUsrKeyboardFunc(myKey);
	//sceneDis.startTimer();
	sceneDis.start();
}