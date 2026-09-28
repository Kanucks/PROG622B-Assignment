#pragma once
#include "Event.h"

using namespace std;

// An Event that additionally carries a priority, used on the LIFO
// emergency-override stack. Demonstrates inheritance (extends Event).
class EmergencyEvent : public Event {
private:
    int priorityLevel; // higher value = must be resolved sooner

public:
    EmergencyEvent();
    EmergencyEvent(int id, EventType t, const string& desc, int sev, int priority);

    int getPriorityLevel() const;

    string toString() const override;
    string serialize() const override;
    static EmergencyEvent deserialize(const string& line);
};
