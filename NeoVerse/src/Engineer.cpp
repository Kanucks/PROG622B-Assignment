#include "Engineer.h"
#include "Utils.h"
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

Engineer::Engineer()
    : engineerID(""), username(""), encryptedPassword(""), clearanceLevel("Low") {}

Engineer::Engineer(const std::string& id, const std::string& uname,
                    const std::string& rawPassword, const std::string& clearance)
    : engineerID(id), username(uname), clearanceLevel(clearance) {
    encryptedPassword = Utils::encrypt(rawPassword);
}

Engineer::~Engineer() {}

std::string Engineer::getEngineerID() const { return engineerID; }
std::string Engineer::getUsername() const { return username; }
std::string Engineer::getClearanceLevel() const { return clearanceLevel; }
std::string Engineer::getEncryptedPassword() const { return encryptedPassword; }

void Engineer::setClearanceLevel(const std::string& level) { clearanceLevel = level; }
void Engineer::setEncryptedPasswordDirect(const std::string& encHex) { encryptedPassword = encHex; }

bool Engineer::verifyPassword(const std::string& rawPassword) const {
    return Utils::decrypt(encryptedPassword) == rawPassword;
}

std::string Engineer::serialize() const {
    return engineerID + "|" + username + "|" + encryptedPassword + "|" + clearanceLevel;
}

Engineer Engineer::deserialize(const std::string& line) {
    std::vector<std::string> parts;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, '|')) parts.push_back(token);

    Engineer e;
    if (parts.size() == 4) {
        e.engineerID = parts[0];
        e.username = parts[1];
        e.encryptedPassword = parts[2];
        e.clearanceLevel = parts[3];
    }
    return e;
}

void Engineer::display() const {
    cout << engineerID << "  " << username << "  [" << clearanceLevel << "]\n";
}
