#ifndef AVERAGE_UNKNOWN_VALUES_HANDLER_HPP
#define AVERAGE_UNKNOWN_VALUES_HANDLER_HPP

#include "unknown_values_handler.h"

#include <string>

class AverageUnkownHandler: public UnkownHandler
{
private:
	const static std::string HANDLER_IDENTIFIER;
public:
	void readValues(const std::string& fileName, double **&points, int& numPoints, int& numDimensions)const;
	std::string getName()const;
};
#endif //AVERAGE_UNKNOWN_VALUES_HANDLER_HPP