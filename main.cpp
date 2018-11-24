#define _USE_MATH_DEFINES

#include <iostream>
#include <string>
#include <cmath>
#include <chrono>

namespace ag {
using namespace std;

enum coordinate_type { LATITUDE, LONGITUDE };

template<typename T>
inline constexpr T deg2rad(const T deg)
{ return deg * (M_PI / 180); };

template<typename T>
inline constexpr T rad2deg(const T rad)
{ return rad * (180 / M_PI); }

template<class T, coordinate_type C>
class Coordinate {
public:
    T value;
    coordinate_type type {C};
    constexpr Coordinate(T value): value{value} {}
    constexpr T radians() const noexcept { return deg2rad(value); };
    operator T() const noexcept { return value; }
};

template<class T>
struct GeoPoint {
    Coordinate<T, LATITUDE> lat;
    Coordinate<T, LONGITUDE> lng;
};

template<typename T>
ostream& operator<< (ostream& o, GeoPoint<T>& p)
{
    o << "(" << p.lat << ", " << p.lng << ")";

    return o;
}

constexpr double factor()
{
    return 60 * 1.1515 * 1609.344;
}

typedef int distance_t;

template<typename T>
constexpr distance_t distance(GeoPoint<T> a, GeoPoint<T> b) noexcept
{
    return int(rad2deg(
        acos(
            sin(a.lat.radians())
              * sin(b.lat.radians())
            + cos(a.lat.radians())
              * cos(b.lat.radians())
              * cos(deg2rad(a.lng - b.lng))
            )
        ) * factor());
}

}

int main()
{
    for (int i = 0; i < 90; i++) {
        ag::GeoPoint<double> test {0, 0};
        ag::GeoPoint<double> testB {i, i};

        std::cout << test << " to " << testB << ": " << (ag::distance(testB, test)/1000) << std::endl;
    }

    return 0;
}
