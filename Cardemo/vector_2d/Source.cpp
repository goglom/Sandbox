#include "vector_2d.h"

#include <iostream>
#include <numbers>
#include <cmath>
#include <vector>

using namespace std;


ostream& operator<<(ostream& ostr, const vector_2d<float>& vector)
{
	ostr << "(" << vector.x << ", " << vector.y << ")";
	return ostr;
}


int main()
{
	vector<int> a(5);

	a.push_back(4);

	cout << a.front();
	
	return 0;
}