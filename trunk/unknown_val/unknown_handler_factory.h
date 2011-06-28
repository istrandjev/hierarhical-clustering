#ifndef UNKOWN_HANDLER_FACTORY_HPP
#define UNKOWN_HANDLER_FACTORY_HPP

#include "unknown_values_handler.h"

#include <vector>
#include <string>

class UnknownHandlerFactory {
public:
	static const UnkownHandler* getUnkownHandler();
	static void InitUnkownHandlers();
	static const std::vector<const UnkownHandler*> getAllUnkownHandlers();
	static void setUnknownHandlerName(const std::string& name);
private:
	static std::vector<const UnkownHandler*> unkownHandlers;
	static std::string unknownHandlerName;
	static bool initialized;
};

class UnsupportedUnknownHandlerException : public std::exception {
public:
	virtual const char* what() const throw() {
		return "Unsupported unknown handler type!";
	}
};

#endif //UNKOWN_HANDLER_FACTORY_HPP