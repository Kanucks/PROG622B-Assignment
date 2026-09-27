#include <iostream>
#include <vector>
#include <limits>
#include <chrono>

#include "Engineer.h"
#include "EngineerManager.h"
#include "CityComponent.h"
#include "PowerSystem.h"
#include "TransportSystem.h"
#include "HealthSystem.h"
#include "SecuritySystem.h"
#include "Event.h"
#include "EmergencyEvent.h"
#include "CityDataManager.h"
#include "EventManager.h"
#include "ReportGenerator.h"
#include "FileManager.h"

using namespace std;

namespace {

const std::string ENGINEERS_FILE  = "data/engineers.dat";
const std::string SENSORS_FILE    = "data/sensors.dat";
const std::string LOGS_FILE       = "data/city_logs.dat";
const std::string EVENTS_FILE     = "data/events.dat";
const std::string CONFIG_FILE     = "data/config.txt";
const std::string EXPORT_CSV_FILE = "data/events_export.csv";

// Note: after a successful cin >> extraction, the trailing '\n' left in
// the buffer is consumed immediately (not by the next readLine call) so
// that a readInt()/readDouble() followed by a readLine() never causes the
// following getline() to read an empty line.
int readInt(const std::string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        cout << "Please enter a valid whole number.\n";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

double readDouble(const std::string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        cout << "Please enter a valid number.\n";
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string readLine(const std::string& prompt) {
    cout << prompt;
    std::string line;
    std::getline(cin, line);
    return line;
}

EventType intToEventType(int choice) {
    switch (choice) {
        case 1: return EventType::TrafficAccident;
        case 2: return EventType::PowerFailure;
        case 3: return EventType::NetworkOverload;
        default: return EventType::WeatherAlert;
    }
}

void seedDefaultEngineers(EngineerManager& mgr) {
    mgr.addEngineer(Engineer("ENG001", "admin",  "admin123", "High"));
    mgr.addEngineer(Engineer("ENG002", "jsmith",  "pass123",  "Medium"));
    mgr.addEngineer(Engineer("ENG003", "tnandi",  "city2035", "Low"));
    mgr.saveToFile(ENGINEERS_FILE);
    std::cout << "[Setup] No engineers.dat found - seeded 3 default accounts and saved them.\n";
}

Engineer* login(EngineerManager& mgr) {
    for (int attempt = 1; attempt <= 3; ++attempt) {
        std::string uname = readLine("Username: ");
        std::string pass  = readLine("Password: ");
        Engineer* e = mgr.loginLinear(uname, pass);
        if (e) {
            std::cout << "\nAccess granted. Welcome, " << e->getUsername()
                      << " (" << e->getClearanceLevel() << " clearance).\n";
            return e;
        }
        std::cout << "Invalid credentials. Attempt " << attempt << "/3.\n";
    }
    return nullptr;
}

void compareSearchAlgorithms(EngineerManager& mgr) {
    if (mgr.getAll().empty()) { std::cout << "No engineers loaded.\n"; return; }
    std::string target = mgr.getAll().back().getUsername();

    auto t1 = std::chrono::high_resolution_clock::now();
    mgr.loginLinear(target, "___wrong___");
    auto t2 = std::chrono::high_resolution_clock::now();
    mgr.loginBinary(target, "___wrong___");
    auto t3 = std::chrono::high_resolution_clock::now();

    auto linearNs = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    auto binaryNs = std::chrono::duration_cast<std::chrono::nanoseconds>(t3 - t2).count();

    std::cout << "\n=== LOGIN SEARCH COMPARISON (n = " << mgr.getAll().size() << " engineers) ===\n";
    std::cout << "Linear search (std::find_if):  O(n)      -> " << linearNs << " ns\n";
    std::cout << "Binary search (sorted vector): O(log n)  -> " << binaryNs << " ns\n";
    std::cout << "With only a handful of engineers the difference is negligible, but\n"
                 "binary search scales far better as the workforce grows - the cost is\n"
                 "that the vector must be kept sorted by username.\n";
}

void printMenu() {
    std::cout << "\n========== NEOVERSE CITY CONTROL ==========\n"
                 " 1. View city component status\n"
                 " 2. Add sensor reading\n"
                 " 3. Remove sensor reading by ID\n"
                 " 4. Display all sensor readings\n"
                 " 5. Add city log entry\n"
                 " 6. Display city logs\n"
                 " 7. Raise a new city event (enqueue)\n"
                 " 8. Process next event (FIFO)\n"
                 " 9. Raise an emergency override\n"
                 "10. Resolve next emergency (LIFO)\n"
                 "11. View reports & analytics\n"
                 "12. Export event history to CSV\n"
                 "13. Compare login search algorithms (Big-O demo)\n"
                 "14. Save & Exit\n"
                 "=============================================\n";
}

} // namespace

int main() {
    cout << "======================================================\n"
            "   NeoVerse: AI City Survival System (2035 Prototype)\n"
            "======================================================\n\n";

    auto config = FileManager::loadConfig(CONFIG_FILE);
    std::string cityName = config.count("city_name") ? config["city_name"] : "NeoVerse City";
    cout << "Loaded configuration for: " << cityName << "\n\n";

    EngineerManager engineerMgr;
    engineerMgr.loadFromFile(ENGINEERS_FILE);
    if (engineerMgr.getAll().empty()) seedDefaultEngineers(engineerMgr);

    cout << "\n--- AI Engineer Login ---\n";
    Engineer* current = login(engineerMgr);
    if (!current) {
        cout << "\nToo many failed attempts. Access denied. Shutting down.\n";
        return 1;
    }

    // Fixed order relied on by EventManager's routing table:
    // 0 = Power, 1 = Transport, 2 = Health, 3 = Security.
    std::vector<CityComponent*> components;
    components.push_back(new PowerSystem(1, "Power Grid"));
    components.push_back(new TransportSystem(2, "Transport Network"));
    components.push_back(new HealthSystem(3, "Health Network"));
    components.push_back(new SecuritySystem(4, "Security Grid"));
    for (auto* c : components) c->activate();

    CityDataManager cityData;
    cityData.loadSensorReadings(SENSORS_FILE);
    cityData.loadLogs(LOGS_FILE);

    EventManager eventMgr;
    eventMgr.loadProcessed(EVENTS_FILE);

    bool running = true;
    while (running) {
        printMenu();
        int choice = readInt("Select an option: ");

        switch (choice) {
            case 1:
                cout << "\n--- CITY COMPONENT STATUS ---\n";
                for (auto* c : components) cout << "  " << c->getStatus() << "\n";
                break;
            case 2: {
                std::string type = readLine("Reading type (Energy/Traffic/Population/Weather): ");
                double value = readDouble("Value: ");
                cityData.addSensorReading(type, value);
                cout << "Sensor reading recorded.\n";
                break;
            }
            case 3: {
                int id = readInt("Reading ID to remove: ");
                cout << (cityData.removeSensorReadingById(id) ? "Removed.\n" : "Not found.\n");
                break;
            }
            case 4:
                cout << "\n--- SENSOR READINGS ---\n";
                cityData.displaySensorReadings();
                break;
            case 5: {
                std::string msg = readLine("Log message: ");
                cityData.addLogEntry(msg);
                cout << "Log entry added.\n";
                break;
            }
            case 6:
                cout << "\n--- CITY LOGS ---\n";
                cityData.displayLogs();
                break;
            case 7: {
                cout << "1) Traffic Accident  2) Power Failure  3) Network Overload  4) Weather Alert\n";
                int t = readInt("Event type: ");
                std::string desc = readLine("Description: ");
                int sev = readInt("Severity (1-5): ");
                eventMgr.raiseEvent(intToEventType(t), desc, sev);
                cout << "Event queued (" << eventMgr.pendingEvents() << " pending).\n";
                break;
            }
            case 8:
                if (!eventMgr.processNextEvent(components))
                    cout << "No pending events.\n";
                break;
            case 9: {
                cout << "1) Traffic Accident  2) Power Failure  3) Network Overload  4) Weather Alert\n";
                int t = readInt("Emergency type: ");
                std::string desc = readLine("Description: ");
                int sev = readInt("Severity (1-5): ");
                int pri = readInt("Priority (1-10): ");
                eventMgr.raiseEmergency(intToEventType(t), desc, sev, pri);
                cout << "Emergency pushed onto override stack ("
                     << eventMgr.pendingEmergencies() << " pending).\n";
                break;
            }
            case 10:
                if (!eventMgr.resolveEmergency(components))
                    cout << "No pending emergencies.\n";
                break;
            case 11:
                ReportGenerator::printEventReport(eventMgr.getProcessedEvents());
                ReportGenerator::printSensorReport(cityData.getSensorReadings());
                break;
            case 12:
                FileManager::exportEventsToCSV(eventMgr.getProcessedEvents(), EXPORT_CSV_FILE);
                break;
            case 13:
                compareSearchAlgorithms(engineerMgr);
                break;
            case 14:
                running = false;
                break;
            default:
                cout << "Unknown option, please choose 1-14.\n";
        }
    }

    cout << "\nSaving system state...\n";
    engineerMgr.saveToFile(ENGINEERS_FILE);
    cityData.saveSensorReadings(SENSORS_FILE);
    cityData.saveLogs(LOGS_FILE);
    eventMgr.saveProcessed(EVENTS_FILE);

    for (auto* c : components) delete c; // destructors run here - cleanup demo
    cout << "Goodbye, " << current->getUsername() << ". NeoVerse City Engine shut down safely.\n";
    return 0;
}
