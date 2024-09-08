// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andre Schollmeyer and Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Pixel
// -----------------------------------------------------------------------------
#ifndef BUW_PIXEL_HPP              // Verhindert mehrfaches Einbinden dieser Datei
#define BUW_PIXEL_HPP

// header, system
#include <iosfwd>                  // Für vorwärts-deklarierte Ein-/Ausgabe-Operationen

// header, project
#include <color.hpp>               // Bindet die Farbdefinitionen ein

class Pixel
{
public:
	Pixel(unsigned int = 0, unsigned int = 0); // Konstruktor mit Standardwerten für x und y
	void print(std::ostream&) const; // Methode zum Ausgeben des Pixels

	unsigned int x = 0;              // x-Koordinate des Pixels
	unsigned int y = 0;              // y-Koordinate des Pixels
	Color color = { 1.0, 1.0, 1.0 };   // Farbe des Pixels, standardmäßig weiß
};

std::ostream& operator<<(std::ostream&, const Pixel&); // Überladener Ausgabeoperator für Pixel

#endif // BUW_PIXEL_HPP            // Beendet die Schutz vor Mehrfacheinbindung