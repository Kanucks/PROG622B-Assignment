#include "CityComponent.h"
#include <iostream>

CityComponent::CityComponent(int id, const std::string& n)
    : componentID(id), name(n), active(false) {
    std::cout << "[CityComponent] Constructing " << name << " (ID " << componentID << ")\n";
}

CityComponent::~CityComponent() {
    std::cout << "[CityComponent] Destroying " << name << " (ID " << componentID << ")\n";
}

void CityComponent::activate() { active = true; }
void CityComponent::deactivate() { active = false; }

std::string CityComponent::getStatus() const {
    return name + " [ID " + std::to_string(componentID) + "] - " + (active ? "ACTIVE" : "INACTIVE");
}

int CityComponent::getComponentID() const { return componentID; }
std::string CityComponent::getName() const { return name; }
