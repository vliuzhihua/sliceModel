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
	//���ȱ���һ�飬Ȼ��Ѱ��һ����ǡ���İ�Χ�У�Ȼ��������μ����е�epsilonֵ��
	
	vector<NewVertex*> nvVec = s.vtxs;
	Box box = s.GetBox();
	
	
	//cout << box;
	//��Χ�м������******************************************

	//2.
	double epsilon = (box.b - box.s).length() / 1000000000;
	//�����µõ���epsilon��Ȼ���������ƽ���ϵĶ���ĸ���������Ƚ��١�
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
			//��˫��ѭ������������Ӽ��ɡ�
			if(niVec == NULL){
				niVec = tmpNi;
			}else{
				niVec->next->pre = tmpNi;
				tmpNi->next = niVec->next;
				tmpNi->pre = niVec;
				niVec->next = tmpNi;
			}
			//����һ��������ѯ�ı��У�ע������
			nvToNiMap.insert(pair<NewVertex*, NewIntersection*>(nvVec[i], tmpNi));

			j++;
		}
	}
	
	cout << "inplane:" << vtxInPlaneVec.size() << endl;
	//���¼����������ƽ���ϵĶ���Ƚ��ٵļ��裬���ܹ���Ч�ʡ��������ʵ��̫���ˡ�

	//3.������Щ��ƽ���ϵĵ㶼����Щ���С�
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
	//ͨ����һ������������е���ƽ���ϵĵ������Ϣ��


	//int cb = 0;
	
	
	vector<NewEdge*> neVec = s.edges;

	
	//vector<NewEdgeIntersect*> neiVec;

	map<NewIntersection*, NewIntersection*> edgeCoupleMap;
	//4.
	//������е��н��߶Ρ�
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
		
		if((nv1->pos.dot(np.abc) + np.D) * (nv2->pos.dot(np.abc) + np.D) < 0){//������жϺ��п��ܳ��ָ����������жϲ���ȷ�������
			//����ļ��㷽ʽ���ܷǳ��Ĳ����١�
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
					//����Ӧ������Ҳ����Ͻ������Ϣ��
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
			//û�н��㡣
		}

	}

	
	int cBorderNum = 0;
	NewIntersection* niTemp = NULL;
	int markRunTime = 0;

	int roundCount = 0;


	//5.
	//���Ӽ��㡣

	vector<NewIntersection*> loopNiVec;

	list<int> li;
	
	//li.erase(li.);
		
	while(niVec){//��niVecΪ������Ϊֹͣ������
		NewIntersection* niCurRound = niVec;
		NewIntersection* firNi = niVec;
		NewIntersection* orderedLoop = NULL;
		while(1){

			NewIntersection* oldNi = niCurRound;
			
			
			//���ȼ���Ƿ���һ���������ϵıߡ��������ôִ����Ӧ�Ĳ�����Ȼ��������ɡ�
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
						
							//��niCurRound�������������С�
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
							//��niCurRound�ӻ���������ɾ����
							niCurRound->next->pre = niCurRound->pre;
							niCurRound->pre->next = niCurRound->next;
							//��niCurRound�������������С�
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


			//Ȼ�����Ƿ����������ϵĵ㣬����ǣ���ִ����Ӧ�Ĳ�����Ȼ��������ɡ�

			if(niCurRound->type == NewIntersection::VERTEX_TYPE){
				NewIntersection* nextNi = NULL;
				for(int i = 0; i < niCurRound->facetSz; i++){
					if(niCurRound->facet[i]->intersectCount >= 2 ){
						for(int j = 0; j < niCurRound->facet[j]->intersectCount; j++){
							if(niCurRound->facet[j]->intsects[j] != niCurRound 
								&& niCurRound->facet[j]->intsects[j]->visited == 0){

								nextNi = niCurRound->facet[j]->intsects[j];//����������ĺã�

							}
						}
					}
				}
				//��ʼ���д������ҵ�����һ���˵Ĵ���
				//������κ������Ƕ������Ƶģ�Ҫע��ѽ~~~
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
						
							//��niCurRound�������������С�
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
							//��niCurRound�ӻ���������ɾ����
							niCurRound->next->pre = niCurRound->pre;
							niCurRound->pre->next = niCurRound->next;
							//��niCurRound�������������С�
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
			

			//���žͲ�����������ˣ����ǿ��Բ���ֱ����Ѱ��һ��������


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
						
						//��niCurRound�������������С�
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
						//��niCurRound�ӻ���������ɾ����
						niCurRound->next->pre = niCurRound->pre;
						niCurRound->pre->next = niCurRound->next;
						//��niCurRound�������������С�
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
				
							if(niCurRound->next != niCurRound){//�������һ��ʣ�µ�Ԫ���ˡ�
					
								if(niCurRound == niVec){//��ǰȫ��ָ��ľ�����Ҫɾ���ģ���ᵼ�º�����������
									if(niVec->next == niVec){
										niVec = NULL;
									}else{
										niVec = niVec->next;
									}
								}

								niCurRound->pre->next = niCurRound->next;
								niCurRound->next->pre = niCurRound->pre;
								
								//����ò������û��ɾ���

								NewIntersection* newNi = niCurRound->facet[l]->intsects[q];
								//����������ӷ�ʽ��
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

			//���������һ�β�ѯ����û�иı��˵Ļ��ͣ��������һ�������Ȼ���ͷ�ˡ�



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
				
							if(niCurRound->next != niCurRound){//�������һ��ʣ�µ�Ԫ���ˡ�
					

								if(niCurRound == niVec){//��ǰȫ��ָ��ľ�����Ҫɾ���ģ���ᵼ�º�����������
									if(niVec->next == niVec){
										niVec = NULL;
									}else{
										niVec = niVec->next;
									}
								}
								niCurRound->pre->next = niCurRound->next;
								niCurRound->next->pre = niCurRound->pre;
								
								
								NewIntersection* newNi = niCurRound->facet[l]->intsects[q];
								//������ǰ����ӷ�ʽ��
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


		//����ļ�����������ģ��ᵼ�²�ֹͣ���ʶ�ֻ��ͨ��������Ķ����жϽ�����ֹ���жϡ�
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
