#ifndef TRIANGLE_HPP               // Beginn der Include-Guard: Verhindert mehrfaches Einbinden dieser Datei
#define TRIANGLE_HPP               // Definition des Include-Guard-Makros

#include "shape.hpp"               // Inkludiert die Basisklasse Shape
#include <glm/vec3.hpp>            // Inkludiert GLM-Vektorklasse f�r 3D-Vektoren

class Triangle : public Shape {    // Beginn der Triangle-Klassen-Definition, erbt von Shape
public:
    Triangle();                    // Standardkonstruktor
    Triangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3); // Konstruktor mit drei Eckpunkten
    Triangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3,
        const std::string& name, const std::shared_ptr<Material>& material); // Konstruktor mit Eckpunkten, Name und Material
    Triangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3,
        const std::shared_ptr<Material>& material); // Konstruktor mit Eckpunkten und Material

       float area() const override;   // �berschreibt die Methode zur Fl�chenberechnung
    float volume() const override; // �berschreibt die Methode zur Volumenberechnung (f�r Dreiecke immer 0)
    std::ostream& print(std::ostream& os) const override; // �berschreibt die Methode zum Ausgeben des Dreiecks

protected:
    Hitpoint intersectImpl(Ray const& ray) const override; // �berschreibt die Methode zur Schnittpunktberechnung

private:
    glm::vec3 v1_{ 0.0f, 0.0f, 0.0f }; // Erster Eckpunkt des Dreiecks, standardm��ig (0,0,0)
    glm::vec3 v2_{ 1.0f, 0.0f, 0.0f }; // Zweiter Eckpunkt des Dreiecks, standardm��ig (1,0,0)
    glm::vec3 v3_{ 0.0f, 1.0f, 0.0f }; // Dritter Eckpunkt des Dreiecks, standardm��ig (0,1,0)
};

#endif                             // Ende der Include-Guard