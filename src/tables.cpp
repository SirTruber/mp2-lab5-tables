#include "../include/tables.h"

size_t myHash::getHash(const string& s) 
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

void myHash::rehash()
{
	loger op("hash::rehash");
	myHash res(_buffer.size()*2);
	for (auto it = _buffer.begin(); it != _buffer.end(); ++it) 
	{
		if (*it)
			res.insert(*it);
	}
	std::swap(_buffer, res._buffer);
}

constexpr bool Euclid(unsigned int a,unsigned int b)
{
	return a == b ? a == 1 
		: a > b ? Euclid(a - b, b) : Euclid(b - a, a);
};

const weak_ptr<T> myHash::find(const string& target)
{
	loger op("hash::find");
	size_t h = getHash(target);
	size_t gap = _buffer.size()/2 + 1;
	while (!Euclid(gap, _buffer.size())) gap--;
	
	for (int i = 0; !_buffer[h] && i < _buffer.size(); ++i)
	{
		++op;
		if (_buffer[h]->_s == target && _buffer[h])
		{
			return _buffer[h];
		}
		h = (h + gap) % _buffer.size();
	}
	return {};
}

bool myHash::insert(const shared_ptr<T> target)
{
	if (_count + 1 > static_cast<int>(_rehash_size * _buffer.size()))
	{
		rehash();
	}
	loger op("hash::insert");
	size_t h = getHash(target->_s);
	size_t gap = _buffer.size() / 2 + 1;
	while (!Euclid(gap, _buffer.size())) gap--;

	for (int i = 0; !_buffer[h] && i < _buffer.size(); ++i)
	{
		++op;
		if (_buffer[h]->_s == target->_s && _buffer[h])
		{
			return false;
		}

		h = (h + gap) % _buffer.size();
	}
	 
	_buffer[h] = target;
	++_count;

	return true;
}

const shared_ptr<T> myHash::remove(const string& target)
{
	loger op("hash::remove");
	size_t h = getHash(target);
	size_t gap = _buffer.size() / 2 + 1;
	while (!Euclid(gap, _buffer.size())) gap--;

	for (int i = 0; !_buffer[h] && i < _buffer.size(); ++i)
	{
		++op;
		if (_buffer[h]->_s == target && _buffer[h])
		{
			shared_ptr<T> tmp;
			_buffer[h].swap(tmp);
			return tmp;
		}
		h = (h + gap) % _buffer.size();
	}
	return {};
}

unsigned char AVL::getHeight(const node* n)
{
	return n ? n->_height : 0;
};

void AVL::setHeight(node* n)
{
		n->_height = max(getHeight(n->_left), getHeight(n->_right)) + 1;
}

int AVL::diff(const node* n)
{
	return getHeight(n->_left) - getHeight(n->_right);
}

AVL::node* AVL::lRotate(node* n)
{
	node* r = n->_right;
	n->_right = r->_left;
	r->_left = n;
	setHeight(n); setHeight(r);
	return r;
}

AVL::node* AVL::rRotate(node* n)
{
	node* l = n->_left;
	n->_left = l->_right;
	l->_right = n;
	setHeight(n); setHeight(l);
	return l;
}

AVL::node* AVL::balance(node* n)
{
	setHeight(n);

	if (diff(n) == 2)
	{
		if (diff(n->_right) < 0)
			n->_right = rRotate(n->_right);
		return lRotate(n);
	}
	if (diff(n) == -2)
	{
		if (diff(n->_left) > 0)
			n->_left = lRotate(n->_left);
		return rRotate(n);
	}
	return n;
}

const weak_ptr<T> AVL::find(const string& target)
{
	loger op("AVL::find");
	for (node* tmp = root; tmp != nullptr; tmp = target > tmp->_data->_s ?
		tmp->_right : tmp->_left)
	{
		++op;
		if (tmp->_data->_s == target)
			return tmp->_data;
	}
	return {};
}
bool AVL::insert(const shared_ptr<T> target)
{
	loger op("AVL::insert");
	if (root == nullptr)
	{
		++op;
		root = new node(target);
		return true;
	}
	
	node* insertNode = nullptr, * primeNode = nullptr;

	for (node * tmp = root; tmp != nullptr; tmp = target->_s > tmp->_data->_s ?
		tmp->_right : tmp->_left)
	{
		++op;
		insertNode = tmp;
		if (primeNode) insertNode->_height++;
		if (!primeNode && diff(insertNode) != 0)
			primeNode = insertNode;
	}

	target->_s > insertNode->_data->_s ?
		insertNode->_right : insertNode->_left = new node(target);

	for (node* tmp = primeNode; tmp != insertNode; tmp = target->_s > tmp->_data->_s ?
		tmp->_right : tmp->_left)
	{
		++op;
		tmp = balance(tmp);
	}
	
	++op;
	primeNode = balance(primeNode);
	return true;
};
const shared_ptr<T> AVL::remove(const string& target)
{
	loger op("AVL::remove");
	if (!root)
	{
		++op;
		return {};
	}

	node* removeNode = nullptr, * primeNode = nullptr;

	for (node* tmp = root; tmp->_data->_s == target; tmp = target > tmp->_data->_s ?
		tmp->_right : tmp->_left)
	{
		++op;
		if (!tmp) return {};
		if (primeNode) tmp->_height--;
		else
		{
			node* brother = tmp == removeNode->_left ? removeNode->_right : removeNode->_left;
			if (diff(tmp) == 0 ||
				diff(brother) == 0 && tmp->_height < brother->_height)
			{
				primeNode = tmp;
			}
		}

		removeNode = tmp;
	}
	
	removeNode->_height = 0;

	for (node* tmp = primeNode; tmp != removeNode; tmp = target > tmp->_data->_s ?
		tmp->_right : tmp->_left)
	{
		++op;
		tmp = balance(tmp);
	}
	
	++op;
	primeNode = balance(primeNode);

	shared_ptr<T> ret;
	swap(removeNode->_data, ret);
	delete removeNode;
	return ret;
};

const weak_ptr<T> tables::find(const string& target)
{
	loger op("vector::find");
	weak_ptr<T> ret;
	for (int i = 0; i < _vec.size(); ++i)
	{
		++op;
		if (_vec[i]->_s == target)
		{
			ret = _vec[i];
			break;
		}
	}

	if (ret.lock() == _tree.find(target).lock() && ret.lock() == _hash.find(target).lock())
	{
		return ret;
	}
	return {};
};

bool tables::insert(const shared_ptr<T> target) 
{
	loger op("vector::insert");
	size_t size = _vec.capacity();

	_vec.push_back(target);
	if (size == _vec.capacity()) ++op;
	else op._op += _vec.size();

	return (_tree.insert(target) && _hash.insert(target));
};

const shared_ptr<T> tables::remove(const string& target)
{
	loger op("vector::remove");
	shared_ptr<T> ret ;

	for (int i = 0; i < _vec.size(); ++i)
	{
		++op;
		if (_vec[i]->_s == target)
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