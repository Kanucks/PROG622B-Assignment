#pragma once
#include <vector>
#include <string>
#include "Engineer.h"

// Owns the collection of engineers and provides the two login search
// strategies discussed in the Big-O section of the assignment.
class EngineerManager {
private:
    std::vector<Engineer> engineers; // STL container required by the brief

public:
    void addEngineer(const Engineer& e);
    std::vector<Engineer>& getAll();

    // O(n) - std::find_if walks the vector from the start until it finds
    // a matching username. Works on an unsorted vector.
    Engineer* loginLinear(const std::string& username, const std::string& password);

    // O(log n) - requires the vector to be sorted by username first
    // (the sort itself is O(n log n), done once here for the demo).
    // std::lower_bound then halves the search space each comparison.
    Engineer* loginBinary(const std::string& username, const std::string& password);

    void loadFromFile(const std::string& path);
    void saveToFile(const std::string& path) const;
};
