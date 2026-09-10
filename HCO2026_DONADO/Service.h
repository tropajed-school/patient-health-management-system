#pragma once
#include <string>

class Service {
private:
    int serviceID;
    std::string serviceName;

public:
    // Multi-argument constructor with default parameters 
    Service(int id = 0, const std::string& name = "");
    
    // Destructor
    ~Service();
    
    // Accessors
    int getServiceID() const;
    std::string getServiceName() const;
    
    // Mutators
    void setServiceID(int id);
    void setServiceName(const std::string& name);
};