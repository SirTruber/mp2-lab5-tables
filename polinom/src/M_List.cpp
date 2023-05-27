#include "../include/polinom.h"

polinom polinom::operator +(const polinom& right)
{
    polinom fus;
    auto first = begin();
    auto second = right.begin();
    while ( first != end() && second != right.end())
    {
        if (*first < *second)
        {
            fus.push(*first);
            first ++;
            continue;
        }

        if (*first > *second)
        {
            fus.push(*second);
            second ++;
            continue;
        }
        
        fus.push(monom{ first->_a + second->_a,first->_xyz });
        first ++;
        second ++;
        continue;
        
    }
    while (first != end())
    {
        fus.push(*first);
        first ++;
    }

    while(second != right.end())
    {
        fus.push(*second);
        second ++;
    }

    fus.butifie();
    return fus;
};

polinom polinom::operator *(double a)
{
    polinom fus;
    for (auto tmp = begin(); tmp != end(); tmp++)
    {
        fus.push(monom{ tmp->_a * a,tmp->_xyz });
    }
    return fus;
};

polinom polinom::operator -(const polinom& right)
{
    polinom fus = right;
    return ((fus * -1.) + *this);
};

polinom polinom::operator *(const polinom& right) 
{
    polinom fus;
    for(auto i = begin(); i != end(); i++ )
    {
        if (right.begin() == right.end()) return fus; // для умножения на ноль справа
        polinom add;
        for (auto j = right.begin(); j != right.end(); j++ ) {
            int deg = i->_xyz + j->_xyz;
            if (deg / 100 % 10 < i->_xyz / 100) throw std::domain_error("Overflow x-degree");
            if (deg / 10 % 10 < i->_xyz / 10 % 10) throw std::domain_error("Overflow y-degree");
            if (deg % 10 < i->_xyz % 10) throw std::domain_error("Overflow z-degree");

            
            add.push(monom{ i->_a * j->_a, i->_xyz + j->_xyz });
        }
        fus = fus + add;
    }
    
    fus.butifie();
    return fus;
};

void polinom::show()
{
    for (auto tmp = begin(); tmp != end(); )
    {
        int x = tmp->_xyz / 100;
        int y = tmp->_xyz / 10 % 10;
        int z = tmp->_xyz % 10;

        if (tmp->_a != 1)
            std::cout << tmp->_a;
        if (x != 0)
        {
            std::cout << "x";
            if (x != 1)
            {
                std::cout << "^(" << x << ")";
            }
        }
        if (y != 0) {
            std::cout << "y";
            if (y != 1)
            {
                std::cout << "(" << y << ")";
            }
        }
        if (z != 0) {
            std::cout << "z";
            if (z != 1)
            {
                std::cout << "(" << tmp->_xyz % 10 << ")";
            }
        }

        tmp++;

        if (tmp->_a > 0)
        {
            std::cout << " + ";
        }
    }
};

void contSwitch(bool& cont)
{
    std::string s;
    for (;;) {
        std::cout << "Continue input? y/n" << std::endl;
        std::cin >> s;
        if (s == "y" || s == "yes")
        {
            cont = true;
            return;
        }
        if (s == "n" || s == "no")
        {
            cont = false;
            return;
        }
    }
};

void polinom::set()
{
    int tmp;
    bool cont = true;
    while (cont) {
        double a = 0;
        int xyz = 0;
        std::cout << "Enter the polinomial degree."
            << std::endl << "x: ";
        std::cin >> tmp;

        if (std::cin.fail() || tmp > 9 || tmp < 0)
        {
            ignoreLine();
            contSwitch(cont);
            continue;
        }
        xyz += tmp * 100;

        std::cout << "y: ";
        std::cin >> tmp;

        if (std::cin.fail() || tmp > 9 || tmp < 0)
        {
            ignoreLine();
            contSwitch(cont);
            continue;
        }

        xyz += tmp * 10;

        std::cout << "z: ";
        std::cin >> tmp;

        if (std::cin.fail() || tmp > 9 || tmp < 0)
        {
            ignoreLine();
            contSwitch(cont);
            continue;
        }

        xyz += tmp;

        std::cout << "Coef: ";
        std::cin >> a;

        if (std::cin.fail())
        {
            ignoreLine();
            contSwitch(cont);
            continue;
        }

        push(monom{ a,xyz });
        contSwitch(cont);
    }
    butifie();
    show();
    std::cout << std::endl;
    system("PAUSE");
}

void polinom::butifie()
{
    polinom p;
    for (ItemIterator first = begin(), second = begin(); second != end();first = second )
    {
        ++second;
        double a = first->_a; int xyz = first->_xyz;

        while (first->_xyz == second->_xyz)
        {
            a += second->_a;
            ++second;
        }

        p.push(monom{a,xyz});
    }
    *this = p;
}