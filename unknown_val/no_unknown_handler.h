#ifndef NO_UNKNOWN_VALUES_HANDLER
#define NO_UNKNOWN_VALUES_HANDLER

#include "unknown_values_handler.h"

#include <string>

class NoUnkownHandler: public UnkownHandler
{
private:
	const static std::string HANDLER_IDENTIFIER;
public:
	void readValues(const std::string& fileName, double **&points, int& numPoints, int& numDimensions)const;
	std::string getName()const;
};
#endif //NO_UNKNOWN_VALUES_HANDLER