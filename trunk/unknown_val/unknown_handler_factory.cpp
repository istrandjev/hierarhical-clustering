#include "unknown_handler_factory.h"

#include "unknown_values_handler.h"
#include "ignore_unknown_elements_handler.h"
#include "average_unknown_handler.h"
#include "no_unknown_handler.h"

#include <vector>
#include <string>

std::vector<const UnkownHandler*> UnknownHandlerFactory::unkownHandlers;
std::string UnknownHandlerFactory::unknownHandlerName;

bool UnknownHandlerFactory::initialized = false;

// static
void UnknownHandlerFactory::InitUnkownHandlers() {
	unkownHandlers.push_back(new NoUnkownHandler());
	unkownHandlers.push_back(new AverageUnkownHandler());
	unkownHandlers.push_back(new IgnoreUnkownHandler());
	initialized = true;
}

// static
const UnkownHandler* UnknownHandlerFactory::getUnkownHandler() {
	if (!initialized) {
		InitUnkownHandlers();
	}

	for (size_t index = 0; index < unkownHandlers.size(); ++index) {
		if (unkownHandlers[index]->getName() == unknownHandlerName) {
			return unkownHandlers[index];
		}
	}
	throw new UnsupportedUnknownHandlerException();
	return NULL;
}

// static
const std::vector<const UnkownHandler*> UnknownHandlerFactory::getAllUnkownHandlers() {
	if (!initialized) {
		InitUnkownHandlers();
	}
	return unkownHandlers;
}

//static
void UnknownHandlerFactory::setUnknownHandlerName(const std::string& name)
{
	unknownHandlerName = name;
}
