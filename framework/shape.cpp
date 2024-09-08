// shape.cpp

#include "shape.hpp"  // Einbindung der Shape-Klasse und ihrer Methoden
#include "Hitpoint.hpp"  // Einbindung der Hitpoint-Klasse, um Informationen über Kollisionen zu speichern
#include <iostream>  // Einbindung für Ausgabe-Streams wie std::cout
#include <glm/gtc/matrix_transform.hpp>  // Einbindung von GLM-Funktionen zur Matrixtransformation

// Konstruktor ohne Parameter: Initialisiert die Form mit Standardwerten (kein Name, keine Farbe, Identitätsmatrix)
Shape::Shape()
    : name_("Unnamed"),  // Der Name der Form wird auf "Unnamed" gesetzt
    color_(nullptr),  // Die Farbe der Form wird auf Nullzeiger gesetzt (keine Farbe zugewiesen)
    world_transformation_(1.0f),  // Die Welttransformation wird als Identitätsmatrix initialisiert (keine Transformation)
    world_transformation_inv_(1.0f) {}  // Die inverse Welttransformation ebenfalls als Identitätsmatrix initialisiert

// Konstruktor mit Namensparameter: Erzeugt eine Form mit einem gegebenen Namen, aber ohne Farbe
Shape::Shape(std::string const& name)
    : name_(name),  // Setzt den Namen der Form auf den übergebenen Wert
    color_(nullptr),  // Die Farbe wird wieder auf Nullzeiger gesetzt (keine Farbe zugewiesen)
    world_transformation_(1.0f),  // Die Welttransformation wird auf die Identitätsmatrix gesetzt
    world_transformation_inv_(1.0f) {}  // Die inverse Welttransformation bleibt ebenfalls die Identitätsmatrix

// Konstruktor mit Name und Farbe: Erzeugt eine Form mit einem Namen und einer zugewiesenen Farbe
Shape::Shape(std::string const& name, std::shared_ptr<Material> const& color)
    : name_(name),  // Setzt den Namen der Form auf den übergebenen Wert
    color_(color),  // Weist der Form die übergebene Farbe (Material) zu
    world_transformation_(1.0f),  // Die Welttransformation wird auf die Identitätsmatrix gesetzt
    world_transformation_inv_(1.0f) {}  // Die inverse Welttransformation bleibt die Identitätsmatrix

// Methode zur Ausgabe der Form-Daten (Name und Farbe) in einen Ausgabestream
std::ostream& Shape::print(std::ostream& os) const {
    return os << name_ << " " << " " << color_ << " " << std::endl;  // Gibt den Namen und die Farbe der Form aus
}

// Setzt die Transformationsmatrix der Form und berechnet deren Inverse
void Shape::setTransformation(glm::mat4 const& transformation) {
    world_transformation_ = transformation * world_transformation_;  // Multipliziert die übergebene Matrix mit der aktuellen Welttransformation
    world_transformation_inv_ = glm::inverse(world_transformation_);  // Berechnet die Inverse der Welttransformation
}

// Transformiert einen Strahl in den Raum der Form (lokales Koordinatensystem)
Ray Shape::transformRay(Ray const& ray) const {
    Ray transformedRay;  // Erstellt einen neuen Strahl, um den transformierten Strahl zu speichern
    transformedRay.origin = glm::vec3(world_transformation_inv_ * glm::vec4(ray.origin, 1.0f));  // Transformiert den Ursprung des Strahls in das lokale Koordinatensystem
    transformedRay.direction = glm::normalize(glm::vec3(world_transformation_inv_ * glm::vec4(ray.direction, 0.0f)));  // Transformiert und normalisiert die Richtung des Strahls
    return transformedRay;  // Gibt den transformierten Strahl zurück
}

// Transformiert einen Punkt vom lokalen Koordinatensystem in das Weltkoordinatensystem
glm::vec3 Shape::transformPoint(glm::vec3 const& point) const {
    return glm::vec3(world_transformation_ * glm::vec4(point, 1.0f));  // Multipliziert den Punkt mit der Welttransformation und konvertiert ihn zurück in einen 3D-Vektor
}

// Transformiert eine Normale von lokalen Koordinaten in Weltkoordinaten
glm::vec3 Shape::transformNormal(glm::vec3 const& normal, glm::vec3 const& point) const {
    glm::mat3 invTranspose = glm::inverseTranspose(glm::mat3(world_transformation_));  // Berechnet die inverse transponierte Matrix der Transformation
    return glm::normalize(invTranspose * normal);  // Transformiert und normalisiert die Normale
}

// Berechnet, ob und wo ein Strahl eine Form schneidet, und transformiert den Schnittpunkt in Weltkoordinaten
Hitpoint Shape::intersect(Ray const& ray) const {
    Ray transformedRay = transformRay(ray);  // Transformiert den Strahl in den lokalen Raum der Form
    transformedRay.direction = glm::normalize(transformedRay.direction);  // Normalisiert die Richtung des transformierten Strahls

    Hitpoint hit = intersectImpl(transformedRay);  // Berechnet den Schnittpunkt (wird in der abgeleiteten Klasse implementiert)

    if (hit.hit) {  // Überprüft, ob es einen Treffer gab
        hit.hit_p = transformPoint(hit.hit_p);  // Transformiert den Treffpunkt in den Welt-Raum
        hit.normal = glm::normalize(transformNormal(hit.normal, hit.hit_p));  // Transformiert und normalisiert die Normale am Treffpunkt
        hit.distance = glm::length(hit.hit_p - ray.origin);  // Berechnet die Distanz zwischen dem Ursprung des Strahls und dem Treffpunkt
    }
    return hit;  // Gibt den Hitpoint mit den berechneten Informationen zurück
}

// Gibt den Namen der Form zurück
const std::string& Shape::getName() const {
    return name_;
}

// Gibt die Welttransformation der Form zurück
const glm::mat4& Shape::getWorldTransformation() const {
    return world_transformation_;
}

// Überladung des << Operators, um eine Form direkt in einen Ausgabestream zu schreiben
std::ostream& operator<<(std::ostream& os, Shape const& s) {
    return s.print(os);  // Ruft die print-Methode der Form auf, um sie auszugeben
}
