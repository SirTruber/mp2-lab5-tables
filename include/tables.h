#pragma once

#define _CRT_SECURE_NO_WARNINGS
 
#include <vector>
#include <string>
#include <memory>
#include "logger.h"
#include "hash.h"
#include "tree.h"

#include "../polinom/include/polinom.h"

using namespace std;
struct Polinom_case
{
	string  _s;
	polinom _p;
	Polinom_case(const string& s, const polinom& p) :
		_s(s),
		_p(p) 
	{};
	Polinom_case(const Polinom_case& out) :
		_s(out._s),
		_p(out._p)
	{};
	const Polinom_case& operator=(const Polinom_case& out)
	{
		if (this != &out)
		{
			_s = out._s;
			_p = out._p;
		}
		return *this;
	};
	Polinom_case(Polinom_case&& out) noexcept
	{
		_s = move(out._s);
		_p = move(out._p);
	}
	Polinom_case& operator=(Polinom_case&& out) noexcept
	{
		if (this != &out)
		{
			_s = move(out._s);
			_p = move(out._p);
		}
		return *this;
	}
	bool operator == (const Polinom_case& right) { return _s == right._s; };
	bool operator != (const Polinom_case& right) { return _s != right._s; };
	bool operator == (const string & right)      { return _s == right; };
	bool operator != (const string& right)       { return _s != right; };
	bool operator >  (const string& right)       { return _s > right; };
	bool operator <  (const string& right)       { return _s < right; };
};

template <typename K, typename D>
class tables
{
private:
	vector<shared_ptr<D>>         _vec;
	AVL<K,D>                      _tree;
	myHash<K,D>                   _hash;
public:
	const weak_ptr<D> find(const K& target)
	{
		loger op("vector::find");
		weak_ptr<D> ret;
		for (size_t i = 0; i < _vec.size(); ++i)
		{
			++op;
			if (*_vec[i] == target)
			{
				ret = _vec[i];
				break;
			}
		}

		if (!(ret.lock() == _tree.find(target).lock())
		 || !(ret.lock() == _hash.find(target).lock()))
		{
			return {};
		}
		else
			return ret;
	}
	bool insert(const shared_ptr<D> target,const K& key)
	{
		loger op("vector::insert");
		size_t size = _vec.capacity();

		_vec.push_back(target);
		if (size == _vec.capacity()) ++op;
		else op._op += _vec.size();

		if (!_tree.insert(target, key) || !_hash.insert(target, key))
		{
			_vec.pop_back();
			return false;
		}
		else
			return true;
	}

	const shared_ptr<D> remove(const K& target)
	{
		loger op("vector::remove");
		shared_ptr<D> ret;

		for (size_t i = 0; i < _vec.size(); ++i)
		{
			++op;
			if (*_vec[i] == target)
			{
				ret = _vec[i];
				_vec[i].swap(_vec[_vec.size() - 1]);
				break;
			}
		}

		_vec.pop_back();
		if (_tree.remove(target) && _hash.remove(target))
			return ret;
		return {};
	};
};