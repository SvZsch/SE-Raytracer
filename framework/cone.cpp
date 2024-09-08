// cone.cpp
#include "cone.hpp"                // Inkludiert die Header-Datei f�r die Cone-Klasse
#include "Hitpoint.hpp"            // Inkludiert die Header-Datei f�r die Hitpoint-Struktur
#include <cmath>                   // Inkludiert die Standard-Mathematikbibliothek
#include <glm/glm.hpp>             // Inkludiert die GLM-Bibliothek f�r Vektoroperationen
#include <algorithm>               // Inkludiert die Standard-Algorithmusbibliothek

const double M_PI = 3.14159265358979323846; // Definiert die Konstante Pi

Cone::Cone() : Shape(), radius_(1.0f), height_(1.0f) { // Standardkonstruktor, initialisiert Radius und H�he mit 1.0
    //std::cout << "Cone default constructor called" << std::endl; // Auskommentierte Debug-Ausgabe
}

Cone::Cone(std::string const& name, float radius, float height) // Konstruktor mit Namen, Radius und H�he
    : Shape(name), radius_(radius), height_(height) { // Initialisiert die Basisklasse und die Member-Variablen
    //std::cout << "Cone constructor called: " << name << std::endl; // Auskommentierte Debug-Ausgabe
}

Cone::Cone(std::string const& name, float radius, float height, std::shared_ptr<Material> const& material) // Konstruktor mit Namen, Radius, H�he und Material
    : Shape(name, material), radius_(radius), height_(height) { // Initialisiert die Basisklasse und die Member-Variablen
    //std::cout << "Cone constructor with material called: " << name << std::endl; // Auskommentierte Debug-Ausgabe
    setMaterial(material);  // Setzt das Material explizit
    if (material) { // Wenn ein Material �bergeben wurde
        //std::cout << "Material set: " << material->name << std::endl; // Auskommentierte Debug-Ausgabe
    }
    else { // Wenn kein Material �bergeben wurde
        //std::cout << "Warning: Null material provided for Cone " << name << std::endl; // Auskommentierte Debug-Ausgabe
    }
}

float Cone::area() const { // Methode zur Berechnung der Oberfl�che des Kegels
    return M_PI * radius_ * (radius_ + std::sqrt(height_ * height_ + radius_ * radius_)); // Berechnet und gibt die Oberfl�che zur�ck
}

float Cone::volume() const { // Methode zur Berechnung des Volumens des Kegels
    return (1.0f / 3.0f) * M_PI * radius_ * radius_ * height_; // Berechnet und gibt das Volumen zur�ck
}

Hitpoint Cone::intersectImpl(Ray const& ray) const { // Implementierung der Schnittpunktberechnung
    Hitpoint result; // Erstellt ein Hitpoint-Objekt zur Speicherung des Ergebnisses
    glm::vec3 co = ray.origin - glm::vec3(0, 0, 0);  // Berechnet den Vektor vom Kegelursprung zum Strahlursprung
    glm::vec3 dir = glm::normalize(ray.direction); // Normalisiert die Richtung des Strahls
    float k = radius_ / height_; // Berechnet den Tangens des halben �ffnungswinkels
    float k2 = k * k; // Berechnet das Quadrat von k

    // Berechnung f�r die Mantelfl�che
    float a = dir.x * dir.x + dir.z * dir.z - k2 * dir.y * dir.y; // Berechnet den Koeffizienten a der quadratischen Gleichung
    float b = 2 * (co.x * dir.x + co.z * dir.z - k2 * co.y * dir.y + k2 * height_ * dir.y); // Berechnet den Koeffizienten b
    float c = co.x * co.x + co.z * co.z - k2 * (co.y * co.y - 2 * height_ * co.y + height_ * height_); // Berechnet den Koeffizienten c
    float discriminant = b * b - 4 * a * c; // Berechnet die Diskriminante
    float t_mantle = std::numeric_limits<float>::max(); // Initialisiert t_mantle mit dem gr��tm�glichen Float-Wert

    if (std::abs(a) > 1e-6 && discriminant >= 0) { // Pr�ft, ob eine L�sung existiert
        float sqrt_discriminant = std::sqrt(discriminant); // Berechnet die Wurzel der Diskriminante
        float t1 = (-b - sqrt_discriminant) / (2 * a); // Berechnet die erste L�sung
        float t2 = (-b + sqrt_discriminant) / (2 * a); // Berechnet die zweite L�sung
        if (t1 > t2) std::swap(t1, t2); // Sortiert t1 und t2, sodass t1 <= t2

        auto check_mantle = [&](float t_check) { // Lambda-Funktion zur �berpr�fung des Mantelschnittpunkts
            if (t_check > 0) { // Wenn t_check positiv ist
                glm::vec3 p = co + t_check * dir; // Berechnet den Schnittpunkt
                if (p.y >= 0 && p.y <= height_) { // Wenn der Schnittpunkt innerhalb der Kegelh�he liegt
                    return t_check; // Gibt t_check zur�ck
                }
            }
            return std::numeric_limits<float>::max(); // Gibt den gr��tm�glichen Float-Wert zur�ck, wenn kein g�ltiger Schnittpunkt gefunden wurde
            };
        t_mantle = std::min(check_mantle(t1), check_mantle(t2)); // W�hlt den n�heren g�ltigen Schnittpunkt
    }

    // Berechnung f�r die Basis
    float t_base = std::numeric_limits<float>::max(); // Initialisiert t_base mit dem gr��tm�glichen Float-Wert
    if (std::abs(dir.y) > 1e-6) { // Pr�ft, ob der Strahl nicht parallel zur Basis ist
        float t = -co.y / dir.y; // Berechnet den t-Wert f�r den Schnittpunkt mit der Basisebene
        if (t > 0) { // Wenn t positiv ist
            glm::vec3 p = co + t * dir; // Berechnet den Schnittpunkt
            if (p.x * p.x + p.z * p.z <= radius_ * radius_) { // Wenn der Schnittpunkt innerhalb des Basiskreises liegt
                t_base = t; // Setzt t_base auf t
            }
        }
    }

    // W�hlt den n�heren Schnittpunkt zwischen Mantel und Basis
    float t = std::min(t_mantle, t_base); // W�hlt den kleineren Wert von t_mantle und t_base
    if (t < std::numeric_limits<float>::max()) { // Wenn ein g�ltiger Schnittpunkt gefunden wurde
        result.hit = true; // Setzt hit auf true
        result.distance = t; // Setzt die Distanz auf t
        result.hit_p = ray.origin + t * dir; // Berechnet den Schnittpunkt
        if (t == t_base) { // Wenn der Schnittpunkt auf der Basis liegt
            result.normal = glm::vec3(0, -1, 0); // Setzt die Normale auf (0, -1, 0)
        }
        else { // Wenn der Schnittpunkt auf der Mantelfl�che liegt
            float y = result.hit_p.y; // Extrahiert die y-Koordinate des Schnittpunkts
            float r = std::sqrt(result.hit_p.x * result.hit_p.x + result.hit_p.z * result.hit_p.z); // Berechnet den Radius an der Schnittpunkth�he
            result.normal = glm::normalize(glm::vec3(result.hit_p.x, r * k, result.hit_p.z)); // Berechnet und normalisiert die Normale
            if (y >= height_ - 1e-4f) { // Wenn der Schnittpunkt nahe der Kegelspitze liegt
                result.normal = glm::vec3(0, 1, 0); // Setzt die Normale auf (0, 1, 0)
            }
        }
        result.color_obj = material_; // Setzt das Material des Trefferpunkts
        result.name_obj = getName(); // Setzt den Namen des getroffenen Objekts
    }
    return result; // Gibt das Hitpoint-Objekt zur�ck
}