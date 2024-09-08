// cylinder.hpp
#ifndef CYLINDER_HPP               // Verhindert mehrfaches Einbinden dieser Datei
#define CYLINDER_HPP

#include "shape.hpp"               // Bindet die Basisklasse Shape ein

class Cylinder : public Shape {    // Definiert die Cylinder-Klasse, die von Shape erbt
public:
    Cylinder();                    // Standardkonstruktor für einen Zylinder
    Cylinder(std::string const& name, float radius, float height); // Konstruktor mit Name, Radius und Höhe
    Cylinder(std::string const& name, float radius, float height, std::shared_ptr<Material> const& material); // Konstruktor mit zusätzlichem Material

    float area() const override;   // Berechnet und gibt die Oberfläche des Zylinders zurück
    float volume() const override; // Berechnet und gibt das Volumen des Zylinders zurück

protected:
    Hitpoint intersectImpl(Ray const& ray) const override; // Berechnet den Schnittpunkt eines Strahls mit dem Zylinder

private:
    float radius_;                 // Speichert den Radius des Zylinders
    float height_;                 // Speichert die Höhe des Zylinders
};

#endif // CYLINDER_HPP             // Beendet die Schutz vor Mehrfacheinbindung