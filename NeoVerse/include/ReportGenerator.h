#pragma once
#include <vector>
#include "Event.h"
#include "CityDataManager.h"

// Free functions that turn the raw containers into the analytics
// required in section 6, using the STL algorithms required in section 5
// (sort, find, min_element, max_element, count_if).
namespace ReportGenerator {
    void printEventReport(const std::vector<Event>& events);
    void printSensorReport(std::vector<SensorReading>& readings); // sorts in place
}
