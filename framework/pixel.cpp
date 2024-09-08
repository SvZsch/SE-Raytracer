// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andre Schollmeyer and Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Pixel
// -----------------------------------------------------------------------------

#include "pixel.hpp"               // Inkludiert die Header-Datei für die Pixel-Klasse
#include <ostream>                 // Inkludiert die Standard-Ausgabestrom-Bibliothek

Pixel::Pixel(unsigned int a, unsigned int b) // Konstruktor für die Pixel-Klasse mit zwei Parametern
    : x(a),                          // Initialisiert die x-Koordinate mit dem Wert von a
    y(b),                          // Initialisiert die y-Koordinate mit dem Wert von b
    color{ 0,0,0 }                   // Initialisiert die Farbe mit Schwarz (0,0,0)
{}

void Pixel::print(std::ostream& os) const // Methode zum Ausgeben des Pixels in einen Ausgabestrom
{
    os << "Pixel[" << (int)x << ',' << (int)y << "](" // Gibt die Position des Pixels aus
        << color.r << ','                     // Gibt den Rotwert der Farbe aus
        << color.g << ','                     // Gibt den Grünwert der Farbe aus
        << color.b << ')';                    // Gibt den Blauwert der Farbe aus
}

std::ostream& operator<<(std::ostream& os, const Pixel& a) // Überladener Ausgabeoperator für Pixel
{
    std::ostream::sentry cerberus(os);      // Erstellt einen sentry-Objekt für den Ausgabestrom
    if (cerberus)                            // Wenn der sentry-Objekt gültig ist
        a.print(os);                           // Ruft die print-Methode des Pixels auf
    return os;                               // Gibt den Ausgabestrom zurück
}