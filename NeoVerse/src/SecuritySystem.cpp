#include "SecuritySystem.h"
#include <algorithm>
#include <iostream>

SecuritySystem::SecuritySystem(int id, const std::string& n, int startThreat)
    : CityComponent(id, n), threatLevel(startThreat) {}

SecuritySystem::~SecuritySystem() {}

void SecuritySystem::monitorCity() {
    // Routine monitoring slowly de-escalates threat level over time.
    threatLevel = std::max(0, threatLevel - 1);
}

int SecuritySystem::getThreatLevel() const { return threatLevel; }

void SecuritySystem::processEvent(const Event& e) {
    if (e.getType() == EventType::NetworkOverload) {
        threatLevel = std::min(100, threatLevel + e.getSeverity() * 12);
        std::cout << "  -> SecuritySystem: intrusion risk rising, threat level now "
                  << threatLevel << "\n";
    } else {
        std::cout << "  -> SecuritySystem: no direct action required for this event\n";
    }
}

std::string SecuritySystem::getStatus() const {
    return CityComponent::getStatus() + " | Threat level: " + std::to_string(threatLevel);
}
