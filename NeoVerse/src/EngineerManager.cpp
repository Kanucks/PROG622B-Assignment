#include "EngineerManager.h"
#include <algorithm>
#include <fstream>

void EngineerManager::addEngineer(const Engineer& e) { engineers.push_back(e); }

std::vector<Engineer>& EngineerManager::getAll() { return engineers; }

Engineer* EngineerManager::loginLinear(const std::string& username, const std::string& password) {
    // std::find_if scans from the start until a match is found -> worst
    // case O(n), best case O(1). No ordering requirement on the vector.
    auto it = std::find_if(engineers.begin(), engineers.end(),
        [&](const Engineer& e) { return e.getUsername() == username; });

    if (it != engineers.end() && it->verifyPassword(password)) {
        return &(*it);
    }
    return nullptr;
}

Engineer* EngineerManager::loginBinary(const std::string& username, const std::string& password) {
    // Binary search needs a sorted key range, so we sort by username first.
    // Sorting costs O(n log n); the lookup that follows is O(log n).
    std::sort(engineers.begin(), engineers.end(),
        [](const Engineer& a, const Engineer& b) { return a.getUsername() < b.getUsername(); });

    auto it = std::lower_bound(engineers.begin(), engineers.end(), username,
        [](const Engineer& e, const std::string& uname) { return e.getUsername() < uname; });

    if (it != engineers.end() && it->getUsername() == username && it->verifyPassword(password)) {
        return &(*it);
    }
    return nullptr;
}

void EngineerManager::loadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) return;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        engineers.push_back(Engineer::deserialize(line));
    }
}

void EngineerManager::saveToFile(const std::string& path) const {
    std::ofstream out(path);
    for (const auto& e : engineers) out << e.serialize() << "\n";
}
