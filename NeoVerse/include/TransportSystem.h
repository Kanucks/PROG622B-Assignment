#pragma once
#include "CityComponent.h"

class TransportSystem : public CityComponent {
private:
    int trafficFlow; // congestion index: 0 = clear, 100 = gridlock

public:
    TransportSystem(int id, const std::string& n, int startFlow = 20);
    ~TransportSystem() override;

    void manageTraffic(int adjustment);
    int getTrafficFlow() const;

    void processEvent(const Event& e) override;
    std::string getStatus() const override;
};
