// cylinder.cpp
#include "cylinder.hpp"            // Inkludiert die Header-Datei f�r die Cylinder-Klasse
#include <cmath>                   // Inkludiert die Standard-Mathematikbibliothek
#include <algorithm>               // Inkludiert die Standard-Algorithmusbibliothek

const double M_PI = 3.14159265358979323846; // Definiert die Konstante Pi

Cylinder::Cylinder() : Shape(), radius_(1.0f), height_(1.0f) {} // Standardkonstruktor, initialisiert Radius und H�he mit 1.0

Cylinder::Cylinder(std::string const& name, float radius, float height) // Konstruktor mit Namen, Radius und H�he
    : Shape(name), radius_(radius), height_(height) {} // Initialisiert die Basisklasse und die Member-Variablen

Cylinder::Cylinder(std::string const& name, float radius, float height, std::shared_ptr<Material> const& material) // Konstruktor mit Namen, Radius, H�he und Material
    : Shape(name, material), radius_(radius), height_(height) { // Initialisiert die Basisklasse und die Member-Variablen
    setMaterial(material); // Setzt das Material explizit
}

float Cylinder::area() const { // Methode zur Berechnung der Oberfl�che des Zylinders
    return 2 * M_PI * radius_ * height_ + 2 * M_PI * radius_ * radius_; // Berechnet und gibt die Oberfl�che zur�ck
}

float Cylinder::volume() const { // Methode zur Berechnung des Volumens des Zylinders
    return M_PI * radius_ * radius_ * height_; // Berechnet und gibt das Volumen zur�ck
}

Hitpoint Cylinder::intersectImpl(Ray const& ray) const { // Implementierung der Schnittpunktberechnung
    Hitpoint result; // Erstellt ein Hitpoint-Objekt zur Speicherung des Ergebnisses

    // Seitenberechnung des Zylinders
    float a = ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z; // Berechnet den Koeffizienten a der quadratischen Gleichung
    float b = 2 * (ray.origin.x * ray.direction.x + ray.origin.z * ray.direction.z); // Berechnet den Koeffizienten b
    float c = ray.origin.x * ray.origin.x + ray.origin.z * ray.origin.z - radius_ * radius_; // Berechnet den Koeffizienten c
    float discriminant = b * b - 4 * a * c; // Berechnet die Diskriminante

    if (discriminant < 0) { // Wenn die Diskriminante negativ ist, gibt es keinen Schnittpunkt
        return result; // Gibt das leere Hitpoint-Objekt zur�ck
    }

    float t1 = (-b - std::sqrt(discriminant)) / (2 * a); // Berechnet den ersten m�glichen t-Wert
    float t2 = (-b + std::sqrt(discriminant)) / (2 * a); // Berechnet den zweiten m�glichen t-Wert

    // Deckfl�chenberechnung
    float t_bottom = (0 - ray.origin.y) / ray.direction.y; // Berechnet den t-Wert f�r die untere Deckfl�che
    float t_top = (height_ - ray.origin.y) / ray.direction.y; // Berechnet den t-Wert f�r die obere Deckfl�che

    // Finde den n�chsten g�ltigen Schnittpunkt
    float t = std::numeric_limits<float>::max(); // Initialisiert t mit dem gr��tm�glichen Float-Wert
    glm::vec3 normal; // Deklariert einen Vektor f�r die Normale

    // Pr�fe Seitenfl�che
    if (t1 > 0 && t1 < t) { // Wenn t1 positiv und kleiner als der aktuelle t-Wert ist
        glm::vec3 p = ray.origin + t1 * ray.direction; // Berechnet den Schnittpunkt
        if (p.y >= 0 && p.y <= height_) { // Wenn der Schnittpunkt innerhalb der Zylinderh�he liegt
            t = t1; // Aktualisiert t
            normal = glm::normalize(glm::vec3(p.x, 0, p.z)); // Berechnet die Normale
        }
    }
    if (t2 > 0 && t2 < t) { // Wenn t2 positiv und kleiner als der aktuelle t-Wert ist
        glm::vec3 p = ray.origin + t2 * ray.direction; // Berechnet den Schnittpunkt
        if (p.y >= 0 && p.y <= height_) { // Wenn der Schnittpunkt innerhalb der Zylinderh�he liegt
            t = t2; // Aktualisiert t
            normal = glm::normalize(glm::vec3(p.x, 0, p.z)); // Berechnet die Normale
        }
    }

    // Pr�fe untere Deckfl�che
    if (t_bottom > 0 && t_bottom < t) { // Wenn t_bottom positiv und kleiner als der aktuelle t-Wert ist
        glm::vec3 p = ray.origin + t_bottom * ray.direction; // Berechnet den Schnittpunkt
        if (p.x * p.x + p.z * p.z <= radius_ * radius_) { // Wenn der Schnittpunkt innerhalb des Zylinderradius liegt
            t = t_bottom; // Aktualisiert t
            normal = glm::vec3(0, -1, 0); // Setzt die Normale auf (0, -1, 0)
        }
    }

    // Pr�fe obere Deckfl�che
    if (t_top > 0 && t_top < t) { // Wenn t_top positiv und kleiner als der aktuelle t-Wert ist
        glm::vec3 p = ray.origin + t_top * ray.direction; // Berechnet den Schnittpunkt
        if (p.x * p.x + p.z * p.z <= radius_ * radius_) { // Wenn der Schnittpunkt innerhalb des Zylinderradius liegt
            t = t_top; // Aktualisiert t
            normal = glm::vec3(0, 1, 0); // Setzt die Normale auf (0, 1, 0)
        }
    }

    if (t < std::numeric_limits<float>::max()) { // Wenn ein g�ltiger Schnittpunkt gefunden wurde
        result.hit = true; // Setzt hit auf true
        result.distance = t; // Setzt die Distanz auf t
        result.hit_p = ray.origin + t * ray.direction; // Berechnet den Schnittpunkt
        result.normal = normal; // Setzt die Normale
        result.color_obj = material_; // Setzt das Material des Trefferpunkts
        result.name_obj = getName(); // Setzt den Namen des getroffenen Objekts
    }

    return result; // Gibt das Hitpoint-Objekt zur�ck
}