// cone.hpp
#ifndef CONE_HPP                   // Verhindert mehrfaches Einbinden dieser Datei
#define CONE_HPP

#include "shape.hpp"               // Bindet die Basisklasse Shape ein

class Cone : public Shape {        // Definiert die Cone-Klasse, die von Shape erbt
public:
    Cone();                        // Standardkonstruktor f�r einen Kegel
    Cone(std::string const& name, float radius, float height); // Konstruktor mit Name, Radius und H�he
    Cone(std::string const& name, float radius, float height, std::shared_ptr<Material> const& material); // Konstruktor mit zus�tzlichem Material

    float area() const override;   // Berechnet und gibt die Oberfl�che des Kegels zur�ck
    float volume() const override; // Berechnet und gibt das Volumen des Kegels zur�ck

protected:
    Hitpoint intersectImpl(Ray const& ray) const override; // Berechnet den Schnittpunkt eines Strahls mit dem Kegel

private:
    float radius_;                 // Speichert den Radius des Kegels
    float height_;                 // Speichert die H�he des Kegels
};

#endif // CONE_HPP                 // Beendet die Schutz vor Mehrfacheinbindung