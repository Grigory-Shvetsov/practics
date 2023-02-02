#pragma once

#include "Item.h"

#include <functional>
#include <algorithm>
#include <cmath>

namespace BranchAndBoundMethod {

	/*��������� ������� find_bests(...)
	* Input: begin, end (���������, ������ ������ ���� ������������ �� �������� �������� ���������),
	*        ��������� ����� ��� max_weight
	* Output: ������ ������� res (������� ���� �������, �� ��� ���������), �� ��������� total_cost
	* 1. �������������� ����������: total_cost = 0; res - ������ ������
	*    P.S. total_cost ����� ��� ���������� ��� �������� �������� ������� �� ���������
	* 2. ���������, ������� �������� ����� ����� �������� begin (���� ������ ����� ���� � ����) - max_count
	* 3. ���� begin+1 == end (�.�. ��� ��������� ������� �� �������� ������ ���������), ��
	* 3.1. ���� max_count �� ����, ���������� ���� �������, ��������� �� ������ ����� �������� � ���-�� max_count,
	*      total_cost = max_count * ����_��������
	* 3.2. ����� ������ ������ ������� � total_cost = 0
	* (3.) P.S. ����� 3 - ��� ������ ������, ����� ����� �� ��������
	* 4. ��������� "��-��������" ��� max_count ������ �����������, total_cost � �.�.,
	*    ������� find_bests(begin+1, end, max_weight - max_count * ���_��������)
	*    (������ �����, ����� ���� � �������� �������, �.�. ������ ������������ �� �������� ���������,
	*    �.�. � ������, ��� ����� ����� �������� ����� �������� ���������)
	* 5. � ������ ������ ���������� ��������� begin � ���-�� max_count (���� max_count != 0),
	*    total_cost += max_count * ����_�������� (�.�. find_bests � ������ 4 �� ��������� ������������� ��������� �������� ��������)
	* 6. �������� ������ �� ���-�� count = 0..max_count-1 (� �������� �������; ���� max_count = 0, �� �� ������� ���� �� ����)
	* 6.1. ������ ������ ������ - maximum (����� ����� ���� �� ��������) (�� �������� ��������� � ����� count * ��������� ��������)
	* 6.2. ���� maximum < total_cost, �� ��������� � ��������� ��������
	* 6.3. ����� ������� "��-��������", ������� find_bests(begin+1, end, max_weight - count * ���_��������)
	* 6.4. ����� total_cost_count - ��������� total_cost ������ find_bests � ������ 6.3, �� �������� ���������
	*      total_cost_count += count * ����_��������
	*      P.S. total_cost_count - ���� total_cost ��� ����������� count, ����� ��������� ��� �������
	* 6.5. ���� ��������� ����� ������ total_cost_count ������ total_cost, �� ��������� � ��������� ��������
	* 6.6. ���� total_cost_count == total_cost, �� ��������� � ������ ����������� ����� �������,
	*      �������� � ������ ����� ������� ������� begin � ���-�� count, ���� count != 0
	* 6.7. ���� total_cost_count > total_cost, �� ������� ������ res � �������� ���� ����� �������,
	*      �������� � ������ ����� ������� ������� begin � ���-�� count, ���� count != 0
	* (6.) P.S. ���� ������ �������, �� ���� � ���� ����� �������� ��� ������ �����
	* (6.) P.P.S. ������ �������, ���������� �� find_bests � ���� �����, ����� ���� ������ ������ � ��� ������, ���� count == 0
	*           (��������, ���� ��������� �������� ����� ��� ������ �������� � �� ������� � ������; � ������ ������ �� ��� �����
	*           ������� ������� ���� �� ���� ���), �� ����� total_cost_count ����� ���� � �� ������ ��������� ��� ��������
	*           ��-�� ������ 6.5
	* 7. �������, ���������� ���������
	*/

	// ��������� namespace ��� ��������� �������
	namespace {
		// �� ������ ������ ������: https://neerc.ifmo.ru/wiki/index.php?title=������_�_�������
		// ��������� O(n)
		template<typename NameT, typename WeightT, typename CostT,
			typename ItemT = typename Item<NameT, WeightT, CostT>,
			typename ItemsT = typename Items<NameT, WeightT, CostT>,
			typename It = typename ItemsT::const_iterator>
			CostT find_best_greedily(It begin, It end, WeightT max_weight) {
			CostT max_cost = 0;

			for (It i = begin; i != end; ++i) {
				ItemT cur_item = *i;

				// ������������ ���-�� �� ����
				size_t coef = (size_t)(max_weight / cur_item.weight);

				// ���, ��� ����, ������� � ������
				if (coef > cur_item.count) {
					max_weight -= WeightT(cur_item.count) * cur_item.weight;
					max_cost += CostT(cur_item.count) * cur_item.cost;
				}
				// ����� ������� ������� ���������� � ������� �� �����
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
			// �������� �� ��������� �������
			auto next = std::next(begin);
			ItemT cur_item = *begin;

			// 1
			SolutionT sol{};

			if (max_weight == 0) {
				return sol;
			}

			// 2
			// ��� ����� ���������� ����, ������� ��� ������� ������ � double ��������� �� ����
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
			// �.�. size_t - ����������� ���, �� ��� ���������� (--0) ���������� ������������ �
			// count ���������� ����� ������������� �������� ���� size_t (�.�. SIZE_MAX).
			// �� ��� �������� ������� ��������� � ���������
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

		// ��������� �� �������� �������� ���������
		std::sort(items.begin(), items.end(),
			[](Item<NameT, WeightT, CostT> l, Item<NameT, WeightT, CostT> r) -> bool {
				return (double(l.cost) / l.weight) > (double(r.cost) / r.weight);
			}
		);

		return find_bests<NameT, WeightT, CostT>(items.cbegin(), items.cend(), max_weight);
	}

}