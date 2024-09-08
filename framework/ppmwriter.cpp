// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andre Schollmeyer and Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// PpmWriter
// -----------------------------------------------------------------------------

#include "ppmwriter.hpp"           // Inkludiert die Header-Datei für die PpmWriter-Klasse

// header, system
#include <iostream>                // Inkludiert die Standard-Ein-/Ausgabe-Bibliothek
#include <fstream>                 // Inkludiert die Dateistream-Bibliothek
#include <cmath>                   // Inkludiert die mathematische Bibliothek
#include <algorithm>               // Inkludiert die Algorithmus-Bibliothek

PpmWriter::PpmWriter(std::size_t w, std::size_t h, std::string const& file)
    : file_(file),                   // Initialisiert den Dateinamen
    width_(w),                     // Initialisiert die Breite
    height_(h) {                   // Initialisiert die Höhe
    data_.resize(w * h * (sizeof(Color) / sizeof(float))); // Reserviert Speicher für die Bilddaten
}

PpmWriter::PpmWriter(std::size_t w, std::size_t h)
    : file_("untitled.ppm"),         // Setzt den Standarddateinamen
    width_(w),                     // Initialisiert die Breite
    height_(h) {                   // Initialisiert die Höhe
    data_.resize(w * h * (sizeof(Color) / sizeof(float))); // Reserviert Speicher für die Bilddaten
}

void
PpmWriter::write(Pixel const& p)
{
    size_t buf_pos = width_ * (height_ - 1 - p.y) + p.x; // Berechnet die Position im Puffer
    std::size_t pos = (sizeof(Color) / sizeof(float)) * buf_pos; // Berechnet die Position in data_
    if (pos + ((sizeof(Color) / sizeof(float)) - 1) > data_.size() || (int)buf_pos < 0) {
        std::cerr << "Fatal Error PpmWriter::write(Pixel p) : Critical write position\n"; // Gibt eine Fehlermeldung aus
    }
    else {
        data_[pos] = (unsigned int)std::max(0.0, std::min(255.0 * p.color.r, 255.0));   // Schreibt den Rotwert
        data_[pos + 1] = (unsigned int)std::max(0.0, std::min(255.0 * p.color.g, 255.0)); // Schreibt den Grünwert
        data_[pos + 2] = (unsigned int)std::max(0.0, std::min(255.0 * p.color.b, 255.0)); // Schreibt den Blauwert
    }
}

void
PpmWriter::save(std::string const& file)
{
    file_ = file;                    // Aktualisiert den Dateinamen
    save();                          // Ruft die save()-Methode auf
}

void
PpmWriter::save()
{
    const std::size_t line_length = 18; // Definiert die maximale Länge einer Zeile
    std::fstream file(file_.c_str(), std::ios::out); // Öffnet die Datei zum Schreiben
    file.clear();                    // Löscht eventuelle Fehlerflags
    file << "P3 "                    // Schreibt den PPM-Header
        << (unsigned int)width_ << " "
        << (unsigned int)height_ << " "
        << 255 << " \n";
    std::size_t pos = 1;             // Initialisiert die Positionsvariable
    for (auto i = data_.begin(); i < data_.end(); ++i, ++pos) {
        file << (int)*i << " ";      // Schreibt den Farbwert
        if (pos > line_length) {     // Wenn die Zeilenlänge überschritten wird
            file << "\n";              // Fügt einen Zeilenumbruch ein
            pos = 0;                     // Setzt die Position zurück
        }
    }
    file.close();                    // Schließt die Datei
}