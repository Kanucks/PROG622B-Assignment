#pragma once
#include <vector>
#include <string>
#include "LinkedList.h"

using namespace std;

// A single daily sensor reading (population, energy usage, traffic
// density, etc.) - stored in a vector for fast, index-based access.
struct SensorReading {
    int id;
    string type;   // "Energy", "Traffic", "Population", "Weather"
    double value;
    string timestamp;

    string serialize() const;
    static SensorReading deserialize(const string& line);
};

// A single historical log line - stored in the unbounded LinkedList.
struct CityLogEntry {
    int id;
    string message;
    string timestamp;

    string serialize() const;
    static CityLogEntry deserialize(const string& line);
};

// Owns both dynamic containers required in section 2 of the brief:
// a vector of "today's" sensor readings and a linked list of historical
// city logs.
class CityDataManager {
private:
    vector<SensorReading> sensorReadings; // fast, contiguous, cache-friendly
    LinkedList<CityLogEntry> cityLogs;          // unbounded, pointer-based growth
    int nextReadingId;
    int nextLogId;

public:
    CityDataManager();

    void addSensorReading(const string& type, double value);
    bool removeSensorReadingById(int id);
    void displaySensorReadings() const;

    void addLogEntry(const string& message);
    void displayLogs() const;

    vector<SensorReading>& getSensorReadings();
    LinkedList<CityLogEntry>& getLogs();

    void loadSensorReadings(const string& path);
    void saveSensorReadings(const string& path) const;
    void loadLogs(const string& path);
    void saveLogs(const string& path) const;
};
