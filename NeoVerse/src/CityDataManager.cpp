#include "CityDataManager.h"
#include "Utils.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

std::string SensorReading::serialize() const {
    std::ostringstream oss;
    oss << id << "|" << type << "|" << value << "|" << timestamp;
    return oss.str();
}

SensorReading SensorReading::deserialize(const std::string& line) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, '|')) parts.push_back(token);

    SensorReading r{};
    if (parts.size() >= 4) {
        r.id = std::stoi(parts[0]);
        r.type = parts[1];
        r.value = std::stod(parts[2]);
        r.timestamp = parts[3];
    }
    return r;
}

std::string CityLogEntry::serialize() const {
    return std::to_string(id) + "|" + message + "|" + timestamp;
}

CityLogEntry CityLogEntry::deserialize(const std::string& line) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, '|')) parts.push_back(token);

    CityLogEntry l{};
    if (parts.size() >= 3) {
        l.id = std::stoi(parts[0]);
        l.message = parts[1];
        l.timestamp = parts[2];
    }
    return l;
}

CityDataManager::CityDataManager() : nextReadingId(1), nextLogId(1) {}

void CityDataManager::addSensorReading(const std::string& type, double value) {
    // push_back is amortised O(1): the vector only reallocates and copies
    // its existing elements occasionally as it grows, which is why it
    // suits fast, random-access reads of "today's" readings.
    sensorReadings.push_back(SensorReading{nextReadingId++, type, value, Utils::currentTimestamp()});
}

bool CityDataManager::removeSensorReadingById(int id) {
    // erase-remove idiom: O(n). Every element after the removed one has
    // to shift left to keep the vector contiguous.
    auto it = std::remove_if(sensorReadings.begin(), sensorReadings.end(),
        [id](const SensorReading& r) { return r.id == id; });
    if (it == sensorReadings.end()) return false;
    sensorReadings.erase(it, sensorReadings.end());
    return true;
}

void CityDataManager::displaySensorReadings() const {
    for (const auto& r : sensorReadings) {
        std::cout << "  #" << r.id << " " << r.type << ": " << r.value
                  << " @ " << r.timestamp << "\n";
    }
}

void CityDataManager::addLogEntry(const std::string& message) {
    // O(1) append at the tail - no shifting or reallocation of existing
    // nodes, which is why the linked list suits an ever-growing log.
    cityLogs.append(CityLogEntry{nextLogId++, message, Utils::currentTimestamp()});
}

void CityDataManager::displayLogs() const {
    // O(n) traversal - a linked list has no random access, so every node
    // must be visited in order through its next pointer.
    cityLogs.forEach([](const CityLogEntry& l) {
        std::cout << "  [Log #" << l.id << "] " << l.message << " @ " << l.timestamp << "\n";
    });
}

std::vector<SensorReading>& CityDataManager::getSensorReadings() { return sensorReadings; }
LinkedList<CityLogEntry>& CityDataManager::getLogs() { return cityLogs; }

void CityDataManager::loadSensorReadings(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) return;
    std::string line;
    int maxId = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        SensorReading r = SensorReading::deserialize(line);
        sensorReadings.push_back(r);
        maxId = std::max(maxId, r.id);
    }
    nextReadingId = maxId + 1;
}

void CityDataManager::saveSensorReadings(const std::string& path) const {
    std::ofstream out(path);
    for (const auto& r : sensorReadings) out << r.serialize() << "\n";
}

void CityDataManager::loadLogs(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) return;
    std::string line;
    int maxId = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        CityLogEntry l = CityLogEntry::deserialize(line);
        cityLogs.append(l);
        maxId = std::max(maxId, l.id);
    }
    nextLogId = maxId + 1;
}

void CityDataManager::saveLogs(const std::string& path) const {
    std::ofstream out(path);
    cityLogs.forEach([&out](const CityLogEntry& l) {
        out << l.serialize() << "\n";
    });
}
