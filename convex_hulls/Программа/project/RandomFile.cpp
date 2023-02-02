#ifndef RANDOMFILE
#define RANDOMFILE
#include <fstream>
#include <iomanip> 
#include <string> 
#include <ctime> 
#include <vector>
#include "Point.h"
#include <iostream>
using namespace std;
static string RandomFile(size_t N)
{
	srand(time(NULL));
	double a = -1.0;
	double b = 1.0;
	ofstream file("question.txt");
	file << N << endl;
	for (size_t i = 0; i < N; ++i)
		file << setw(17) << setprecision(17) << (b - a) * rand() / RAND_MAX + a << "\t"
		<< setw(17) << setprecision(17) << (b - a) * rand() / RAND_MAX + a << endl;
	file.close();
	string inputfile("question.txt");
	return inputfile;
}

static vector<Point> RandomVector(size_t N)
{
	srand(time(NULL));
	double a = -1.0;
	double b = 1.0;
	vector<Point> vec;
	Point point;
	for (size_t i = 0; i < N; ++i)
	{
		point.x = (b - a) * rand() / RAND_MAX + a;
		point.y = (b - a) * rand() / RAND_MAX + a;
		vec.push_back(point);
	}
	cout << "Количество точек: " << N << endl;
	return vec;
}


#endif

