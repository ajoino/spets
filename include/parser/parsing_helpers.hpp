#pragma once

#include <iostream>
#include <vector>
#include <optional>

template <class T>
inline std::vector<T> append_vector(std::vector<T>& Ts, const T& t) {
    Ts.push_back(t);
    return Ts;
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& vs);
std::ostream& operator<<(std::ostream& os, const std::optional<std::vector<std::string>>& vs);
std::ostream& operator<<(std::ostream& os, const std::optional<std::string>& vs);
