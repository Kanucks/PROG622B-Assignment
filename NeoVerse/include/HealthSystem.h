#pragma once
#include "CityComponent.h"

using namespace std;

class HealthSystem : public CityComponent {
private:
    int hospitalCnt; // hospitals currently on standby/alert

public:
    HealthSystem(int id, const string& n, int startCount = 3);
    ~HealthSystem() override;

    void provideCare(int hospitalsNeeded);
    int getHospitalCnt() const;

    void processEvent(const Event& e) override;
    string getStatus() const override;
};
