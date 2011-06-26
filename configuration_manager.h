#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H
#include "distance.h"
#include <string>

class ConfigurationManager {
public:
	void doConfiguration();
	const std::string& getInputFileName() const;
	const std::string& getRemapFileName() const;
	const Distance* getDistance() const;
private:
	const Distance* distance;
	std::string inputFileName;
	std::string remapFileName;
};
#endif  // CONFIGURATION_MANAGER_H