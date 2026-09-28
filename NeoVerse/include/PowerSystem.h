#pragma once
#include "CityComponent.h"

using namespace std;

class PowerSystem : public CityComponent {
private:
    double powerLevel; // percentage, 0-100

public:
    PowerSystem(int id, const string& n, double startLevel = 100.0);
    ~PowerSystem() override;

    void supplyPower(double amount);
    double getPowerLevel() const;

    void processEvent(const Event& e) override;
    string getStatus() const override;
};
