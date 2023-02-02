#ifndef FILEVECTOR
#define FILEVECTOR
#include<iostream>
#include<vector>
#include<fstream>
#include<iomanip>
#include<string>
#include"Point.h"
using namespace std;


static vector<Point> FileToVector(string inputfile)
{
	vector<Point> ProblemPoints;
	size_t count;

	ifstream file(inputfile);
	file >> count; //считываем кол-во точек
	cout << "Количество точек: " << count << endl;
	Point point;
	for (size_t i = 0; i < count; ++i) //считываем точки
	{
		file >> point.x;
		file >> point.y;
		ProblemPoints.push_back(point);
	}
	file.close();
	return ProblemPoints;
}

static void VectorToFile(vector<Point>& SolutionPoints_)
{
	ofstream result("result.txt"); //создаём файл с крайними точками
	result << SolutionPoints_.size() << endl; //записываем кол-во крайних точек в файл
	for (size_t i = 0; i < SolutionPoints_.size(); ++i) //записываем точки в файл
	{
		result << setw(17) << setprecision(17) << SolutionPoints_[i].x << "\t";
		result << setw(17) << setprecision(17) << SolutionPoints_[i].y << endl;
	}
	result.close();
}
#endif