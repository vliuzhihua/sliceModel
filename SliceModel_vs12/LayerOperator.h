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

	//�ж�p1,p2���������߶��Ƿ���p3,p4���������߶��н���
	//�˴��ļ���߶εķ����Ƚϼ�ª���е�Ч�ʲ��ߣ����Կ�����ߵġ�
	bool CheckLineIntersect(Point3d p1, Point3d p2, Point3d p3, Point3d p4){
		//���ж��Ƿ����
		Tuple3d v1 = p3 - p2;
		Tuple3d v2 = p4 - p2;
		Tuple3d fn = v1.cross(v2);  
		if(fn.dot(p2 - p1) != 0){
			//���������0˵����p1������Щ�㹲��ġ�
			return false;
		}

		Tuple3d tn = (p3 - p4).cross(fn);

		double d = -p3.dot(tn);

		if((p2 - p1).dot(tn) == 0){
			return false;
		}

		double t = (-d - p1.dot(tn)) / (p2 - p1).dot(tn);
	
		if(t < 0 || t > 1){
			return false;//˵���޽���
		}
		//���ż����жϣ��Ǹ������ϵĵ��Ƿ���p3, p4֮�䡣
		Tuple3d intersect = p1 + t * (p2 - p1);

		//����ʹ����һ��epsilon��ע���б��Ƿ�׳��
		if((p3 - intersect).length() + (p4 - intersect).length() - (p3 - p4).length() <= 0.00001){
			return true;
		}else{
			return false;
		}
	}
	//����Ƿ��ཻ��
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
			//�߽���������ۡ�
			if(CheckLineIntersect(a.data[i - 1], a.data[i], b.data[0], b.data[b.sz - 1]) == true){
				rePos[0] = a.data[i - 1];
				rePos[1] = a.data[i];
				rePos[2] = b.data[0];
				rePos[3] = b.data[b.sz - 1];
				return true;
			}
		}
		//�߽���������ۡ�
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