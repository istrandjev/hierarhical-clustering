#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H
#include "distance.h"
#include <string>

class ConfigurationManager {
private:
	static const std::string RUN_ID_FILE_NAME; 
public:
	bool doConfiguration();
	const std::string& getInputFileName() const;
	const std::string& getRemapFileName() const;
	const Distance* getDistance() const;
private:
	const Distance* distance;
	std::string inputFileName;
	std::string remapFileName;

	std::string getRunIdFromFile()const;
	void setRunIdInFile(int runId)const;
	void incrementRunId(std::string previosRunId)const;
};
#endif  // CONFIGURATION_MANAGER_H