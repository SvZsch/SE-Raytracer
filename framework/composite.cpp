// composite.cpp
#include "composite.hpp"           // Inkludiert die Deklaration der Composite-Klasse
#include <algorithm>               // Inkludiert Standard-Algorithmen wie std::remove

Composite::Composite(const std::string& name) : Shape(name) {} // Konstruktor, ruft den Basisklassenkonstruktor mit dem Namen auf

void Composite::add(std::shared_ptr<Shape> shape) {
    //std::cout << "Adding shape " << shape->getName() << " to composite " << getName() << std::endl; // Auskommentierte Debug-Ausgabe
    children_.push_back(shape);    // Fügt das übergebene Shape-Objekt zur Liste der Kinder hinzu
    //std::cout << "Composite " << getName() << " now has " << children_.size() << " children" << std::endl; // Auskommentierte Debug-Ausgabe
}

void Composite::remove(std::shared_ptr<Shape> shape) {
    children_.erase(std::remove(children_.begin(), children_.end(), shape), children_.end()); // Entfernt das übergebene Shape-Objekt aus der Liste der Kinder
}

float Composite::area() const {
    float total_area = 0.0f;       // Initialisiert die Gesamtfläche mit 0
    for (const auto& child : children_) { // Iteriert über alle Kinder
        total_area += child->area(); // Addiert die Fläche jedes Kindes zur Gesamtfläche
    }
    return total_area;             // Gibt die Gesamtfläche zurück
}

float Composite::volume() const {
    float total_volume = 0.0f;     // Initialisiert das Gesamtvolumen mit 0
    for (const auto& child : children_) { // Iteriert über alle Kinder
        total_volume += child->volume(); // Addiert das Volumen jedes Kindes zum Gesamtvolumen
    }
    return total_volume;           // Gibt das Gesamtvolumen zurück
}

std::ostream& Composite::print(std::ostream& os) const {
    Shape::print(os);              // Ruft die print-Methode der Basisklasse auf
    //os << "Composite with " << children_.size() << " children:" << std::endl; // Auskommentierte Debug-Ausgabe
    for (const auto& child : children_) { // Iteriert über alle Kinder
        child->print(os);          // Ruft die print-Methode für jedes Kind auf
    }
    return os;                     // Gibt den Ausgabestrom zurück
}

Hitpoint Composite::intersectImpl(Ray const& ray) const {
    //std::cout << "Intersecting Composite " << getName() << std::endl; // Auskommentierte Debug-Ausgabe
    Hitpoint closest_hit;          // Erstellt ein Hitpoint-Objekt für den nächsten Treffer
    closest_hit.distance = std::numeric_limits<float>::max(); // Setzt die anfängliche Distanz auf den maximalen Float-Wert
    for (const auto& child : children_) { // Iteriert über alle Kinder
        Hitpoint hit = child->intersect(ray); // Berechnet den Schnittpunkt für jedes Kind
        if (hit.hit && hit.distance < closest_hit.distance) { // Wenn ein Treffer gefunden wurde und er näher ist als der bisherige nächste Treffer
            closest_hit = hit;     // Aktualisiert den nächsten Treffer
        }
    }
    if (closest_hit.hit) {         // Wenn ein Treffer gefunden wurde
        //std::cout << "Composite " << getName() << " hit detected at distance " << closest_hit.distance << std::endl; // Auskommentierte Debug-Ausgabe
    }
    else {                         // Wenn kein Treffer gefunden wurde
        //std::cout << "No hit for Composite " << getName() << std::endl; // Auskommentierte Debug-Ausgabe
    }
    return closest_hit;            // Gibt den nächsten Treffer zurück
}