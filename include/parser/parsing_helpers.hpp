#pragma once

#include <iostream>
#include <vector>
#include <optional>

template <class T>
inline std::vector<T> append_vector(std::vector<T>& Ts, const T& t) {
    Ts.push_back(t);
    return Ts;
}

template <class T>
inline std::vector<T> create_vector(const T& t) {
    return {t};
}

template <class T>
inline std::vector<T> concat(std::vector<T>& v1, const std::vector<T>& v2) {
    v1.insert( v1.end(), v2.begin(), v2.end() );
    return v1;
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& vs);
std::ostream& operator<<(std::ostream& os, const std::optional<std::vector<std::string>>& vs);
std::ostream& operator<<(std::ostream& os, const std::optional<std::string>& vs);
