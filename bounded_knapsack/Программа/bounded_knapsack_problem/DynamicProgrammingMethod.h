#pragma once

#include "Item.h"

#include <algorithm>
#include <stdexcept>

namespace DynamicProgrammingMethod {

	namespace {
		// объединение решений
		template<typename NameT, typename WeightT, typename CostT,
			typename ItemT = typename Item<NameT, WeightT, CostT>,
			typename ItemsT = typename Items<NameT, WeightT, CostT>,
			typename SolutionT = typename Solution<NameT, WeightT, CostT>
		>
			SolutionT join_solution(SolutionT& l, const SolutionT& r) {
			if (l.total_cost != r.total_cost)
				throw std::exception("sets with different costes!");

			join_vectors(l.results, r.results);

			return l;
		}


		// TODO Мне кажется, что если искать все решения, то сложность в худшем случае будет аналогична перебору
		// восстановление всех решений
		template<typename NameT, typename WeightT, typename CostT,
			typename ItemT = typename Item<NameT, WeightT, CostT>,
			typename ItemsT = typename Items<NameT, WeightT, CostT>,
			typename SolutionT = typename Solution<NameT, WeightT, CostT>
		>
			SolutionT find_ans(
				const std::vector<std::vector<CostT>>& dp,
				const ItemsT& items,
				size_t k, WeightT s) {

			if (k == 0 || s == 0)
				return SolutionT{};

			// количество предметов k отсчитывается с 1, но индексы массива идут с 0
			ItemT cur_item = items[k - 1];

			size_t max_count = std::min(cur_item.count, (size_t)(s / cur_item.weight));

			// возьмем в качестве начальных значений случай, если возьмем 0 раз предмет k
			CostT max_value = dp[k - 1][s];
			std::vector<size_t> max{ 0 };

			for (size_t i = 1; i <= max_count; ++i) {
				CostT value = dp[k - 1][s - i * cur_item.weight] + CostT(i) * cur_item.cost;

				if (value == max_value)
					max.push_back(i);
				else if (value > max_value) {
					max_value = value;
					max.clear();
					max.push_back(i);
				}
			}

			// в max будет хотя бы один индекс
			SolutionT sol = find_ans<NameT, WeightT, CostT>(dp, items, k - 1, s - max[0] * cur_item.weight);
			sol.append_item(cur_item, max[0]);

			for (size_t i = 1; i < max.size(); ++i) {
				SolutionT new_sol = find_ans<NameT, WeightT, CostT>(dp, items, k - 1, s - max[i] * cur_item.weight);
				new_sol.append_item(cur_item, max[i]);

				join_solution<NameT, WeightT, CostT>(sol, new_sol);
			}

			return sol;
		}

		// восстановление одного решения без рекурсии
		template<typename NameT, typename WeightT, typename CostT,
			typename ItemT = typename Item<NameT, WeightT, CostT>,
			typename ItemsT = typename Items<NameT, WeightT, CostT>,
			typename SolutionT = typename Solution<NameT, WeightT, CostT>
		>
			SolutionT find_ans_simple(
				const std::vector<std::vector<CostT>>& dp,
				const ItemsT& items,
				size_t k, WeightT s) {

			if (k == 0 || s == 0)
				return SolutionT{};

			SolutionT sol{};

			for (; k > 0; --k) {
				if (s <= 0)
					break;

				// количество предметов k отсчитывается с 1, но индексы массива идут с 0
				ItemT cur_item = items[k - 1];

				size_t max_count = std::min(cur_item.count, (size_t)(s / cur_item.weight));

				// возьмем в качестве начальных значений случай, если возьмем 0 раз предмет k
				CostT max_value = dp[k - 1][s];
				size_t max = 0;

				for (size_t i = 1; i <= max_count; ++i) {
					CostT value = dp[k - 1][s - i * cur_item.weight] + CostT(i) * cur_item.cost;
					if (value > max_value) {
						max_value = value;
						max = i;
					}
				}

				sol.append_item(cur_item, max);

				s -= cur_item.weight * max;
			}

			return sol;
		}
	}

	// каноническая версия
	/*template<typename NameT, typename WeightT, typename CostT>
	Solution<NameT, WeightT, CostT> problemSolve(const Items<NameT, WeightT, CostT>& items, WeightT backpackWeight) {
		if (items.size() == 0 || backpackWeight == 0)
			return Solution<NameT, WeightT, CostT>{};

		std::vector<std::vector<CostT>> dp(items.size() + 1, std::vector<CostT>(backpackWeight + 1, 0));

		// сложность алгоритма O(n w^2)
		//
		// Перебираем для каждого i все вместимости
		for (size_t i = 1; i <= items.size(); ++i) {
			for (size_t c = 1; c <= backpackWeight; ++c) {
				dp[i][c] = dp[i - 1][c];
				//Ищем l для которого выполняется максимум
				for (size_t l = std::min((size_t)(items[i - 1].count), (size_t)(c / items[i - 1].weight)); l >= 1; --l) {
					dp[i][c] = std::max(dp[i][c], (CostT)(dp[i - 1][c - l * items[i - 1].weight] + items[i - 1].cost * l));
				}
			}
		}

		return find_ans<NameT, WeightT, CostT>(dp, items, items.size(), backpackWeight);
	}*/

	// оптимизированная по времени версия
	template<typename NameT, typename WeightT, typename CostT>
	Solution<NameT, WeightT, CostT> problemSolve(const Items<NameT, WeightT, CostT>& items, WeightT backpackWeight) {
		if (items.size() == 0 || backpackWeight == 0)
			return Solution<NameT, WeightT, CostT>{};

		//auto start = clock();

		std::vector<std::vector<CostT>> dp(items.size() + 1, std::vector<CostT>(backpackWeight + 1, 0));

		//auto end2 = clock();
		//std::cout << double(end2 - start) / CLOCKS_PER_SEC << " seconds" << std::endl;

		// сложность алгоритма O(n w^2)
		// Перебираем для каждого i все вместимости
		CostT max = 0;
		Item<NameT, WeightT, CostT> itemi_1;
		std::vector<CostT> dpi_1;
		for (size_t i = 1; i <= items.size(); ++i) {
			itemi_1 = items[i - 1];
			dpi_1 = dp[i - 1];
			for (size_t c = 1; c <= backpackWeight; ++c) {
				max = dpi_1[c];
				//Ищем l для которого выполняется максимум
				for (size_t l = std::min((size_t)(itemi_1.count), (size_t)(c / itemi_1.weight)); l >= 1; --l) {
					max = std::max(max, (CostT)(dpi_1[c - l * itemi_1.weight] + itemi_1.cost * l));
				}
				dp[i][c] = max;
			}
		}

		//auto end = clock();
		//std::cout << "time dp: " << double(end - start) / CLOCKS_PER_SEC << " seconds" << std::endl;

		return find_ans<NameT, WeightT, CostT>(dp, items, items.size(), backpackWeight);
		//return find_ans_simple<NameT, WeightT, CostT>(dp, items, items.size(), backpackWeight);

	}

}