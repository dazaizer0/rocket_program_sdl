#ifndef YUMESDL_MATH_HPP
#define YUMESDL_MATH_HPP

#include "../../config.h"

namespace mathy {
    // VECTOR 2
    template <typename T>
    class vec2 {
    public:
        T x {};
        T y {};

        vec2(T xValue, T yValue)
                : x(xValue), y(yValue) {}

        void out() {
            std::cout << "x: " << x << "\n";
            std::cout << "y: " << y << "\n";
        }

        vec2<T> operator=(const vec2<T>& other) {
            return vec2<T>(x = other.x, y = other.y);
        }

        vec2<T> operator+(const vec2<T>& other) const {
            return vec2<T>(x + other.x, y + other.y);
        }

        vec2<T> operator-(const vec2<T>& other) const {
            return vec2<T>(x - other.x, y - other.y);
        }

        vec2<T> operator*(const vec2<T>& other) const {
            return vec2<T>(x * other.x, y * other.y);
        }

        vec2<T> operator/(const vec2<T>& other) const {
            return vec2<T>(x / other.x, y / other.y);
        }

        static vec2<T> ZERO() { return { T(0), T(0)}; };
        static vec2<T> ONE() { return { T(1), T(1)}; };

        static vec2<T> UP() { return { T(0), T(-1)}; };
        static vec2<T> DOWN() { return { T(0), T(1)}; };
        static vec2<T> RIGHT() { return { T(1), T(0)}; };
        static vec2<T> LEFT() { return { T(-1), T(0)}; };
    };


    // VECTOR 3
    template <typename T>
    class vec3 {
    public:
        T x {};
        T y {};
        T z {};

        vec3(T xValue, T yValue, T zValue)
                : x(xValue), y(yValue), z(zValue) {}

        void out() {
            std::cout << "x: " << x << "\n";
            std::cout << "y: " << y << "\n";
            std::cout << "z: " << z << "\n";
        }

        vec3<T> operator=(const vec3<T>& other) {
            return vec3<T>(x = other.x, y = other.y, z = other.z);
        }

        vec3<T> operator+(const vec3<T>& other) const {
            return vec3<T>(x + other.x, y + other.y, z + other.z);
        }

        vec3<T> operator-(const vec3<T>& other) const {
            return vec3<T>(x - other.x, y - other.y, z - other.z);
        }

        vec3<T> operator*(const vec3<T>& other) const {
            return vec3<T>(x * other.x, y * other.y, z * other.z);
        }

        vec3<T> operator/(const vec3<T>& other) const {
            return vec3<T>(x / other.x, y / other.y, z / other.z);
        }

        static vec3<T> ZERO() { return { T(0), T(0), T(0)}; };
        static vec3<T> ONE() { return { T(1), T(1), T(1)}; };

        static vec3<T> UPV2() { return { T(0), T(-1), T(0) }; };
        static vec3<T> DOWNV2() { return { T(0), T(1), T(0) }; };
        static vec3<T> RIGHTV2() { return { T(1), T(0), T(0) }; };
        static vec3<T> LEFTV2() { return { T(-1), T(0), T(0) }; };
    };

    class colorRGBA {
    public:
        Uint8 r{};
        Uint8 g{};
        Uint8 b{};
        Uint8 a{};

        colorRGBA(Uint8 rValue, Uint8 gValue, Uint8 bValue, Uint8 aValue)
                : r(rValue), g(gValue), b(bValue), a(aValue) {}

        static colorRGBA BLACK() { return {(Uint8)255, (Uint8)255, (Uint8)255, (Uint8)255}; };
        static colorRGBA WHITE() { return {(Uint8)0, (Uint8)0, (Uint8)0, (Uint8)255}; };
        static colorRGBA RED() { return {(Uint8)255, (Uint8)0, (Uint8)0, (Uint8)255}; };
        static colorRGBA GREEN() { return {(Uint8)0, (Uint8)255, (Uint8)0, (Uint8)255}; };
        static colorRGBA BLUE() { return {(Uint8)0, (Uint8)0, (Uint8)255, (Uint8)255}; };
    };

    // VECTOR 2 FUNCTIONS
    double distance(mathy::vec2<int> v_1, mathy::vec2<int> v_2) {
        return std::sqrt(std::pow(v_2.x - v_1.x, 2) + std::pow(v_2.y - v_1.y, 2));
    }

    // VECTOR 3 FUNCTIONS
}

#endif