#pragma once
#include "DataUtility.h"
#include "NewPlane.h"
#include <iostream>
#include <string>
using std::string;
using namespace std;

class StateRecover{
public:
	NewPlane np;
	int rotateStyle;
	double degree;
	string objFilePath;
	Point3d errorPos[4];
	int a, b;
	static enum{ROTATE_MODEL, ROTATE_PLANE};
	StateRecover(){
		
	}
	StateRecover(NewPlane np, int rotateStyle, double degree, string objFilePath, Point3d rePos[], int a, int b){
		this->np = np;
		this->rotateStyle = rotateStyle;
		this->degree = degree;
		this->objFilePath = objFilePath;
		for(int i = 0; i < 4; i++){
			this->errorPos[i] = rePos[i];
		}
		this->a = a;
		this->b = b;
	}

	friend ostream& operator << (ostream& out, StateRecover &sr){
		out << sr.np << endl;
		out << sr.rotateStyle << endl;
		out << sr.degree << endl;

		for(int i = 0; i < 4; i++){
			out << sr.errorPos[i] << endl;
		}
		out << sr.a << endl;
		out << sr.b << endl;
		out << sr.objFilePath << endl;
		
		return out;
	}
	friend istream& operator >> (istream& in, StateRecover &sr){
		in >> sr.np;
		in >> sr.rotateStyle;
		in >> sr.degree;
		
		for(int i = 0; i < 4; i++){
			in >> sr.errorPos[i];
		}
		in >> sr.a;
		in >> sr.b;

		char str[2048];
		in.getline(str, 2048);
		in.getline(str, 2048);
		sr.objFilePath = str;
		//in >> sr.objFilePath;
		return in;
	}
	void displayInfo(){
		cout << "current info:" << endl;
		cout << *this;
	}
	void ShowError(){
		
		//先突出顶点。
		glPointSize(4.0);
		glBegin(GL_POINTS);
		for(int i = 0; i < 4; i++){
			glVertex3d(errorPos[i].m_data[0], errorPos[i].m_data[1], errorPos[i].m_data[2]);
		}
		glEnd();

		//绘制线条。
		glBegin(GL_LINES);
		for(int i = 0; i < 4; i++){
			glVertex3d(errorPos[i].m_data[0], errorPos[i].m_data[1], errorPos[i].m_data[2]);
		}
		glEnd();
	}
	/*void Save(ostream& out){
		out << np << endl;
		out << rotateStyle << endl;
		out << degree << endl;
		out << objFilePath << endl;
	}

	void load(ifstream &in){
		in >> np;
		in >> rotateStyle;
		in >> degree;
		in >> objFilePath;
	}*/
};

