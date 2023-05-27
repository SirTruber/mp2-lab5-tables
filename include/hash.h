#pragma once

using namespace std;

template <typename K, typename D>
class myHash
{
private:
	vector<shared_ptr<D>>    _buffer;
	constexpr static const double _rehash_size = 0.6;
	size_t                   _count = 0;

	// \brief Высчитывает хэш строки
	// \param s Целевая строка
	// \return Хэш для выбранной строки
	size_t getHashString(const std::string& s)
	{
		size_t size = _buffer.size();
		const char* v = s.c_str();
		size_t h = 0, a = 127;
		for (; *v != 0; ++v)
		{
			h = (a * h + *v) % size;
		}
		return h;
	};

	// \brief Высчитывает хэш целого числа
	// \param i Целевое число
	// \return Хэш числа
	size_t getHashInt(const int& i)
	{
		size_t a = 16363;
		return static_cast<size_t>(a * static_cast<unsigned>(i) % _buffer.size());
	}
	// \brief Высчитывает хэш числа с плавающей точкой
	// \param d Целевое число
	// \return Хэш числа
	size_t getHashDouble(const double& d) 
	{
		double a = .61616;
		return static_cast<size_t>(fmod(a * d, 1) * _buffer.size());
	};

	size_t getHash(const K& key)
	{
		if (std::is_same<K, string>::value)
			return getHashString(key);
		//if (std::is_same<K, int>::value)
		//	return getHashInt(key);
		//if (std::is_same<K, double>::value)
		//	return getHashDouble(key);
		//static_assert(true,"Strange data type");
	};
	// \brief Проверяет два числа на взаимную простоту
	// \param a первое число
	// \param b второе число
	// \return true, если числа взаимопростые, false в обратном случае
	constexpr bool Euclid(unsigned int a, unsigned int b)
	{
		return a == b ? a == 1
			: a > b ? Euclid(a - b, b) : Euclid(b - a, a);
	};

	// \brief Перехеширует таблицу в более объемную
	/*void rehash()
	{
		loger op("hash::rehash");
		myHash res(_buffer.size() * 2);
		for (auto it = _buffer.begin(); it != _buffer.end(); ++it)
		{
			if (*it)
				res.insert(*it);
		}
		std::swap(_buffer, res._buffer);
	};*/
public:
	myHash(size_t size = 16) :_buffer(size) {};
	const weak_ptr<D> find(const K& target)
	{
		loger op("hash::find");
		size_t h = getHash(target);
		size_t gap = _buffer.size() / 2 + 1;
		while (!Euclid(gap, _buffer.size())) gap--;

		for (int i = 0; !_buffer[h] && i < _buffer.size(); ++i)
		{
			++op;
			if (*_buffer[h] == target && _buffer[h])
			{
				return _buffer[h];
			}
			h = (h + gap) % _buffer.size();
		}
		return {};
	};
	bool insert(const shared_ptr<D> target,const K& key) 
	{
		/*if (_count + 1 > static_cast<int>(_rehash_size * _buffer.size()))
		{
			rehash();
		}*/
		loger op("hash::insert");
		size_t h = getHash(key);
		size_t gap = _buffer.size() / 2 + 1;
		while (!Euclid(gap, _buffer.size())) gap--;

		for (int i = 0; !_buffer[h] && i < _buffer.size(); ++i)
		{
			++op;
			if (*_buffer[h] == key && _buffer[h])
			{
				return false;
			}

			h = (h + gap) % _buffer.size();
		}

		_buffer[h] = target;
		++_count;

		return true;
	};
	const shared_ptr<D> remove(const K& target)
	{
		loger op("hash::remove");
		size_t h = getHash(target);
		size_t gap = _buffer.size() / 2 + 1;
		while (!Euclid(gap, _buffer.size())) gap--;

		for (int i = 0; !_buffer[h] && i < _buffer.size(); ++i)
		{
			++op;
			if (*_buffer[h] == target && _buffer[h])
			{
				shared_ptr<D> tmp;
				_buffer[h].swap(tmp);
				return tmp;
			}
			h = (h + gap) % _buffer.size();
		}
		return {};
	};
};

