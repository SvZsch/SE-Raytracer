// Light.hpp
#ifndef LIGHT_HPP                  // Verhindert mehrfaches Einbinden dieser Datei
#define LIGHT_HPP

#include <glm/vec3.hpp>            // Für 3D-Vektoren
#include "color.hpp"               // Bindet Farbdefinitionen ein
#include <string>                  // Für string-Objekte

struct Light                       // Definiert eine Struktur für Lichtquellen
{
    glm::vec3 position;            // Position der Lichtquelle im 3D-Raum
    Color color;                   // Farbe des Lichts
    std::string name;              // Name der Lichtquelle

    Light(glm::vec3 const& pos, Color const& col, std::string const& n = "default_light")
        : position(pos), color(col), name(n) {} // Konstruktor zur Initialisierung aller Licht-Eigenschaften
};

#endif                             // Beendet die Schutz vor Mehrfacheinbindung