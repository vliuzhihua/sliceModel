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

	//找出在平面中的顶点
	void CalInPlaneVtx(NewPlane np){
		//计算出包围盒。
		vector<NewVertex*>* nvVec = &(s->vtxs);
		Box box = s->GetBox();
		
		//这个epsilon的值的表示非常的重要，之前曾经因为这个设置得太大出现了错误。
		//
		double epsilon = abs((box.b - box.s).length() / 10000000);
		//利用新得到的epsilon，然后计算在这平面上的顶点的个数，假设比较少。

		//遍历一次所有的顶点。
		for(int i = 0, j = 0; i < nvVec->size(); i++){

			if(abs(nvVec->at(i)->pos.dot(np.abc) + np.D) <= epsilon){

				vtxInPlaneVec.push_back(nvVec->at(i));

				NewIntersection* tmpNi = new NewIntersection();

				//设置标记。
				tmpNi->type = NewIntersection::VERTEX_TYPE;
				tmpNi->vtx = nvVec->at(i);
				
				//将tmpNi插入vector中
				niVec.push_back(tmpNi);//事后也方便删除的。
				//插入一张用来查询的表中，注意啦。记录了那些在平面上的顶点的信息。如果这个顶点对应有一个交点，那么它
				nvToNiMap.insert(pair<NewVertex*, NewIntersection*>(nvVec->at(i), tmpNi));

				j++;
			}
		}
		//输出一些信息，暂时关闭掉了。
		//cout << "in planes vtx is: " << vtxInPlaneVec.size() <<endl;
	}

	//对在平面中的顶点的一些信息进行填充
	void CalInPlaneVtxInfo(){
		//计算这些切割平面上的点都在哪些面中。
		vector<NewFacet*> *nfVec = &(s->facets);

		//遍历一次所有面的顶点。
		//这次记录把NewIntersection::VERTEX_TYPE类型的信息都记录好了的。
		for(int i = 0; i < nfVec->size(); i++){
			vector<NewVertex*> ttnv = nfVec->at(i)->vtxs;
			for(int j = 0; j < ttnv.size(); j++){
				if(ttnv[j] != NULL){
					map<NewVertex*, NewIntersection*>::iterator iter;
					iter = nvToNiMap.find(ttnv[j]);
					//iter = nvToNiMap.find(NULL);
					//bug:怎么这里会有错呢？？？2015/4/27 注意啦！！！
					//为什么出错呢？？？？难道000是map特殊用途的东西。
					//上述bug已修复，确实存在bug，原因是之前使用固定大小的数组，越界了。
					if(iter != nvToNiMap.end()){
						iter->second->facets.push_back(nfVec->at(i));
						
						//面中的交点信息记录一下。
						nfVec->at(i)->intsects.push_back(iter->second);
						
					}
				}
			}
		}
	}

	//计算出其他的相交点
	void CalIntersection(NewPlane np){
		vector<NewEdge*>* neVec = &(s->edges);

		//算出所有的有交线段。
		for(int i = 0; i < neVec->size(); i++){
			NewVertex* nv1 = neVec->at(i)->vtxs[0];
			NewVertex* nv2 = neVec->at(i)->vtxs[1];

			//首先检查边的两个顶点都在切面上的情况，那么说明这条边在这个切面上。
			map<NewVertex*, NewIntersection*>::iterator iterLoc1, iterLoc2;

			iterLoc1 = nvToNiMap.find(nv1);
			iterLoc2 = nvToNiMap.find(nv2);

			if(iterLoc1 != nvToNiMap.end() && iterLoc2 != nvToNiMap.end()){

				map<NewIntersection*, vector<NewIntersection*>>::iterator iterV1, iterV2;
				iterV1 = edgeCoupleMap.find(iterLoc1->second);
				if(iterV1 != edgeCoupleMap.end()){
					//难道这样是有问题的？
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
				
		
			//这里漏掉了考虑是一个顶点在该边的情况了。注意下。
			if(iterLoc1 != nvToNiMap.end() || iterLoc2 != nvToNiMap.end()){
				continue;
			}

			if((nv1->pos.dot(np.abc) + np.D) * (nv2->pos.dot(np.abc) + np.D) < 0){//这里的判断很有可能出现浮点误差，导致判断不正确的情况。
				//这里的计算方式可能非常的不快速。
				Point3d O = Point3d(nv1->pos.m_data);
				Tuple3d vecn = Tuple3d(nv2->pos - nv1->pos);
			

				//这里需要注意vecn.dot(np.abc)可能为0的情况，这里没有考虑周全的。注意了。
				double t = (-np.D - O.dot(np.abc)) / vecn.dot(np.abc);


				Point3d newP = O + vecn * t;

				NewIntersection* tni = new NewIntersection;

				tni->pos = newP;
				tni->edge = neVec->at(i);

				
				for(int j = 0; j < neVec->at(i)->facets.size(); j++){
					if(neVec->at(i)->facets[j] != NULL){
						tni->facets.push_back(neVec->at(i)->facets[j]);
						//想相应的面中也添加上交点的信息。
						tni->facets[j]->intsects.push_back(tni);
					}

				}
			
				//将tni插入niVec中
				niVec.push_back(tni);

			}else{
				//没有交点。
			}

		}
		//输出一些信息，暂时关闭掉了
		//cout << "number of intersections is: " << niVec.size() << endl;
	}


	//该函数返回通过既定规则能够找到的下一个交点，并返回，如果找不到则返回NULL
	NewIntersection* CalNextIntersection(NewIntersection* niCur){
		//首先检查是否是一条在切面上的边。如果是那么执行相应的操作，然后结束即可。

		//bug :发现这里有问题，因为可能一个点在多条在切面的线段上。2015/4/27 by lzh 这里非常有问题的。注意！！！
		//上述问题，已经修复，by lzh 2015/4/27

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


		//然后检查是否是在切面上的点，如果是，则执行相应的操作，然后结束即可。
		if(niCur->type == NewIntersection::VERTEX_TYPE){
			NewIntersection* nextNi = NULL;

			for(int i = 0; i < niCur->facets.size(); i++){
				if(niCur->facets[i]->intsects.size() >= 2 ){
					for(int j = 0; j < niCur->facets[i]->intsects.size(); j++){

						if(niCur->facets[i]->intsects[j] != niCur 
							&& niCur->facets[i]->intsects[j]->visited == 0){
							
								nextNi = niCur->facets[i]->intsects[j];//这样处理真的好？
							return nextNi;
						}
					}
				}
			}
			//开始进行处理，即找到了下一项了的处理。
			//下面这段和上面那段是类似的，要注意呀~~~
		}
			

		//接着就不是特殊情况了，我们可以采用直接找寻下一个点啦，


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

		//如果找不到，那么返回空，表示找不到。
		return NULL;
	}

	//求得是否已经全都访问过了，如果存在没有访问过的NewIntersection，则返回之，否则返回NULL
	NewIntersection* GetVisitedAll(vector<NewIntersection*> niVecTmp){
		for(int i = 0; i < niVecTmp.size(); i++){
			if(niVecTmp[i]->visited == 0){
				return niVecTmp[i];
			}
		}
		return NULL;
	}

	//利用之前的信息进行交点的连接。
	vector<list<NewIntersection*>> ConnectIntersection(){
		
		vector<list<NewIntersection*>> reListVec;
		NewIntersection* niCurStart = NULL;

		while((niCurStart = GetVisitedAll(niVec))){

			list<NewIntersection*> niCurList;
			NewIntersection* nextNi = NULL;
			NewIntersection* niCurRound = niCurStart;
			//先朝向某个方向来一次。
			while(nextNi = CalNextIntersection(niCurRound)){

				nextNi->visited = 1;
				niCurRound = nextNi;
				niCurList.push_back(nextNi);
			}

			//设置初始点为访问过了的。
			niCurStart->visited = 1;
			niCurRound = niCurStart;
			niCurList.push_front(niCurStart);
			//可能会反向再来一次
			while(nextNi = CalNextIntersection(niCurRound)){

				nextNi->visited = 1;
				niCurRound = nextNi;
				niCurList.push_front(nextNi);
			}

			//插入最终结果中去。
			reListVec.push_back(niCurList);

		}

		return reListVec;
	}

	//将之前那么多的步骤放到一步中取执行。
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