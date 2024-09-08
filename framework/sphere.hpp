// sphere.hpp
#ifndef SPHERE_HPP                 // Beginn der Include-Guard: Verhindert mehrfaches Einbinden dieser Datei
#define SPHERE_HPP                 // Definition des Include-Guard-Makros

#include "shape.hpp"               // Inkludiert die Basisklasse Shape
#include <glm/vec3.hpp>            // Inkludiert GLM-Vektorklasse f�r 3D-Vektoren
#include "Hitpoint.hpp"            // Inkludiert die Definitionen f�r Trefferpunkte
#include "Ray.hpp"                 // Inkludiert die Definitionen f�r Strahlen

class Sphere : public Shape {      // Beginn der Sphere-Klassen-Definition, erbt von Shape
private:

public:
    Sphere();                      // Standardkonstruktor
    ~Sphere();                     // Destruktor
    Sphere(float r, glm::vec3 p);  // Konstruktor mit Radius und Mittelpunkt
    Sphere(float r, glm::vec3 p, std::string const& name, std::shared_ptr<Material> const& color); // Konstruktor mit Radius, Mittelpunkt, Name und Material

    float area() const override;   // �berschreibt die Methode zur Fl�chenberechnung
    float volume() const override; // �berschreibt die Methode zur Volumenberechnung
    std::ostream& print(std::ostream& os) const override; // �berschreibt die Methode zum Ausgeben der Kugel

    float rad{ 10.0f };            // Radius der Kugel, standardm��ig 10.0
    glm::vec3 point{ 100.0f, 100.0f, 100.0f }; // Mittelpunkt der Kugel, standardm��ig (100, 100, 100)

protected:
    Hitpoint intersectImpl(Ray const& ray) const override; // �berschreibt die Methode zur Schnittpunktberechnung
};

#endif                             // Ende der Include-Guard