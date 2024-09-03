#include "Material.hpp"
#include "camera.hpp"
#include "shape.hpp"

#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm> // für std::all_of
//
//
//
//std::tuple<std::vector<std::shared_ptr<Material>>, std::vector<std::shared_ptr<Camera>>> sdf_reader(const std::string& sdf_file_path) {
//    std::ifstream sdf_file(sdf_file_path);
//    if (!sdf_file.is_open()) {
//        std::cerr << "Konnte Datei nicht finden oder oeffnen: " << sdf_file_path << std::endl;
//        return {};
//    }
//
//    std::vector<std::shared_ptr<Material>> mat_vec;
//    std::vector<std::shared_ptr<Camera>> cam_vec;
//    std::string line_buffer;
//
//    while (std::getline(sdf_file, line_buffer)) {
//        // Entferne führende Leerzeichen
//        line_buffer.erase(line_buffer.begin(), std::find_if(line_buffer.begin(), line_buffer.end(), [](unsigned char ch) {
//            return !std::isspace(ch);
//            }));
//
//        // Ignoriere leere Zeilen
//        if (line_buffer.empty()) {
//            continue;
//        }
//
//        // Ignoriere einzeilige Kommentare
//        if (line_buffer[0] == '#') {
//            continue;
//        }
//
//        // Ignoriere mehrzeilige Kommentare
//        if (line_buffer.substr(0, 2) == "/*") {
//            while (std::getline(sdf_file, line_buffer) && line_buffer.find("*/") == std::string::npos) {
//                // Lese weiter, bis das Ende des Kommentars gefunden wird
//            }
//            continue;
//        }
//
//        std::istringstream line_as_stream(line_buffer);
//        std::string token;
//
//        line_as_stream >> token;
//        if (token == "define") {
//            line_as_stream >> token;
//            if (token == "material") {
//                auto mat_ptr = std::make_shared<Material>();
//
//                // Material-Eigenschaften parsen
//                line_as_stream >> mat_ptr->name
//                    >> mat_ptr->ka.r >> mat_ptr->ka.g >> mat_ptr->ka.b
//                    >> mat_ptr->kd.r >> mat_ptr->kd.g >> mat_ptr->kd.b
//                    >> mat_ptr->ks.r >> mat_ptr->ks.g >> mat_ptr->ks.b
//                    >> mat_ptr->m;
//
//                mat_vec.push_back(mat_ptr);
//
//                std::cout << "Material geparst: " << mat_ptr->name << " "
//                    << mat_ptr->ka << " " << mat_ptr->kd << " " << mat_ptr->ks << " " << mat_ptr->m << std::endl;
//            }
//            else if (token == "camera") {
//                std::string name;
//                float fov_x;
//                glm::vec3 eye, dir, up;
//
//                line_as_stream >> name >> fov_x
//                    >> eye.x >> eye.y >> eye.z
//                    >> dir.x >> dir.y >> dir.z
//                    >> up.x >> up.y >> up.z;
//
//                auto cam_ptr = std::make_shared<Camera>(name, fov_x, eye, dir, up);
//                cam_vec.push_back(cam_ptr);
//
//                std::cout << "Kamera geparst: " << name << " FOV: " << fov_x
//                    << " Auge: (" << eye.x << "," << eye.y << "," << eye.z << ")"
//                    << " Richtung: (" << dir.x << "," << dir.y << "," << dir.z << ")"
//                    << " Oben: (" << up.x << "," << up.y << "," << up.z << ")" << std::endl;
//            }
//            else {
//                std::cerr << "Unerwartetes Schlüsselwort nach 'define': " << token << std::endl;
//            }
//        }
//        else if (!std::all_of(line_buffer.begin(), line_buffer.end(), ::isspace)) {
//            std::cerr << "Unerwartetes Schlüsselwort oder ungültige Zeile: " << line_buffer << std::endl;
//        }
//    }
//
//    return std::make_tuple(mat_vec, cam_vec);
//}

// sdf-reader.cpp
#include "Material.hpp"
#include "camera.hpp"
#include "shape.hpp"
#include "sphere.hpp"
#include "box.hpp"
#include "light.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <memory>

std::tuple<std::vector<std::shared_ptr<Material>>,
    std::vector<std::shared_ptr<Camera>>,
    std::vector<std::shared_ptr<Shape>>,
    std::vector<std::shared_ptr<Light>>>
    sdf_reader(const std::string& sdf_file_path) {
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

         //Ignoriere leere Zeilen
        if (line_buffer.empty()) {
            continue;
        }

        // Ignoriere einzeilige Kommentare
        if (line_buffer[0] == '#') {
            continue;
        }

        // Ignoriere mehrzeilige Kommentare
        if (line_buffer.substr(0, 2) == "/*") {
            while (std::getline(sdf_file, line_buffer) && line_buffer.find("*/") == std::string::npos) {
                // Lese weiter, bis das Ende des Kommentars gefunden wird
            }
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
                    >> mat_ptr->m >> mat_ptr->refraction_index;
                mat_vec.push_back(mat_ptr);
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
            }
            else if (token == "sphere") {
                std::string name, material_name;
                float radius;
                glm::vec3 center;
                line_as_stream >> name >> radius >> center.x >> center.y >> center.z >> material_name;
                auto material = std::find_if(mat_vec.begin(), mat_vec.end(),
                    [&material_name](const auto& mat) { return mat->name == material_name; });
                if (material != mat_vec.end()) {
                    shape_vec.push_back(std::make_shared<Sphere>(radius, center, name, *material));
                }
            }
            else if (token == "box") {
                std::string name, material_name;
                glm::vec3 min, max;
                line_as_stream >> name >> min.x >> min.y >> min.z >> max.x >> max.y >> max.z >> material_name;
                auto material = std::find_if(mat_vec.begin(), mat_vec.end(),
                    [&material_name](const auto& mat) { return mat->name == material_name; });
                if (material != mat_vec.end()) {
                    shape_vec.push_back(std::make_shared<Box>(min, max, name, *material));
                }
            }
            else if (token == "light") {
                std::string name;
                glm::vec3 position;
                Color color;
                line_as_stream >> name >> position.x >> position.y >> position.z
                    >> color.r >> color.g >> color.b;
                light_vec.push_back(std::make_shared<Light>(position, color, name));
            }
        }
    }

    return std::make_tuple(mat_vec, cam_vec, shape_vec, light_vec);
}