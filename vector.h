#pragma once

struct Vector3 {
    double x = 0.0, y = 0.0, z = 0.0;

    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-() const;
    Vector3 operator-(const Vector3& other) const;
    template<typename T>
    Vector3 operator*(T s) const {
        return { x * static_cast<double>(s), y * static_cast<double>(s), z * static_cast<double>(s) };
    }
    template<typename T>
    Vector3 operator/(T s) const {
        return { x / static_cast<double>(s), y / static_cast<double>(s), z / static_cast<double>(s) };
    }
    double Dot(const Vector3& other) const;
    Vector3 Cross(const Vector3& other) const;
    double Magnitude() const;
    Vector3 Normalize() const;
    Vector3 Floor() const;
    Vector3 Round() const;
    bool operator==(const Vector3 other) const;
};

template<typename T>
inline Vector3 operator*(T s, const Vector3& v) {
    return { v.x * static_cast<double>(s), v.y * static_cast<double>(s), v.z * static_cast<double>(s) };
}

template<typename T>
inline Vector3 operator/(T s, const Vector3& v) {
    return { v.x / static_cast<double>(s), v.y / static_cast<double>(s), v.z / static_cast<double>(s) };
}

extern Vector3 forward;
extern Vector3 back;
extern Vector3 right;
extern Vector3 left;
extern Vector3 up;
extern Vector3 down;
