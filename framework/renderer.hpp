// renderer.hpp
#ifndef BUW_RENDERER_HPP           // Verhindert mehrfaches Einbinden dieser Datei
#define BUW_RENDERER_HPP

#include "color.hpp"               // Bindet Farbdefinitionen ein
#include "shape.hpp"               // Bindet Formdefinitionen ein
#include "light.hpp"               // Bindet Lichtdefinitionen ein
#include "pixel.hpp"               // Bindet Pixeldefinitionen ein
#include "ppmwriter.hpp"           // Bindet PPM-Schreiber ein
#include "ray.hpp"                 // Bindet Strahlendefinitionen ein
#include "hitpoint.hpp"            // Bindet Trefferpunktdefinitionen ein
#include "camera.hpp"              // Bindet Kameradefinitionen ein
#include "shade.hpp"               // Bindet Schattierungsdefinitionen ein
#include <string>                  // F�r string-Objekte
#include <glm/glm.hpp>             // F�r mathematische Operationen
#include <vector>                  // F�r vector-Container
#include <memory>                  // F�r smart pointers

class Renderer {
public:
    Renderer(unsigned w, unsigned h, std::string const& file, Camera const& cam); // Konstruktor
    void render();                 // Rendert die Szene
    void write(Pixel const& p);    // Schreibt ein Pixel in den Puffer
    inline std::vector<Color> const& color_buffer() const { return colorbuffer_; } // Gibt den Farbpuffer zur�ck
    void add_shapes(const std::vector<std::shared_ptr<Shape>>& new_shapes); // F�gt Formen zur Szene hinzu
    void add_lights(const std::vector<std::shared_ptr<Light>>& new_lights); // F�gt Lichter zur Szene hinzu

private:
    unsigned width_;               // Breite des Bildes
    unsigned height_;              // H�he des Bildes
    std::vector<Color> colorbuffer_; // Speichert die Farben der gerenderten Pixel
    std::string filename_;         // Name der Ausgabedatei
    PpmWriter ppm_;                // PPM-Schreiber f�r die Ausgabe
    Camera camera_;                // Kamera f�r die Szene
    Shading shading_;              // Schattierungsmodul

    Ray generate_primary_ray(unsigned x, unsigned y); // Erzeugt einen prim�ren Strahl f�r ein Pixel
    Color trace_ray(Ray const& ray, int depth); // Verfolgt einen Strahl durch die Szene

    std::shared_ptr<Shape> root_shape_;  // Wurzel der Szenenhierarchie
    std::vector<std::shared_ptr<Light>> lights_; // Liste aller Lichtquellen in der Szene
};

#endif // #ifndef BUW_RENDERER_HPP // Beendet die Schutz vor Mehrfacheinbindung