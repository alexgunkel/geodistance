#define _USE_MATH_DEFINES

#include <iostream>
#include <string>
#include <cmath>
#include <chrono>
#include <stdexcept>

namespace ag {
using namespace std;

enum coordinate_type { LATITUDE, LONGITUDE };
const double LATITUDE_MAX {90.};
const double LONGITUDE_MAX {180.};

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
    constexpr Coordinate(T value) throw (logic_error): value{value} {
    auto maximum = C == LATITUDE ? LATITUDE_MAX : LONGITUDE_MAX;
        if (abs(value) > maximum) {
            throw logic_error("invalid value: " + to_string(value));
        }
    }
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
    for (int i = 0; i < 91; i++) {
        ag::GeoPoint<double> test {0, 0};
        ag::GeoPoint<double> testB {-i, 2*i};

        std::cout << test << " to " << testB << ": " << (ag::distance(testB, test)/1000) << std::endl;
    }

    return 0;
}
