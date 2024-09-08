// shape.hpp
#ifndef SHAPE_HPP                  // Beginn der Include-Guard: Verhindert mehrfaches Einbinden dieser Datei
#define SHAPE_HPP                  // Definition des Include-Guard-Makros

#include <string>                  // Inkludiert die Standard-String-Bibliothek
#include <memory>                  // Inkludiert die Smart-Pointer-Bibliothek
#include <glm/glm.hpp>             // Inkludiert die OpenGL Mathematics (GLM) Bibliothek
#include <glm/gtc/matrix_inverse.hpp> // Inkludiert GLM-Funktionen f�r Matrixinversionen
#include "Ray.hpp"                 // Inkludiert die Definitionen f�r Strahlen
#include "Hitpoint.hpp"            // Inkludiert die Definitionen f�r Trefferpunkte
#include "Material.hpp"            // Inkludiert die Definitionen f�r Materialien

class Shape {                      // Beginn der Shape-Klassen-Definition (abstrakte Basisklasse)
public:                            // �ffentlicher Bereich der Klasse
    Shape();                       // Standardkonstruktor
    Shape(std::string const& name); // Konstruktor mit Namensparameter
    Shape(std::string const& name, std::shared_ptr<Material> const& color); // Konstruktor mit Name und Material
    virtual ~Shape() = default;    // Virtueller Destruktor

    virtual float area() const = 0; // Rein virtuelle Methode zur Fl�chenberechnung
    virtual float volume() const = 0; // Rein virtuelle Methode zur Volumenberechnung
    virtual std::ostream& print(std::ostream& os) const; // Virtuelle Methode zum Ausgeben der Form

    Hitpoint intersect(Ray const& ray) const; // Methode zur Schnittpunktberechnung
    void setTransformation(glm::mat4 const& transformation); // Setzt die Transformationsmatrix
    const std::string& getName() const; // Gibt den Namen der Form zur�ck
    const glm::mat4& getWorldTransformation() const; // Gibt die Welttransformationsmatrix zur�ck

    void setMaterial(std::shared_ptr<Material> const& material) { // Setzt das Material der Form
        material_ = material;
    }

    // Neue Methoden f�r Composite-Funktionalit�t
    virtual void add(std::shared_ptr<Shape> shape) {} // Virtuelle Methode zum Hinzuf�gen einer Unterform
    virtual void remove(std::shared_ptr<Shape> shape) {} // Virtuelle Methode zum Entfernen einer Unterform

protected:                         // Gesch�tzter Bereich der Klasse
    virtual Hitpoint intersectImpl(Ray const& ray) const = 0; // Rein virtuelle Methode f�r die spezifische Schnittpunktberechnung
    Ray transformRay(Ray const& ray) const; // Transformiert einen Strahl
    glm::vec3 transformPoint(glm::vec3 const& point) const; // Transformiert einen Punkt
    glm::vec3 transformNormal(glm::vec3 const& normal, glm::vec3 const& point) const; // Transformiert eine Normale

    std::string name_;             // Name der Form
    std::shared_ptr<Material> color_; // Farbe der Form (veraltet, wird durch material_ ersetzt)
    std::shared_ptr<Material> material_; // Material der Form
    glm::mat4 world_transformation_; // Welttransformationsmatrix
    glm::mat4 world_transformation_inv_; // Inverse der Welttransformationsmatrix
};

std::ostream& operator<<(std::ostream& os, Shape const& s); // �berladener Ausgabeoperator f�r Formen

#endif // SHAPE_HPP               // Ende der Include-Guard