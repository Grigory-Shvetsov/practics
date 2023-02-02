#pragma once

#include "Item.h"

#include <functional>
#include <algorithm>
#include <cmath>

namespace BranchAndBoundMethod {

	/*Псевдокод функции find_bests(...)
	* Input: begin, end (итераторы, вектор должен быть отсортирован по убыванию удельной стоимости),
	*        доступный общий вес max_weight
	* Output: вектор решений res (которые тоже векторы, но уже предметов), их стоимость total_cost
	* 1. Инициализируем переменные: total_cost = 0; res - пустой вектор
	*    P.S. total_cost будет еще переменной для хранения текущего рекорда по стоимости
	* 2. Вычисляем, сколько максимум можем взять предмета begin (этим числом может быть и ноль) - max_count
	* 3. Если begin+1 == end (т.е. это последний предмет из входного списка предметов), то
	* 3.1. если max_count не ноль, возвращаем одно решение, состоящее из одного этого предмета в кол-ве max_count,
	*      total_cost = max_count * цена_предмета
	* 3.2. иначе пустой список решений и total_cost = 0
	* (3.) P.S. пункт 3 - это особый случай, чтобы выйти из рекурсии
	* 4. Вычисляем "по-честному" для max_count вектор результатов, total_cost и т.п.,
	*    вызывая find_bests(begin+1, end, max_weight - max_count * вес_предмета)
	*    (Скорее всего, лучше идти в обратном порядке, т.к. вектор отсортирован по удельной стоимости,
	*    т.е. в теории, нам лучше взять побольше более выгодных предметов)
	* 5. В каждый вектор результата добавляем begin в кол-ве max_count (если max_count != 0),
	*    total_cost += max_count * цена_предмета (т.к. find_bests в пункте 4 не посчитала дополнительно стоимость текущего предмета)
	* 6. Проходим циклом по кол-ву count = 0..max_count-1 (в обратном порядке; если max_count = 0, то не пройдем цикл ни разу)
	* 6.1. Делаем оценку сверху - maximum (лучше имени пока не придумал) (не забываем прибавить к этому count * стоимость предмета)
	* 6.2. Если maximum < total_cost, то переходим к следующей итерации
	* 6.3. Иначе считаем "по-честному", вызывая find_bests(begin+1, end, max_weight - count * вес_предмета)
	* 6.4. Пусть total_cost_count - результат total_cost вызова find_bests в пункте 6.3, не забываем прибавить
	*      total_cost_count += count * цена_предмета
	*      P.S. total_cost_count - типа total_cost для конкретного count, можно придумать имя получше
	* 6.5. Если результат этого вызова total_cost_count меньше total_cost, то переходим к следующей итерации
	* 6.6. Если total_cost_count == total_cost, то добавляем в список результатов новые решения,
	*      добавляя в каждое новое решение предмет begin в кол-ве count, если count != 0
	* 6.7. Если total_cost_count > total_cost, то очищаем вектор res и добаляем туда новые решения,
	*      добавляя в каждое новое решение предмет begin в кол-ве count, если count != 0
	* (6.) P.S. Если сильно хочется, то сюда в цикл можно добавить еще оценку снизу
	* (6.) P.P.S. Вектор решений, полученный из find_bests в этом цикле, может быть пустым только в том случае, если count == 0
	*           (например, если следующие предметы имеют вес больше текущего и не влезают в рюкзак; в другом случае мы уже взяли
	*           текущий предмет хотя бы один раз), но тогда total_cost_count равен нулю и мы просто пропустим эту итерацию
	*           из-за пункта 6.5
	* 7. Наконец, возвращаем результат
	*/

	// анонимный namespace для приватных методов
	namespace {
		// На основе метода отсюда: https://neerc.ifmo.ru/wiki/index.php?title=Задача_о_рюкзаке
		// Сложность O(n)
		template<typename NameT, typename WeightT, typename CostT,
			typename ItemT = typename Item<NameT, WeightT, CostT>,
			typename ItemsT = typename Items<NameT, WeightT, CostT>,
			typename It = typename ItemsT::const_iterator>
			CostT find_best_greedily(It begin, It end, WeightT max_weight) {
			CostT max_cost = 0;

			for (It i = begin; i != end; ++i) {
				ItemT cur_item = *i;

				// максимальное кол-во по весу
				size_t coef = (size_t)(max_weight / cur_item.weight);

				// Все, что есть, влезает в рюкзак
				if (coef > cur_item.count) {
					max_weight -= WeightT(cur_item.count) * cur_item.weight;
					max_cost += CostT(cur_item.count) * cur_item.cost;
				}
				// Иначе считаем дробным количесвом и выходим из цикла
				else {
					max_cost += CostT(std::ceil(double(max_weight) / cur_item.weight * cur_item.cost));

					break;
				}

			}

			return max_cost;
		}

		template<typename NameT, typename WeightT, typename CostT,
			typename ItemT = typename Item<NameT, WeightT, CostT>,
			typename ItemsT = typename Items<NameT, WeightT, CostT>,
			typename SolutionT = typename Solution<NameT, WeightT, CostT>,
			typename It = typename ItemsT::const_iterator>
			SolutionT find_bests(It begin, It end, WeightT max_weight) {
			// итератор на следующий элемент
			auto next = std::next(begin);
			ItemT cur_item = *begin;

			// 1
			SolutionT sol{};

			if (max_weight == 0) {
				return sol;
			}

			// 2
			// нам нужно округление вниз, поэтому при делении ничего к double приводить не надо
			size_t max_count = std::min(cur_item.count, (size_t)(max_weight / cur_item.weight));

			// 3
			if (next == end) {
				sol.append_item(cur_item, max_count);

				return sol;
			}

			// 4
			sol = find_bests<NameT, WeightT, CostT>(next, end, max_weight - WeightT(max_count) * cur_item.weight);

			// 5
			sol.append_item(cur_item, max_count);

			// 6
			// Т.к. size_t - беззнаковый тип, то при вычислении (--0) происходит переполнение и
			// count становится равен максимальному значению типа size_t (т.е. SIZE_MAX).
			// Но это выглядит немного костыльно и ненадежно
			for (size_t count = max_count - 1; count != SIZE_MAX; --count) {

				//if (max_count >= 1)
				//for (size_t count = 0; count <= max_count - 1; ++count) {

				//for (size_t count = 0; count <= max_count; ++count) {

				WeightT max_weight_count = max_weight - WeightT(count) * cur_item.weight;

				// 6.1
				CostT total_cost_count = find_best_greedily<NameT, WeightT, CostT>(next, end,
					max_weight_count) + CostT(count) * cur_item.cost;

				// 6.2
				if (total_cost_count < sol.total_cost)
					continue;

				// 6.3
				SolutionT newSol = find_bests<NameT, WeightT, CostT>(next, end, max_weight_count);

				// 6.4
				total_cost_count = newSol.total_cost + CostT(count) * cur_item.cost;

				// 6.5
				if (total_cost_count < sol.total_cost)
					continue;

				// 6.6-6.7
				newSol.append_item(cur_item, count);
				// 6.6
				if (total_cost_count == sol.total_cost)
					join_vectors(sol.results, newSol.results);
				// 6.7
				else
					sol = newSol;
			}

			return sol;
		}

	}

	template<typename NameT, typename WeightT, typename CostT>
	Solution<NameT, WeightT, CostT> problemSolve(Items<NameT, WeightT, CostT>& items, WeightT max_weight) {

		// сортируем по убыванию удельной стоимости
		std::sort(items.begin(), items.end(),
			[](Item<NameT, WeightT, CostT> l, Item<NameT, WeightT, CostT> r) -> bool {
				return (double(l.cost) / l.weight) > (double(r.cost) / r.weight);
			}
		);

		return find_bests<NameT, WeightT, CostT>(items.cbegin(), items.cend(), max_weight);
	}

}