#include "configuration_manager.h"
#include "distances/distance_factory.h"
#include "unknown_val/unknown_handler_factory.h"

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

	std::vector<const UnkownHandler*>  unknownHandlers = UnknownHandlerFactory::getAllUnkownHandlers();
	std::string unknown_handler_names = "";
	for (size_t index = 0; index < unknownHandlers.size();++index) {
		unknown_handler_names += unknownHandlers[index]->getName();
		if (index + 1 != unknownHandlers.size()) {
			unknown_handler_names += ",";
		}
	}
	std::string command = "java -jar SettingsConfigurator\\settings_configurator.jar";
	system((command + " " + run_id + " " + distance_names + " " + unknown_handler_names).c_str());
	std::ifstream settings("SettingsConfigurator\\settings.conf");
	if (!settings) {
		throw new std::exception("Could not open the configuration file");
	}
	supervised = false;

	std::string distance_name;
	std::string unknown_handler_name;

	std::string returned_run_id;
	std::getline(settings, returned_run_id);
	if(returned_run_id != run_id)
		return false;

	std::getline(settings, inputFileName);
	std::getline(settings, remapFileName);
	std::getline(settings, distance_name);

	std::getline(settings, unknown_handler_name);
	UnknownHandlerFactory::setUnknownHandlerName(unknown_handler_name);

	std::getline(settings, classifiedFileName);
	if (classifiedFileName.size() > 0) {
		supervised = true;
	}
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

bool ConfigurationManager::isSupervised() const {
	return supervised;
}

std::string ConfigurationManager::getClassifiedFileName() const {
	if (!isSupervised()) {
		throw new std::exception("Can not get classified file name when not supervised");
	}
	return classifiedFileName;
}