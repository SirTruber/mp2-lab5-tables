#pragma once

#define _CRT_SECURE_NO_WARNINGS
 
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include "../polinom/include/polinom.h"

using namespace std;
struct T 
{
	string  _s;
	polinom _p;
	T(const T& out) :
		_s(out._s),
		_p(out._p)
	{};
	const T& operator=(const T& out)
	{
		if (this != &out)
		{
			_s = out._s;
			_p = out._p;
		}
		return *this;
	};
	T(T&& out) noexcept
	{
		_s = move(out._s);
		_p = move(out._p);
	}
	T& operator=(T&& out) noexcept
	{
		if (this != &out)
		{
			_s = move(out._s);
			_p = move(out._p);
		}
		return *this;
	}
};

struct loger
{

	string _name;
	size_t _op = 0;
	time_t _t = time(NULL);

	loger(const string& s) :_name(s) {};
	~loger()
	{
		ofstream logfile("user.log");
		logfile <<ctime(&_t) <<":"<< _name << "-count of op:" << _op;
		logfile.close();
	}
	loger& operator++() { _op++; return *this; };
	loger operator++(int) { loger tmp = *this; ++_op; return tmp; };
};

class AVL
{
private:
	struct node
	{
		shared_ptr<T> _data;
		unsigned char _height;
		node* _left;
		node* _right;
		node(shared_ptr<T> data) :
			_data(data),
			_height(1),
			_left(nullptr),
			_right(nullptr)
		{};
	};

	node* root;

	// \brief Обёртка над высотой узла
	// \param n Указатель на выбранный узел
	// \return Высоту выбранного узла, если он не NULL, ноль в обратном случае
	unsigned char getHeight(const node* n);

	// \brief Восстанавливает высоту узла  
	// param n Указатель на выбранный узел
	void setHeight(node* n);

	// \brief Считает диссбаланс узла
	// \param n Указатель на выбранный узел
	// \return Разница между левым и правым поддеревом
	int diff(const node* n);

	node* lRotate(node* n);

	node* rRotate(node* n);
	
	// \brief Производит балансировку узла
	// \param n Указатель на выбранный узел
	// \return Указатель на новую вершину
	node* balance(node* n);

public:
	const weak_ptr<T> find(const string& target);
	bool insert(const shared_ptr<T> target);
	const shared_ptr<T> remove(const string& target);
};

class myHash 
{
private:
	constexpr static const double _rehash_size = 0.6;

	vector<shared_ptr<T>> _buffer;
	size_t                   _count = 0;
	
	// \brief Высчитывает хэш строки
	// \param s Целевая строка
	// \param size Размер буффера
	// \return Хэш для выбранной строки
	size_t getHash(const string& s);

	// \brief Перехеширует таблицу в более объемную
	void rehash();
public:
	myHash(size_t size = 16) :_buffer(size) {};
	const weak_ptr<T> find(const string& target);
	bool insert(const shared_ptr<T> target);
	const shared_ptr<T> remove(const string& target);
};

class tables
{
private:
	vector<shared_ptr<T>>         _vec;
	AVL                           _tree;
	myHash                        _hash;
public:
	const weak_ptr<T> find(const string& target);
	bool insert(const shared_ptr<T> target);
	const shared_ptr<T> remove(const string& target);
};