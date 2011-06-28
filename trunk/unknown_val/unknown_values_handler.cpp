#include "unknown_values_handler.h"

#include<fstream>
#include<sstream>
using namespace std;

bool UnkownHandler::readValue(ifstream& in, double& val)const
{
	string str;
	in >> str;
	if(str == UNKNOWN_VALUE)
	{
		val = 0;
		return false;
	}

	stringstream ss;
	ss << str;
	ss >> val;
	return true;
}