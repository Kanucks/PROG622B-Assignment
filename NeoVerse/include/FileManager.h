#pragma once
#include <string>
#include <map>
#include <vector>
#include "Event.h"

using namespace std;

// Small standalone helpers for config.txt and CSV export (section 7).
namespace FileManager {
    map<string, string> loadConfig(const string& path);
    void exportEventsToCSV(const vector<Event>& events, const string& path);
}
