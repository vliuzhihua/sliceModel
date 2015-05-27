#pragma once
#include <map>
#include <vector>
#include <list>
#include "NewSolid.h"
#include "NewPlane.h"

#include <string>

using namespace std;


class SliceModel{
public:
	//double epsilon;
	NewSolid *s;
	vector<NewVertex*> vtxInPlaneVec;
	map<NewVertex*, NewIntersection*> nvToNiMap;
	//NewIntersection* niVec;
	map<NewIntersection*, vector<NewIntersection*>> edgeCoupleMap;
	vector<NewIntersection*> niVec;

	SliceModel(NewSolid* s){
		vtxInPlaneVec.clear();
		
		nvToNiMap.clear();
		edgeCoupleMap.clear();
		niVec.clear();
		this->s = s;
	}
	~SliceModel(){
		int sz = niVec.size();
		for(int i = 0; i < sz; i++){
			delete niVec[i];
		}

	}

	//�ҳ���ƽ���еĶ���
	void CalInPlaneVtx(NewPlane np){
		//�������Χ�С�
		vector<NewVertex*>* nvVec = &(s->vtxs);
		Box box = s->GetBox();
		
		//���epsilon��ֵ�ı�ʾ�ǳ�����Ҫ��֮ǰ������Ϊ������õ�̫������˴���
		//
		double epsilon = abs((box.b - box.s).length() / 10000000);
		//�����µõ���epsilon��Ȼ���������ƽ���ϵĶ���ĸ���������Ƚ��١�

		//����һ�����еĶ��㡣
		for(int i = 0, j = 0; i < nvVec->size(); i++){

			if(abs(nvVec->at(i)->pos.dot(np.abc) + np.D) <= epsilon){

				vtxInPlaneVec.push_back(nvVec->at(i));

				NewIntersection* tmpNi = new NewIntersection();

				//���ñ�ǡ�
				tmpNi->type = NewIntersection::VERTEX_TYPE;
				tmpNi->vtx = nvVec->at(i);
				
				//��tmpNi����vector��
				niVec.push_back(tmpNi);//�º�Ҳ����ɾ���ġ�
				//����һ��������ѯ�ı��У�ע��������¼����Щ��ƽ���ϵĶ������Ϣ�������������Ӧ��һ�����㣬��ô��
				nvToNiMap.insert(pair<NewVertex*, NewIntersection*>(nvVec->at(i), tmpNi));

				j++;
			}
		}
		//���һЩ��Ϣ����ʱ�رյ��ˡ�
		//cout << "in planes vtx is: " << vtxInPlaneVec.size() <<endl;
	}

	//����ƽ���еĶ����һЩ��Ϣ�������
	void CalInPlaneVtxInfo(){
		//������Щ�и�ƽ���ϵĵ㶼����Щ���С�
		vector<NewFacet*> *nfVec = &(s->facets);

		//����һ��������Ķ��㡣
		//��μ�¼��NewIntersection::VERTEX_TYPE���͵���Ϣ����¼���˵ġ�
		for(int i = 0; i < nfVec->size(); i++){
			vector<NewVertex*> ttnv = nfVec->at(i)->vtxs;
			for(int j = 0; j < ttnv.size(); j++){
				if(ttnv[j] != NULL){
					map<NewVertex*, NewIntersection*>::iterator iter;
					iter = nvToNiMap.find(ttnv[j]);
					//iter = nvToNiMap.find(NULL);
					//bug:��ô������д��أ�����2015/4/27 ע����������
					//Ϊʲô�����أ��������ѵ�000��map������;�Ķ�����
					//����bug���޸���ȷʵ����bug��ԭ����֮ǰʹ�ù̶���С�����飬Խ���ˡ�
					if(iter != nvToNiMap.end()){
						iter->second->facets.push_back(nfVec->at(i));
						
						//���еĽ�����Ϣ��¼һ�¡�
						nfVec->at(i)->intsects.push_back(iter->second);
						
					}
				}
			}
		}
	}

	//������������ཻ��
	void CalIntersection(NewPlane np){
		vector<NewEdge*>* neVec = &(s->edges);

		//������е��н��߶Ρ�
		for(int i = 0; i < neVec->size(); i++){
			NewVertex* nv1 = neVec->at(i)->vtxs[0];
			NewVertex* nv2 = neVec->at(i)->vtxs[1];

			//���ȼ��ߵ��������㶼�������ϵ��������ô˵������������������ϡ�
			map<NewVertex*, NewIntersection*>::iterator iterLoc1, iterLoc2;

			iterLoc1 = nvToNiMap.find(nv1);
			iterLoc2 = nvToNiMap.find(nv2);

			if(iterLoc1 != nvToNiMap.end() && iterLoc2 != nvToNiMap.end()){

				map<NewIntersection*, vector<NewIntersection*>>::iterator iterV1, iterV2;
				iterV1 = edgeCoupleMap.find(iterLoc1->second);
				if(iterV1 != edgeCoupleMap.end()){
					//�ѵ�������������ģ�
					iterV1->second.push_back(iterLoc2->second);

				}else{
					vector<NewIntersection*> vec;
					vec.push_back(iterLoc2->second);
					edgeCoupleMap.insert(pair<NewIntersection*, vector<NewIntersection*>>(iterLoc1->second, vec));
				}

				iterV2 = edgeCoupleMap.find(iterLoc2->second);
				if(iterV2 != edgeCoupleMap.end()){
					iterV2->second.push_back(iterLoc1->second);
				}else{
					vector<NewIntersection*> vec;
					vec.push_back(iterLoc1->second);
					edgeCoupleMap.insert(pair<NewIntersection*, vector<NewIntersection*>>(iterLoc2->second, vec));
				}
				continue;
			}
				
		
			//����©���˿�����һ�������ڸñߵ�����ˡ�ע���¡�
			if(iterLoc1 != nvToNiMap.end() || iterLoc2 != nvToNiMap.end()){
				continue;
			}

			if((nv1->pos.dot(np.abc) + np.D) * (nv2->pos.dot(np.abc) + np.D) < 0){//������жϺ��п��ܳ��ָ����������жϲ���ȷ�������
				//����ļ��㷽ʽ���ܷǳ��Ĳ����١�
				Point3d O = Point3d(nv1->pos.m_data);
				Tuple3d vecn = Tuple3d(nv2->pos - nv1->pos);
			

				//������Ҫע��vecn.dot(np.abc)����Ϊ0�����������û�п�����ȫ�ġ�ע���ˡ�
				double t = (-np.D - O.dot(np.abc)) / vecn.dot(np.abc);


				Point3d newP = O + vecn * t;

				NewIntersection* tni = new NewIntersection;

				tni->pos = newP;
				tni->edge = neVec->at(i);

				
				for(int j = 0; j < neVec->at(i)->facets.size(); j++){
					if(neVec->at(i)->facets[j] != NULL){
						tni->facets.push_back(neVec->at(i)->facets[j]);
						//����Ӧ������Ҳ����Ͻ������Ϣ��
						tni->facets[j]->intsects.push_back(tni);
					}

				}
			
				//��tni����niVec��
				niVec.push_back(tni);

			}else{
				//û�н��㡣
			}

		}
		//���һЩ��Ϣ����ʱ�رյ���
		//cout << "number of intersections is: " << niVec.size() << endl;
	}


	//�ú�������ͨ���ȶ������ܹ��ҵ�����һ�����㣬�����أ�����Ҳ����򷵻�NULL
	NewIntersection* CalNextIntersection(NewIntersection* niCur){
		//���ȼ���Ƿ���һ���������ϵıߡ��������ôִ����Ӧ�Ĳ�����Ȼ��������ɡ�

		//bug :�������������⣬��Ϊ����һ�����ڶ�����������߶��ϡ�2015/4/27 by lzh ����ǳ�������ġ�ע�⣡����
		//�������⣬�Ѿ��޸���by lzh 2015/4/27

		map<NewIntersection*, vector<NewIntersection*>>::iterator edgeCoupleIter;
		edgeCoupleIter = edgeCoupleMap.find(niCur);

		if(edgeCoupleIter != edgeCoupleMap.end()){
			vector<NewIntersection*> *edgeTVec = &(edgeCoupleIter->second);
			for(int i = 0; i < edgeTVec->size(); i++){
			
				NewIntersection* nextNi = edgeTVec->at(i);

				if(nextNi != NULL && nextNi->visited == 0){

					return nextNi;
				}
			
			}
		}
		
		//done a perfect job


		//Ȼ�����Ƿ����������ϵĵ㣬����ǣ���ִ����Ӧ�Ĳ�����Ȼ��������ɡ�
		if(niCur->type == NewIntersection::VERTEX_TYPE){
			NewIntersection* nextNi = NULL;

			for(int i = 0; i < niCur->facets.size(); i++){
				if(niCur->facets[i]->intsects.size() >= 2 ){
					for(int j = 0; j < niCur->facets[i]->intsects.size(); j++){

						if(niCur->facets[i]->intsects[j] != niCur 
							&& niCur->facets[i]->intsects[j]->visited == 0){
							
								nextNi = niCur->facets[i]->intsects[j];//����������ĺã�
							return nextNi;
						}
					}
				}
			}
			//��ʼ���д������ҵ�����һ���˵Ĵ���
			//������κ������Ƕ������Ƶģ�Ҫע��ѽ~~~
		}
			

		//���žͲ�����������ˣ����ǿ��Բ���ֱ����Ѱ��һ��������


		for(int l = 0; l < niCur->facets.size(); l++){
			NewIntersection* nextNi = NULL;

			for(int k = 0; k < niCur->facets[l]->intsects.size(); k++){
				if(niCur->facets[l]->intsects[k] != niCur 
					&& niCur->facets[l]->intsects[k]->visited == 0){
					
					nextNi = niCur->facets[l]->intsects[k];
					return nextNi;
				}
			}
		}

		//����Ҳ�������ô���ؿգ���ʾ�Ҳ�����
		return NULL;
	}

	//����Ƿ��Ѿ�ȫ�����ʹ��ˣ��������û�з��ʹ���NewIntersection���򷵻�֮�����򷵻�NULL
	NewIntersection* GetVisitedAll(vector<NewIntersection*> niVecTmp){
		for(int i = 0; i < niVecTmp.size(); i++){
			if(niVecTmp[i]->visited == 0){
				return niVecTmp[i];
			}
		}
		return NULL;
	}

	//����֮ǰ����Ϣ���н�������ӡ�
	vector<list<NewIntersection*>> ConnectIntersection(){
		
		vector<list<NewIntersection*>> reListVec;
		NewIntersection* niCurStart = NULL;

		while((niCurStart = GetVisitedAll(niVec))){

			list<NewIntersection*> niCurList;
			NewIntersection* nextNi = NULL;
			NewIntersection* niCurRound = niCurStart;
			//�ȳ���ĳ��������һ�Ρ�
			while(nextNi = CalNextIntersection(niCurRound)){

				nextNi->visited = 1;
				niCurRound = nextNi;
				niCurList.push_back(nextNi);
			}

			//���ó�ʼ��Ϊ���ʹ��˵ġ�
			niCurStart->visited = 1;
			niCurRound = niCurStart;
			niCurList.push_front(niCurStart);
			//���ܻᷴ������һ��
			while(nextNi = CalNextIntersection(niCurRound)){

				nextNi->visited = 1;
				niCurRound = nextNi;
				niCurList.push_front(nextNi);
			}

			//�������ս����ȥ��
			reListVec.push_back(niCurList);

		}

		return reListVec;
	}

	//��֮ǰ��ô��Ĳ���ŵ�һ����ȡִ�С�
	vector<list<NewIntersection*>> AllInOne(NewPlane np){
		//1.
		CalInPlaneVtx(np);
		//2.
		CalInPlaneVtxInfo();
		//3.
		CalIntersection(np);
		//4.
		return ConnectIntersection();
	}


};