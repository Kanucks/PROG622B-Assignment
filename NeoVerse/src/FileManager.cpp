#include "FileManager.h"
#include <fstream>
#include <iostream>

namespace FileManager {

std::map<std::string, std::string> loadConfig(const std::string& path) {
    std::map<std::string, std::string> config;
    std::ifstream in(path);
    if (!in.is_open()) return config;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = line.substr(0, eq);
        std::string value = line.substr(eq + 1);
        config[key] = value;
    }
    return config;
}

void exportEventsToCSV(const std::vector<Event>& events, const std::string& path) {
    std::ofstream out(path);
    out << "EventID,Type,Description,Severity,Timestamp\n";
    for (const auto& e : events) {
        out << e.getEventID() << "," << eventTypeToString(e.getType()) << ","
            << e.getDescription() << "," << e.getSeverity() << "," << e.getTimestamp() << "\n";
    }
    std::cout << "Exported " << events.size() << " events to " << path << "\n";
}

}
