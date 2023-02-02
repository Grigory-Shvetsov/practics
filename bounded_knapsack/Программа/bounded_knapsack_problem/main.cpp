// я поменял размер стека на 64 мегабайта, чтобы не падать с stack overflow при
// рекурсии, по этой ссылке:
// https://docs.microsoft.com/ru-ru/cpp/build/reference/stack-stack-allocations?view=msvc-160

#include <iostream>

#include <ctime>
#include <chrono>
#include <iomanip>

#include "BranchAndBoundMethod.h"
#include "DynamicProgrammingMethod.h"

// TODO нужен пример, когда решений нет

template<typename NameT, typename WeightT, typename CostT>
void print_solution(Solution<NameT, WeightT, CostT> sol) {
	std::cout << "max cost:            " << sol.total_cost << std::endl;
	std::cout << "number of solutions: " << sol.results.size() << std::endl;
	for (size_t i = 0; i < sol.results.size(); ++i) {
		std::cout << "---solution " << i + 1 << "---" << std::endl;
		WeightT sum_weight = 0;
		for (auto& item : sol.results[i])
			sum_weight += WeightT(item.count) * item.weight;

		std::cout << "sum weight:          " << sum_weight << std::endl;

		for (auto& item : sol.results[i])
			std::cout << item.info() << std::endl;
	}
}

template<typename NameT, typename WeightT, typename CostT>
void short_print_solution(Solution<NameT, WeightT, CostT> sol) {
	std::cout << "max cost:            " << sol.total_cost << std::endl;
	std::cout << "number of solutions: " << sol.results.size() << std::endl;

	if (sol.results.size() <= 3)
		for (size_t i = 0; i < sol.results.size(); ++i) {
			std::cout << "---solution " << i + 1 << "---" << std::endl;
			size_t itemsCount = sol.results[i].size();
			std::cout << "items count:         " << itemsCount << std::endl;
			WeightT sum_weight = 0;
			for (auto& item : sol.results[i])
				sum_weight += WeightT(item.count) * item.weight;

			std::cout << "sum weight:          " << sum_weight << std::endl;
		}
}

/* Возвращает псевдослучайное число из диапазона[min, max)*/
size_t Random(size_t min, size_t max) {
	return min + rand() % (max - min);
}

// maxCount - максимальное количество одного предмета
Items<size_t, size_t, size_t> generateItems(
	size_t itemsCount,
	size_t maxCount,
	size_t maxWeight,
	size_t maxCost
) {
	Items<size_t, size_t, size_t> res{};
	res.reserve(itemsCount);

	for (size_t i = 0; i < itemsCount; ++i) {
		res.push_back(Item<size_t, size_t, size_t>{
			i + 1,
				Random(1, maxCount),
				Random(1, maxWeight),
				Random(1, maxCost)
		});

		/*res.emplace_back(
			i + 1,
			Random(1, maxCount),
			Random(1, maxWeight),
			Random(1, maxCost)
		);*/
	}

	return res;
}

int main()
{
	using ItemT = Item<std::string, int, int>;

	// 1. пример отсюда:
	//https://ru.stackoverflow.com/questions/867446/Написанием-алгоритма-о-рюкзаке-используя-метод-ветвей-и-границ
	std::vector<ItemT> example1{
		// имя, кол-во, вес, стоимость
		{ "item 1", 1, 200, 40 },
		{ "item 2", 1, 314, 50 },
		{ "item 3", 1, 198, 100 },
		{ "item 4", 1, 500, 60 },
		{ "item 5", 1, 300, 30 },
		{ "item 6", 1, 400, 45 }
	};
	int max_weight1 = 1500;

	////auto res = BranchAndBoundMethod::problemSolve(example1, max_weight1);
	//// ожиадаемый результат: total_cost = 265
	////print_solution(res);

	//std::cout << std::endl << "//////////////////////////" << std::endl;

	//// 2. пример отсюда:
	//// https://www.codeproject.com/Articles/706838/Bounded-Knapsack-Algorithm
	std::vector<ItemT> example2{
		// имя, кол-во, вес, стоимость
		{"Apple", 4, 39, 40}, //          1
		{"Banana", 4, 27, 60}, //         2
		{"Beer", 12, 52, 10}, //          3
		{"Book", 2, 30, 10}, //           4
		{"Camera", 1, 32, 30}, //         5
		{"Cheese", 4, 23, 30}, //         6
		{"Chocolate Bar", 10, 15, 60}, // 7
		{"Compass", 1, 13, 35}, //        8
		{"Jeans", 1, 48, 10}, //          9
		{"Map", 1, 9, 150}, //            10
		{"Notebook", 1, 22, 80}, //       11
		{"Sandwich", 4, 50, 160}, //      12
		{"Ski Jacket", 1, 43, 75}, //     13
		{"Ski Pants", 1, 42, 70}, //      14
		{"Socks", 2, 4, 50}, //           15
		{"Sunglasses", 1, 7, 20}, //      16
		{"Suntan Lotion", 1, 11, 70}, //  17
		{"T-Shirt", 1, 24, 15}, //        18
		{"Tin", 1, 68, 45}, //            19
		{"Towel", 1, 18, 12}, //          20
		{"Umbrella", 1, 73, 40}, //       21
		{"Water", 1, 153, 200} //         22
	};
	int max_weight2 = 1000;
	// ожидаемый результат
	/*
	Knapsack Capacity : 1000
	Filled Weight : 999
	Filled Value : 2535
	Contents :
		Apple(3)
		Banana(4)
		Cheese(4)
		Chocolate Bar(10)
		Compass(1)
		Map(1)
		Notebook(1)
		Sandwich(4)
		Ski Jacket(1)
		Ski Pants(1)
		Socks(2)
		Sunglasses(1)
		Suntan Lotion(1)
		T - Shirt(1)
		Water(1)*/
	std::vector<ItemT> example3{
		// имя, кол-во, вес, стоимость
		{ "item 1", 6, 54, 8 },
		{ "item 2", 5, 63, 12 },
		{ "item 3", 4, 55, 2 },
		{ "item 4", 3, 63, 7 },
		{ "item 5", 8, 97, 6 },
		{ "item 6", 6, 26, 11 },
		{ "item 7", 4, 91, 16 },
		{ "item 8", 7, 42, 2 },
		{ "item 9", 5, 8, 18 },
		{ "item 10",6, 18, 3 }
	};
	int max_weight3 = 1000;

	//auto startBnbEx2 = clock();
//using namespace std::chrono;
//std::cout << std::setprecision(7);
//high_resolution_clock::time_point BB1 = high_resolution_clock::now();
//res = BranchAndBoundMethod::problemSolve(example2, max_weight2);
//high_resolution_clock::time_point BB2 = high_resolution_clock::now();
//std::cout << "time: " << duration_cast<duration<double>>(BB2 - BB1).count() << " seconds" << std::endl;
////auto endBnbEx2 = clock();
////std::cout << "time: " << double(endBnbEx2 - startBnbEx2) / CLOCKS_PER_SEC << " seconds" << std::endl;
//print_solution(res);

/*std::cout << std::endl << "//////////////////////////";
std::cout << std::endl << "//*  *  *  *  *  *  *  *//*/
std::cout << std::endl << "//////////////////////////" << std::endl;

std::cout << std::endl << "dynamic:" << std::endl;
res = DynamicProgrammingMethod::problemSolve(example1, max_weight1);
print_solution(res);

std::cout << std::endl << "//////////////////////////" << std::endl;

high_resolution_clock::time_point DP1 = high_resolution_clock::now();
res = DynamicProgrammingMethod::problemSolve(example2, max_weight2);
high_resolution_clock::time_point DP2 = high_resolution_clock::now();
std::cout << "time: " << duration_cast<duration<double>>(DP2 - DP1).count() << " seconds" << std::endl;
print_solution(res);*/

///*
	std::cout << std::endl << "-------------------------" << std::endl;

	//auto items = generateItems(100, 10, 100, 20);
	//size_t max_weight = 1000;

	// тут ДП выигрывает
	//auto items = generateItems(500, 10, 50, 100);
	//size_t max_weight = 2000;

	// ДП сильно выигрывает
	//auto items = generateItems(500, 10, 50, 100);
	//size_t max_weight = 2900;

	// ДП невероятно сильно выигрывает
	//auto items = generateItems(500, 10, 50, 100);
	//size_t max_weight = 3000;

	//auto items = generateItems(800, 10, 50, 100);
	/*auto items = generateItems(200, 10, 50, 100);
	size_t max_weight = 10000;*/

	// стандартный пример, метод ветвей и границ выигрывает в 150 раз
	//auto items = generateItems(500, 10, 50, 100);
	//size_t max_weight = 100000;

	// очень сложный пример, его выполняет только ДП с одним возвращаемым решением
	// остальные варианты не работают (происходит stack overflow), скорее всего, из-за рекурсии
	//auto items = generateItems(50000, 10, 50, 100);
	//size_t max_weight = 10000;

	//for (auto& item : items) {
	//	std::cout << item.info() << std::endl;
	//}

	/*auto startDyn = clock();
	auto resDyn = DynamicProgrammingMethod::problemSolve(items, max_weight);
	auto endDyn = clock();
	std::cout << "dynamic:" << std::endl;
	std::cout << "time:                " << double(endDyn - startDyn) / CLOCKS_PER_SEC << " seconds" << std::endl;*/
	//short_print_solution(resDyn);
	//print_solution(resDyn);

	std::cout << std::endl;

	/*auto startBnb = clock();
	auto resBnb = BranchAndBoundMethod::problemSolve(items, max_weight);
	auto endBnb = clock();
	std::cout << "branch and bound:" << std::endl;
	std::cout << "time:                " << double(endBnb - startBnb) / CLOCKS_PER_SEC << " seconds" << std::endl;*/
	//short_print_solution(resBnb);
	//print_solution(resBnb);


	//setlocale(LC_ALL, "Russian");

	auto items = generateItems(130, 10, 50, 100);
	size_t max_weight = 10000;

	using namespace std::chrono;
	std::cout << std::setprecision(7);
	high_resolution_clock::time_point BB_1 = high_resolution_clock::now();
	auto res_1 = BranchAndBoundMethod::problemSolve(items, max_weight);
	high_resolution_clock::time_point BB_2 = high_resolution_clock::now();
	std::cout << "BranchAndBound time: " << std::endl << duration_cast<duration<double>>(BB_2 - BB_1).count() << " seconds" << std::endl;
	//short_print_solution(res_1);
	//print_solution(res_1);
	/*std::cout << std::endl << "//////////////////////////";
	std::cout << std::endl << "//*  *  *  *  *  *  *  * //";
	std::cout << std::endl << "//////////////////////////" << std::endl;*/

	high_resolution_clock::time_point DP_1 = high_resolution_clock::now();
	auto res_2 = DynamicProgrammingMethod::problemSolve(items, max_weight);
	high_resolution_clock::time_point DP_2 = high_resolution_clock::now();
	std::cout << std::endl << "DynamicProgramming time: " << std::endl << duration_cast<duration<double>>(DP_2 - DP_1).count() << " seconds" << std::endl;
	//short_print_solution(res_2);
	//print_solution(res_2);
	//std::cout << std::endl << "//////////////////////////";
	//std::cout << std::endl << "//*  *  *  *  *  *  *  *//";
	//std::cout << std::endl << "//////////////////////////" << std::endl;


	//high_resolution_clock::time_point BB1 = high_resolution_clock::now(); // начальное время
	//auto res1 = BranchAndBoundMethod::problemSolve(items, max_weight);
	//high_resolution_clock::time_point BB2 = high_resolution_clock::now(); // конечное время
	//duration<double> time_span_BB = duration_cast<duration<double>>(BB2 - BB1);
	//std::cout << std::endl << "МВГ. Вместимость: " << max_weight << std::endl;
	//std::cout << "Время BranchAndBoundMethod: " << std::setprecision(7) << time_span_BB.count() << std::endl;
	//

	//std::cout << std::endl << "--------------------------------------------" << std::endl;

	//high_resolution_clock::time_point DP1 = high_resolution_clock::now(); // начальное время
	//auto res2 = DynamicProgrammingMethod::problemSolve(items, max_weight);
	//high_resolution_clock::time_point DP2 = high_resolution_clock::now(); // конечное время
	//duration<double> time_span_DP = duration_cast<duration<double>>(DP2 - DP1);
	//std::cout << std::endl << "ДП. Вместимость: " << max_weight << std::endl;
	//std::cout << "Время DynamicProgrammingMethod: " << std::setprecision(7) << time_span_DP.count() << std::endl;


}
