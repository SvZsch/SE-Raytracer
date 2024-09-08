#include <glm/glm.hpp>             // Inkludiert die OpenGL Mathematics (GLM) Hauptbibliothek für Vektoroperationen
#include <glm/gtc/constants.hpp>   // Inkludiert GLM-Konstanten wie pi
#include "shade.hpp"               // Inkludiert die Shade-Header-Datei, die die Shading-Klasse definiert
#include "Material.hpp"            // Inkludiert die Material-Header-Datei für Materialeigenschaften
#include "Hitpoint.hpp"            // Inkludiert die Hitpoint-Header-Datei für Trefferpunktinformationen

#include "Ray.hpp"                 // Inkludiert die Ray-Header-Datei für Strahldefinitionen
#include "Shape.hpp"               // Inkludiert die Shape-Header-Datei für geometrische Formen
#include <memory>                  // Inkludiert die memory-Bibliothek für std::shared_ptr

#include <algorithm>               // Inkludiert die algorithm-Bibliothek für std::max und andere Funktionen
#include <iostream>                // Inkludiert die iostream-Bibliothek für Ein-/Ausgabeoperationen

Hitpoint Shading::compute_intersection(Ray const& ray, const std::shared_ptr<Shape>& root_shape) const { // Definiert die Methode zur Berechnung des Schnittpunkts
    return root_shape->intersect(ray); // Ruft die intersect-Methode des root_shape auf und gibt das Ergebnis zurück
}

Color Shading::compute_lighting(const Hitpoint& hitpoint, const Ray& ray,    // Definiert die Hauptmethode zur Berechnung der Beleuchtung
    const std::shared_ptr<Shape>& root_shape,
    const std::vector<std::shared_ptr<Light>>& lights,
    int depth) const
{
    const float EPSILON = 0.001f;  // Definiert einen kleinen Wert zur Vermeidung von Self-Intersection
    if (depth <= 0) return Color(0.0f, 0.0f, 0.0f); // Beendet die Rekursion, wenn die maximale Tiefe erreicht ist

    if (!hitpoint.color_obj) {     // Prüft, ob ein gültiges Material vorhanden ist
        return Color(0.0f, 0.0f, 0.0f);  // Gibt Schwarz zurück, wenn kein Material vorhanden ist
    }

    Color color(0.0f, 0.0f, 0.0f); // Initialisiert die Farbe mit Schwarz

    Color ambient = hitpoint.color_obj->ka * Color(0.1f, 0.1f, 0.1f); // Berechnet die ambiente Komponente
    color += ambient;              // Addiert die ambiente Komponente zur Gesamtfarbe

    color += hitpoint.color_obj->emission; // Addiert die Emissionskomponente zur Gesamtfarbe

    for (const auto& light : lights) { // Schleife über alle Lichtquellen
        glm::vec3 light_dir = glm::normalize(light->position - hitpoint.hit_p); // Berechnet die normalisierte Richtung zum Licht
        float light_distance = glm::length(light->position - hitpoint.hit_p); // Berechnet die Distanz zum Licht

        Ray shadow_ray(hitpoint.hit_p + hitpoint.normal * EPSILON, light_dir); // Erzeugt einen Schattenstrahl
        Hitpoint shadow_hit = compute_intersection(shadow_ray, root_shape); // Berechnet den Schnittpunkt für den Schattenstrahl

        float shadow_factor = 1.0f; // Initialisiert den Schattenfaktor mit 1 (kein Schatten)
        if (shadow_hit.hit && shadow_hit.distance < light_distance) { // Prüft, ob der Punkt im Schatten liegt
            shadow_factor = 0.1f;  // Setzt den Schattenfaktor auf 0.1, wenn im Schatten
        }

        float diff = std::max(glm::dot(hitpoint.normal, light_dir), 0.0f); // Berechnet den diffusen Faktor
        Color diffuse = shadow_factor * hitpoint.color_obj->kd * light->color * diff; // Berechnet die diffuse Komponente

        glm::vec3 view_dir = glm::normalize(-ray.direction); // Berechnet die normalisierte Blickrichtung
        glm::vec3 reflect_dir = glm::reflect(-light_dir, hitpoint.normal); // Berechnet die Reflexionsrichtung
        float spec = std::pow(std::max(glm::dot(view_dir, reflect_dir), 0.0f), hitpoint.color_obj->m); // Berechnet den spekularen Faktor
        Color specular = shadow_factor * hitpoint.color_obj->ks * light->color * spec; // Berechnet die spekulare Komponente

        color += diffuse + specular; // Addiert diffuse und spekulare Komponenten zur Gesamtfarbe
    }

    if (glm::length(glm::vec3(hitpoint.color_obj->ks.r, hitpoint.color_obj->ks.g, hitpoint.color_obj->ks.b)) > 0.0f) { // Prüft, ob Reflexion berechnet werden soll
        Ray reflect_ray(hitpoint.hit_p + hitpoint.normal * EPSILON, hitpoint.reflection_dir); // Erzeugt einen Reflexionsstrahl
        Hitpoint reflect_hit = compute_intersection(reflect_ray, root_shape); // Berechnet den Schnittpunkt für den Reflexionsstrahl
        if (reflect_hit.hit) { // Wenn ein Reflexionstreffer gefunden wurde
            Color reflect_color = compute_lighting(reflect_hit, reflect_ray, root_shape, lights, depth - 1); // Berechnet die Farbe des reflektierten Strahls
            color += hitpoint.color_obj->ks * reflect_color; // Addiert die reflektierte Farbe zur Gesamtfarbe
        }
    }

    if (hitpoint.color_obj->transparency > 0.0f && depth > 0) { // Prüft, ob Brechung berechnet werden soll
        float n1 = 1.0f;  // Brechungsindex von Luft
        float n2 = hitpoint.color_obj->refraction_index; // Brechungsindex des Materials

        glm::vec3 normal = hitpoint.normal; // Normale am Trefferpunkt
        float cos_i = glm::dot(-ray.direction, normal); // Cosinus des Einfallswinkels
        bool entering = cos_i > 0; // Prüft, ob der Strahl in das Objekt eintritt

        if (!entering) { // Wenn der Strahl das Objekt verlässt
            std::swap(n1, n2); // Tauscht die Brechungsindizes
            normal = -normal; // Kehrt die Normale um
            cos_i = -cos_i; // Korrigiert den Cosinus des Einfallswinkels
        }

        float n = n1 / n2; // Berechnet das Verhältnis der Brechungsindizes
        float sin_t2 = n * n * (1.0f - cos_i * cos_i); // Berechnet das Quadrat des Sinus des Brechungswinkels

        Color refracted_color; // Deklariert die Variable für die gebrochene Farbe
        Color reflected_color; // Deklariert die Variable für die reflektierte Farbe

        if (sin_t2 <= 1.0f) { // Prüft, ob Brechung möglich ist
            float cos_t = std::sqrt(1.0f - sin_t2); // Berechnet den Cosinus des Brechungswinkels
            glm::vec3 refracted_dir = glm::normalize(n * ray.direction + (n * cos_i - cos_t) * normal); // Berechnet die Richtung des gebrochenen Strahls

            Ray refracted_ray(hitpoint.hit_p - normal * EPSILON, refracted_dir); // Erzeugt den gebrochenen Strahl
            Hitpoint refracted_hit = compute_intersection(refracted_ray, root_shape); // Berechnet den Schnittpunkt für den gebrochenen Strahl
            if (refracted_hit.hit) { // Wenn ein Brechungstreffer gefunden wurde
                refracted_color = compute_lighting(refracted_hit, refracted_ray, root_shape, lights, depth - 1); // Berechnet die Farbe des gebrochenen Strahls

                float distance = glm::length(refracted_hit.hit_p - hitpoint.hit_p); // Berechnet die Distanz im Material
                Color absorption = Color( // Berechnet die Absorption basierend auf dem Beer'schen Gesetz
                    std::exp(-hitpoint.color_obj->kd.r * distance * 0.15f),
                    std::exp(-hitpoint.color_obj->kd.g * distance * 0.15f),
                    std::exp(-hitpoint.color_obj->kd.b * distance * 0.15f)
                );
                refracted_color = refracted_color * absorption; // Wendet die Absorption auf die gebrochene Farbe an
            }
            else { // Wenn kein Brechungstreffer gefunden wurde
                Color background_color(0.6f, 0.8f, 1.0f); // Definiert eine Hintergrundfarbe
                refracted_color = background_color * 1.2f; // Setzt die gebrochene Farbe auf eine hellere Version der Hintergrundfarbe
            }
        }

        glm::vec3 reflected_dir = glm::normalize(glm::reflect(ray.direction, normal)); // Berechnet die Richtung des reflektierten Strahls
        Ray reflected_ray(hitpoint.hit_p + normal * EPSILON, reflected_dir); // Erzeugt den reflektierten Strahl
        Hitpoint reflected_hit = compute_intersection(reflected_ray, root_shape); // Berechnet den Schnittpunkt für den reflektierten Strahl
        if (reflected_hit.hit) { // Wenn ein Reflexionstreffer gefunden wurde
            reflected_color = compute_lighting(reflected_hit, reflected_ray, root_shape, lights, depth - 1); // Berechnet die Farbe des reflektierten Strahls
        }
        else { // Wenn kein Reflexionstreffer gefunden wurde
            Color background_color(0.6f, 0.8f, 1.0f); // Definiert eine Hintergrundfarbe
            reflected_color = background_color; // Setzt die reflektierte Farbe auf die Hintergrundfarbe
        }

        float r0 = ((n1 - n2) / (n1 + n2)) * ((n1 - n2) / (n1 + n2)); // Berechnet den Fresnel-Reflexionskoeffizienten bei senkrechtem Einfall
        float fresnel = r0 + (1 - r0) * std::pow(1 - cos_i, 5); // Berechnet den Fresnel-Faktor

        Color mixed_color = Shading::blend_colors(reflected_color, refracted_color, fresnel, 1.0f - fresnel); // Mischt reflektierte und gebrochene Farbe

        color = Shading::blend_colors(mixed_color, color, 0.8f, 0.2f); // Mischt die kombinierte Farbe mit der lokalen Beleuchtung
    }

    //std::cout << "Final color: (" << color.r << ", " << color.g << ", " << color.b << ")" << std::endl; // Auskommentierte Debug-Ausgabe

    return color; // Gibt die berechnete Farbe zurück
}

Color Shading::compute_diffuse(Hitpoint const& hitpoint, Light const& light) { // Definiert die Methode zur Berechnung der diffusen Komponente
    glm::vec3 light_dir = glm::normalize(light.position - hitpoint.hit_p); // Berechnet die normalisierte Richtung zum Licht
    float diff = std::max(glm::dot(hitpoint.normal, light_dir), 0.0f); // Berechnet den diffusen Faktor
    return hitpoint.color_obj->kd * light.color * diff; // Gibt die diffuse Farbe zurück
}

Color Shading::compute_specular(Hitpoint const& hitpoint, Light const& light, Ray const& ray) { // Definiert die Methode zur Berechnung der spekularen Komponente
    glm::vec3 view_dir = glm::normalize(-ray.direction); // Berechnet die normalisierte Blickrichtung
    glm::vec3 reflect_dir = glm::reflect(-light.position, hitpoint.normal); // Berechnet die Reflexionsrichtung
    float spec = std::pow(std::max(glm::dot(view_dir, reflect_dir), 0.0f), hitpoint.color_obj->m); // Berechnet den spekularen Faktor
    return hitpoint.color_obj->ks * light.color * spec; // Gibt die spekulare Farbe zurück
}

Color Shading::blend_colors(Color c1, Color c2, float weight1, float weight2) { // Definiert die statische Methode zum Mischen zweier Farben
    const float MIN_COLOR_VALUE = 1e-6f; // Definiert den minimalen Farbwert zur Vermeidung von Divisionen durch Null
    float total_weight = weight1 + weight2; // Berechnet das Gesamtgewicht
    float normalized_weight1 = weight1 / total_weight; // Normalisiert das erste Gewicht
    float normalized_weight2 = weight2 / total_weight; // Normalisiert das zweite Gewicht
    return Color( // Gibt die gemischte Farbe zurück
        std::clamp(normalized_weight1 * c1.r + normalized_weight2 * c2.r, MIN_COLOR_VALUE, 1.0f), // Mischt und begrenzt die Rot-Komponente
        std::clamp(normalized_weight1 * c1.g + normalized_weight2 * c2.g, MIN_COLOR_VALUE, 1.0f), // Mischt und begrenzt die Grün-Komponente
        std::clamp(normalized_weight1 * c1.b + normalized_weight2 * c2.b, MIN_COLOR_VALUE, 1.0f)  // Mischt und begrenzt die Blau-Komponente
    );
}