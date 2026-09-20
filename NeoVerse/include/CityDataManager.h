#pragma once
#include <vector>
#include <string>
#include "LinkedList.h"

// A single daily sensor reading (population, energy usage, traffic
// density, etc.) - stored in a vector for fast, index-based access.
struct SensorReading {
    int id;
    std::string type;   // "Energy", "Traffic", "Population", "Weather"
    double value;
    std::string timestamp;

    std::string serialize() const;
    static SensorReading deserialize(const std::string& line);
};

// A single historical log line - stored in the unbounded LinkedList.
struct CityLogEntry {
    int id;
    std::string message;
    std::string timestamp;

    std::string serialize() const;
    static CityLogEntry deserialize(const std::string& line);
};

// Owns both dynamic containers required in section 2 of the brief:
// a vector of "today's" sensor readings and a linked list of historical
// city logs.
class CityDataManager {
private:
    std::vector<SensorReading> sensorReadings; // fast, contiguous, cache-friendly
    LinkedList<CityLogEntry> cityLogs;          // unbounded, pointer-based growth
    int nextReadingId;
    int nextLogId;

public:
    CityDataManager();

    void addSensorReading(const std::string& type, double value);
    bool removeSensorReadingById(int id);
    void displaySensorReadings() const;

    void addLogEntry(const std::string& message);
    void displayLogs() const;

    std::vector<SensorReading>& getSensorReadings();
    LinkedList<CityLogEntry>& getLogs();

    void loadSensorReadings(const std::string& path);
    void saveSensorReadings(const std::string& path) const;
    void loadLogs(const std::string& path);
    void saveLogs(const std::string& path) const;
};
