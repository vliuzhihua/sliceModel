// Author(s) : Camille Wormser, Pierre Alliez

#include "GL/glew.h"
#include "GL/glut.h"
#include "Solid.h"
#include "AboutSlice.h"
#include <map>


#include <iostream>
#include <list>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_triangle_primitive.h>
typedef CGAL::Simple_cartesian<double> K;
typedef K::FT FT;
typedef K::Ray_3 Ray;
typedef K::Line_3 Line;
typedef K::Point_3 Point;

typedef K::Plane_3 Plane;
typedef K::Vector_3 Vector;
typedef K::Segment_3 Segment;

typedef K::Triangle_3 Triangle;
typedef std::list<Triangle>::iterator Iterator;

typedef CGAL::AABB_triangle_primitive<K, Iterator> Primitive;
typedef CGAL::AABB_traits<K, Primitive> AABB_triangle_traits;
typedef CGAL::AABB_tree<AABB_triangle_traits> Tree;

void slice(Solid s, double edges[][6], int &sz)
{

	std::list<Triangle> triangles;

	int szOfFaces = s.facets.size();
	vector<Facet> f = s.facets;
	for(int i = 0; i < szOfFaces; i++){
		Point a(f.at(i).point[0].m_data[0], f.at(i).point[0].m_data[1], f.at(i).point[0].m_data[2]);
		Point b(f.at(i).point[1].m_data[0], f.at(i).point[1].m_data[1], f.at(i).point[1].m_data[2]);
		Point c(f.at(i).point[2].m_data[0], f.at(i).point[2].m_data[1], f.at(i).point[2].m_data[2]);
		triangles.push_back(Triangle(a,b,c));
	}
    // constructs AABB tree
    Tree tree(triangles.begin(),triangles.end());
    // counts #intersections
    


	for(int k = -150; k <= 150; k += 100){
		
	

		Point e(k, 0, 0);

		// constructs plane query
		Vector vec(1.0, 0.0, 1.0);
		Plane plane_query(e,vec);


		typedef std::vector<Tree::Object_and_primitive_id> Intersections;
		Intersections intersections;

		tree.all_intersections(plane_query, std::back_inserter(intersections));
    
		// Fill data structure
		for ( Intersections::iterator it = intersections.begin(),
			 end = intersections.end() ; it != end ; ++it )
		{
			const Segment* inter_seg = CGAL::object_cast<Segment>(&(it->first));
        
			static int i = 0;
		
			if ( NULL != inter_seg )
			{
				//m_cut_segments.push_back(*inter_seg);
				//std::cout << i << std::endl;
				//std::cout << inter_seg->vertex(0).x() << " " << inter_seg->vertex(0).y() << " " << inter_seg->vertex(0).z()  << std::endl;
			
				glBegin(GL_LINES);

				glVertex3d(inter_seg->source().x(), inter_seg->source().y(), inter_seg->source().z());
				glVertex3d(inter_seg->target().x(), inter_seg->target().y(), inter_seg->target().z());
				glEnd();
			

				/*
				edges[i][0] = inter_seg->source().x();
				edges[i][1] = inter_seg->source().y();
				edges[i][2] = inter_seg->source().z();
				edges[i][3] = inter_seg->target().x();
				edges[i][4] = inter_seg->target().y();
				edges[i][5] = inter_seg->target().z();

			

				i++;
				sz = i;*/



				//std::cout << "source: " << inter_seg->source().x() << " " << inter_seg->source().x() << " " << inter_seg->source().x()  << std::endl;
				//std::cout << "target: " << inter_seg->target().x() << " " << inter_seg->target().x() << " " << inter_seg->target().x()  << std::endl;
		
				//std::cout << "-------" << std::endl;
				//i++;
			}
		}
	
	}
}

void slice(Solid s, double edges[][6], int &sz, double offset)
{

	std::list<Triangle> triangles;

	int szOfFaces = s.facets.size();
	vector<Facet> f = s.facets;
	for(int i = 0; i < szOfFaces; i++){
		Point a(f.at(i).point[0].m_data[0], f.at(i).point[0].m_data[1], f.at(i).point[0].m_data[2]);
		Point b(f.at(i).point[1].m_data[0], f.at(i).point[1].m_data[1], f.at(i).point[1].m_data[2]);
		Point c(f.at(i).point[2].m_data[0], f.at(i).point[2].m_data[1], f.at(i).point[2].m_data[2]);
		triangles.push_back(Triangle(a,b,c));
	}
    // constructs AABB tree
    Tree tree(triangles.begin(),triangles.end());
    // counts #intersections
    


	
		Point e(offset, 0, 0);

		// constructs plane query
		Vector vec(1.0, 0.0, 1.0);
		Plane plane_query(e,vec);


		typedef std::vector<Tree::Object_and_primitive_id> Intersections;
		Intersections intersections;

		tree.all_intersections(plane_query, std::back_inserter(intersections));
    
		// Fill data structure
		for(Intersections::iterator it = intersections.begin(),
			 end = intersections.end() ; it != end ; ++it )
		{
			const Segment* inter_seg = CGAL::object_cast<Segment>(&(it->first));
        
			static int i = 0;
		
			if ( NULL != inter_seg )
			{
				edges[i][0] = inter_seg->source().x();
				edges[i][1] = inter_seg->source().y();
				edges[i][2] = inter_seg->source().z();
				edges[i][3] = inter_seg->target().x();
				edges[i][4] = inter_seg->target().y();
				edges[i][5] = inter_seg->target().z();

			

				i++;
				sz = i;
			}
		}

}


class BPoint{
public:
	Point3d pos;
	BPoint* next;
	BPoint* pre;
	BPoint(){
		next = NULL;
		pre = NULL;
	}
};


//bool linkNode


void slice(NewSolid s, NewPlane np, double border[][3][40], int sz[], int &num){
	
	
	//1.
	//首先遍历一遍，然后寻找一个最恰当的包围盒，然后求出本次计算中的epsilon值。
	
	vector<NewVertex*> nvVec = s.vtxs;
	Box box = s.GetBox();
	
	
	//cout << box;
	//包围盒计算结束******************************************

	//2.
	double epsilon = (box.b - box.s).length() / 1000000000;
	//利用新得到的epsilon，然后计算在这平面上的顶点的个数，假设比较少。
	vector<NewVertex*> vtxInPlaneVec;
	map<NewVertex*, int> vtxInPlaneMap;
	map<NewVertex*, NewIntersection*> nvToNiMap;
	
	NewIntersection* niVec = NULL;

	for(int i = 0, j = 0; i < nvVec.size(); i++){

		if(abs(nvVec.at(i)->pos.dot(np.abc) + np.D) <= epsilon){
			vtxInPlaneVec.push_back(nvVec.at(i));
			
			vtxInPlaneMap.insert(pair<NewVertex*, int>(nvVec[i], j));

			NewIntersection* tmpNi = new NewIntersection();
			tmpNi->type = NewIntersection::VERTEX_TYPE;
			tmpNi->vtx = nvVec[i];
			//将双向循环链表进行连接即可。
			if(niVec == NULL){
				niVec = tmpNi;
			}else{
				niVec->next->pre = tmpNi;
				tmpNi->next = niVec->next;
				tmpNi->pre = niVec;
				niVec->next = tmpNi;
			}
			//插入一张用来查询的表中，注意啦。
			nvToNiMap.insert(pair<NewVertex*, NewIntersection*>(nvVec[i], tmpNi));

			j++;
		}
	}
	
	cout << "inplane:" << vtxInPlaneVec.size() << endl;
	//以下计算基于了在平面上的顶点比较少的假设，才能够有效率。否则估计实在太慢了。

	//3.计算这些切平面上的点都在哪些面中。
	vector<NewFacet*> *nfVec = &(s.facets);

	for(int i = 0; i < nfVec->size(); i++){
		for(int j = 0; j < 3; j++){
			std::map<NewVertex*, NewIntersection*>::iterator iter;
			iter = nvToNiMap.find(nfVec->at(i)->vtx[j]);
			if(iter != nvToNiMap.end()){
				iter->second->facet[iter->second->facetSz] = nfVec->at(i);
				//iter->second->facet[iter->second->facetSz]
				iter->second->facetSz++;
			}
		}
	}
	//通过这一步添加上了所有的在平面上的点的面信息。


	//int cb = 0;
	
	
	vector<NewEdge*> neVec = s.edges;

	
	//vector<NewEdgeIntersect*> neiVec;

	map<NewIntersection*, NewIntersection*> edgeCoupleMap;
	//4.
	//算出所有的有交线段。
	for(int i = 0; i < neVec.size(); i++){
		NewVertex* nv1 = neVec.at(i)->vtxs[0];
		NewVertex* nv2 = neVec.at(i)->vtxs[1];

		
		std::map<NewVertex*, int>::iterator iter1;
		std::map<NewVertex*, int>::iterator iter2;
		iter1 = vtxInPlaneMap.find(nv1);
		iter2 = vtxInPlaneMap.find(nv2);

		if(iter1 != vtxInPlaneMap.end() && iter2 != vtxInPlaneMap.end()){
			NewEdgeIntersect* tmpNei = new NewEdgeIntersect();
			
			std::map<NewVertex*, NewIntersection*>::iterator iterLoc1;
			std::map<NewVertex*, NewIntersection*>::iterator iterLoc2;

			iterLoc1 = nvToNiMap.find(vtxInPlaneVec[iter1->second]);
			if(iterLoc1 != nvToNiMap.end() && iterLoc2 != nvToNiMap.end()){
				tmpNei->vtxIntersect[0] = iterLoc1->second;
				tmpNei->vtxIntersect[1] = iterLoc2->second;

				edgeCoupleMap.insert(pair<NewIntersection*, NewIntersection*>(iterLoc1->second, iterLoc2->second));
				edgeCoupleMap.insert(pair<NewIntersection*, NewIntersection*>(iterLoc2->second, iterLoc1->second));
			}
			
			continue;
		}
		
		if((nv1->pos.dot(np.abc) + np.D) * (nv2->pos.dot(np.abc) + np.D) < 0){//这里的判断很有可能出现浮点误差，导致判断不正确的情况。
			//这里的计算方式可能非常的不快速。
			Point3d O = Point3d(nv1->pos.m_data);
			Tuple3d vecn = Tuple3d(nv2->pos - nv1->pos);
			
			double t = (-np.D - O.dot(np.abc)) / vecn.dot(np.abc);

			Point3d newP = O + vecn * t;

			NewIntersection* tni = new NewIntersection;

			tni->pos = newP;
			tni->edge = neVec.at(i);


			for(int j = 0; j < 3; j++){
				tni->facet[j] = neVec.at(i)->facet[j];
				if(tni->facet[j] != NULL){
					//想相应的面中也添加上交点的信息。
					tni->facet[j]->intsects[tni->facet[j]->intersectCount] = tni;
					tni->facet[j]->intersectCount++;
				}
			}
			
			if(niVec == NULL){
				niVec = tni;
			}else{
				tni->next = niVec;
				tni->pre = niVec->pre;
				niVec->pre = tni;
				niVec = tni;
			}

		}else{
			//没有交点。
		}

	}

	
	int cBorderNum = 0;
	NewIntersection* niTemp = NULL;
	int markRunTime = 0;

	int roundCount = 0;


	//5.
	//连接计算。

	vector<NewIntersection*> loopNiVec;

	list<int> li;
	
	//li.erase(li.);
		
	while(niVec){//以niVec为空了作为停止条件。
		NewIntersection* niCurRound = niVec;
		NewIntersection* firNi = niVec;
		NewIntersection* orderedLoop = NULL;
		while(1){

			NewIntersection* oldNi = niCurRound;
			
			
			//首先检查是否是一条在切面上的边。如果是那么执行相应的操作，然后结束即可。
			map<NewIntersection*, NewIntersection*>::iterator edgeCoupleIter;
			edgeCoupleIter = edgeCoupleMap.find(niCurRound);

			if(edgeCoupleIter != edgeCoupleMap.end()){
				NewIntersection* nextNi = edgeCoupleIter->second;
				
				if(nextNi != NULL && nextNi->visited == 0){
					
					niCurRound->visited = 1;
						if(niCurRound == niVec){
							if(niVec->next == niVec){
								niVec = NULL;
							}else{
								niVec = niVec->next;
							}
						}
					
						if(niCurRound->next == niCurRound){
						
							//将niCurRound加入有序连接中。
							if(orderedLoop == NULL){
								orderedLoop = niCurRound;
								niCurRound->orderedNext = niCurRound;
								niCurRound->orderedPre = niCurRound;
							}else{
								orderedLoop->orderedNext->orderedPre = niCurRound;
								niCurRound->orderedNext = orderedLoop->orderedNext;
								niCurRound->orderedPre = orderedLoop;
								orderedLoop->orderedNext = niCurRound;
								orderedLoop = niCurRound;
							}
							niCurRound = NULL;
						}else{
							//将niCurRound从基本连接中删除。
							niCurRound->next->pre = niCurRound->pre;
							niCurRound->pre->next = niCurRound->next;
							//将niCurRound加入有序连接中。
							if(orderedLoop == NULL){
								orderedLoop = niCurRound;
								niCurRound->orderedNext = niCurRound;
								niCurRound->orderedPre = niCurRound;
							}else{
								orderedLoop->orderedNext->orderedPre = niCurRound;
								niCurRound->orderedNext = orderedLoop->orderedNext;
								niCurRound->orderedPre = orderedLoop;
								orderedLoop->orderedNext = niCurRound;
								orderedLoop = niCurRound;
							}

						}

						niCurRound = nextNi;
						continue;
					
				}

				
			}
			//down a perfect jod


			//然后检查是否是在切面上的点，如果是，则执行相应的操作，然后结束即可。

			if(niCurRound->type == NewIntersection::VERTEX_TYPE){
				NewIntersection* nextNi = NULL;
				for(int i = 0; i < niCurRound->facetSz; i++){
					if(niCurRound->facet[i]->intersectCount >= 2 ){
						for(int j = 0; j < niCurRound->facet[j]->intersectCount; j++){
							if(niCurRound->facet[j]->intsects[j] != niCurRound 
								&& niCurRound->facet[j]->intsects[j]->visited == 0){

								nextNi = niCurRound->facet[j]->intsects[j];//这样处理真的好？

							}
						}
					}
				}
				//开始进行处理，即找到了下一项了的处理。
				//下面这段和上面那段是类似的，要注意呀~~~
				if(nextNi != NULL && nextNi->visited == 0){
					
					niCurRound->visited = 1;
						if(niCurRound == niVec){
							if(niVec->next == niVec){
								niVec = NULL;
							}else{
								niVec = niVec->next;
							}
						}
					
						if(niCurRound->next == niCurRound){
						
							//将niCurRound加入有序连接中。
							if(orderedLoop == NULL){
								orderedLoop = niCurRound;
								niCurRound->orderedNext = niCurRound;
								niCurRound->orderedPre = niCurRound;
							}else{
								orderedLoop->orderedNext->orderedPre = niCurRound;
								niCurRound->orderedNext = orderedLoop->orderedNext;
								niCurRound->orderedPre = orderedLoop;
								orderedLoop->orderedNext = niCurRound;
								orderedLoop = niCurRound;
							}
							niCurRound = NULL;
						}else{
							//将niCurRound从基本连接中删除。
							niCurRound->next->pre = niCurRound->pre;
							niCurRound->pre->next = niCurRound->next;
							//将niCurRound加入有序连接中。
							if(orderedLoop == NULL){
								orderedLoop = niCurRound;
								niCurRound->orderedNext = niCurRound;
								niCurRound->orderedPre = niCurRound;
							}else{
								orderedLoop->orderedNext->orderedPre = niCurRound;
								niCurRound->orderedNext = orderedLoop->orderedNext;
								niCurRound->orderedPre = orderedLoop;
								orderedLoop->orderedNext = niCurRound;
								orderedLoop = niCurRound;
							}

						}

						niCurRound = nextNi;
						continue;
					
				}
				

			}
			

			//接着就不是特殊情况了，我们可以采用直接找寻下一个点啦，


			for(int l = 0; l < niCurRound->facetSz; l++){
				NewIntersection* nextNi = NULL;
				for(int k = 0; k < niCurRound->facet[l]->intersectCount; k++){
					if(niCurRound->facet[l]->intsects[k] != niCurRound 
						&& niCurRound->facet[l]->intsects[k]->visited == 0){
						
							nextNi = niCurRound->facet[l]->intsects[k];
					}
				}

				if(nextNi != NULL && nextNi->visited == 0){
					
					niCurRound->visited = 1;
					if(niCurRound == niVec){
						if(niVec->next == niVec){
							niVec = NULL;
						}else{
							niVec = niVec->next;
						}
					}
					
					if(niCurRound->next == niCurRound){
						
						//将niCurRound加入有序连接中。
						if(orderedLoop == NULL){
							orderedLoop = niCurRound;
							niCurRound->orderedNext = niCurRound;
							niCurRound->orderedPre = niCurRound;
						}else{
							orderedLoop->orderedNext->orderedPre = niCurRound;
							niCurRound->orderedNext = orderedLoop->orderedNext;
							niCurRound->orderedPre = orderedLoop;
							orderedLoop->orderedNext = niCurRound;
							orderedLoop = niCurRound;
						}
						niCurRound = NULL;
					}else{
						//将niCurRound从基本连接中删除。
						niCurRound->next->pre = niCurRound->pre;
						niCurRound->pre->next = niCurRound->next;
						//将niCurRound加入有序连接中。
						if(orderedLoop == NULL){
							orderedLoop = niCurRound;
							niCurRound->orderedNext = niCurRound;
							niCurRound->orderedPre = niCurRound;
						}else{
							orderedLoop->orderedNext->orderedPre = niCurRound;
							niCurRound->orderedNext = orderedLoop->orderedNext;
							niCurRound->orderedPre = orderedLoop;
							orderedLoop->orderedNext = niCurRound;
							orderedLoop = niCurRound;
						}
					}
					niCurRound = nextNi;
					continue;
				}

				/*if(niCurRound->facet[l] != NULL && niCurRound->facet[l]->visited == 0){
					 
					niCurRound->facet[l]->visited = 1;
					for(int q = 0; q < 2; q++){
					

					
						if(niCurRound->facet[l]->intsects[q] != niCurRound){
				
							if(niCurRound->next != niCurRound){//并非最后一个剩下的元素了。
					
								if(niCurRound == niVec){//当前全局指向的就是我要删除的，这会导致后续操作错误。
									if(niVec->next == niVec){
										niVec = NULL;
									}else{
										niVec = niVec->next;
									}
								}

								niCurRound->pre->next = niCurRound->next;
								niCurRound->next->pre = niCurRound->pre;
								
								//这里貌似现在没有删除额。

								NewIntersection* newNi = niCurRound->facet[l]->intsects[q];
								//采用向后的添加方式。
								newNi->orderedNext = niCurRound->orderedNext;
								newNi->orderedPre = niCurRound;
								niCurRound->orderedNext->orderedPre = newNi;
								niCurRound->orderedNext = newNi;
								
								niCurRound = newNi;
								//
							}else{
								//delete niCurRound;	
								niCurRound = NULL;
							}
							break;
						}
					}
					break;
				}*/
			}
		//}

			//如果经历了一次查询，都没有改变了的话就，将这最后一个点插入然后回头了。



			int outt = false;
			if(niCurRound == oldNi){
				if(firNi != NULL){
					niCurRound = firNi;
					firNi = NULL;
					outt = true;
					break;
				}else{
					outt = true;
					break;
				}
				
			}
			if(outt){
				break;
			}

		}


		while(1){
			NewIntersection* oldNi = niCurRound;
			
			for(int l = 0; l < 2; l++){
			

				if(niCurRound->facet[l] != NULL && niCurRound->facet[l]->visited == 0){
					niCurRound->facet[l]->visited = 1;
					for(int q = 0; q < 2; q++){
					
					
						if(niCurRound->facet[l]->intsects[q] != niCurRound){
				
							if(niCurRound->next != niCurRound){//并非最后一个剩下的元素了。
					

								if(niCurRound == niVec){//当前全局指向的就是我要删除的，这会导致后续操作错误。
									if(niVec->next == niVec){
										niVec = NULL;
									}else{
										niVec = niVec->next;
									}
								}
								niCurRound->pre->next = niCurRound->next;
								niCurRound->next->pre = niCurRound->pre;
								
								
								NewIntersection* newNi = niCurRound->facet[l]->intsects[q];
								//采用向前的添加方式。
								newNi->orderedPre = niCurRound->orderedPre;
								newNi->orderedNext = niCurRound;
								niCurRound->orderedPre->orderedNext = newNi;
								niCurRound->orderedPre = newNi;
								
								niCurRound = newNi;

							}else{
							

								niCurRound = NULL;
							}
							break;

						}
					}
					break;
				}
			}

			int outt = false;
			if(niCurRound == oldNi){
				if(firNi != NULL){
					niCurRound = firNi;
					firNi = NULL;
					outt = true;
					break;
				}else{
					outt = true;
					break;
				}
				
			}
			
			if(outt){
				break;
			}

		}


		//上面的计算是有问题的，会导致不停止，故而只有通过这下面的额外判断进行终止的判断。
		static NewIntersection* oooo = NULL;
		if(oooo == niCurRound){
			break;
		}else{
			loopNiVec.push_back(niCurRound);
		}
		
		oooo = niCurRound;
		
		



		//break;


	}

			


		
	for(int i = 0; i < loopNiVec.size(); i++){
		NewIntersection* ttNi = loopNiVec.at(i);
		int j = 0;
		do{
			border[j][0][i] = ttNi->pos.m_data[0];
			border[j][1][i] = ttNi->pos.m_data[1];
			border[j][2][i] = ttNi->pos.m_data[2];


			j++;
			ttNi = ttNi->orderedNext;
		}while(ttNi != loopNiVec.at(i));
		sz[i] = j;
	}
	num = loopNiVec.size();




}
