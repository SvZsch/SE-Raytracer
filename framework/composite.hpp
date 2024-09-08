// composite.hpp
#ifndef COMPOSITE_HPP              // Verhindert mehrfaches Einbinden dieser Datei
#define COMPOSITE_HPP

#include "shape.hpp"               // Bindet die Basisklasse Shape ein
#include <vector>                  // F�r die Verwendung von Vektoren
#include <memory>                  // F�r die Verwendung von shared_ptr

class Composite : public Shape {   // Definiert die Composite-Klasse, die von Shape erbt
public:
    Composite(const std::string& name); // Konstruktor mit Namensparameter
    ~Composite() override = default; // Standarddestruktor

    void add(std::shared_ptr<Shape> shape) override;    // F�gt eine Form zur Komposition hinzu
    void remove(std::shared_ptr<Shape> shape) override; // Entfernt eine Form aus der Komposition
    float area() const override;   // Berechnet die Gesamtfl�che der Komposition
    float volume() const override; // Berechnet das Gesamtvolumen der Komposition
    std::ostream& print(std::ostream& os) const override; // Gibt Informationen �ber die Komposition aus

    auto begin() { return children_.begin(); }          // Erm�glicht Iteration �ber die Kindobjekte
    auto end() { return children_.end(); }              // Erm�glicht Iteration �ber die Kindobjekte
    auto begin() const { return children_.begin(); }    // Konstante Version von begin()
    auto end() const { return children_.end(); }        // Konstante Version von end()

    const std::vector<std::shared_ptr<Shape>>& getChildren() const { return children_; } // Gibt Zugriff auf die Kindobjekte

    Hitpoint intersectImpl(Ray const& ray) const override; // Berechnet Schnittpunkte mit allen Kindobjekten

protected:

private:
    std::vector<std::shared_ptr<Shape>> children_;      // Speichert die Kindobjekte der Komposition
};

#endif // COMPOSITE_HPP            // Beendet die Schutz vor Mehrfacheinbindung