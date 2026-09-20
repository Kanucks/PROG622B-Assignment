#pragma once
#include <queue>
#include <stack>
#include <vector>
#include "Event.h"
#include "EmergencyEvent.h"
#include "CityComponent.h"

// Coordinates the two real-time event structures required in section 3:
//  - a FIFO queue for ordinary events (processed in arrival order)
//  - a LIFO stack for emergency overrides (most recent handled first)
class EventManager {
private:
    std::queue<Event> eventQueue;
    std::stack<EmergencyEvent> emergencyStack;
    std::vector<Event> processedEvents;            // history, feeds the reports
    std::vector<EmergencyEvent> resolvedEmergencies;
    int nextEventId;

public:
    EventManager();

    void raiseEvent(EventType type, const std::string& desc, int severity);
    void raiseEmergency(EventType type, const std::string& desc, int severity, int priority);

    // Dequeues the oldest event and dispatches it (via virtual call) to
    // the responsible CityComponent. Returns false if the queue is empty.
    bool processNextEvent(std::vector<CityComponent*>& components);

    // Pops the most recently raised emergency and dispatches it.
    bool resolveEmergency(std::vector<CityComponent*>& components);

    size_t pendingEvents() const;
    size_t pendingEmergencies() const;

    std::vector<Event>& getProcessedEvents();
    std::vector<EmergencyEvent>& getResolvedEmergencies();

    void loadProcessed(const std::string& path);
    void saveProcessed(const std::string& path) const;
};
