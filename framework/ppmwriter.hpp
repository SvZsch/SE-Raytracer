// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andre Schollmeyer and Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// PpmWriter
// -----------------------------------------------------------------------------
#ifndef BUW_PPMWRITER_HPP          // Verhindert mehrfaches Einbinden dieser Datei
#define BUW_PPMWRITER_HPP

// header, system
#include <string>                  // Für string-Objekte
#include <vector>                  // Für vector-Container

// header, project
#include <pixel.hpp>               // Bindet die Pixel-Definitionen ein

class PpmWriter
{
public:
	PpmWriter(std::size_t w, std::size_t h, std::string const& file); // Konstruktor mit Breite, Höhe und Dateiname
	PpmWriter(std::size_t w, std::size_t h);                          // Konstruktor nur mit Breite und Höhe

public:
	void write(Pixel const& p);      // Schreibt ein Pixel in den Bilddaten-Buffer
	void save(std::string const& file); // Speichert das Bild in eine spezifizierte Datei
	void save();                     // Speichert das Bild in die beim Konstruktor angegebene Datei

private:
	std::string file_;               // Speichert den Dateinamen
	std::vector<unsigned int> data_; // Speichert die Bilddaten
	size_t width_;                   // Breite des Bildes
	size_t height_;                  // Höhe des Bildes
};

#endif // BUW_PPMWRITER_HPP        // Beendet die Schutz vor Mehrfacheinbindung