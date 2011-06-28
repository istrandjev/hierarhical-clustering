#ifndef UNKNOWN_VALUES_HANDLER
#define UNKNOWN_VALUES_HANDLER

#include <string>
#include <fstream>

const std::string UNKNOWN_VALUE = "?";
class UnkownHandler
{
public:
	virtual void readValues(const std::string& fileName, double **&points, int& numPoints, int& numDimensions) const = 0;
	virtual std::string getName()const = 0;

protected:
	/* Reads one possibly unknown value from the given input file
	 *     If the value is known it is stored in val and the methods returns true;
	 *     If the value is unkown val is set to zero and the method returns false
	 */
	bool readValue(std::ifstream& in, double& val)const;
};
#endif //UNKNOWN_VALUES_HANDLER