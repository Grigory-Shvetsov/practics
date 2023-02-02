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
	file >> count; //��������� ���-�� �����
	cout << "���������� �����: " << count << endl;
	Point point;
	for (size_t i = 0; i < count; ++i) //��������� �����
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
	ofstream result("result.txt"); //������ ���� � �������� �������
	result << SolutionPoints_.size() << endl; //���������� ���-�� ������� ����� � ����
	for (size_t i = 0; i < SolutionPoints_.size(); ++i) //���������� ����� � ����
	{
		result << setw(17) << setprecision(17) << SolutionPoints_[i].x << "\t";
		result << setw(17) << setprecision(17) << SolutionPoints_[i].y << endl;
	}
	result.close();
}
#endif