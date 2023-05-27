#pragma once

#include <iostream>

// Контейнер monom обеспечивает хранение монома
struct monom
{
    double         _a;       // Коэффицент монома
    int            _xyz;     // Степени переменных в виде 100x + 10y + z. Степени не выше десяти

    /*\brief Конструктор со списком инициализаторов элементов
    * \param a Коэффицент монома
    * \param xyz Степени переменных
    */
    monom(double a = -1, int xyz = 1000) :
        _a(a),
        _xyz(xyz)
    {};

    bool operator==(const monom& r) const { return _xyz == r._xyz ; }; //Рассматриваем эти операторы как проверку на подобие
    bool operator!=(const monom& r) const { return _xyz != r._xyz ; };
    bool operator<(const monom& r)  const { return _xyz <  r._xyz ; };
    bool operator<=(const monom& r) const { return _xyz <= r._xyz ; };
    bool operator>(const monom& r)  const { return _xyz >  r._xyz ; };
    bool operator>=(const monom& r) const { return _xyz >= r._xyz ; };

    bool operator==(const double& r)const { return std::fabs(_a - r) < std::numeric_limits<double>::epsilon(); };
    bool operator!=(const double& r)const { return ! (* this == r); };
};

// Структура хранения списка 
template<typename T>
class sortedList
{
private:
    // Вершины, хранящие произвольные типы данных
    struct node
    {
        T          _data = T();        // Данные 
        node *     _next = nullptr;    // Указатель на следующий элемент
    };
    node* _head;   // Указатель на голову 
    node  _nil;    // Фиктивный узел
public:
    // Создает "пустой" список с фиктивным узлом
    sortedList() :
        _head(&_nil),
        _nil{ T(), &_nil }
    {};

    // Деструктор
    ~sortedList()
    {
        for (node* tmp = _head; _head != &_nil;_head = tmp)
        {
            tmp = _head->_next;
            delete _head;
        }
        _nil._next = nullptr;
    };

    //\brief Конструктор копии
    //\param right Копируемый список
    sortedList(const sortedList<T>& right):
        _head(&_nil),
        _nil{ T(),&_nil }
    {
        if (right.begin() == right.end()) return;
        _head = new node{T(*right.begin()), &_nil};
        node * cur = _head;
        for (node * tmp = right._head->_next; tmp != &right._nil; tmp = tmp->_next )
        {
            cur->_next = new node{ T(tmp->_data), &_nil };
            cur = cur->_next;
        }
        _nil._next = _head;
    };
    //\brief Оператор присваивания
    //\param right Копируемый список
    //\return Константная ссылка на скопированный список
    const sortedList& operator=(const sortedList<T>& right)
    {
        if (this != &right)
        {
            this->~sortedList();
            if (right.begin() == right.end())
            {
                _nil._next = &_nil;
                return *this;
            }
            _head = new node{ T(*right.begin()), &_nil };
            node* cur = _head;
            for (node * tmp = right._head->_next; tmp != &right._nil; tmp = tmp->_next)
            {
                cur->_next = new node{ T(tmp->_data), &_nil };
                cur = cur->_next;
            }
            _nil._next = _head;
        }
        return *this;
    };
    
    //\brief Вставка с упорядочиванием
    //\param data Данные для вставки
    virtual void push(const T data)
    {
        node* cur = &_nil;

        if (data == 0) return;

        // Сдвигаем указатель, пока не дойдем до края или данные не превысят порог 
        while (cur->_next != &_nil && cur->_next->_data < data)
        {
            cur = cur->_next;
        }

        // Вставляем данные между отсортированными промежутками и перепривязываем 
        // указатели
        
        node* target = new node{data,cur->_next};
            
        cur->_next = target;
        if (target == _nil._next)
        {
            _head = target;
            _nil._next = _head;
        }
    };

    //\brief Метод, удаляющий минимальный элемент
    //\return Минимальный элемент
    T popMin()
    {
        if (_head == &_nil) throw std::underflow_error("underflow list");
        T target = _head->_data;
        node* tmp = _head->_next;
        delete _head;
        _head = tmp;
        _nil._next = _head;
        return target;
    };
    
    class iterator {
    private:
        node* _me = nullptr;
    public:
        iterator(const node* r) { _me = const_cast< node* >(r); };
        iterator(const iterator& it) { _me = it._me; };
        T& operator*() { return _me->_data; };
        T* operator->() { return &_me->_data; }
        bool operator==(const iterator& it) { return _me == it._me; };
        bool operator!=(const iterator& it) { return _me != it._me; };

        iterator& operator++() { _me = _me->_next; return *this; };
        iterator operator++(int) { iterator r(*this); _me = _me->_next; return r; };
        iterator operator+(int i)
        {
            if (i < 0) throw std::logic_error("Unidirectional list");

            iterator r(*this);
            for (size_t k = 0; k < i; ++k) r++;
            return r;
        }
    };
    // \brief Получение первого элемента списка
    // \return Указатель на первый элемент
    iterator begin() const { return iterator(_head); };
    // \brief Получение конца списка
    // \return Указатель на один элемент после последнего
    iterator end()   const { return iterator(&_nil); };

};
using ItemIterator = typename sortedList<monom>::iterator;
//Класс polinom обеспечивает хранение и взаимодействие со списком мономов.
class polinom: public sortedList<monom>
{
public:

    polinom operator +(const polinom& right);
    
    polinom operator *(double a);

    polinom operator -(const polinom& right);

    polinom operator *(const polinom& right);
    //\brief Процедура вывода полинома на экран
    void show();

    void set();

    //\brief Процедура приведения подобных
    void butifie();
};

void ignoreLine()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Input error." << std::endl;;
};