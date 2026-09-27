#include "PowerSystem.h"
#include <algorithm>
#include <iostream>

using namespace std;

PowerSystem::PowerSystem(int id, const std::string& n, double startLevel)
    : CityComponent(id, n), powerLevel(startLevel) {}

PowerSystem::~PowerSystem() {}

void PowerSystem::supplyPower(double amount) {
    powerLevel = std::min(100.0, powerLevel + amount);
}

double PowerSystem::getPowerLevel() const { return powerLevel; }

void PowerSystem::processEvent(const Event& e) {
    if (e.getType() == EventType::PowerFailure) {
        powerLevel = std::max(0.0, powerLevel - (e.getSeverity() * 10.0));
        cout << "  -> PowerSystem: grid strain detected, power level now "
             << powerLevel << "%\n";
    } else {
        cout << "  -> PowerSystem: no direct action required for this event\n";
    }
}

std::string PowerSystem::getStatus() const {
    return CityComponent::getStatus() + " | Power level: " + std::to_string(powerLevel) + "%";
}
