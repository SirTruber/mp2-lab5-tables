#pragma once

using namespace std;

template <typename K, typename D>
class AVL
{
private:
	struct node
	{
		shared_ptr<D>    _data;
		unsigned char    _height;
		unique_ptr<node> _left;
		unique_ptr<node> _right;
		node(shared_ptr<D> data) :
			_data(data),
			_height(1)
		{};
	};

	unique_ptr<node> root;

	// \brief Обёртка над высотой узла
	// \param n Указатель на выбранный узел
	// \return Высоту выбранного узла, если он не NULL, ноль в обратном случае
	unsigned char getHeight(const node * n)
	{
		return n ? n->_height : 0;
	};

	// \brief Восстанавливает высоту узла  
	// \param n Указатель на выбранный узел
	void setHeight(node* n)
	{
		n->_height = max(getHeight(n->_left.get()), getHeight(n->_right.get())) + 1;
	};

	// \brief Считает диссбаланс узла
	// \param n Указатель на выбранный узел
	// \return Разница между левым и правым поддеревом
	int diff(const node * n)
	{ 
		return getHeight(n->_left.get()) - getHeight(n->_right.get()); 
	};

	node* lRotate(node* n) 
	{
		node * r = n->_right.get();
		n->_right = move(r->_left);
		r->_left = unique_ptr<node>(n);
		
		setHeight(n); setHeight(r);
		return r;
	};

	node* rRotate(node* n)
	{
		node* l = n->_left.get();
		n->_left = move( l->_right);
		l->_right = unique_ptr<node>(n);

		setHeight(n); setHeight(l);
		return l;
	};

	// \brief Производит балансировку узла
	// \param n Указатель на выбранный узел
	// \return Указатель на новую вершину
	node* balance(node* n)
	{
		setHeight(n);

		if (diff(n) == 2)
		{
			if (diff(n->_right.get()) < 0)
				n->_right = unique_ptr<node>(rRotate(n->_right.get()));
			return lRotate(n);
		}
		if (diff(n) == -2)
		{
			if (diff(n->_left.get()) > 0)
				n->_left = unique_ptr<node>(lRotate(n->_left.get()));
			return rRotate(n);
		}
		return n;
	};

public:
	const weak_ptr<D> find(const K& target)
	{
		loger op("AVL::find");
		for (node * tmp = root.get(); tmp != nullptr; tmp = *tmp->_data < target ?
			tmp->_right.get() : tmp->_left.get())
		{
			++op;
			if (*tmp->_data == target)
				return tmp->_data;
		}
		return {};
	};
	bool insert(const shared_ptr<D> target, const K& key) 
	{
		loger op("AVL::insert");
		if (!root)
		{
			++op;
			root = make_unique<node>(target);
			return true;
		}

		node* insertNode = nullptr, * primeNode = nullptr;

		for (node* tmp = root.get(); tmp != nullptr; tmp = *tmp->_data < key ?
			tmp->_right.get() : tmp->_left.get())
		{
			++op;

			if (*tmp->_data == key)
				return false;

			insertNode = tmp;
			if (primeNode) insertNode->_height++;
			if (!primeNode && diff(insertNode) != 0)
				primeNode = insertNode;
		}

		*insertNode->_data < key ?
			insertNode->_right : insertNode->_left = make_unique<node>(target);

		for (node* tmp = primeNode; tmp != insertNode; tmp = *tmp->_data < key ?
			tmp->_right.get() : tmp->_left.get())
		{
			++op;
			tmp = balance(tmp);
		}

		++op;
		primeNode = balance(primeNode);
		return true;
	};
	const shared_ptr<D> remove(const K& target) 
	{
		loger op("AVL::remove");
		if (!root)
		{
			++op;
			return {};
		}

		node* removeNode = nullptr, * primeNode = nullptr;

		for (node* tmp = root.get(); *tmp->_data == target; tmp = *tmp->_data < target ?
			tmp->_right.get() : tmp->_left.get())
		{
			++op;
			if (!tmp) return {};
			if (primeNode) tmp->_height--;
			else
			{
				node* brother = tmp == removeNode->_left.get() ? removeNode->_right.get() : removeNode->_left.get();
				if (diff(tmp) == 0 ||
					diff(brother) == 0 && tmp->_height < brother->_height)
				{
					primeNode = tmp;
				}
			}

			removeNode = tmp;
		}

		removeNode->_height = 0;

		for (node* tmp = primeNode; tmp != removeNode; tmp = *tmp->_data < target ?
			tmp->_right.get() : tmp->_left.get())
		{
			++op;
			tmp = balance(tmp);
		}

		++op;
		primeNode = balance(primeNode);

		shared_ptr<D> ret;
		swap(removeNode->_data, ret);

		return ret;
	};
};