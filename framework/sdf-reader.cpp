// sdf-reader.cpp
#include "Material.hpp"
#include "camera.hpp"
#include "shape.hpp"
#include "sphere.hpp"
#include "box.hpp"
#include "triangle.hpp"
#include "cone.hpp"
#include "cylinder.hpp"
#include "light.hpp"
#include "composite.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <memory>
#include <map>


std::shared_ptr<Shape> find_shape(const std::string& name, const std::vector<std::shared_ptr<Shape>>& shapes) {
    auto it = std::find_if(shapes.begin(), shapes.end(),
        [&name](const auto& shape) { return shape->getName() == name; });
    return (it != shapes.end()) ? *it : nullptr;
}

glm::mat4 parseTransformation(std::istringstream& line_as_stream) {
    std::string transform_type;
    line_as_stream >> transform_type;

    if (transform_type == "scale") {
        float sx, sy, sz;
        line_as_stream >> sx >> sy >> sz;
        return glm::scale(glm::mat4(1.0f), glm::vec3(sx, sy, sz));
    }
    else if (transform_type == "translate") {
        float tx, ty, tz;
        line_as_stream >> tx >> ty >> tz;
        return glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));
    }
    else if (transform_type == "rotate") {
        float angle, rx, ry, rz;
        line_as_stream >> angle >> rx >> ry >> rz;
        return glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(rx, ry, rz));
    }

    return glm::mat4(1.0f);
}

std::tuple<std::vector<std::shared_ptr<Material>>,
    std::vector<std::shared_ptr<Camera>>,
    std::vector<std::shared_ptr<Shape>>,
    std::vector<std::shared_ptr<Light>>>
    sdf_reader(const std::string& sdf_file_path) {
    std::cout << "Versuche SDF-Datei zu %C3%B6ffnen: " << sdf_file_path << std::endl;
    std::ifstream sdf_file(sdf_file_path);
    if (!sdf_file.is_open()) {
        std::cerr << "Konnte Datei nicht finden oder oeffnen: " << sdf_file_path << std::endl;
        return {};
    }

    std::vector<std::shared_ptr<Material>> mat_vec;
    std::vector<std::shared_ptr<Camera>> cam_vec;
    std::vector<std::shared_ptr<Shape>> shape_vec;
    std::vector<std::shared_ptr<Light>> light_vec;

    std::string line_buffer;

    std::cout << "SDF Reader: Starte Parsing von " << sdf_file_path << std::endl;

    while (std::getline(sdf_file, line_buffer)) {
        // Entferne führende Leerzeichen
        line_buffer.erase(line_buffer.begin(), std::find_if(line_buffer.begin(), line_buffer.end(), [](unsigned char ch) {
            return !std::isspace(ch);
            }));

        // Ignoriere leere Zeilen und Kommentare
        if (line_buffer.empty() || line_buffer[0] == '#' || line_buffer.substr(0, 2) == "/*") {
            continue;
        }

        std::istringstream line_as_stream(line_buffer);
        std::string token;
        line_as_stream >> token;

        if (token == "define") {
            line_as_stream >> token;
            if (token == "material") {
                auto mat_ptr = std::make_shared<Material>();
                line_as_stream >> mat_ptr->name
                    >> mat_ptr->ka.r >> mat_ptr->ka.g >> mat_ptr->ka.b
                    >> mat_ptr->kd.r >> mat_ptr->kd.g >> mat_ptr->kd.b
                    >> mat_ptr->ks.r >> mat_ptr->ks.g >> mat_ptr->ks.b
                    >> mat_ptr->m >> mat_ptr->refraction_index >> mat_ptr->transparency;
                mat_vec.push_back(mat_ptr);
                std::cout << "Material geladen: " << mat_ptr->name << std::endl;
            }
            else if (token == "camera") {
                std::string name;
                float fov_x;
                glm::vec3 eye, dir, up;
                line_as_stream >> name >> fov_x
                    >> eye.x >> eye.y >> eye.z
                    >> dir.x >> dir.y >> dir.z
                    >> up.x >> up.y >> up.z;
                auto cam_ptr = std::make_shared<Camera>(name, fov_x, eye, dir, up);
                cam_vec.push_back(cam_ptr);
                std::cout << "Kamera geladen: " << name << std::endl;
            }
            else if (token == "shape") {
                line_as_stream >> token;
                if (token == "sphere") {
                    std::string name, material_name;
                    float radius;
                    glm::vec3 center;
                    line_as_stream >> name >> center.x >> center.y >> center.z >> radius >> material_name;
                    auto material = std::find_if(mat_vec.begin(), mat_vec.end(),
                        [&material_name](const auto& mat) { return mat->name == material_name; });
                    if (material != mat_vec.end()) {
                        shape_vec.push_back(std::make_shared<Sphere>(radius, center, name, *material));
                    }
                    std::cout << "Kugel geladen: " << name << std::endl;
                }
                else if (token == "box") {
                    std::string name, material_name;
                    glm::vec3 min, max;
                    line_as_stream >> name >> min.x >> min.y >> min.z >> max.x >> max.y >> max.z >> material_name;
                    auto material = std::find_if(mat_vec.begin(), mat_vec.end(),
                        [&material_name](const auto& mat) { return mat->name == material_name; });
                    if (material != mat_vec.end()) {
                        auto box = std::make_shared<Box>(min, max, name, *material);
                        box->setMaterial(*material);  // Explizit das Material setzen
                        shape_vec.push_back(box);
                        std::cout << "Box geladen: " << name << " mit Material: " << material_name << std::endl;
                    }
                    else {
                        std::cerr << "Warnung: Material '" << material_name << "' nicht gefunden für Box '" << name << "'" << std::endl;
                    }
                }
                else if (token == "triangle") {
                    std::string name, material_name;
                    glm::vec3 v1, v2, v3;
                    line_as_stream >> name
                        >> v1.x >> v1.y >> v1.z
                        >> v2.x >> v2.y >> v2.z
                        >> v3.x >> v3.y >> v3.z
                        >> material_name;
                    auto material = std::find_if(mat_vec.begin(), mat_vec.end(),
                        [&material_name](const auto& mat) { return mat->name == material_name; });
                    if (material != mat_vec.end()) {
                        auto triangle = std::make_shared<Triangle>(v1, v2, v3, name, *material);
                        shape_vec.push_back(triangle);
                        std::cout << "Dreieck geladen: " << name << " mit Material: " << material_name << std::endl;
                    }
                    else {
                        std::cerr << "Warnung: Material '" << material_name << "' nicht gefunden für Dreieck '" << name << "'" << std::endl;
                        // Optional: Erstellen Sie das Dreieck trotzdem mit einem Standardmaterial
                        // auto triangle = std::make_shared<Triangle>(v1, v2, v3, name, std::make_shared<Material>());
                        // shape_vec.push_back(triangle);
                    }
                }
                else if (token == "cone") {
                    std::string name, material_name;
                    float radius, height;
                    glm::vec3 base_center;
                    line_as_stream >> name >> base_center.x >> base_center.y >> base_center.z >> radius >> height >> material_name;
                    auto material = std::find_if(mat_vec.begin(), mat_vec.end(),
                        [&material_name](const auto& mat) { return mat->name == material_name; });
                    if (material != mat_vec.end()) {
                        shape_vec.push_back(std::make_shared<Cone>(name, radius, height, *material));
                        auto& cone = shape_vec.back();
                        cone->setTransformation(glm::translate(glm::mat4(1.0f), base_center));
                    }
                    std::cout << "Kegel geladen: " << name << std::endl;
                }
                else if (token == "cylinder") {
                    std::string name, material_name;
                    float radius, height;
                    glm::vec3 base_center;
                    line_as_stream >> name >> base_center.x >> base_center.y >> base_center.z >> radius >> height >> material_name;
                    auto material = std::find_if(mat_vec.begin(), mat_vec.end(),
                        [&material_name](const auto& mat) { return mat->name == material_name; });
                    if (material != mat_vec.end()) {
                        shape_vec.push_back(std::make_shared<Cylinder>(name, radius, height, *material));
                        auto& cylinder = shape_vec.back();
                        cylinder->setTransformation(glm::translate(glm::mat4(1.0f), base_center));
                    }
                    std::cout << "Zylinder geladen: " << name << std::endl;
                }
                else if (token == "composite") {
                    std::string composite_name;
                    line_as_stream >> composite_name;
                    auto composite = std::make_shared<Composite>(composite_name);

                    std::string child_name;
                    while (line_as_stream >> child_name) {
                        auto child = find_shape(child_name, shape_vec);
                        if (child) {
                            composite->add(child);
                        }
                    }

                    shape_vec.push_back(composite);
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
                line_as_stream >> name >> position.x >> position.y >> position.z
                    >> color.r >> color.g >> color.b;
                light_vec.push_back(std::make_shared<Light>(position, color, name));
                std::cout << "Licht geladen: " << name << std::endl;
            }
        }
        else if (token == "transform") {
            std::string shape_name;
            line_as_stream >> shape_name;

            glm::mat4 transformation = parseTransformation(line_as_stream);

            auto shape_it = std::find_if(shape_vec.begin(), shape_vec.end(),
                [&shape_name](const auto& shape) { return shape->getName() == shape_name; });

            if (shape_it != shape_vec.end()) {
                (*shape_it)->setTransformation(transformation);
                //std::cout << "Transformation applied to " << shape_name << std::endl;
            }
            else {
                //std::cerr << "Warnung: Shape '" << shape_name << "' nicht gefunden für Transformation." << std::endl;
            }
        }
    }

    shape_vec.erase(
        std::remove_if(shape_vec.begin(), shape_vec.end(),
            [&shape_vec](const auto& shape) {
                return std::any_of(shape_vec.begin(), shape_vec.end(),
                    [&shape](const auto& potential_parent) {
                        auto composite = std::dynamic_pointer_cast<Composite>(potential_parent);
                        return composite && composite.get() != shape.get() &&
                            std::find_if(composite->getChildren().begin(), composite->getChildren().end(),
                                [&shape](const auto& child) { return child.get() == shape.get(); }) != composite->getChildren().end();
                    });
            }),
        shape_vec.end()
    );

    //std::cout << "Geladene Objekte:" << std::endl; std::cout << "Materialien: " << mat_vec.size() << std::endl; std::cout << "Kameras: " << cam_vec.size() << std::endl; std::cout << "Shapes: " << shape_vec.size() << std::endl; std::cout << "Lichter: " << light_vec.size() << std::endl;

    return std::make_tuple(mat_vec, cam_vec, shape_vec, light_vec);
}