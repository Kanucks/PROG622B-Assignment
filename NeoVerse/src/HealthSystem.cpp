#include "HealthSystem.h"
#include <iostream>

HealthSystem::HealthSystem(int id, const std::string& n, int startCount)
    : CityComponent(id, n), hospitalCnt(startCount) {}

HealthSystem::~HealthSystem() {}

void HealthSystem::provideCare(int hospitalsNeeded) {
    hospitalCnt += hospitalsNeeded;
}

int HealthSystem::getHospitalCnt() const { return hospitalCnt; }

void HealthSystem::processEvent(const Event& e) {
    if (e.getType() == EventType::WeatherAlert) {
        hospitalCnt += e.getSeverity();
        std::cout << "  -> HealthSystem: placing " << e.getSeverity()
                  << " additional hospital(s) on standby (total " << hospitalCnt << ")\n";
    } else {
        std::cout << "  -> HealthSystem: no direct action required for this event\n";
    }
}

std::string HealthSystem::getStatus() const {
    return CityComponent::getStatus() + " | Hospitals on standby: " + std::to_string(hospitalCnt);
}
