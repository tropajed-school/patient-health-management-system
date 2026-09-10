#include "Service.h"

// Constructor with default parameters
Service::Service(int id, const std::string& name)
    : serviceID(id), serviceName(name) {
}

// Destructor
Service::~Service() {
 
}

// Accessors 
int Service::getServiceID() const {
    return serviceID;
}

std::string Service::getServiceName() const {
    return serviceName;
}

// Mutators
void Service::setServiceID(int id) {
    serviceID = id;
}

void Service::setServiceName(const std::string& name) {
    serviceName = name;
}