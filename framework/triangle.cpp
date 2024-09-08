#include "triangle.hpp"  // Einbindung der Header-Datei der Triangle-Klasse
#include <cmath>  // Einbindung von mathematischen Funktionen
#include <iostream>  // Einbindung von Ausgabe-Streams wie std::cout und std::cerr für Konsolenausgaben
#include <glm/glm.hpp>  // Einbindung der GLM-Bibliothek für mathematische Operationen mit Vektoren und Matrizen
#include <memory>  // Einbindung für Smart Pointers wie std::shared_ptr
#include <vector>  // Einbindung der Vektor-Klasse, falls später benötigt

// Konstante für numerische Präzision. Wird verwendet, um zu überprüfen, ob Werte nahe bei Null liegen.
const float EPSILON = 0.0001f;

// Standardkonstruktor: Initialisiert ein Dreieck mit Standardwerten für die Eckpunkte (v1, v2, v3)
Triangle::Triangle() :
    Shape{ "Triangle" },  // Der Name des Dreiecks wird auf "Triangle" gesetzt, es erbt dies von der Basisklasse Shape
    v1_{ 0.0f, 0.0f, 0.0f },  // Der erste Eckpunkt (v1) wird auf die Koordinaten (0, 0, 0) gesetzt
    v2_{ 1.0f, 0.0f, 0.0f },  // Der zweite Eckpunkt (v2) wird auf die Koordinaten (1, 0, 0) gesetzt
    v3_{ 0.0f, 1.0f, 0.0f }  // Der dritte Eckpunkt (v3) wird auf die Koordinaten (0, 1, 0) gesetzt
{}

// Konstruktor: Initialisiert ein Dreieck mit gegebenen Eckpunkten
Triangle::Triangle(glm::vec3 const& v1, glm::vec3 const& v2, glm::vec3 const& v3) :
    Shape{ "Triangle" },  // Setzt den Namen des Dreiecks auf "Triangle"
    v1_(v1),  // Setzt den ersten Eckpunkt auf den übergebenen Wert
    v2_(v2),  // Setzt den zweiten Eckpunkt auf den übergebenen Wert
    v3_(v3)  // Setzt den dritten Eckpunkt auf den übergebenen Wert
{}

// Konstruktor: Initialisiert ein Dreieck mit gegebenen Eckpunkten, Namen und Material
Triangle::Triangle(glm::vec3 const& v1, glm::vec3 const& v2, glm::vec3 const& v3, std::string const& name, std::shared_ptr<Material> const& material) :
    Shape{ name, material },  // Setzt den Namen und das Material des Dreiecks
    v1_(v1),  // Setzt den ersten Eckpunkt auf den übergebenen Wert
    v2_(v2),  // Setzt den zweiten Eckpunkt auf den übergebenen Wert
    v3_(v3)  // Setzt den dritten Eckpunkt auf den übergebenen Wert
{
    setMaterial(material);  // Weist explizit das Material zu
    if (!material) {  // Wenn kein Material gesetzt wurde, gibt es eine Warnung aus
        std::cerr << "Warnung: Kein Material für Triangle " << name << " gesetzt" << std::endl;
    }
}

// Konstruktor: Initialisiert ein Dreieck mit gegebenen Eckpunkten und Material (ohne expliziten Namen)
Triangle::Triangle(glm::vec3 const& v1, glm::vec3 const& v2, glm::vec3 const& v3, std::shared_ptr<Material> const& material) :
    Shape{ "Triangle", material },  // Setzt den Namen auf "Triangle" und weist das Material zu
    v1_(v1),  // Setzt den ersten Eckpunkt auf den übergebenen Wert
    v2_(v2),  // Setzt den zweiten Eckpunkt auf den übergebenen Wert
    v3_(v3)  // Setzt den dritten Eckpunkt auf den übergebenen Wert
{
    setMaterial(material);  // Weist explizit das Material zu
}

// Methode zur Berechnung der Fläche des Dreiecks
float Triangle::area() const {
    glm::vec3 edge1 = v2_ - v1_;  // Berechnet die Kante zwischen v1 und v2
    glm::vec3 edge2 = v3_ - v1_;  // Berechnet die Kante zwischen v1 und v3
    return 0.5f * glm::length(glm::cross(edge1, edge2));  // Berechnet die Fläche als Hälfte des Kreuzprodukts der beiden Kanten (geometrische Definition für Dreiecksfläche)
}

// Methode zur Berechnung des Volumens des Dreiecks (immer 0, da ein Dreieck eine flache Figur ist)
float Triangle::volume() const {
    return 0.0f;  // Ein Dreieck hat kein Volumen
}

// Methode zur Ausgabe der Details des Dreiecks (von Shape geerbte Details plus die Eckpunkte) in einen Ausgabestream
std::ostream& Triangle::print(std::ostream& os) const {
    Shape::print(os);  // Ruft die print-Methode der Basisklasse auf, um den Namen und die Farbe (Material) auszugeben
    return os << v1_.x << " " << v1_.y << " " << v1_.z << " "
        << v2_.x << " " << v2_.y << " " << v2_.z << " "
        << v3_.x << " " << v3_.y << " " << v3_.z << std::endl;  // Gibt die Koordinaten der Eckpunkte aus
}

// Methode zur Berechnung, ob und wo ein Strahl (Ray) das Dreieck schneidet
Hitpoint Triangle::intersectImpl(Ray const& ray) const {
    glm::vec3 edge1 = v2_ - v1_;  // Berechnet die Kante zwischen v1 und v2
    glm::vec3 edge2 = v3_ - v1_;  // Berechnet die Kante zwischen v1 und v3
    glm::vec3 h = glm::cross(ray.direction, edge2);  // Berechnet das Kreuzprodukt der Strahlrichtung und der zweiten Kante (wird für die Berechnung des Schnittpunkts verwendet)
    float a = glm::dot(edge1, h);  // Berechnet das Skalarprodukt der ersten Kante und h (benutzt für die Entscheidung, ob der Strahl parallel zum Dreieck ist)
    if (a > -EPSILON && a < EPSILON) {  // Überprüft, ob das Skalarprodukt nahe bei Null liegt (was bedeutet, dass der Strahl parallel ist)
        return Hitpoint();  // Gibt einen leeren Hitpoint zurück, wenn der Strahl parallel ist und das Dreieck nicht schneidet
    }
    float f = 1.0f / a;  // Berechnet den Kehrwert von a
    glm::vec3 s = ray.origin - v1_;  // Berechnet den Vektor vom Ursprung des Strahls zum ersten Eckpunkt
    float u = f * glm::dot(s, h);  // Berechnet den u-Parameter der baryzentrischen Koordinaten (gibt an, ob der Schnittpunkt innerhalb des Dreiecks liegt)
    if (u < 0.0f || u > 1.0f) {  // Überprüft, ob u außerhalb des Bereichs [0, 1] liegt, was bedeutet, dass der Schnittpunkt außerhalb des Dreiecks liegt
        return Hitpoint();  // Gibt einen leeren Hitpoint zurück, wenn der Schnittpunkt außerhalb des Dreiecks liegt
    }
    glm::vec3 q = glm::cross(s, edge1);  // Berechnet das Kreuzprodukt von s und der ersten Kante (wird für die Berechnung des Schnittpunkts verwendet)
    float v = f * glm::dot(ray.direction, q);  // Berechnet den v-Parameter der baryzentrischen Koordinaten (gibt an, ob der Schnittpunkt innerhalb des Dreiecks liegt)
    if (v < 0.0f || u + v > 1.0f) {  // Überprüft, ob v außerhalb des Bereichs [0, 1] liegt oder ob u + v > 1 ist (was bedeutet, dass der Schnittpunkt außerhalb des Dreiecks liegt)
        return Hitpoint();  // Gibt einen leeren Hitpoint zurück, wenn der Schnittpunkt außerhalb des Dreiecks liegt
    }
    // An dieser Stelle können wir t berechnen, um herauszufinden, wo der Schnittpunkt auf der Linie liegt
    float t = f * glm::dot(edge2, q);  // Berechnet den t-Parameter, der die Entfernung des Schnittpunkts entlang des Strahls angibt
    if (t > EPSILON) {  // Überprüft, ob t positiv ist (was bedeutet, dass der Schnittpunkt vor dem Strahlursprung liegt)
        Hitpoint hit;  // Erstellt ein Hitpoint-Objekt, um die Ergebnisse zu speichern
        hit.hit = true;  // Markiert, dass ein Treffer stattgefunden hat
        hit.distance = t;  // Speichert die Entfernung zum Schnittpunkt
        hit.hit_p = ray.origin + t * ray.direction;  // Berechnet die genaue Position des Schnittpunkts
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));  // Berechnet die Normale des Dreiecks (senkrecht zur Fläche)
        if (glm::dot(normal, ray.direction) > 0) {  // Überprüft, ob die Normale in dieselbe Richtung wie der Strahl zeigt (in diesem Fall invertieren wir sie)
            normal = -normal;  // Invertiert die Normale, wenn sie in dieselbe Richtung wie der Strahl zeigt
        }
        hit.normal = normal;  // Speichert die Normale des Schnittpunkts
        hit.color_obj = material_;  // Weist dem Treffer die Farbe (Material) des Dreiecks zu
        hit.name_obj = getName();  // Weist dem Treffer den Namen des Dreiecks zu
        return hit;  // Gibt das Hitpoint-Objekt zurück (enthält alle Informationen über den Schnittpunkt)
    }
    else {  // Wenn t negativ ist, bedeutet das, dass der Schnittpunkt hinter dem Strahlursprung liegt (kein gültiger Schnittpunkt)
        return Hitpoint();  // Gibt einen leeren Hitpoint zurück
    }
}
