#pragma once

// Convert any container to a string
#include <string>
using namespace std::literals;
template<typename T>
std::string to_string (const T& v, const std::string& sep=" "s) {
    std::string result;
    for( const auto& e:v ) {
        result += std::string(e) + sep;
    }
    if( result.empty() ) {
        result = "<<empty>>";
    } else {
        // Remove trailing separator
        result.erase(result.rfind(sep));
    }
    return result;
}
