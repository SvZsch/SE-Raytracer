// Material.hpp
#ifndef MATERIAL_HPP               // Verhindert mehrfaches Einbinden dieser Datei
#define MATERIAL_HPP

#include "color.hpp"               // Bindet Farbdefinitionen ein
#include <string>                  // F�r string-Objekte

struct Material {                  // Definiert eine Struktur f�r Materialeigenschaften
    std::string name;              // Name des Materials
    Color ka;                      // Ambiente Farbkomponente
    Color kd;                      // Diffuse Farbkomponente
    Color ks;                      // Spekulare (gl�nzende) Farbkomponente
    float m;                       // Glanzfaktor (Shininess)
    float refraction_index;        // Brechungsindex des Materials
    float transparency;            // Transparenz (0.0 = undurchsichtig, 1.0 = v�llig durchsichtig)
    Color emission;                // Emissionsfarbe (f�r selbstleuchtende Materialien)

    Material(                      // Konstruktor mit Standardwerten
        std::string name = "default_material", // Standardname f�r das Material
        Color ka = Color(0.1f, 0.1f, 0.1f),    // Schwache ambiente Reflexion (dunkelgrau)
        Color kd = Color(0.7f, 0.7f, 0.7f),    // Mittlere diffuse Reflexion (hellgrau)
        Color ks = Color(0.2f, 0.2f, 0.2f),    // Schwache spekulare Reflexion (leicht gl�nzend)
        float m = 10.0f,                       // Mittlerer Glanzfaktor (m��ig scharfe Glanzlichter)
        float refraction_index = 1.0f,         // Standardbrechungsindex (wie Luft)
        float transparency = 0.0f              // Standardm��ig undurchsichtig
    ) : name(name), ka(ka), kd(kd), ks(ks), m(m),
        refraction_index(refraction_index), transparency(transparency) {} // Initialisierung der Membervariablen

    friend std::ostream& operator<<(std::ostream& os, const Material& mat) {
        // �berladener Ausgabeoperator f�r einfache Ausgabe
        os << "Material: " << mat.name         // Gibt den Namen des Materials aus
            << " ka: " << mat.ka                // Gibt die ambiente Farbkomponente aus
            << " kd: " << mat.kd                // Gibt die diffuse Farbkomponente aus
            << " ks: " << mat.ks                // Gibt die spekulare Farbkomponente aus
            << " m: " << mat.m                  // Gibt den Glanzfaktor aus
            << " refraction_index: " << mat.refraction_index // Gibt den Brechungsindex aus
            << " transparency: " << mat.transparency;        // Gibt die Transparenz aus
        return os;                 // Gibt den Ausgabestream zur�ck
    }
};
#endif                             // Beendet die Schutz vor Mehrfacheinbindung