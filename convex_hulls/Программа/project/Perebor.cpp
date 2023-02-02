#ifndef PEREBOR
#define PEREBOR
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include "Point.h"
using namespace std;

static ostream& operator<< (ostream& out, const vector<Point>& Set)
{
	for (size_t i = 0; i < Set.size(); ++i)
	{
		out << setw(18) << setprecision(17) << Set[i].x << "\t" << setw(18) << setprecision(17) << Set[i].y << endl;
	}
	return out;
}

static bool is_equal(double x, double y)
{
	const double eps = min(fabs(x), fabs(y)) * 1e-10;
	return fabs(x - y) <= eps;
}
static ostream& operator<< (ostream& out, const Point& Set)
{
	out << setw(17) << "( " << Set.x << " , " << setw(17) << Set.y << " )" << endl;
	return out;
}



static size_t leftbottom_point(vector<Point>& ProblemPoints_)
{
	//���� ������� �� ������ ����� ����� ������� (����� �����)
	size_t k = 0;
	vector<Point> vec;
	for (size_t i = 1; i < ProblemPoints_.size() - 1; ++i)
		if ((ProblemPoints_[i].x < ProblemPoints_[k].x) ||
			(ProblemPoints_[i].x == ProblemPoints_[k].x && ProblemPoints_[i].y < ProblemPoints_[k].y))
			k = i;
	return k;
}

static vector<Point> perebor(vector<Point>& ProblemPoints_)
{
	size_t k = leftbottom_point(ProblemPoints_);
	/*cout << "���������� ����� ����� �����: " << ProblemPoints_[k] << endl;
	cout << "����� ��� �������:   " << k << endl << endl;*/

	vector<Point> SolutionPoints_; //������ ������� �����
	SolutionPoints_.push_back(ProblemPoints_[k]);
	swap(ProblemPoints_[k], ProblemPoints_.back());
	size_t count = ProblemPoints_.size() - 1;

	//��������
	size_t r = 0;
	size_t j = 0;
	Point q_1, q_2;
	do
	{
		for (size_t i = 1; i < count - r + 1; ++i)
		{
			q_1.x = ProblemPoints_[j].x - SolutionPoints_[r].x; //���������� x ��������� �������
			q_1.y = ProblemPoints_[j].y - SolutionPoints_[r].y; //���������� y ��������� �������
			q_2.x = ProblemPoints_[i].x - SolutionPoints_[r].x; //���������� x ������������� �������
			q_2.y = ProblemPoints_[i].y - SolutionPoints_[r].y; //���������� y ������������� �������

			/*cout << ProblemPoints_[j].x << "\t" << ProblemPoints_[j].y << endl;
			cout << ProblemPoints_[i].x << "\t" << ProblemPoints_[i].y << endl << endl;*/

			if ((q_1.x * q_2.y - q_2.x * q_1.y) < 0)
			{
				/*cout << "���������� �����, ����������� ���� ������ ([i]) : (" << ProblemPoints_[i].x << " , " << ProblemPoints_[i].y << ")" << endl;
				cout << "���������� ����� ��������� ������� ([j]) : (" << ProblemPoints_[j].x << " , " << ProblemPoints_[j].y << ")" << endl;*/
				j = i;
			}
			if (is_equal(q_1.x * q_2.y - q_2.x * q_1.y, 0.0))
			{
				if ((pow(q_1.x, 2) + pow(q_1.y, 2)) <= (pow(q_2.x, 2) + pow(q_2.y, 2)))
				{
					/*cout << "���������� �����, ����������� �� ������ ([i]) : (" << ProblemPoints_[i].x << " , " << ProblemPoints_[i].y << ")" << endl;
					cout << "���������� ����� ��������� ������� ([j]) : (" << ProblemPoints_[j].x << " , " << ProblemPoints_[j].y << ")" << endl;*/
					j = i;
				}

			}
		}
		SolutionPoints_.push_back(ProblemPoints_[j]);
		ProblemPoints_.erase(ProblemPoints_.cbegin() + j);

		j = 0;
		++r;
		/*cout << ProblemPoints_ << endl;
		cout << "������ ������� �����" << endl;
		cout << SolutionPoints_ << endl;*/


	} while (ProblemPoints_[count - r] == SolutionPoints_[0]);
	

	SolutionPoints_.erase(SolutionPoints_.cbegin() + SolutionPoints_.size() - 1);
	//cout << "������ ������� �����" << endl;
	//cout << SolutionPoints << endl << endl;
	
	return SolutionPoints_;
}
#endif