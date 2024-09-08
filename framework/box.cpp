#include "box.hpp"                 // Inkludiert die Deklaration der Box-Klasse
#include "cmath"                   // Inkludiert mathematische Funktionen wie abs()
#include "ostream"                 // Inkludiert Funktionen für Ausgabeströme
#include "Hitpoint.hpp"            // Inkludiert die Definition der Hitpoint-Struktur
#include <iostream>                // Inkludiert Standard-Ein-/Ausgabefunktionen
#include <glm/glm.hpp>             // Inkludiert die GLM-Bibliothek für Vektoroperationen
#include <algorithm>               // Inkludiert Algorithmen wie std::min und std::max

const float EPSILON = 0.0001f;     // Definiert eine kleine Konstante für Fließkommavergleiche

Box::Box() :
    Shape{ "Quader" },             // Ruft den Konstruktor der Basisklasse mit dem Namen "Quader" auf
    min_{ 0.0f, 0.0f , 0.0f },     // Initialisiert den Minimalpunkt mit (0,0,0)
    max_{ 100.0f, 100.0f, 100.0f } // Initialisiert den Maximalpunkt mit (100,100,100)
{}

Box::Box(glm::vec3 min, glm::vec3 max) :
    Shape{ "Quader" },             // Ruft den Konstruktor der Basisklasse mit dem Namen "Quader" auf
    min_(min),                     // Initialisiert den Minimalpunkt mit dem übergebenen Wert
    max_(max)                      // Initialisiert den Maximalpunkt mit dem übergebenen Wert
{}

Box::Box(glm::vec3 min, glm::vec3 max, std::string const& name, std::shared_ptr<Material> const& material) :
    Shape{ name, material },       // Ruft den Konstruktor der Basisklasse mit Name und Material auf
    min_(min),                     // Initialisiert den Minimalpunkt mit dem übergebenen Wert
    max_(max)                      // Initialisiert den Maximalpunkt mit dem übergebenen Wert
{
    if (!material) {               // Prüft, ob kein Material übergeben wurde
        //std::cerr << "Warnung: Kein Material für Box " << name << " gesetzt" << std::endl;
    }
}

Box::Box(glm::vec3 min, glm::vec3 max, std::shared_ptr<Material> const& material) :
    Shape{ "Quader", material },   // Ruft den Konstruktor der Basisklasse mit "Quader" und Material auf
    min_(min),                     // Initialisiert den Minimalpunkt mit dem übergebenen Wert
    max_(max)                      // Initialisiert den Maximalpunkt mit dem übergebenen Wert
{}

float Box::area() const {
    float a = abs(max_.x - min_.x); // Berechnet die Länge der Box
    float b = abs(max_.y - min_.y); // Berechnet die Breite der Box
    float c = abs(max_.z - min_.z); // Berechnet die Höhe der Box
    return 2 * a * b + 2 * a * c + 2 * b * c; // Berechnet die Gesamtoberfläche
}

float Box::volume() const {
    return abs(max_.x - min_.x) * abs(max_.y - min_.y) * abs(max_.z - min_.z); // Berechnet das Volumen
}

std::ostream& Box::print(std::ostream& os) const {
    Shape::print(os);              // Ruft die print-Methode der Basisklasse auf
    return os << min_.x << " " << min_.y << " " << min_.z << " "
        << max_.x << " " << max_.y << " " << max_.z << std::endl; // Gibt die Eckpunkte aus
}

Hitpoint Box::intersectImpl(Ray const& ray) const {
    glm::vec3 t_min = (min_ - ray.origin) / ray.direction; // Berechnet t-Werte für den Minimalpunkt
    glm::vec3 t_max = (max_ - ray.origin) / ray.direction; // Berechnet t-Werte für den Maximalpunkt
    glm::vec3 t1 = glm::min(t_min, t_max); // Findet die kleineren t-Werte
    glm::vec3 t2 = glm::max(t_min, t_max); // Findet die größeren t-Werte
    float t_near = glm::max(glm::max(t1.x, t1.y), t1.z); // Findet den größten der kleineren t-Werte
    float t_far = glm::min(glm::min(t2.x, t2.y), t2.z);  // Findet den kleinsten der größeren t-Werte

    Hitpoint hit;
    if (t_far > t_near && t_far > 0) { // Prüft, ob ein gültiger Schnitt vorliegt
        hit.hit = true;
        hit.distance = (t_near > 0) ? t_near : t_far; // Wählt den näheren Schnittpunkt
        hit.hit_p = ray.origin + hit.distance * ray.direction; // Berechnet den Schnittpunkt

        // Berechnet die Normale basierend auf der getroffenen Seite
        if (std::abs(hit.hit_p.x - min_.x) < EPSILON) hit.normal = glm::vec3(-1, 0, 0);      // Wenn Treffer auf der linken Seite, Normale nach links
        else if (std::abs(hit.hit_p.x - max_.x) < EPSILON) hit.normal = glm::vec3(1, 0, 0);  // Wenn Treffer auf der rechten Seite, Normale nach rechts
        else if (std::abs(hit.hit_p.y - min_.y) < EPSILON) hit.normal = glm::vec3(0, -1, 0); // Wenn Treffer auf der unteren Seite, Normale nach unten
        else if (std::abs(hit.hit_p.y - max_.y) < EPSILON) hit.normal = glm::vec3(0, 1, 0);  // Wenn Treffer auf der oberen Seite, Normale nach oben
        else if (std::abs(hit.hit_p.z - min_.z) < EPSILON) hit.normal = glm::vec3(0, 0, -1); // Wenn Treffer auf der hinteren Seite, Normale nach hinten
        else hit.normal = glm::vec3(0, 0, 1);                                                // Wenn Treffer auf der vorderen Seite, Normale nach vorne

        hit.color_obj = material_;     // Setzt das Material des Trefferpunkts auf das Material der Box
        hit.name_obj = getName();      // Setzt den Namen des getroffenen Objekts auf den Namen der Box
    }
    else {
        hit.hit = false;           // Kein Treffer gefunden
    }
    return hit;                    // Gibt den Trefferpunkt zurück
}