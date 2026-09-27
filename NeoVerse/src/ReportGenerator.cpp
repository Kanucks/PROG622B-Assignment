#include "ReportGenerator.h"
#include <algorithm>
#include <iostream>
#include <map>

using namespace std;

namespace ReportGenerator {

void printEventReport(const std::vector<Event>& events) {
    cout << "\n=== EVENT REPORT ===\n";
    cout << "Total events processed: " << events.size() << "\n";
    if (events.empty()) { cout << "(no events yet)\n"; return; }

    // std::count_if - O(n): one linear scan, tallying matches.
    int critical = static_cast<int>(std::count_if(events.begin(), events.end(),
        [](const Event& e) { return e.getSeverity() >= 4; }));
    cout << "Critical alerts (severity >= 4): " << critical << "\n";

    // std::max_element / std::min_element - O(n) each: a single linear
    // scan tracking the best candidate seen so far.
    auto worst = std::max_element(events.begin(), events.end(),
        [](const Event& a, const Event& b) { return a.getSeverity() < b.getSeverity(); });
    auto mildest = std::min_element(events.begin(), events.end(),
        [](const Event& a, const Event& b) { return a.getSeverity() < b.getSeverity(); });
    cout << "Most severe event: " << worst->toString() << "\n";
    cout << "Mildest event: " << mildest->toString() << "\n";

    // Most common event type - O(n) to tally into a map, then O(k) to
    // scan the (at most 4) buckets for the largest one.
    std::map<std::string, int> tally;
    for (const auto& e : events) tally[eventTypeToString(e.getType())]++;
    std::string mostCommon;
    int best = -1;
    for (const auto& entry : tally) {
        if (entry.second > best) { best = entry.second; mostCommon = entry.first; }
    }
    cout << "Most common event type: " << mostCommon << " (" << best << " occurrence(s))\n";

    // Average severity, used as a stand-in "response urgency" workload
    // metric - a single O(n) linear accumulation.
    double total = 0;
    for (const auto& e : events) total += e.getSeverity();
    cout << "Average severity (proxy for response urgency): "
              << (total / static_cast<double>(events.size())) << "\n";

    // std::find_if - O(n): stops as soon as a match is located.
    int searchId = events.front().getEventID();
    auto found = std::find_if(events.begin(), events.end(),
        [searchId](const Event& e) { return e.getEventID() == searchId; });
    if (found != events.end()) {
        cout << "Lookup demo - found event #" << searchId << ": " << found->toString() << "\n";
    }
}

void printSensorReport(std::vector<SensorReading>& readings) {
    cout << "\n=== SENSOR REPORT ===\n";
    if (readings.empty()) { cout << "(no sensor readings yet)\n"; return; }

    // std::sort - O(n log n), introsort (a quicksort/heapsort/insertion
    // sort hybrid) - the standard choice whenever a full ordering is needed.
    std::sort(readings.begin(), readings.end(),
        [](const SensorReading& a, const SensorReading& b) { return a.value < b.value; });
    cout << "Readings sorted by value (ascending):\n";
    for (const auto& r : readings) {
        cout << "  " << r.type << ": " << r.value << "\n";
    }

    // std::min_element / std::max_element - O(n) single linear scans.
    auto lo = std::min_element(readings.begin(), readings.end(),
        [](const SensorReading& a, const SensorReading& b) { return a.value < b.value; });
    auto hi = std::max_element(readings.begin(), readings.end(),
        [](const SensorReading& a, const SensorReading& b) { return a.value < b.value; });
    cout << "Lowest reading: " << lo->type << " = " << lo->value << "\n";
    cout << "Highest reading: " << hi->type << " = " << hi->value << "\n";
}

}
