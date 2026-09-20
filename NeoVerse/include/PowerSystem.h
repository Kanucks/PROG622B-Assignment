#pragma once
#include "CityComponent.h"

class PowerSystem : public CityComponent {
private:
    double powerLevel; // percentage, 0-100

public:
    PowerSystem(int id, const std::string& n, double startLevel = 100.0);
    ~PowerSystem() override;

    void supplyPower(double amount);
    double getPowerLevel() const;

    void processEvent(const Event& e) override;
    std::string getStatus() const override;
};
