#pragma once
#include "GL/glew.h"
#include "GL/glut.h"
#include <math.h>
//SceneDisplay version implement by lzh 
//email: vliuzhihua@gmail.com
class SceneDisplay{
public:
	SceneDisplay();
	SceneDisplay(void (*disFunc)());
	SceneDisplay(int leftx, int topy, int width, int height);
	static GLfloat rad;//the distance from eye to (0, 0, 0)  
	void start();
	//start timer will call redisplay every 200 ms;
	void startTimer();
	static void setXYZSpan(GLfloat xStart, GLfloat xEnd, GLfloat yStart, GLfloat yEnd, GLfloat zStart, GLfloat zEnd);
	static void setUsrMouseClickFunc(void(*p)(int, int, int, int));
	static void setUsrKeyboardFunc(void (*p)(unsigned char, int, int));
	static void setUsrDisplayFunc(void (*p)());
	static void setViewRotate(int state);
	//set the eye pos, you need to change eye position when you use shader.
	//default value is 0, 0, 1
	static void setEyePos(double x, double y, double z);
	//set the windows position.
	//default value is 100, 100, 900, 900
	static void setWindowsPos(int leftx, int topy, int width, int height);
	//set the shader function enable or not.
	static void setShaderState(int state);

	static void update();

private:
	class Tuple{
	public:
		double pos[3];

		Tuple(){
			pos[0] = 0;
			pos[1] = 0;
			pos[2] = 0;
		}
		Tuple(double x, double y, double z){
			pos[0] = x;
			pos[1] = y;
			pos[2] = z;
		}
		Tuple(double p[]){
			pos[0] = p[0];
			pos[1] = p[1];
			pos[2] = p[2];
		}
		double length(){
			return sqrt(pos[0] * pos[0] + pos[1] * pos[1] + pos[2] * pos[2]);
		}

		Tuple cross(Tuple p){
			return Tuple(pos[1] * p.pos[2] - pos[2] * p.pos[1], 
						pos[2] * p.pos[0] - pos[0] * p.pos[2], 
						pos[0] * p.pos[1] - pos[1] * p.pos[0]);
		}

		friend Tuple operator + (Tuple a, Tuple b){
			return Tuple(a.pos[0] + b.pos[0], a.pos[1] + b.pos[1], a.pos[2] + b.pos[2]);
		}
		friend Tuple operator - (Tuple a, Tuple b){
			return Tuple(a.pos[0] - b.pos[0], a.pos[1] - b.pos[1], a.pos[2] - b.pos[2]);
		}
		friend Tuple operator * (double a, Tuple b){
			return Tuple(a * b.pos[0], a * b.pos[1], a * b.pos[2]);
		}
		friend Tuple operator * (Tuple a, double b){
			return Tuple(a.pos[0] * b, a.pos[1] * b, a.pos[2] * b);
		}
		friend double operator * (Tuple a, Tuple b){
			return a.pos[0] * b.pos[0] + a.pos[1] * b.pos[1] + a.pos[2] * b.pos[2];
		}
	};
	typedef Tuple Vector;
	typedef Tuple Point;

	static GLint mouseXOld, mouseYOld;//̽�������ƶ�λ�ñ仯��
	static GLfloat eyeX, eyeY, eyeZ;//�۾�����λ��
	static GLfloat upX, upY, upZ;//�۾��Ϸ��������
	static GLfloat bertaDegree;//ˮƽ��y��ļн�
	static GLfloat alphaDegree;//�۾�λ�õ�(0, 0, 0)��������XoZƽ����ͶӰ��Z������ļнǡ�
	static GLint winX, winY, posX, posY;
	static GLfloat xStart, xEnd, yStart, yEnd, zStart, zEnd;
	static GLint viewRotateOnOrOff;
	static GLint shaderState;
	//shaderҪʹ�õļ���������
	static GLhandleARB vertexShader;
	static GLhandleARB fragShader;
	static GLhandleARB shaderPro;

	//the user display function, will been invoked by draw funciton.
	static void (*usrDisplay)();
	static void (*usrKeyboardDownFunc)(unsigned char key, int x, int y);
	static void (*usrMouseClickFunc)(int button, int state, int x, int y);
	//use to calculate the new tangent direct and the point position, d[] and p[] will update after the function.
	//�˴��м�������һ�����Ͻ��е������ƶ���
	static void calNewDirAndPosInSphere(Vector &d, Point &p, GLfloat a);
	static void calculateXYZbyDegree(GLfloat, GLfloat, GLfloat, GLfloat*, GLfloat*, GLfloat*);
	static void mouseClickFunc(int button, int state, int x, int y);
	static void mouseMoveFunc(int x, int y);
	static void keyboardDownFunc(unsigned char key, int x, int y);
	static void init();
	static void draw();
	static void resharp(int, int);
	static void onTimer(int);
	
	

	
};
