#include "TransportSystem.h"
#include <algorithm>
#include <iostream>

TransportSystem::TransportSystem(int id, const std::string& n, int startFlow)
    : CityComponent(id, n), trafficFlow(startFlow) {}

TransportSystem::~TransportSystem() {}

void TransportSystem::manageTraffic(int adjustment) {
    trafficFlow = std::max(0, std::min(100, trafficFlow + adjustment));
}

int TransportSystem::getTrafficFlow() const { return trafficFlow; }

void TransportSystem::processEvent(const Event& e) {
    if (e.getType() == EventType::TrafficAccident) {
        trafficFlow = std::min(100, trafficFlow + e.getSeverity() * 15);
        std::cout << "  -> TransportSystem: rerouting traffic, congestion index now "
                  << trafficFlow << "\n";
    } else {
        std::cout << "  -> TransportSystem: no direct action required for this event\n";
    }
}

std::string TransportSystem::getStatus() const {
    return CityComponent::getStatus() + " | Traffic flow: " + std::to_string(trafficFlow);
}
