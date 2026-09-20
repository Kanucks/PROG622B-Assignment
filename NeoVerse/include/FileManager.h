#pragma once
#include <string>
#include <map>
#include <vector>
#include "Event.h"

// Small standalone helpers for config.txt and CSV export (section 7).
namespace FileManager {
    std::map<std::string, std::string> loadConfig(const std::string& path);
    void exportEventsToCSV(const std::vector<Event>& events, const std::string& path);
}
