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
// утилитарные функции

// Создает копию предмета, но с измененным количеством
template<typename NameT, typename WeightT, typename CostT,
	typename ItemT = typename Item<NameT, WeightT, CostT>>
	ItemT set_count(ItemT item, size_t count) {
	ItemT res(item);
	res.count = count;
	return res;
}

// Объединение векторов
// Example: join_vectors({1, 2, 3}, {3, 4, 5}) -> {1, 2, 3, 3, 4, 5}
template<typename T>
std::vector<T> join_vectors(std::vector<T>& l, const std::vector<T>& r) {
	l.reserve(l.size() + r.size());
	for (auto& item : r)
		l.push_back(item);
	return l;
};

/////////////////////////////////////////////////////////////////////////////



// структура для описания решения
template<typename NameT, typename WeightT, typename CostT,
	typename ItemT = typename Item<NameT, WeightT, CostT>,
	typename ItemsT = typename Items<NameT, WeightT, CostT>
>
struct Solution {

	// TODO лучше переделать в что-то вроде std::pair<NameT, size_t>, size_t - кол-во предмета
	std::vector<ItemsT> results = std::vector<ItemsT>{};
	CostT total_cost = 0;

	// если количсетво item != 0, добавляет в каждое решение элемент item и пересчитывает total_cost
	void append_item(ItemT item) {
		//std::cout << "results count = " << results.size() << std::endl;

		if (item.count == 0)
			return;

		// если решений еще нет, то создадим одно пустое решение и потом добавим в новое решение предмет
		if (results.size() == 0)
			results.push_back(ItemsT{});

		for (ItemsT& items : results)
			items.push_back(item);

		total_cost += CostT(item.count) * item.cost;
	}

	// то же самое, что и append_item(item), но с возможность выбора количества
	void append_item(ItemT item, size_t count) {
		append_item(set_count<NameT, WeightT, CostT>(item, count));
	}
};
