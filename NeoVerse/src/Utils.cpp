#include "Utils.h"
#include <sstream>
#include <iomanip>
#include <ctime>

namespace Utils {

std::string encrypt(const std::string& plain, char key) {
    std::ostringstream oss;
    for (unsigned char c : plain) {
        unsigned char x = c ^ static_cast<unsigned char>(key);
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(x);
    }
    return oss.str();
}

std::string decrypt(const std::string& cipherHex, char key) {
    std::string result;
    for (size_t i = 0; i + 1 < cipherHex.size(); i += 2) {
        std::string byteStr = cipherHex.substr(i, 2);
        unsigned char x = static_cast<unsigned char>(std::stoi(byteStr, nullptr, 16));
        result += static_cast<char>(x ^ static_cast<unsigned char>(key));
    }
    return result;
}

std::string currentTimestamp() {
    std::time_t t = std::time(nullptr);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return std::string(buf);
}

}
