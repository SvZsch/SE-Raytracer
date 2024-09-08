// sdf-reader.cpp - Implementierung des SDF (Scene Description File) Readers

// Einbinden der ben�tigten Header-Dateien
#include "Material.hpp"            // F�r die Material-Klasse
#include "camera.hpp"              // F�r die Camera-Klasse
#include "shape.hpp"               // F�r die Shape-Basisklasse
#include "sphere.hpp"              // F�r die Sphere-Klasse
#include "box.hpp"                 // F�r die Box-Klasse
#include "triangle.hpp"            // F�r die Triangle-Klasse
#include "cone.hpp"                // F�r die Cone-Klasse
#include "cylinder.hpp"            // F�r die Cylinder-Klasse
#include "light.hpp"               // F�r die Light-Klasse
#include "composite.hpp"           // F�r die Composite-Klasse
#include <glm/glm.hpp>             // F�r grundlegende GLM-Funktionalit�ten
#include <glm/gtc/matrix_transform.hpp> // F�r GLM-Matrixtransformationen
#include <iostream>                // F�r Ein-/Ausgabeoperationen
#include <fstream>                 // F�r Dateioperationen
#include <string>                  // F�r String-Operationen
#include <sstream>                 // F�r String-Stream-Operationen
#include <vector>                  // F�r dynamische Arrays
#include <tuple>                   // F�r Tuple-Operationen
#include <algorithm>               // F�r STL-Algorithmen
#include <memory>                  // F�r Smart Pointer
#include <map>                     // F�r assoziative Arrays

// Funktion zum Finden eines Shape-Objekts anhand seines Namens
std::shared_ptr<Shape> find_shape(const std::string& name, const std::vector<std::shared_ptr<Shape>>& shapes) {
    // Sucht in shapes nach einem Shape mit dem gegebenen Namen
    auto it = std::find_if(shapes.begin(), shapes.end(),
        [&name](const auto& shape) { return shape->getName() == name; });
    // Gibt das gefundene Shape zur�ck oder nullptr, wenn keins gefunden wurde
    return (it != shapes.end()) ? *it : nullptr;
}

// Funktion zum Parsen einer Transformationszeile
glm::mat4 parseTransformation(std::istringstream& line_as_stream) {
    std::string transform_type;
    line_as_stream >> transform_type; // Liest den Transformationstyp

    if (transform_type == "scale") {
        float sx, sy, sz;
        line_as_stream >> sx >> sy >> sz; // Liest die Skalierungsfaktoren
        return glm::scale(glm::mat4(1.0f), glm::vec3(sx, sy, sz)); // Erstellt eine Skalierungsmatrix
    }
    else if (transform_type == "translate") {
        float tx, ty, tz;
        line_as_stream >> tx >> ty >> tz; // Liest die Translationswerte
        return glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz)); // Erstellt eine Translationsmatrix
    }
    else if (transform_type == "rotate") {
        float angle, rx, ry, rz;
        line_as_stream >> angle >> rx >> ry >> rz; // Liest den Winkel und die Rotationsachse
        return glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(rx, ry, rz)); // Erstellt eine Rotationsmatrix
    }

    return glm::mat4(1.0f); // Gibt eine Identit�tsmatrix zur�ck, wenn kein g�ltiger Typ erkannt wurde
}

// Hauptfunktion zum Lesen der SDF-Datei
std::tuple<std::vector<std::shared_ptr<Material>>,
    std::vector<std::shared_ptr<Camera>>,
    std::vector<std::shared_ptr<Shape>>,
    std::vector<std::shared_ptr<Light>>>
    sdf_reader(const std::string& sdf_file_path) {
    std::cout << "Versuche SDF-Datei zu �ffnen: " << sdf_file_path << std::endl;
    std::ifstream sdf_file(sdf_file_path); // �ffnet die SDF-Datei
    if (!sdf_file.is_open()) {
        std::cerr << "Konnte Datei nicht finden oder oeffnen: " << sdf_file_path << std::endl;
        return {}; // Gibt leere Vektoren zur�ck, wenn die Datei nicht ge�ffnet werden kann
    }

    // Vektoren zur Speicherung der gelesenen Objekte
    std::vector<std::shared_ptr<Material>> mat_vec;
    std::vector<std::shared_ptr<Camera>> cam_vec;
    std::vector<std::shared_ptr<Shape>> shape_vec;
    std::vector<std::shared_ptr<Light>> light_vec;

    std::string line_buffer; // Buffer f�r jede gelesene Zeile

    std::cout << "SDF Reader: Starte Parsing von " << sdf_file_path << std::endl;

    while (std::getline(sdf_file, line_buffer)) { // Liest die Datei zeilenweise
        // Entfernt f�hrende Leerzeichen
        line_buffer.erase(line_buffer.begin(), std::find_if(line_buffer.begin(), line_buffer.end(), [](unsigned char ch) {
            return !std::isspace(ch);
            }));

        // Ignoriert leere Zeilen und Kommentare
        if (line_buffer.empty() || line_buffer[0] == '#' || line_buffer.substr(0, 2) == "/*") {
            continue;
        }

        std::istringstream line_as_stream(line_buffer); // Konvertiert die Zeile in einen Stream
        std::string token;
        line_as_stream >> token; // Liest das erste Token

        if (token == "define") {
            line_as_stream >> token; // Liest das n�chste Token
            if (token == "material") {
                auto mat_ptr = std::make_shared<Material>(); // Erstellt ein neues Material-Objekt
                // Liest die Material-Eigenschaften
                line_as_stream >> mat_ptr->name
                    >> mat_ptr->ka.r >> mat_ptr->ka.g >> mat_ptr->ka.b
                    >> mat_ptr->kd.r >> mat_ptr->kd.g >> mat_ptr->kd.b
                    >> mat_ptr->ks.r >> mat_ptr->ks.g >> mat_ptr->ks.b
                    >> mat_ptr->m >> mat_ptr->refraction_index >> mat_ptr->transparency;
                mat_vec.push_back(mat_ptr); // F�gt das Material zum Vektor hinzu
                std::cout << "Material geladen: " << mat_ptr->name << std::endl;
            }
            else if (token == "camera") {
                std::string name;
                float fov_x;
                glm::vec3 eye, dir, up;
                // Liest die Kamera-Eigenschaften
                line_as_stream >> name >> fov_x
                    >> eye.x >> eye.y >> eye.z
                    >> dir.x >> dir.y >> dir.z
                    >> up.x >> up.y >> up.z;
                auto cam_ptr = std::make_shared<Camera>(name, fov_x, eye, dir, up); // Erstellt ein neues Camera-Objekt
                cam_vec.push_back(cam_ptr); // F�gt die Kamera zum Vektor hinzu
                std::cout << "Kamera geladen: " << name << std::endl;
            }
            else if (token == "shape") {
                line_as_stream >> token; // Liest den Shape-Typ
                if (token == "sphere") {
                    std::string name, material_name;
                    float radius;
                    glm::vec3 center;
                    // Liest die Sphere-Eigenschaften
                    line_as_stream >> name >> center.x >> center.y >> center.z >> radius >> material_name;
                    // Sucht das passende Material
                    auto material = std::find_if(mat_vec.begin(), mat_vec.end(),
                        [&material_name](const auto& mat) { return mat->name == material_name; });
                    if (material != mat_vec.end()) {
                        shape_vec.push_back(std::make_shared<Sphere>(radius, center, name, *material)); // Erstellt und f�gt die Sphere hinzu
                    }
                    std::cout << "Kugel geladen: " << name << std::endl;
                }
                else if (token == "box") {
                    std::string name, material_name;
                    glm::vec3 min, max;
                    // Liest die Box-Eigenschaften
                    line_as_stream >> name >> min.x >> min.y >> min.z >> max.x >> max.y >> max.z >> material_name;
                    // Sucht das passende Material
                    auto material = std::find_if(mat_vec.begin(), mat_vec.end(),
                        [&material_name](const auto& mat) { return mat->name == material_name; });
                    if (material != mat_vec.end()) {
                        auto box = std::make_shared<Box>(min, max, name, *material); // Erstellt eine neue Box
                        box->setMaterial(*material);  // Setzt das Material explizit
                        shape_vec.push_back(box); // F�gt die Box zum Vektor hinzu
                        std::cout << "Box geladen: " << name << " mit Material: " << material_name << std::endl;
                    }
                    else {
                        std::cerr << "Warnung: Material '" << material_name << "' nicht gefunden f�r Box '" << name << "'" << std::endl;
                    }
                }
                else if (token == "triangle") {
                    std::string name, material_name;
                    glm::vec3 v1, v2, v3;
                    // Liest die Triangle-Eigenschaften
                    line_as_stream >> name
                        >> v1.x >> v1.y >> v1.z
                        >> v2.x >> v2.y >> v2.z
                        >> v3.x >> v3.y >> v3.z
                        >> material_name;
                    // Sucht das passende Material
                    auto material = std::find_if(mat_vec.begin(), mat_vec.end(),
                        [&material_name](const auto& mat) { return mat->name == material_name; });
                    if (material != mat_vec.end()) {
                        auto triangle = std::make_shared<Triangle>(v1, v2, v3, name, *material); // Erstellt ein neues Triangle
                        shape_vec.push_back(triangle); // F�gt das Triangle zum Vektor hinzu
                        std::cout << "Dreieck geladen: " << name << " mit Material: " << material_name << std::endl;
                    }
                    else {
                        std::cerr << "Warnung: Material '" << material_name << "' nicht gefunden f�r Dreieck '" << name << "'" << std::endl;
                    }
                }
                else if (token == "cone") {
                    std::string name, material_name;
                    float radius, height;
                    glm::vec3 base_center;
                    // Liest die Cone-Eigenschaften
                    line_as_stream >> name >> base_center.x >> base_center.y >> base_center.z >> radius >> height >> material_name;
                    // Sucht das passende Material
                    auto material = std::find_if(mat_vec.begin(), mat_vec.end(),
                        [&material_name](const auto& mat) { return mat->name == material_name; });
                    if (material != mat_vec.end()) {
                        shape_vec.push_back(std::make_shared<Cone>(name, radius, height, *material)); // Erstellt und f�gt den Cone hinzu
                        auto& cone = shape_vec.back();
                        cone->setTransformation(glm::translate(glm::mat4(1.0f), base_center)); // Setzt die Transformation
                    }
                    std::cout << "Kegel geladen: " << name << std::endl;
                }
                else if (token == "cylinder") {
                    std::string name, material_name;
                    float radius, height;
                    glm::vec3 base_center;
                    // Liest die Cylinder-Eigenschaften
                    line_as_stream >> name >> base_center.x >> base_center.y >> base_center.z >> radius >> height >> material_name;
                    // Sucht das passende Material
                    auto material = std::find_if(mat_vec.begin(), mat_vec.end(),
                        [&material_name](const auto& mat) { return mat->name == material_name; });
                    if (material != mat_vec.end()) {
                        shape_vec.push_back(std::make_shared<Cylinder>(name, radius, height, *material)); // Erstellt und f�gt den Cylinder hinzu
                        auto& cylinder = shape_vec.back();
                        cylinder->setTransformation(glm::translate(glm::mat4(1.0f), base_center)); // Setzt die Transformation
                    }
                    std::cout << "Zylinder geladen: " << name << std::endl;
                }
                else if (token == "composite") {
                    std::string composite_name;
                    line_as_stream >> composite_name;
                    auto composite = std::make_shared<Composite>(composite_name); // Erstellt ein neues Composite-Objekt

                    std::string child_name;
                    while (line_as_stream >> child_name) { // Liest alle Kinder-Namen
                        auto child = find_shape(child_name, shape_vec); // Sucht das Kind-Shape
                        if (child) {
                            composite->add(child); // F�gt das Kind zum Composite hinzu
                        }
                    }

                    shape_vec.push_back(composite); // F�gt das Composite zum Vektor hinzu
                    std::cout << "Composite geladen: " << composite_name << std::endl;

                    for (const auto& child : composite->getChildren()) {
                        std::cout << "Child added to composite " << composite_name << ": " << child->getName() << std::endl;
                    }
                }
            }
            else if (token == "light") {
                std::string name;
                glm::vec3 position;
                Color color;
                // Liest die Light-Eigenschaften
                line_as_stream >> name >> position.x >> position.y >> position.z
                    >> color.r >> color.g >> color.b;
                light_vec.push_back(std::make_shared<Light>(position, color, name)); // Erstellt und f�gt das Light hinzu
                std::cout << "Licht geladen: " << name << std::endl;
            }
        }
        else if (token == "transform") {
            std::string shape_name;
            line_as_stream >> shape_name; // Liest den Namen des zu transformierenden Shapes

            glm::mat4 transformation = parseTransformation(line_as_stream); // Parst die Transformationsmatrix

            // Sucht das Shape mit dem gegebenen Namen
            auto shape_it = std::find_if(shape_vec.begin(), shape_vec.end(),
                [&shape_name](const auto& shape) { return shape->getName() == shape_name; });

            if (shape_it != shape_vec.end()) {
                (*shape_it)->setTransformation(transformation); // Wendet die Transformation auf das gefundene Shape an
            }
        }
    }

    // Entfernt Shapes, die Teil eines Composites sind, aus dem Haupt-Vektor
    shape_vec.erase(
        // Verwendet std::remove_if, um Elemente zu identifizieren, die entfernt werden sollen
        std::remove_if(shape_vec.begin(), shape_vec.end(),
            // Lambda-Funktion, die f�r jedes Shape aufgerufen wird
            [&shape_vec](const auto& shape) {
                // Pr�ft, ob irgendein Element in shape_vec die Bedingung erf�llt
                return std::any_of(shape_vec.begin(), shape_vec.end(),
                    // Lambda-Funktion f�r jedes potenzielle Elternobjekt
                    [&shape](const auto& potential_parent) {
                        // Versucht, das potenzielle Elternobjekt in ein Composite umzuwandeln
                        auto composite = std::dynamic_pointer_cast<Composite>(potential_parent);
                        // Pr�ft, ob es ein Composite ist, nicht das aktuelle Shape selbst ist,
                        // und ob das aktuelle Shape ein Kind dieses Composites ist
                        return composite && composite.get() != shape.get() &&
                            // Sucht in den Kindern des Composites nach dem aktuellen Shape
                            std::find_if(composite->getChildren().begin(), composite->getChildren().end(),
                                // Lambda-Funktion zum Vergleichen von Kindern
                                [&shape](const auto& child) { return child.get() == shape.get(); }) != composite->getChildren().end();
                    });
            }),
        // Ende des zu l�schenden Bereichs
        shape_vec.end()
    );

    // Gibt die geladenen Objekte als Tuple zur�ck
    return std::make_tuple(mat_vec, cam_vec, shape_vec, light_vec);
} // Ende der sdf_reader Funktion