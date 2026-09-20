#pragma once
#include <string>

// The four event categories the city generates, per the brief.
enum class EventType {
    TrafficAccident,
    PowerFailure,
    NetworkOverload,
    WeatherAlert
};

std::string eventTypeToString(EventType t);

// A single city event flowing through the FIFO queue.
class Event {
protected:
    int eventID;
    EventType type;
    std::string description;
    std::string timestamp;
    int severity; // 1 (minor) .. 5 (critical)

public:
    Event();
    Event(int id, EventType t, const std::string& desc, int sev);
    virtual ~Event();

    int getEventID() const;
    EventType getType() const;
    std::string getDescription() const;
    std::string getTimestamp() const;
    int getSeverity() const;

    virtual std::string toString() const;
    virtual std::string serialize() const;
    static Event deserialize(const std::string& line);

protected:
    // Lets derived classes (EmergencyEvent) restore the original timestamp
    // when rebuilding an object from a saved file.
    void setTimestamp(const std::string& ts);
};
