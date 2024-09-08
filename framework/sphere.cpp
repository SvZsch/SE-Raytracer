#include "sphere.hpp"  // Einbindung der Header-Datei der Sphere-Klasse
#include "Hitpoint.hpp"  // Einbindung der Hitpoint-Klasse, um Informationen über Strahlenkollisionen zu speichern
#include <numbers>  // Einbindung von mathematischen Konstanten, z.B. Pi
#include <cmath>  // Einbindung von mathematischen Funktionen wie sqrt
#include <glm/glm.hpp>  // Einbindung der GLM-Bibliothek für mathematische Operationen mit Vektoren und Matrizen
#include <string>  // Einbindung der String-Klasse für Textverarbeitung
#include <sstream>  // Einbindung für String-Streams (wird für Textformatierung genutzt)
#include <glm/gtx/intersect.hpp>  // Einbindung von GLM-Funktionen zur Berechnung von Strahlen-Schnittpunkten
#include <iostream>  // Einbindung für Ausgabe-Streams wie std::cout

// Standardkonstruktor: Initialisiert eine Kugel mit einem festen Radius und einem festen Punkt im Raum
Sphere::Sphere() :
    Shape{ "Kugel" },  // Der Name der Kugel wird auf "Kugel" gesetzt, sie erbt dies von der Basisklasse Shape
    rad{ 10.0f },  // Der Radius der Kugel wird auf 10.0 gesetzt
    point{ 100.0f, 100.0f, 100.0f }  // Der Mittelpunkt der Kugel wird auf die Koordinaten (100, 100, 100) gesetzt
{
    std::cout << "Sphere ctor" << std::endl;  // Gibt eine Nachricht aus, wenn der Konstruktor aufgerufen wird
}

// Destruktor: Wird aufgerufen, wenn eine Kugel gelöscht wird
Sphere::~Sphere()
{
    std::cout << "Sphere dtor" << std::endl;  // Gibt eine Nachricht aus, wenn der Destruktor aufgerufen wird
}

// Konstruktor: Erzeugt eine Kugel mit einem gegebenen Radius und einem gegebenen Mittelpunkt
Sphere::Sphere(float r, glm::vec3 p) :
    Shape{ "Kugel" },  // Setzt den Namen der Kugel auf "Kugel"
    rad(r),  // Setzt den Radius der Kugel auf den übergebenen Wert
    point(p)  // Setzt den Mittelpunkt der Kugel auf die übergebenen Koordinaten
{
    std::cout << "Sphere ctor" << std::endl;  // Gibt eine Nachricht aus, wenn der Konstruktor aufgerufen wird
}

// Konstruktor: Erzeugt eine Kugel mit einem Radius, einem Mittelpunkt, einem Namen und einer Farbe (Material)
Sphere::Sphere(float r, glm::vec3 p, std::string const& name, std::shared_ptr<Material> const& color) :
    Shape{ name, color },  // Setzt den Namen und die Farbe (Material) der Kugel
    rad(r),  // Setzt den Radius der Kugel auf den übergebenen Wert
    point(p)  // Setzt den Mittelpunkt der Kugel auf die übergebenen Koordinaten
{
    std::cout << "Sphere ctor" << std::endl;  // Gibt eine Nachricht aus, wenn der Konstruktor aufgerufen wird
}

// Berechnet das Volumen der Kugel basierend auf dem Radius
float Sphere::volume() const {
    return abs((4.0f / 3.0f) * std::numbers::pi * rad * rad * rad);  // Formel für das Volumen einer Kugel: (4/3) * Pi * r^3
}

// Berechnet die Oberfläche der Kugel basierend auf dem Radius
float Sphere::area() const {
    return abs(4.0f * std::numbers::pi * rad * rad);  // Formel für die Oberfläche einer Kugel: 4 * Pi * r^2
}

// Gibt die Details der Kugel (von Shape geerbte Details plus Radius und Mittelpunkt) in einen Ausgabestream
std::ostream& Sphere::print(std::ostream& os) const {
    Shape::print(os);  // Ruft die print-Methode der Basisklasse auf, um den Namen und die Farbe auszugeben
    return os << rad << " " << point.x << " " << point.y << " " << point.z << std::endl;  // Gibt den Radius und die Koordinaten des Mittelpunktes aus
}

// Hilfsfunktion: Konvertiert einen 3D-Vektor in einen lesbaren String (Text), um ihn leichter ausgeben zu können
std::string vec3_to_string(const glm::vec3& v) {
    std::stringstream ss;  // Erstellt einen String-Stream
    ss << "(" << v.x << ", " << v.y << ", " << v.z << ")";  // Formatiert die x-, y- und z-Koordinaten als "(x, y, z)"
    return ss.str();  // Gibt den formatierten String zurück
}

// Methode zur Berechnung, ob und wo ein Strahl (Ray) die Kugel schneidet
Hitpoint Sphere::intersectImpl(Ray const& ray) const {
    glm::vec3 oc = ray.origin - point;  // Berechnet den Vektor vom Ursprung des Strahls zum Mittelpunkt der Kugel
    float a = glm::dot(ray.direction, ray.direction);  // Berechnet das Quadrat der Länge der Strahlrichtung (normalerweise 1, wenn der Strahl normalisiert ist)
    float b = 2.0f * glm::dot(oc, ray.direction);  // Berechnet das doppelte Skalarprodukt zwischen oc und der Strahlrichtung
    float c = glm::dot(oc, oc) - rad * rad;  // Berechnet den Abstand des Strahls vom Kugelmittelpunkt im Quadrat minus das Quadrat des Radius
    float discriminant = b * b - 4 * a * c;  // Berechnet die Diskriminante der quadratischen Gleichung für den Schnittpunkt

    Hitpoint hitpoint;  // Erstellt ein Hitpoint-Objekt, um die Ergebnisse zu speichern
    if (discriminant >= 0) {  // Überprüft, ob die Diskriminante größer oder gleich Null ist (echte Lösungen existieren)
        float dist = (-b - std::sqrt(discriminant)) / (2.0f * a);  // Berechnet die Entfernung zum nächsten Schnittpunkt
        if (dist > 0) {  // Überprüft, ob der Schnittpunkt vor dem Strahlursprung liegt
            hitpoint.hit = true;  // Markiert, dass ein Treffer stattgefunden hat
            hitpoint.distance = dist;  // Speichert die Entfernung zum Schnittpunkt
            hitpoint.hit_p = ray.origin + dist * ray.direction;  // Berechnet die genaue Position des Schnittpunkts
            hitpoint.normal = glm::normalize(hitpoint.hit_p - point);  // Berechnet die Normale am Schnittpunkt (zeigt vom Mittelpunkt der Kugel weg)
            hitpoint.color_obj = color_;  // Weist dem Treffer die Farbe (Material) der Kugel zu
            hitpoint.name_obj = getName();  // Weist dem Treffer den Namen der Kugel zu
        }
    }

    return hitpoint;  // Gibt das Hitpoint-Objekt zurück (enthält alle Informationen über den Schnittpunkt)
}
