#pragma once
#include "DataUtility.h"


class NewVertex{
public:
	NewVertex(){

	}
	NewVertex(double x, double y, double z){
		pos = Point3d(x, y, z);
	}
	Point3d pos;



};