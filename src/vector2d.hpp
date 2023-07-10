#include <cmath>

class Vector2D {
    float x, y;
public:
    Vector2D(const float p_x, const float p_y) noexcept
        : x(p_x), y(p_y) {}
    float getX() const noexcept
    { return x; }
    float getY() const noexcept
    { return y; }

    void setX(const float p_x) noexcept
    {x = p_x; }
    void setY(const float p_y) noexcept
    {y = p_y; }


    const Vector2D operator+(const Vector2D &v2) const
    { return Vector2D(x + v2.x, y + v2.y); }

    friend Vector2D& operator+=(Vector2D &v1, Vector2D &v2) 
    {
        v1.x += v2.x;
        v2.y += v2.y;
        return v1;
    }
    
    const Vector2D operator-(const Vector2D &v2) const 
    { return Vector2D(x - v2.x, y - v2.y); }
    friend Vector2D& operator-=(Vector2D &v1, Vector2D &v2)
    {
        v1.x -= v2.x;
        v1.y -= v2.y;

        return v1;
    }
    
    const Vector2D operator*(const float scalar)
    { return Vector2D(x * scalar, y * scalar); }

    Vector2D& operator*=(const float scalar)
    {
        x *= scalar;
        y *= scalar;

        return *this;
    }
    
    const Vector2D operator/(const float scalar) const
    { return Vector2D(x / scalar, y / scalar); }

    Vector2D& operator/=(const float scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    float length() const noexcept
    { return std::sqrt(x * x + y * y); }

    void normalize() noexcept
    {
        float l = length();
        if (l > 0 )
            (*this) *= 1.f / 1;
    }
};
