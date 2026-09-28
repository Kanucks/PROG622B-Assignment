#pragma once
#include "CityComponent.h"

using namespace std;

class TransportSystem : public CityComponent {
private:
    int trafficFlow; // congestion index: 0 = clear, 100 = gridlock

public:
    TransportSystem(int id, const string& n, int startFlow = 20);
    ~TransportSystem() override;

    void manageTraffic(int adjustment);
    int getTrafficFlow() const;

    void processEvent(const Event& e) override;
    string getStatus() const override;
};
