// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Color
// -----------------------------------------------------------------------------
#ifndef BUW_COLOR_HPP
#define BUW_COLOR_HPP
#include <iostream>
struct Color
{
    friend std::ostream& operator<<(std::ostream& os, Color const& c)
    {
        os << "(" << c.r << "," << c.g << "," << c.b << ")\n";
        return os;
    }
    Color& operator+=(Color const& other)
    {
        r += other.r;
        g += other.g;
        b += other.b;
        return *this;
    }
    Color& operator-=(Color const& other)
    {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        return *this;
    }
    friend Color operator+(Color const& a, Color const& b)
    {
        auto tmp(a);
        tmp += b;
        return tmp;
    }
    friend Color operator-(Color const& a, Color const& b)
    {
        auto tmp(a);
        tmp -= b;
        return tmp;
    }

    //Aufgabe 7 hinzugefügt
    // Hinzugefügter != Operator
    bool operator!=(Color const& other) const
    {
        return r != other.r || g != other.g || b != other.b;
    }
    float r{ 0.0f };
    float g{ 0.0f };
    float b{ 0.0f };

    // Multiplikation mit einem Skalar
    Color operator*(float scalar) const
    {
        return Color{ r * scalar, g * scalar, b * scalar };
    }

    // Multiplikation mit einer anderen Farbe (komponentenweise)
    Color operator*(Color const& other) const
    {
        return Color{ r * other.r, g * other.g, b * other.b };
    }

    // Freund-Funktion für Multiplikation Skalar * Color
    friend Color operator*(float scalar, Color const& c)
    {
        return c * scalar;
    }
};
#endif //#define BUW_COLOR_HPP