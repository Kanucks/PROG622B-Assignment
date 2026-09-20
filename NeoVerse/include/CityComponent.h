#pragma once
#include <string>
#include "Event.h"

// Abstract base class for every city subsystem (matches the UML in the
// brief: CityComponent -> PowerSystem / TransportSystem / HealthSystem /
// SecuritySystem).
class CityComponent {
protected:
    int componentID;
    std::string name;
    bool active;

public:
    CityComponent(int id, const std::string& n);
    virtual ~CityComponent(); // virtual so `delete` via a base pointer is safe

    virtual void activate();
    virtual void deactivate();
    virtual std::string getStatus() const;

    int getComponentID() const;
    std::string getName() const;

    // Pure virtual: every concrete subsystem must define its own reaction
    // to an event. Calling processEvent() through a CityComponent* is what
    // gives us runtime polymorphism - the correct override runs depending
    // on the actual object type, decided at run time, not compile time.
    virtual void processEvent(const Event& e) = 0;
};
