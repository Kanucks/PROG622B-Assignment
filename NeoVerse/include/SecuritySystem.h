#pragma once
#include "CityComponent.h"

class SecuritySystem : public CityComponent {
private:
    int threatLevel; // 0-100

public:
    SecuritySystem(int id, const std::string& n, int startThreat = 10);
    ~SecuritySystem() override;

    void monitorCity();
    int getThreatLevel() const;

    void processEvent(const Event& e) override;
    std::string getStatus() const override;
};
