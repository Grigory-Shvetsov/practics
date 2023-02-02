#pragma once

#include <vector>

#include <string>
#include <sstream>

template<typename NameT, typename WeightT, typename CostT>
struct Item
{
	NameT name;
	size_t count;
	WeightT weight;
	CostT cost;

	// debug
	/*std::string info() {
		std::stringstream ss;
		ss << this->name << ": weight = " << this->weight << "; cost = " << this->cost
			<< "; count = " << this->count << "; sum cost = " << (this->cost) * (this->count);
		return ss.str();
	};*/
	std::string info() {
		std::stringstream ss;
		ss << this->name << ": count = " << this->count;
		return ss.str();
	};
};

template<typename NameT, typename WeightT, typename CostT>
using Items = std::vector<Item<NameT, WeightT, CostT>>;



/////////////////////////////////////////////////////////////////////////////
// ����������� �������

// ������� ����� ��������, �� � ���������� �����������
template<typename NameT, typename WeightT, typename CostT,
	typename ItemT = typename Item<NameT, WeightT, CostT>>
	ItemT set_count(ItemT item, size_t count) {
	ItemT res(item);
	res.count = count;
	return res;
}

// ����������� ��������
// Example: join_vectors({1, 2, 3}, {3, 4, 5}) -> {1, 2, 3, 3, 4, 5}
template<typename T>
std::vector<T> join_vectors(std::vector<T>& l, const std::vector<T>& r) {
	l.reserve(l.size() + r.size());
	for (auto& item : r)
		l.push_back(item);
	return l;
};

/////////////////////////////////////////////////////////////////////////////



// ��������� ��� �������� �������
template<typename NameT, typename WeightT, typename CostT,
	typename ItemT = typename Item<NameT, WeightT, CostT>,
	typename ItemsT = typename Items<NameT, WeightT, CostT>
>
struct Solution {

	// TODO ����� ���������� � ���-�� ����� std::pair<NameT, size_t>, size_t - ���-�� ��������
	std::vector<ItemsT> results = std::vector<ItemsT>{};
	CostT total_cost = 0;

	// ���� ���������� item != 0, ��������� � ������ ������� ������� item � ������������� total_cost
	void append_item(ItemT item) {
		//std::cout << "results count = " << results.size() << std::endl;

		if (item.count == 0)
			return;

		// ���� ������� ��� ���, �� �������� ���� ������ ������� � ����� ������� � ����� ������� �������
		if (results.size() == 0)
			results.push_back(ItemsT{});

		for (ItemsT& items : results)
			items.push_back(item);

		total_cost += CostT(item.count) * item.cost;
	}

	// �� �� �����, ��� � append_item(item), �� � ����������� ������ ����������
	void append_item(ItemT item, size_t count) {
		append_item(set_count<NameT, WeightT, CostT>(item, count));
	}
};
