// box.hpp
#ifndef BOX_HPP                    // Verhindert mehrfaches Einbinden dieser Datei
#define BOX_HPP

#include "shape.hpp"               // Bindet die Basisklasse Shape ein
#include <glm/vec3.hpp>            // Bindet die Vektorklasse für 3D-Koordinaten ein

class Box : public Shape {         // Definiert die Box-Klasse, die von Shape erbt
private:

public:
    Box();                         // Standardkonstruktor für eine Box
    Box(glm::vec3 min, glm::vec3 max); // Konstruktor mit Minimal- und Maximalkoordinaten
    Box(glm::vec3 min, glm::vec3 max, std::string const& name, std::shared_ptr<Material> const& color); // Konstruktor mit zusätzlichem Namen und Material
    Box(glm::vec3 min, glm::vec3 max, std::shared_ptr<Material> const& material); // Konstruktor mit Koordinaten und Material

    float area() const override;   // Berechnet und gibt die Oberfläche der Box zurück
    float volume() const override; // Berechnet und gibt das Volumen der Box zurück
    std::ostream& print(std::ostream& os) const override; // Gibt Informationen über die Box aus

    glm::vec3 min_{ 0.0f, 0.0f, 0.0f };     // Minimale Koordinaten der Box (untere linke Ecke)
    glm::vec3 max_{ 100.0f, 100.0f, 100.0f }; // Maximale Koordinaten der Box (obere rechte Ecke)

protected:
    Hitpoint intersectImpl(Ray const& ray) const override; // Berechnet den Schnittpunkt eines Strahls mit der Box
};

#endif                             // Beendet die Schutz vor Mehrfacheinbindung