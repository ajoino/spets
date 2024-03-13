#include <vector>

template <class T>
inline std::vector<T> append_vector(std::vector<T>& Ts, const T& t) {
    Ts.push_back(t);
    return Ts;
}
