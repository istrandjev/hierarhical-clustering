#include "distance_factory.h"

#include "all_distances.h"

std::vector<const Distance*> DistanceFactory::distances;
bool DistanceFactory::initialized = false;

// static
void DistanceFactory::InitDistances() {
	distances.push_back(new SingleLink());
	distances.push_back(new CompleteLink());
	distances.push_back(new CentroidDistance());
	distances.push_back(new AverageDistance());
	initialized = true;
}

// static
const Distance* DistanceFactory::getDistanceByName(const std::string& name) {
	if (!initialized) {
		InitDistances();
	}

	for (size_t index = 0; index < distances.size(); ++index) {
		if (distances[index]->getName() == name) {
			return distances[index];
		}
	}
	throw new UnsupportedDistanceException();
	return NULL;
}

// static
const std::vector<const Distance*> DistanceFactory::getAllDistances() {
	if (!initialized) {
		InitDistances();
	}
	return distances;
}
