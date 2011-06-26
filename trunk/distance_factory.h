#ifndef _DISTANCE_FACTORY_H
#define _DISTANCE_FACTORY_H
#include "distance.h"
#include <string>
#include <vector>

class DistanceFactory {
public:
	static const Distance* getDistanceByName(const std::string& name);
	static void InitDistances();
	static const std::vector<const Distance*> getAllDistances();
private:
	static std::vector<const Distance*> distances;
	static bool initialized;
};

class UnsupportedDistanceException : public std::exception {
public:
	virtual const char* what() const throw() {
		return "Unsupported distance type!";
	}
};
#endif  // _DISTANCE_FACTORY_H