// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Color
// -----------------------------------------------------------------------------
#ifndef BUW_COLOR_HPP              // Verhindert mehrfaches Einbinden dieser Datei
#define BUW_COLOR_HPP

#include <iostream>                // Für Ein- und Ausgabeoperationen
#include <algorithm>               // Für die min-Funktion

struct Color
{
    friend std::ostream& operator<<(std::ostream& os, Color const& c)
    {                              // Definiert, wie eine Farbe ausgegeben wird
        os << "(" << c.r << "," << c.g << "," << c.b << ")\n";
        return os;
    }

    Color& operator+=(Color const& other)
    {                              // Addiert eine andere Farbe zu dieser
        r += other.r;
        g += other.g;
        b += other.b;
        return *this;
    }

    Color& operator-=(Color const& other)
    {                              // Subtrahiert eine andere Farbe von dieser
        r -= other.r;
        g -= other.g;
        b -= other.b;
        return *this;
    }

    friend Color operator+(Color const& a, Color const& b)
    {                              // Addiert zwei Farben
        auto tmp(a);
        tmp += b;
        return tmp;
    }

    friend Color operator-(Color const& a, Color const& b)
    {                              // Subtrahiert eine Farbe von einer anderen
        auto tmp(a);
        tmp -= b;
        return tmp;
    }

    bool operator!=(Color const& other) const
    {                              // Vergleicht zwei Farben auf Ungleichheit
        return r != other.r || g != other.g || b != other.b;
    }

    float r{ 0.0f };               // Rotwert der Farbe
    float g{ 0.0f };               // Grünwert der Farbe
    float b{ 0.0f };               // Blauwert der Farbe

    Color operator*(float scalar) const
    {                              // Multipliziert die Farbe mit einem Skalar
        return Color{ r * scalar, g * scalar, b * scalar };
    }

    Color operator*(const Color& other) const
    {                              // Multipliziert zwei Farben komponentenweise
        return Color{
            std::min(1.0f, r * other.r),
            std::min(1.0f, g * other.g),
            std::min(1.0f, b * other.b)
        };
    }

    friend Color operator*(float scalar, Color const& c)
    {                              // Erlaubt Multiplikation in der Form Skalar * Farbe
        return c * scalar;
    }
};

#endif //#define BUW_COLOR_HPP     // Beendet die Schutz vor Mehrfacheinbindung