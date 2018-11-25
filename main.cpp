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
class Coordinate;

template<class T, coordinate_type C>
ostream& operator<< (ostream& out, Coordinate<T, C> c);

template<class T, coordinate_type C>
class Coordinate {
friend ostream& operator<< <T, C> (ostream& out, Coordinate c);
private:
    T value;
public:
    coordinate_type type {C};
    constexpr Coordinate(T value) throw (logic_error): value{value} {
    auto maximum = C == LATITUDE ? LATITUDE_MAX : LONGITUDE_MAX;
        if (abs(value) > maximum) {
            throw domain_error("invalid value for type " + to_string(C) + ": " + to_string(value));
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

template<typename T, coordinate_type C>
ostream& operator<< (ostream& out, Coordinate<T, C> c)
{
    out << c.value << "°";

    return out;
}

template<typename T>
ostream& operator<< (ostream& o, GeoPoint<T>& p)
{
    auto pr = o.precision();
    o.precision(9);
    auto flags = o.flags();
    o.setf(ios::fixed, ios::floatfield);
    o << "(" << p.lat << ", " << p.lng << ")";
    o.precision(pr);
    o.flags(flags);

    return o;
}

constexpr double factor()
{
    return 60 * 1.1515 * 1609.344;
}

typedef double distance_t;

template<typename T>
constexpr distance_t distance(GeoPoint<T> a, GeoPoint<T> b) noexcept
{
    return double(rad2deg(
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
    ag::GeoPoint<double> test {0.123456789, 0.987654321};
    ag::GeoPoint<double> testB {9.87654321, 1.23456789};

    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout.precision(3);
    std::cout << test << " to " << testB << ": " << (ag::distance(testB, test)/1000) << std::endl;

    return 0;
}
