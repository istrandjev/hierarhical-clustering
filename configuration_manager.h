#ifndef CONFIGURATION_MANAGER_H
#define CONFIGURATION_MANAGER_H
#include "distances/distance.h"
#include <string>

class ConfigurationManager {
public:
	bool doConfiguration();
	const std::string& getInputFileName() const;
	const std::string& getRemapFileName() const;
	const Distance* getDistance() const;
	bool isSupervised() const;
	std::string getClassifiedFileName() const;
private:
	std::string getRunIdFromFile()const;
	void setRunIdInFile(int runId)const;
	void incrementRunId(std::string previosRunId)const;
private:
	const Distance* distance;
	std::string inputFileName;
	std::string remapFileName;
	bool supervised;
	std::string classifiedFileName;
private:
	static const std::string RUN_ID_FILE_NAME; 
};
#endif  // CONFIGURATION_MANAGER_H