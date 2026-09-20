#pragma once
#include "Event.h"

// An Event that additionally carries a priority, used on the LIFO
// emergency-override stack. Demonstrates inheritance (extends Event).
class EmergencyEvent : public Event {
private:
    int priorityLevel; // higher value = must be resolved sooner

public:
    EmergencyEvent();
    EmergencyEvent(int id, EventType t, const std::string& desc, int sev, int priority);

    int getPriorityLevel() const;

    std::string toString() const override;
    std::string serialize() const override;
    static EmergencyEvent deserialize(const std::string& line);
};
