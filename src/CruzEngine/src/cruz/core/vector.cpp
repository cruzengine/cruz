#include "vector.h"
#include <cmath>

Vector3::Vector3() : x(0), y(0), z(0) {}
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

Vector3 Vector3::operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
Vector3 Vector3::operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
Vector3 Vector3::operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }

float Vector3::length() const { return std::sqrt(x*x + y*y + z*z); }

Vector3 Vector3::normalized() const {
    float len = length();
    if (len == 0) return Vector3(0,0,0);
    return Vector3(x/len, y/len, z/len);
}

void Vector3::print() const { std::cout << "(" << x << ", " << y << ", " << z << ")\n"; }

Vector2::Vector2() : x(0), y(0) {}
Vector2::Vector2(float x, float y) : x(x), y(y) {}

Vector2 Vector2::operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
Vector2 Vector2::operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
Vector2 Vector2::operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
Vector2 Vector2::operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }

Vector2& Vector2::operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
Vector2& Vector2::operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
Vector2& Vector2::operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
Vector2& Vector2::operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }

float Vector2::length() const { return std::sqrt(x * x + y * y); }

Vector2 Vector2::normalized() const {
    float len = length();
    if (len == 0) return Vector2(0, 0);
    return Vector2(x / len, y / len);
}

void Vector2::print() const { std::cout << "(" << x << ", " << y << ")\n"; }