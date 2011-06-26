#include "configuration_manager.h"
#include "distance_factory.h"

#include <fstream>
void ConfigurationManager::doConfiguration() {
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
	system((command + " "+ distance_names + " "+ zero_handlers).c_str());
	std::ifstream settings("settings.conf");
	if (!settings) {
		throw new std::exception("Could not open the configuration file");
	}
	std::string distance_name;
	std::string zero_handler_name;
	
	std::getline(settings, inputFileName);
	std::getline(settings, remapFileName);
	std::getline(settings, distance_name);
	// TODO(istrandjev): use this value.
	std::getline(settings, zero_handler_name);
	distance = DistanceFactory::getDistanceByName(distance_name);
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