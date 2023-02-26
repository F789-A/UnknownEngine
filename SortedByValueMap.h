#pragma once
#include <unordered_map>
#include <set>

//Все действия по ключу, но итератор сортированный по значению
template<typename K, typename V>
class SortedByValueMap
{
private:
	struct cmp
	{
		std::unordered_map<K, V>& cont;
		K key;

		cmp(K key, std::unordered_map<K, V>& cont) : key(key), cont(cont){}

		bool operator<(const cmp& a) const
		{
			return (cont.at(key) < cont.at(a.key) ||
				(!(cont.at(key) < cont.at(a.key)) && !(cont.at(a.key) < cont.at(key)) && key < a.key));
		}
	};

	std::unordered_map<K, V> store;
	std::set<cmp> sorted;
public:

	class iterator
	{
		typename std::set<cmp>::iterator iter;
		std::unordered_map<K, V>& cont;

	public:
		iterator(std::unordered_map<K, V>& cont, std::set<cmp>::iterator it) : cont(cont)
		{
			iter = it;
		}

		iterator& operator++()
		{
			iter++;
			return *this;
		}

		bool operator==(const iterator& it) const
		{
			return (this->iter == it.iter);
		}
		bool operator!=(const iterator& it) const
		{
			return !(*this == it);
		}

		auto operator->()
		{
			return &(*cont.find(iter->key));
		}
	};

	iterator begin()
	{
		return iterator(store, sorted.begin());
	}

	iterator end()
	{
		return iterator(store, sorted.end());
	}

	void remove(const K& key)
	{
		sorted.erase({ key, store });
		store.erase(key);
	}

	void insert(const K& key, V val)
	{
		store.insert(std::make_pair(key, std::move(val)));
		sorted.insert({ key, store });
	}
	
	void resortElement(const K& key)
	{
		for (auto l = sorted.begin(); l != sorted.end(); ++l)
		{
			if (l->key == key)
			{
				sorted.erase(l);
				break;
			}
		}
		sorted.insert({ key, store });
	}

	V& at(const K& key)
	{
		return store.at(key);
	}

	V& operator[](const K& key)
	{
		return store[key];
	}

	int count()
	{
		return store.size();
	}
};