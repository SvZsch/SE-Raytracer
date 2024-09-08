#ifndef BUW_SHADING_HPP            // Beginn der Include-Guard: Verhindert mehrfaches Einbinden dieser Datei
#define BUW_SHADING_HPP            // Definition des Include-Guard-Makros

#include "ray.hpp"                 // Inkludiert die Definitionen für Strahlen
#include "shape.hpp"               // Inkludiert die Definitionen für geometrische Formen
#include "light.hpp"               // Inkludiert die Definitionen für Lichtquellen
#include "hitpoint.hpp"            // Inkludiert die Definitionen für Trefferpunkte
#include "color.hpp"               // Inkludiert die Definitionen für Farben
#include <vector>                  // Inkludiert die Standard-Vektor-Bibliothek
#include <memory>                  // Inkludiert die Smart-Pointer-Bibliothek

class Shading {                    // Beginn der Shading-Klassen-Definition
public:                            // Öffentlicher Bereich der Klasse
    Color compute_lighting(const Hitpoint& hitpoint, const Ray& ray,
        const std::shared_ptr<Shape>& root_shape,
        const std::vector<std::shared_ptr<Light>>& lights, int depth) const; // Deklaration der Methode zur Berechnung der Beleuchtung

    Hitpoint compute_intersection(Ray const& ray, const std::shared_ptr<Shape>& root_shape) const; // Deklaration der Methode zur Berechnung von Schnittpunkten

    static Color blend_colors(Color c1, Color c2, float weight1, float weight2); // Statische Methode zum Mischen von Farben

private:                           // Privater Bereich der Klasse
    Color compute_diffuse(Hitpoint const& hitpoint, Light const& light); // Private Methode zur Berechnung der diffusen Beleuchtung

    Color compute_specular(Hitpoint const& hitpoint, Light const& light, Ray const& ray); // Private Methode zur Berechnung der spekularen Beleuchtung
};                                 // Ende der Shading-Klassen-Definition

#endif // BUW_SHADING_HPP          // Ende der Include-Guard