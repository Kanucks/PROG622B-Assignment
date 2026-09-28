#pragma once
#include <queue>
#include <stack>
#include <vector>
#include "Event.h"
#include "EmergencyEvent.h"
#include "CityComponent.h"

using namespace std;

// Coordinates the two real-time event structures required in section 3:
//  - a FIFO queue for ordinary events (processed in arrival order)
//  - a LIFO stack for emergency overrides (most recent handled first)
class EventManager {
private:
    queue<Event> eventQueue;
    stack<EmergencyEvent> emergencyStack;
    vector<Event> processedEvents;            // history, feeds the reports
    vector<EmergencyEvent> resolvedEmergencies;
    int nextEventId;

public:
    EventManager();

    void raiseEvent(EventType type, const string& desc, int severity);
    void raiseEmergency(EventType type, const string& desc, int severity, int priority);

    // Dequeues the oldest event and dispatches it (via virtual call) to
    // the responsible CityComponent. Returns false if the queue is empty.
    bool processNextEvent(vector<CityComponent*>& components);

    // Pops the most recently raised emergency and dispatches it.
    bool resolveEmergency(vector<CityComponent*>& components);

    size_t pendingEvents() const;
    size_t pendingEmergencies() const;

    vector<Event>& getProcessedEvents();
    vector<EmergencyEvent>& getResolvedEmergencies();

    void loadProcessed(const string& path);
    void saveProcessed(const string& path) const;
};
