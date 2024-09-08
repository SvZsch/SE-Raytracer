// renderer.cpp
#include "renderer.hpp"            // Inkludiert die Header-Datei für die Renderer-Klasse
#include "shape.hpp"               // Inkludiert die Header-Datei für die Shape-Klasse
#include "camera.hpp"              // Inkludiert die Header-Datei für die Camera-Klasse
#include "renderer.hpp"            // Inkludiert die Header-Datei für die Renderer-Klasse (Doppelinklusion)
#include "box.cpp"                 // Inkludiert die Implementierung der Box-Klasse
#include "sphere.hpp"              // Inkludiert die Header-Datei für die Sphere-Klasse
#include "Hitpoint.hpp"            // Inkludiert die Header-Datei für die Hitpoint-Struktur
#include "composite.hpp"           // Inkludiert die Header-Datei für die Composite-Klasse
#include <glm/gtc/matrix_transform.hpp> // Inkludiert GLM-Funktionen für Matrixtransformationen
#include <iostream>                // Inkludiert die Standard-Ein-/Ausgabe-Bibliothek
#include <functional>              // Inkludiert die Funktions-Bibliothek für std::function

// Konstruktor für die Renderer-Klasse
Renderer::Renderer(unsigned w, unsigned h, std::string const& file, Camera const& cam)
    : width_(w)                    // Initialisiert die Breite
    , height_(h)                   // Initialisiert die Höhe
    , colorbuffer_(w* h, Color{ 0.0, 0.0, 0.0 }) // Initialisiert den Farbpuffer mit schwarzen Pixeln
    , filename_(file)              // Initialisiert den Dateinamen
    , ppm_(w, h)                   // Initialisiert den PPM-Writer
    , camera_(cam)                 // Initialisiert die Kamera
    , shading_()                   // Initialisiert das Shading-Objekt
    , root_shape_(std::make_shared<Composite>("root"))  // Initialisiert das Wurzel-Shape als Composite
{}

// Hauptmethode zum Rendern der Szene
void Renderer::render() {
    // Gibt Kameraeinstellungen aus
    std::cout << "Camera settings:" << std::endl;
    std::cout << "  Position: (" << camera_.eye.x << ", " << camera_.eye.y << ", " << camera_.eye.z << ")" << std::endl;
    std::cout << "  Direction: (" << camera_.dir.x << ", " << camera_.dir.y << ", " << camera_.dir.z << ")" << std::endl;
    std::cout << "  Up vector: (" << camera_.up.x << ", " << camera_.up.y << ", " << camera_.up.z << ")" << std::endl;
    std::cout << "  FOV: " << camera_.fov_x << std::endl;

    // Definiert eine rekursive Lambda-Funktion zum Ausgeben von Shape-Informationen
    std::function<void(const std::shared_ptr<Shape>&, int)> print_shape_info;
    print_shape_info = [&print_shape_info](const std::shared_ptr<Shape>& shape, int depth) {
        for (int i = 0; i < depth; ++i) std::cout << "  "; // Einrückung basierend auf der Tiefe
        std::cout << "Shape: " << shape->getName() << std::endl; // Gibt den Namen des Shapes aus

        for (int i = 0; i < depth; ++i) std::cout << "  "; // Einrückung für die Matrixausgabe
        std::cout << "Transformation matrix:" << std::endl;
        auto trans = shape->getWorldTransformation(); // Holt die Transformationsmatrix
        for (int i = 0; i < 4; ++i) { // Schleife über die Zeilen der Matrix
            for (int j = 0; j < depth + 1; ++j) std::cout << "  "; // Einrückung für jede Matrixzeile
            for (int j = 0; j < 4; ++j) { // Schleife über die Spalten der Matrix
                std::cout << trans[i][j] << " "; // Gibt jedes Element der Matrix aus
            }
            std::cout << std::endl; // Zeilenumbruch nach jeder Matrixzeile
        }

        if (auto composite = std::dynamic_pointer_cast<Composite>(shape)) { // Prüft, ob es sich um ein Composite handelt
            for (const auto& child : composite->getChildren()) { // Schleife über alle Kinder des Composites
                print_shape_info(child, depth + 1); // Rekursiver Aufruf für jedes Kind
            }
        }
        };

    print_shape_info(root_shape_, 0); // Ruft die Funktion für das Wurzel-Shape auf

    int total_pixels = width_ * height_; // Berechnet die Gesamtzahl der Pixel
    int processed_pixels = 0; // Zähler für verarbeitete Pixel

    for (unsigned y = 0; y < height_; ++y) { // Schleife über die Höhe des Bildes
        for (unsigned x = 0; x < width_; ++x) { // Schleife über die Breite des Bildes
            Ray ray = generate_primary_ray(x, y); // Generiert einen Primärstrahl für das aktuelle Pixel
            Color pixel_color = trace_ray(ray, 5); // Verfolgt den Strahl und berechnet die Farbe (max. Tiefe 5)
            Pixel p(x, y); // Erstellt ein Pixel-Objekt
            p.color = pixel_color; // Setzt die Farbe des Pixels
            write(p); // Schreibt das Pixel in den Puffer

            // Aktualisiert und zeigt den Fortschritt an
            processed_pixels++;
            if (processed_pixels % (total_pixels / 100) == 0) {
                std::cout << "\rRendering progress: " << (processed_pixels * 100 / total_pixels) << "%" << std::flush;
            }
        }
    }

    ppm_.save(filename_); // Speichert das gerenderte Bild
    std::cout << "\nRendering completed." << std::endl; // Gibt eine Abschlussmeldung aus
}

// Generiert einen Primärstrahl für ein gegebenes Pixel
Ray Renderer::generate_primary_ray(unsigned x, unsigned y) {
    float aspect_ratio = static_cast<float>(width_) / static_cast<float>(height_); // Berechnet das Seitenverhältnis
    float fov_x_rad = glm::radians(camera_.fov_x); // Konvertiert den horizontalen Sichtwinkel in Radianten
    float fov_y_rad = 2.0f * std::atan(std::tan(fov_x_rad * 0.5f) / aspect_ratio); // Berechnet den vertikalen Sichtwinkel
    float pixel_x = (2.0f * ((x + 0.5f) / width_) - 1.0f) * std::tan(fov_x_rad * 0.5f); // Berechnet die x-Koordinate im Kameraraum
    float pixel_y = (1.0f - 2.0f * ((y + 0.5f) / height_)) * std::tan(fov_y_rad * 0.5f); // Berechnet die y-Koordinate im Kameraraum
    glm::vec3 ray_direction = glm::normalize(camera_.dir + pixel_x * glm::cross(camera_.dir, camera_.up) + pixel_y * camera_.up); // Berechnet die Strahlrichtung

    return Ray{ camera_.eye, ray_direction }; // Gibt den Strahl zurück
}

// Verfolgt einen Strahl durch die Szene und berechnet die resultierende Farbe
Color Renderer::trace_ray(Ray const& ray, int depth) {
    if (depth <= 0) { // Prüft, ob die maximale Rekursionstiefe erreicht ist
        return Color{ 0.0f, 0.0f, 0.0f }; // Gibt Schwarz zurück, wenn die Tiefe erreicht ist
    }

    Hitpoint hitpoint = shading_.compute_intersection(ray, root_shape_); // Berechnet den Schnittpunkt mit der Szene

    if (hitpoint.hit) { // Wenn ein Schnittpunkt gefunden wurde
        return shading_.compute_lighting(hitpoint, ray, root_shape_, lights_, depth); // Berechnet die Beleuchtung am Schnittpunkt
    }
    else { // Wenn kein Schnittpunkt gefunden wurde
        // Berechnet eine Hintergrundfarbe basierend auf der Strahlrichtung
        float t = 0.5f * (glm::normalize(ray.direction).y + 1.0f);
        return Color{ (1.0f - t) * 1.0f + t * 0.5f, (1.0f - t) * 1.0f + t * 0.7f, (1.0f - t) * 1.0f + t * 1.0f };
    }
}

// Schreibt ein Pixel in den Farbpuffer und den PPM-Writer
void Renderer::write(Pixel const& p) {
    size_t buf_pos = (width_ * p.y + p.x); // Berechnet die Position im Puffer
    if (buf_pos >= colorbuffer_.size() || buf_pos < 0) { // Prüft auf ungültige Positionen
        // Fehlerfall: Pixel außerhalb des Puffers (Kommentar entfernt)
    }
    else {
        colorbuffer_[buf_pos] = p.color; // Schreibt die Farbe in den Puffer
    }
    ppm_.write(p); // Schreibt das Pixel in den PPM-Writer
}

// Fügt neue Shapes zur Szene hinzu
void Renderer::add_shapes(const std::vector<std::shared_ptr<Shape>>& new_shapes) {
    for (const auto& shape : new_shapes) { // Schleife über alle neuen Shapes
        root_shape_->add(shape); // Fügt jedes Shape zum Wurzel-Shape hinzu
    }
}

// Fügt neue Lichtquellen zur Szene hinzu
void Renderer::add_lights(const std::vector<std::shared_ptr<Light>>& new_lights) {
    lights_.insert(lights_.end(), new_lights.begin(), new_lights.end()); // Fügt die neuen Lichter zur Lichtliste hinzu
}