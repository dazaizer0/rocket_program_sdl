#ifndef YUME_MATH
#define YUME_MATH

#include "../../config.hpp"

namespace yume {

    // VECTOR 2
    template <typename T>
    class vec2 {
    public:
        T x{};
        T y{};

        vec2() = default;
        vec2(T xValue, T yValue)
            : x(xValue), y(yValue) {}

        void out() const {
            std::cout << "x: " << x << "\n";
            std::cout << "y: " << y << "\n";
        }

        vec2<T>& operator=(const vec2<T>& other) {
            if (this == &other) return *this;
            x = other.x;
            y = other.y;
            return *this;
        }

        vec2<T> operator+(const vec2<T>& other) const {
            return vec2<T>(x + other.x, y + other.y);
        }

        vec2<T> operator-(const vec2<T>& other) const {
            return vec2<T>(x - other.x, y - other.y);
        }

        vec2<T> operator*(T scalar) const {
            return vec2<T>(x * scalar, y * scalar);
        }

        vec2<T> operator/(T scalar) const {
            return vec2<T>(x / scalar, y / scalar);
        }

        T length() const {
            return std::sqrt(x * x + y * y);
        }

        vec2<T> normalize() const {
            T len = length();
            return (len != 0) ? (*this / len) : vec2<T>(0, 0);
        }

        static vec2<T> ZERO() { return vec2<T>(T(0), T(0)); }
        static vec2<T> ONE() { return vec2<T>(T(1), T(1)); }

        static vec2<T> UP() { return vec2<T>(T(0), T(-1)); }
        static vec2<T> DOWN() { return vec2<T>(T(0), T(1)); }
        static vec2<T> RIGHT() { return vec2<T>(T(1), T(0)); }
        static vec2<T> LEFT() { return vec2<T>(T(-1), T(0)); }
    };


    // VECTOR 3
    template <typename T>
    class vec3 {
    public:
        T x{};
        T y{};
        T z{};

        vec3() = default;
        vec3(T xValue, T yValue, T zValue)
            : x(xValue), y(yValue), z(zValue) {}

        void out() const {
            std::cout << "x: " << x << "\n";
            std::cout << "y: " << y << "\n";
            std::cout << "z: " << z << "\n";
        }

        vec3<T>& operator=(const vec3<T>& other) {
            if (this == &other) return *this;
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

        vec3<T> operator+(const vec3<T>& other) const {
            return vec3<T>(x + other.x, y + other.y, z + other.z);
        }

        vec3<T> operator-(const vec3<T>& other) const {
            return vec3<T>(x - other.x, y - other.y, z - other.z);
        }

        vec3<T> operator*(T scalar) const {
            return vec3<T>(x * scalar, y * scalar, z * scalar);
        }

        vec3<T> operator/(T scalar) const {
            return vec3<T>(x / scalar, y / scalar, z / scalar);
        }

        T length() const {
            return std::sqrt(x * x + y * y + z * z);
        }

        vec3<T> normalize() const {
            T len = length();
            return (len != 0) ? (*this / len) : vec3<T>(0, 0, 0);
        }

        static vec3<T> ZERO() { return vec3<T>(T(0), T(0), T(0)); }
        static vec3<T> ONE() { return vec3<T>(T(1), T(1), T(1)); }

        static vec3<T> UPV2() { return vec3<T>(T(0), T(-1), T(0)); }
        static vec3<T> DOWNV2() { return vec3<T>(T(0), T(1), T(0)); }
        static vec3<T> RIGHTV2() { return vec3<T>(T(1), T(0), T(0)); }
        static vec3<T> LEFTV2() { return vec3<T>(T(-1), T(0), T(0)); }
    };


    // VECTOR 2 FUNCTIONS
    template <typename T>
    double distance(const vec2<T>& v1, const vec2<T>& v2) {
        return std::sqrt(std::pow(v2.x - v1.x, 2) + std::pow(v2.y - v1.y, 2));
    }

    // VECTOR 3 FUNCTIONS
    template <typename T>
    double distance(const vec3<T>& v1, const vec3<T>& v2) {
        return std::sqrt(std::pow(v2.x - v1.x, 2) + std::pow(v2.y - v1.y, 2) + std::pow(v2.z - v1.z, 2));
    }

}

#endif
