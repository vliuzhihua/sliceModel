#pragma once
#include "DataUtility.h"

class Loop{
public:
	int sz;
	Point3d* data;
	Loop(){
		sz = 0;
		data = NULL;
	}
	Loop(int sz){
		this->sz = sz;
		data = new Point3d[sz];
	}
	~Loop(){
		if(data != NULL){
			delete[] data;
		}
	}
	void init(int sz){
		this->sz = sz;
		data = new Point3d[sz];
	}
	void Draw(){
		glBegin(GL_LINE_LOOP);
		for(int i = 0; i < sz; i++){
			glVertex3d(data[i].m_data[0], data[i].m_data[1], data[i].m_data[2]);
		}
		glEnd();
	}
};

class Layer{
public:
	//member variable
	Loop* loops;
	int sz;

	void Draw(int tt){
		/*for(int i = 0; i < sz; i++){
			loops[i].Draw();
		}*/
		loops[tt].Draw();
	}

	//member function
	bool IsIntersect(Point3d* rePos, int &a, int &b){
		for(int i = 0; i < sz; i++){
			for(int j = i + 1; j < sz; j++){
				if(GetBox(loops[i]).IsIntersect(GetBox(loops[j]))){
					if(CheckLoopIntersect(loops[i], loops[j], rePos) == true){
						a = i;
						b = j;
						return true;
					}
				}
			}
		}
		return false;
	}
private:
	Box GetBox(Loop &loop){
		Box box;
		if(loop.sz > 0){
			box.s = loop.data[0];
			box.b = loop.data[0];
			for(int i = 1; i < loop.sz; i++){
				for(int j = 0; j < 3; j++){
					if(box.s.m_data[j] > loop.data[i].m_data[j]){
						box.s.m_data[j] = loop.data[i].m_data[j];
					}
					if(box.b.m_data[j] < loop.data[i].m_data[j]){
						box.b.m_data[j] = loop.data[i].m_data[j];
					}
				}
			}
		}
		return box;
	}

	//判断p1,p2连起来的线段是否与p3,p4连起来的线段有交。
	//此处的检查线段的方法比较简陋。有点效率不高，可以考虑提高的。
	bool CheckLineIntersect(Point3d p1, Point3d p2, Point3d p3, Point3d p4){
		//先判断是否共面吧
		Tuple3d v1 = p3 - p2;
		Tuple3d v2 = p4 - p2;
		Tuple3d fn = v1.cross(v2);  
		if(fn.dot(p2 - p1) != 0){
			//如果不等于0说明，p1不和那些点共面的。
			return false;
		}

		Tuple3d tn = (p3 - p4).cross(fn);

		double d = -p3.dot(tn);

		if((p2 - p1).dot(tn) == 0){
			return false;
		}

		double t = (-d - p1.dot(tn)) / (p2 - p1).dot(tn);
	
		if(t < 0 || t > 1){
			return false;//说明无交点
		}
		//接着继续判断，那个交面上的点是否在p3, p4之间。
		Tuple3d intersect = p1 + t * (p2 - p1);

		//这里使用了一个epsilon，注意判别是否健壮。
		if((p3 - intersect).length() + (p4 - intersect).length() - (p3 - p4).length() <= 0.00001){
			return true;
		}else{
			return false;
		}
	}
	//检测是否环相交。
	bool CheckLoopIntersect(Loop &a, Loop &b, Point3d* rePos){
		for(int i = 1; i < a.sz; i++){
			for(int j = 1; j < b.sz; j++){

				if(CheckLineIntersect(a.data[i - 1], a.data[i], b.data[j - 1], b.data[j]) == true){
					rePos[0] = a.data[i - 1];
					rePos[1] = a.data[i];
					rePos[2] = b.data[j - 1];
					rePos[3] = b.data[j];
					return true;
				}

			}
			//边界情况的讨论。
			if(CheckLineIntersect(a.data[i - 1], a.data[i], b.data[0], b.data[b.sz - 1]) == true){
				rePos[0] = a.data[i - 1];
				rePos[1] = a.data[i];
				rePos[2] = b.data[0];
				rePos[3] = b.data[b.sz - 1];
				return true;
			}
		}
		//边界情况的讨论。
		for(int j = 1; j < b.sz; j++){
			if(CheckLineIntersect(a.data[0], a.data[a.sz - 1], b.data[j - 1], b.data[j]) == true){
				rePos[0] = a.data[0];
				rePos[1] = a.data[a.sz - 1];
				rePos[2] = b.data[j - 1];
				rePos[3] = b.data[j];
				return true;
			}
		}

		if(CheckLineIntersect(a.data[0], a.data[a.sz - 1], b.data[0], b.data[b.sz - 1]) == true){
			rePos[0] = a.data[0];
			rePos[1] = a.data[a.sz - 1];
			rePos[2] = b.data[0];
			rePos[3] = b.data[b.sz - 1];
			return true;
		}

		return false;
	}
};