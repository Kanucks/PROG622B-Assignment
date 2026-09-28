#pragma once
#include "CityComponent.h"

using namespace std;

class SecuritySystem : public CityComponent {
private:
    int threatLevel; // 0-100

public:
    SecuritySystem(int id, const string& n, int startThreat = 10);
    ~SecuritySystem() override;

    void monitorCity();
    int getThreatLevel() const;

    void processEvent(const Event& e) override;
    string getStatus() const override;
};
