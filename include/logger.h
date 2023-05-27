#pragma once

// #define GET_LOG_CONSOLE
// #define GET_LOG_FILE

#include <fstream>
#include <string>

using namespace std;

struct loger
{

	string _name;
	size_t _op = 0;
	time_t _t = time(NULL);

	loger(const string& s) :_name(s) {};
	~loger()
	{
#ifdef GET_LOG_FILE
		ofstream logfile("user.log");
		logfile << ctime(&_t) << ":" << _name << "-count of op:" << _op << endl;
		logfile.close();
#endif
#ifdef GET_LOG_CONSOLE
		cout << ctime(&_t) << ":" << _name << "-count of op:" << _op << endl;
#endif
	}
	loger& operator++() { _op++; return *this; };
	loger operator++(int) { loger tmp = *this; ++_op; return tmp; };
};