#include "configuration_manager.h"
#include "distances/distance_factory.h"

#include <fstream>
#include <string>
#include <sstream>

const std::string ConfigurationManager::RUN_ID_FILE_NAME = "SettingsConfigurator\\run_id.txt";

std::string ConfigurationManager::getRunIdFromFile()const {
	std::fstream runIdFile(RUN_ID_FILE_NAME.c_str());
	std::string runId;
	runIdFile >> runId;
	if(runId.size() == 0)
	{
		runIdFile.close();
		runIdFile.clear();
		setRunIdInFile(1);
		runId = "1";
	}
	return runId;
}

void ConfigurationManager::setRunIdInFile(int runId)const {
	std::ofstream runIdFileOut(RUN_ID_FILE_NAME.c_str());
	runIdFileOut << runId + 1;
}

void ConfigurationManager::incrementRunId(std::string previosRunId)const {
	std::stringstream ss;
	ss << previosRunId;
	int runId;
	ss >> runId;
	setRunIdInFile(runId + 1);
}

bool ConfigurationManager::doConfiguration() {
	std::string run_id = getRunIdFromFile();

	std::vector<const Distance*> distances = DistanceFactory::getAllDistances();
	std::string distance_names = "";
	for (size_t index = 0; index < distances.size();++index) {
		distance_names += distances[index]->getName();
		if (index + 1 != distances.size()) {
			distance_names += ",";
		}
	}
	std::string command = "java -jar SettingsConfigurator\\settings_configurator.jar";
	// TODO(istrandjev): implement actual zero handler selection.
	std::string zero_handlers = "a,b";
	system((command + " " + run_id + " " + distance_names + " " + zero_handlers).c_str());
	std::ifstream settings("SettingsConfigurator\\settings.conf");
	if (!settings) {
		throw new std::exception("Could not open the configuration file");
	}
	std::string distance_name;
	std::string zero_handler_name;
	std::string returned_run_id;
	std::getline(settings, returned_run_id);

	if(returned_run_id != run_id)
		return false;

	std::getline(settings, inputFileName);
	std::getline(settings, remapFileName);
	std::getline(settings, distance_name);
	// TODO(istrandjev): use this value.
	std::getline(settings, zero_handler_name);
	distance = DistanceFactory::getDistanceByName(distance_name);

	incrementRunId(run_id);
	return true;
}
const std::string& ConfigurationManager::getInputFileName() const {
	return inputFileName;
}
const std::string& ConfigurationManager::getRemapFileName() const {
	return remapFileName;
}
const Distance* ConfigurationManager::getDistance() const {
	return distance;
}