#ifndef IGNORE_UNKNOWN_VALUES_HANDLER
#define IGNORE_UNKNOWN_VALUES_HANDLER

#include "unknown_values_handler.h"

#include <string>

class IgnoreUnkownHandler: public UnkownHandler
{
private:
	const static std::string HANDLER_IDENTIFIER;
public:
	void readValues(const std::string& fileName, double **&points, int& numPoints, int& numDimensions)const;
	std::string getName()const;
};
#endif //IGNORE_UNKNOWN_VALUES_HANDLER