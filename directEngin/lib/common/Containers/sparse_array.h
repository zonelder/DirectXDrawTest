#pragma once
#include "sparse_set.h"
#include <vector>

template<typename Data,typename Entity,unsigned N>
class SparseArray
{
public:


	using key_type = Entity;
	using sparse_set_type = SparseSet<Entity, N>;
	using data_densed_container_type = std::vector<Data>;

/*
	class reverse_iterator
	{
	public:

		reverse_iterator(data_densed_container_type::iterator it) :_curIt(it) {}

		auto operator++(int) const noexcept
		{
			auto copy = *this;
			++(*this);
			return copy;
		}

		auto operator++() noexcept
		{
			--_curIt;
			return *this;
		}

		bool operator==(const reverse_iterator& other) const noexcept
		{
			return _curIt == other._curIt;
		}

		bool operator!=(const reverse_iterator& other) const noexcept
		{
			return _curIt != other._curIt;
		}

		auto& operator*() const noexcept
		{
			return *_curIt;
		}

	private:
		sparse_set_type::reverse_iterator _itEnttID;
		data_densed_container_type::iterator _curIt;
	};
	*/
	SparseArray()
	{
		_data.reserve(N);
	}




	auto ebegin() noexcept
	{
		return _set.begin();
	}

	auto eend() noexcept
	{
		return _set.end();
	}
	/*
	* 	auto begin() noexcept
	{
		// using reverse_iterator to enshure that modification of SparseArray
		// during iteration will not trigger any exeptions
		return reverse_iterator(_data.begin() + _data.size()); 
	}
	auto end() noexcept
	{
		return reverse_iterator(_data.begin());
	*/

	bool contains(key_type key) const
	{
		return _set.contains(key);
	}

	Data& operator[](key_type key)
	{
		return _data[_set[key]];
	}

	const Data& operator[](key_type key) const
	{
		return _data[_set[key]];
	}

	Data& add(key_type key)
	{
		if(_set.contains(key))
			return this->operator[](key);

		_set.add(key);
		return _data.emplace_back();
	}


	bool remove(key_type key)
	{
		if (!_set.contains(key))
			return false;

		std::swap(_data[_set[key]], _data.back());
		_set.remove(key);
		_data.pop_back();
		return true;

	}

	auto begin()
	{
		return _data.begin();
	}
	auto end()
	{
		return _data.end();
	}

	auto size() const noexcept
	{
		return _set.size();
	}
		 
private:

	sparse_set_type _set;
	data_densed_container_type _data;

};