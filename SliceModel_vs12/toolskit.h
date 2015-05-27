#pragma once
#include "NewSolid.h"
#include "LayerOperator.h"
#include "SliceModel.h"
#include "NewIntersection.h"
class ToolsKit{
public:
	static Layer CalLayer(NewSolid &s, NewPlane np){
		//slice(s, np, tmpB, szOfTmpB, num);
		Layer re;
		SliceModel sm(&s);
		vector<list<NewIntersection*>> vlni = sm.AllInOne(np);

		int ssSz;
		Loop* loops = new Loop[vlni.size()];
		ssSz = vlni.size();
		for(int i = 0; i < vlni.size(); i++){
			
			//bug:������仰��֮ǰ�����ù��캯�������еģ�����
			//ss[i] = Loop(vlni[i].size());
			//�����Ļ����������ִ����������������Ӷ�delete��data�����º���ʹ��data��ʱ�����
			//����bug�Ѿ��޸���

			//������仰�����������˿ռ䣬���趨�˴�С��Ŀ������
			loops[i].init(vlni[i].size());
		
			list<NewIntersection* >::iterator listIter;
			int j = 0;
			for(listIter = vlni[i].begin(); listIter != vlni[i].end(); listIter++){

				if((*listIter)->type == NewIntersection::VERTEX_TYPE){
					loops[i].data[j] = (*listIter)->vtx->pos;
					loops[i].data[j] = (*listIter)->vtx->pos;
					loops[i].data[j] = (*listIter)->vtx->pos;
				}else{
					loops[i].data[j] = (*listIter)->pos;
					loops[i].data[j] = (*listIter)->pos;
					loops[i].data[j] = (*listIter)->pos;
				}
				j++;
			}	
		}

		re.loops = loops;
		re.sz = ssSz;
		return re;
	}

	static void transforNewSolid(NewSolid &s,double* &faces, int &sz){
		//��ת�����ڿ�����ʾ��

		//����ǿգ�Ҫ���ͷ�֮ǰ�Ŀռ䡣
		if(faces != NULL){
			delete faces;
		}
		//��������ģ�������ʾ��
		sz = s.facets.size();
		faces = new double[sz * 12];

		vector<NewFacet*> tv = s.facets;
			
		for (int i = 0; i < sz; i++){

			NewVertex* nv[3];
			nv[0] = tv.at(i)->edges[0]->vtxs[0];
			nv[1] = tv.at(i)->edges[0]->vtxs[1];

			for(int p = 1; p < 3; p++){
				if(tv.at(i)->edges[p]->vtxs[0] != nv[0] && tv.at(i)->edges[p]->vtxs[0] != nv[1]){
					nv[2] = tv.at(i)->edges[p]->vtxs[0]; 
				}else if(tv.at(i)->edges[p]->vtxs[1] != nv[0] && tv.at(i)->edges[p]->vtxs[1] != nv[1]){
					nv[2] = tv.at(i)->edges[p]->vtxs[1]; 
				}
			}

			faces[12 * i + 0] = nv[0]->pos.m_data[0];
			faces[12 * i + 1] = nv[0]->pos.m_data[1];
			faces[12 * i + 2] = nv[0]->pos.m_data[2];

			faces[12 * i + 3] = nv[1]->pos.m_data[0];
			faces[12 * i + 4] = nv[1]->pos.m_data[1];
			faces[12 * i + 5] = nv[1]->pos.m_data[2];

			faces[12 * i + 6] = nv[2]->pos.m_data[0];
			faces[12 * i + 7] = nv[2]->pos.m_data[1];
			faces[12 * i + 8] = nv[2]->pos.m_data[2];

			faces[12 * i + 9] = tv.at(i)->normal.m_data[0];
			faces[12 * i + 10] = tv.at(i)->normal.m_data[1];
			faces[12 * i + 11] = tv.at(i)->normal.m_data[2];

		}
	}

	static void DrawSolid(double* faces, int sz){
		glBegin(GL_TRIANGLES);

		for (int i = 0; i < sz; i++){
			glNormal3d(faces[12 * i + 9], faces[12 * i + 10], faces[12 * i + 11]);
			glVertex3d(faces[12 * i + 0], faces[12 * i + 1], faces[12 * i + 2]);
		
			glNormal3d(faces[12 * i + 9], faces[12 * i + 10], faces[12 * i + 11]);
			glVertex3d(faces[12 * i + 3], faces[12 * i + 4], faces[12 * i + 5]);
		
			glNormal3d(faces[12 * i + 9], faces[12 * i + 10], faces[12 * i + 11]);
			glVertex3d(faces[12 * i + 6], faces[12 * i + 7], faces[12 * i + 8]);
		}

		glEnd();
	}
};
