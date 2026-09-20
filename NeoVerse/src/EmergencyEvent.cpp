#include "EmergencyEvent.h"
#include <sstream>
#include <vector>

EmergencyEvent::EmergencyEvent() : Event(), priorityLevel(1) {}

EmergencyEvent::EmergencyEvent(int id, EventType t, const std::string& desc, int sev, int priority)
    : Event(id, t, desc, sev), priorityLevel(priority) {}

int EmergencyEvent::getPriorityLevel() const { return priorityLevel; }

std::string EmergencyEvent::toString() const {
    return "[EMERGENCY OVERRIDE] " + Event::toString() + " | priority " + std::to_string(priorityLevel);
}

std::string EmergencyEvent::serialize() const {
    return Event::serialize() + "|" + std::to_string(priorityLevel);
}

EmergencyEvent EmergencyEvent::deserialize(const std::string& line) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, '|')) parts.push_back(token);

    EmergencyEvent e;
    if (parts.size() >= 6) {
        Event base = Event::deserialize(line); // reuse base parser for the shared 5 fields
        e = EmergencyEvent(base.getEventID(), base.getType(), base.getDescription(),
                            base.getSeverity(), std::stoi(parts[5]));
        e.setTimestamp(base.getTimestamp()); // preserve the original timestamp
    }
    return e;
}
