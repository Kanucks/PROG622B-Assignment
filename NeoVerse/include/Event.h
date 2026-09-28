#pragma once
#include <string>

using namespace std;

// The four event categories the city generates, per the brief.
enum class EventType {
    TrafficAccident,
    PowerFailure,
    NetworkOverload,
    WeatherAlert
};

string eventTypeToString(EventType t);

// A single city event flowing through the FIFO queue.
class Event {
protected:
    int eventID;
    EventType type;
    string description;
    string timestamp;
    int severity; // 1 (minor) .. 5 (critical)

public:
    Event();
    Event(int id, EventType t, const string& desc, int sev);
    virtual ~Event();

    int getEventID() const;
    EventType getType() const;
    string getDescription() const;
    string getTimestamp() const;
    int getSeverity() const;

    virtual string toString() const;
    virtual string serialize() const;
    static Event deserialize(const string& line);

protected:
    // Lets derived classes (EmergencyEvent) restore the original timestamp
    // when rebuilding an object from a saved file.
    void setTimestamp(const string& ts);
};
