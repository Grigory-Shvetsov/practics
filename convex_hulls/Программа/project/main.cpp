#include"Point.h"
#include"Perebor.cpp"
#include"FileVector.cpp"
#include"RandomFile.cpp"
#include"SimpleAlgorithm.h"
#include"KirkpatrickSeidelAlgorithm.h"
#include<iomanip>
#include<chrono>
#include<vector>
#include<thread>

int main()
{

	setlocale(LC_ALL, "Russian");
	vector<Point> ProblemPoints, SolutionPoints_pb, SolutionPoints_ks, SolutionPoints_sp;
	//ProblemPoints = FileToVector(RandomFile(100000));
    ProblemPoints = RandomVector(1000000);
	//ProblemPoints = FileToVector("question.txt");
	using namespace std::chrono;


	high_resolution_clock::time_point ks1 = high_resolution_clock::now(); // начальное время
	SolutionPoints_ks = KSAlgorithm::convexHull(ProblemPoints);
	high_resolution_clock::time_point ks2 = high_resolution_clock::now(); // конечное время
	duration<double> time_span_ks = duration_cast<duration<double>>(ks2 - ks1);
	cout << "Точки выпуклой оболочки: " << SolutionPoints_ks.size() << endl;
	cout << "Время KirkpatrickSeidelAlgorithm: " << setprecision(7) << time_span_ks.count() << endl;
	
	//high_resolution_clock::time_point sp1 = high_resolution_clock::now(); // начальное время
	//SolutionPoints_sp = SimpleAlgorithm::сonvexHull(ProblemPoints);
	//high_resolution_clock::time_point sp2 = high_resolution_clock::now(); // конечное время
	//duration<double> time_span_sp = duration_cast<duration<double>>(sp2 - sp1);
	//cout << "Точки выпуклой оболочки: " << SolutionPoints_sp.size() << endl;
	//cout << "Время SimpleAlgorithm: " << setprecision(7) << time_span_sp.count() << endl;
	
	high_resolution_clock::time_point pb1 = high_resolution_clock::now(); // начальное время
	SolutionPoints_pb = perebor(ProblemPoints);
	high_resolution_clock::time_point pb2 = high_resolution_clock::now(); // конечное время
	duration<double> time_span_pb = duration_cast<duration<double>>(pb2 - pb1);
	cout << "Точки выпуклой оболочки: " << SolutionPoints_pb.size() << endl;
	cout << "Время PereborAlgorithm: " << setprecision(7) << time_span_pb.count() << endl;

	

	VectorToFile(SolutionPoints_pb);
	/*cout << "Количество точек: 10000000" << endl;
	cout << "Точки выпуклой оболочки: 16" << endl;
	cout << "Время KirkpatrickSeidelAlgorithm: 0.9105154" << endl;
	cout << "Точки выпуклой оболочки: 16" << endl;
	cout << "Время PereborAlgorithm: 17.04947"<< endl;*/
}