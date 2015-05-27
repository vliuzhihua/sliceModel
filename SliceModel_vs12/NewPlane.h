#pragma once

#include <fstream>
using std::ifstream;

class NewPlane{
public:
	Tuple3d abc;
	double D;

	friend istream& operator >> (istream& in, NewPlane &np){
		in >> np.abc;
		in >> np.D;

		return in;
	}
	friend ostream& operator << (ostream& out, NewPlane &np){
		out << np.abc << " " << ((double)np.D);

		return out;
	}
};