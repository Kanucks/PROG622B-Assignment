#include "EventManager.h"
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

namespace {
    // Fixed dispatch order agreed with main.cpp when the components vector
    // is built: 0 = Power, 1 = Transport, 2 = Health, 3 = Security.
    CityComponent* routeComponent(EventType t, std::vector<CityComponent*>& components) {
        switch (t) {
            case EventType::PowerFailure:    return components.size() > 0 ? components[0] : nullptr;
            case EventType::TrafficAccident: return components.size() > 1 ? components[1] : nullptr;
            case EventType::WeatherAlert:    return components.size() > 2 ? components[2] : nullptr;
            case EventType::NetworkOverload: return components.size() > 3 ? components[3] : nullptr;
        }
        return nullptr;
    }
}

EventManager::EventManager() : nextEventId(1) {}

void EventManager::raiseEvent(EventType type, const std::string& desc, int severity) {
    // O(1) push to the back of the queue.
    eventQueue.push(Event(nextEventId++, type, desc, severity));
}

void EventManager::raiseEmergency(EventType type, const std::string& desc, int severity, int priority) {
    // O(1) push onto the top of the stack.
    emergencyStack.push(EmergencyEvent(nextEventId++, type, desc, severity, priority));
}

bool EventManager::processNextEvent(std::vector<CityComponent*>& components) {
    if (eventQueue.empty()) return false;
    // O(1) pop from the front - events are handled strictly in the order
    // they arrived (FIFO), matching the brief's requirement.
    Event e = eventQueue.front();
    eventQueue.pop();
    cout << e.toString() << "\n";

    CityComponent* target = routeComponent(e.getType(), components);
    if (target) target->processEvent(e); // virtual dispatch -> correct override runs

    processedEvents.push_back(e);
    return true;
}

bool EventManager::resolveEmergency(std::vector<CityComponent*>& components) {
    if (emergencyStack.empty()) return false;
    // O(1) pop from the top - the most recently raised emergency is
    // resolved first (LIFO). This suits override alerts: a brand-new
    // critical failure should pre-empt an older one still waiting.
    EmergencyEvent e = emergencyStack.top();
    emergencyStack.pop();
    cout << e.toString() << "\n";

    CityComponent* target = routeComponent(e.getType(), components);
    if (target) target->processEvent(e);

    resolvedEmergencies.push_back(e);
    return true;
}

size_t EventManager::pendingEvents() const { return eventQueue.size(); }
size_t EventManager::pendingEmergencies() const { return emergencyStack.size(); }

std::vector<Event>& EventManager::getProcessedEvents() { return processedEvents; }
std::vector<EmergencyEvent>& EventManager::getResolvedEmergencies() { return resolvedEmergencies; }

void EventManager::loadProcessed(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) return;
    std::string line;
    int maxId = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        Event e = Event::deserialize(line);
        processedEvents.push_back(e);
        maxId = std::max(maxId, e.getEventID());
    }
    nextEventId = maxId + 1;
}

void EventManager::saveProcessed(const std::string& path) const {
    std::ofstream out(path);
    for (const auto& e : processedEvents) out << e.serialize() << "\n";
}
