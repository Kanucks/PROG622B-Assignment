#include "Event.h"
#include "Utils.h"
#include <sstream>
#include <vector>

std::string eventTypeToString(EventType t) {
    switch (t) {
        case EventType::TrafficAccident: return "Traffic Accident";
        case EventType::PowerFailure:    return "Power Failure";
        case EventType::NetworkOverload: return "Network Overload";
        case EventType::WeatherAlert:    return "Weather Alert";
    }
    return "Unknown";
}

Event::Event()
    : eventID(0), type(EventType::TrafficAccident), description(""), timestamp(""), severity(1) {}

Event::Event(int id, EventType t, const std::string& desc, int sev)
    : eventID(id), type(t), description(desc), timestamp(Utils::currentTimestamp()), severity(sev) {}

Event::~Event() {}

int Event::getEventID() const { return eventID; }
EventType Event::getType() const { return type; }
std::string Event::getDescription() const { return description; }
std::string Event::getTimestamp() const { return timestamp; }
int Event::getSeverity() const { return severity; }

void Event::setTimestamp(const std::string& ts) { timestamp = ts; }

std::string Event::toString() const {
    std::ostringstream oss;
    oss << "[Event #" << eventID << "] " << eventTypeToString(type)
        << " - " << description << " (severity " << severity << ") @ " << timestamp;
    return oss.str();
}

std::string Event::serialize() const {
    std::ostringstream oss;
    oss << eventID << "|" << static_cast<int>(type) << "|" << description
        << "|" << severity << "|" << timestamp;
    return oss.str();
}

Event Event::deserialize(const std::string& line) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, '|')) parts.push_back(token);

    Event e;
    if (parts.size() >= 5) {
        e.eventID = std::stoi(parts[0]);
        e.type = static_cast<EventType>(std::stoi(parts[1]));
        e.description = parts[2];
        e.severity = std::stoi(parts[3]);
        e.timestamp = parts[4];
    }
    return e;
}
