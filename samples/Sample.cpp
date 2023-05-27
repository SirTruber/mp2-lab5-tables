// #define GET_LOG_CONSOLE
// #define GET_LOG_FILE

#include "../include/tables.h"
#include "../polinom/include/polinom.h"

enum ch{ex,ins,fnd,rmv,mk,mult_const};
enum op{abrt,add,sub,mult};

tables<string, Polinom_case> t;

bool Message(const string& c )
{
	std::string s;
	while (true) {
		std::cout << c <<" y/n" << std::endl;
		std::cin >> s;
		if (s == "y" || s == "yes")
		{
			return true;
		}
		if (s == "n" || s == "no")
		{
			return false;
		}
	}
};

//void ignoreLine()
//{
//	std::cin.clear();
//	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//	std::cout << "Input error." << std::endl;;
//};

bool insert(polinom& p)
{
	string key;
	bool cont = true;
	while (cont) {
		cout << "Enter a key:";
		cin >> key;
		if (cin.fail()) ignoreLine();
		else
		{
			return t.insert(make_shared<Polinom_case>(key, p), key);
		}
		cont = Message("Continue input?");
	}
	return false;
};

const shared_ptr<Polinom_case> remove()
{
	string key;
	bool cont = true;
	while (cont) {
		cout << "Enter a key:";
		cin >> key;
		if (cin.fail()) ignoreLine();
		else
		{
			if (auto tmp = t.remove(key))
				return tmp;
			else
				cout << "Not find" << endl;
		}
		cont = Message("Continue input?");
	}
	return{};
};

const shared_ptr<Polinom_case> find()
{
	string key;
	bool cont = true;
	while (cont) {
		cout << "Enter a key:";
		cin >> key;
		if (cin.fail()) ignoreLine();
		else
		{
			if (auto tmp = shared_ptr<Polinom_case>(t.find(key)))
				return tmp;
			else
				cout << "Not find" << endl;
		}
		cont = Message("Continue input?");
	}
	return {};
};


bool menu1(polinom& p, polinom& p2)
{
	while (true) {
		double m = 1;
		ch user_ch = ex;
		cout << "1.insert this polinom to table" <<
			endl << "or get another polinom to operation" <<
			endl << "2.find" <<
			endl << "3.remove" <<
			endl << "4.make your own" <<
			endl << "5.mult by const" <<
			endl << "0.do nothing";
		scanf_s("%d", &user_ch);
		switch (user_ch)
		{
		case ex:
			return true;
		case ins:
			if (insert(p))
				return true;
			break;
		case fnd:
			p2 = shared_ptr<Polinom_case>(find())->_p;
			return false;
		case rmv:
			p2 = shared_ptr<Polinom_case>(remove())->_p;
			return false;
		case mk:
			p2.set();
			return false;
		case mult_const:
			std::cout << "Enter the constant:";
			std::cin >> m;
			if (std::cin.fail())
			{
				ignoreLine();
				continue;
			}
			p = p * m;
			return false;
		default:
			std::cout << "Incorrect input" << std::endl;
			break;
		}
	}
};

void menu2(polinom& p, polinom& p2)
{
	while (true) 
	{
		op op_type = abrt;
		cout << "1.add" <<
			endl << "2.sub" <<
			endl << "3.polinomial mult" <<
			endl << "0.abort" << endl;
		scanf_s("%d", &op_type);
		switch (op_type)
		{
		case add:
			p = p + p2;
			break;
		case sub:
			p = p - p2;
			break;
		case mult:
			try { p = p * p2; }
			catch (const std::domain_error& e) { std::cerr << e.what(); exit(-1); }
			break;
		case abrt:
			break;
		default:
			std::cout << "Incorrect input" << std::endl;
			continue;
		}
		std::cout << "Result:" << std::endl;
		p.show();
		system("PAUSE");
    }
};

bool make_operation(polinom& p)
{
	while (true)
	{
		polinom p2;

		if (menu1(p, p2))
			return true;
		menu2(p, p2);
	}
};

int main()
{
	
	bool cont = true;
	while (cont)
	{
		ch user_ch = ex;
		string s;
		polinom p;
		cout << "1.insert polinom" << endl
			<< "2.find polinom" << endl
			<< "3.remove polinom" << endl
			<< "0.exit";
		scanf_s("%d", &user_ch);
		switch (user_ch)
		{
		case ex:
			exit(0);
			break;
		case ins:
			p.set();
			insert(p);
			break;
		case fnd:
			p = find()->_p;
			make_operation(p);
			break;
		case rmv:
			p = remove()->_p;
			make_operation(p);
			break;
		default:
			std::cout << "Incorrect input" << std::endl;
			break;
		}
		cont = Message("Continue input?");
	}
	return 0;
}