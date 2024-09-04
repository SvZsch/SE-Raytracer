
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <renderer.hpp>
#include <window.hpp>
#include <thread>
#include <utility>
#include <cmath>
#include <iostream>
#include <vector>
#include <memory>
#include <filesystem>
#include "sphere.hpp"
#include "box.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "light.hpp"
#include "sdf-reader.cpp"

//#include "box.hpp"
//#include "camera.hpp"
//#include "light.hpp"
//#include "material.hpp"
//#include "sdf-reader.cpp"
//#include "shape.hpp"
//#include "sphere.hpp"
//#include <cmath>
//#include <filesystem>
//#include <GLFW/glfw3.h>
//#include <iostream>
//#include <memory>
//#include <renderer.hpp>
//#include <thread>
//#include <utility>
//#include <vector>
//#include <window.hpp>
//
//int main(int argc, char* argv[])
//{
//    unsigned const image_width = 2024;
//    unsigned const image_height = 1024;
//    std::string const filename = "./raytraced_scene.ppm";
//    std::string sdf_filename = "C:/Users/garud/Desktop/sdfreader/scene.sdf";  // Annahme: Die Datei ist im aktuellen Verzeichnis
//
//    std::cout << "Versuche, SDF-Datei zu öffnen: " << sdf_filename << std::endl;
//
//    if (!std::filesystem::exists(sdf_filename)) {
//        std::cerr << "Die SDF-Datei existiert nicht: " << sdf_filename << std::endl;
//        return 1;
//    }
//
//    // SDF-Datei lesen
//    auto [materials, cameras] = sdf_reader(sdf_filename);
//
//    std::cout << "Anzahl der gelesenen Materialien: " << materials.size() << std::endl;
//    std::cout << "Anzahl der gelesenen Kameras: " << cameras.size() << std::endl;
//
//    if (cameras.empty()) {
//        std::cerr << "Keine Kamera in der SDF-Datei definiert." << std::endl;
//        return 1;
//    }
//
//    // Verwenden Sie die erste Kamera aus der SDF-Datei
//    Camera& camera = *cameras[0];
//
//    std::cout << "Kamera erfolgreich geladen." << std::endl;
//
//    // Erstellen Sie einen Renderer mit der Kamera
//    Renderer renderer{ image_width, image_height, filename, camera };
//
//    std::cout << "Renderer erstellt." << std::endl;
//
//    // Fügen Sie ein Glasmaterial hinzu
//    auto glass_material = std::make_shared<Material>("Glass",
//        Color{ 0.1f, 0.1f, 0.1f }, // ka
//        Color{ 0.1f, 0.1f, 0.1f }, // kd
//        Color{ 0.9f, 0.9f, 0.9f }, // ks
//        50.0f, // m (Shininess)
//        1.5f   // Refraction index
//    );
//    materials.push_back(glass_material);
//
//    // Erstellen Sie Objekte
//    std::vector<std::shared_ptr<Shape>> shapes;
//    shapes.push_back(std::make_shared<Sphere>(1.0f, glm::vec3(-2, 0, 0), "Red Sphere", materials[0]));
//    shapes.push_back(std::make_shared<Sphere>(1.5f, glm::vec3(2, 0, 0), "Blue Sphere", materials[2]));
//    //shapes.push_back(std::make_shared<Box>(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), "Green Box", materials[2]));
//    shapes.push_back(std::make_shared<Sphere>(1.0f, glm::vec3(0, 2, 0), "Glass Sphere", glass_material));
//
//    std::cout << "Objekte erstellt." << std::endl;
//
//    // Erstellen Sie Lichtquellen
//    std::vector<std::shared_ptr<Light>> lights;
//    lights.push_back(std::make_shared<Light>(glm::vec3(5, 5, 5), Color{ 1.0f, 1.0f, 1.0f }));
//    //lights.push_back(std::make_shared<Light>(glm::vec3(-10, 5, 5), Color{ 0.5f, 0.5f, 0.5f }));
//
//    std::cout << "Lichtquellen erstellt." << std::endl;
//
//    // Fügen Sie die Objekte und Lichtquellen zum Renderer hinzu
//    renderer.add_shapes(shapes);
//    renderer.add_lights(lights);
//
//    std::cout << "Objekte und Lichtquellen zum Renderer hinzugefügt." << std::endl;
//
//    // Rendern Sie die Szene
//    std::cout << "Starte Rendering-Prozess..." << std::endl;
//    renderer.render();
//    std::cout << "Rendering abgeschlossen." << std::endl;
//
//    // Zeigen Sie das gerenderte Bild in einem Fenster an
//    std::cout << "Öffne Fenster..." << std::endl;
//    Window window{ {image_width, image_height} };
//
//    std::cout << "Fenster erstellt. Starte Render-Schleife..." << std::endl;
//    while (!window.should_close()) {
//        if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//            window.close();
//        }
//        window.show(renderer.color_buffer());
//    }
//
//    std::cout << "Programm beendet." << std::endl;
//    return 0;
//}

// raytracer.cpp
//#include <renderer.hpp>
//#include <window.hpp>
//#include <GLFW/glfw3.h>
//#include <thread>
//#include <utility>
//#include <cmath>
//#include <iostream>
//#include <vector>
//#include <memory>
//#include <filesystem>
//#include "sphere.hpp"
//#include "box.hpp"
//#include "camera.hpp"
//#include "material.hpp"
//#include "light.hpp"
//#include "sdf-reader.cpp"

int main(int argc, char* argv[])
{
    unsigned const image_width = 2024;
    unsigned const image_height = 1024;
    std::string const filename = "./raytraced_scene.ppm";
    std::string sdf_filename = "C:/Users/garud/Desktop/sdfreader/scene.sdf";
    std::cout << "Versuche, SDF-Datei zu öffnen: " << sdf_filename << std::endl;
    if (!std::filesystem::exists(sdf_filename)) {
        std::cerr << "Die SDF-Datei existiert nicht: " << sdf_filename << std::endl;
        return 1;
    }
    // SDF-Datei lesen
    auto [materials, cameras, shapes, lights] = sdf_reader(sdf_filename);
    std::cout << "Anzahl der gelesenen Materialien: " << materials.size() << std::endl;
    std::cout << "Anzahl der gelesenen Kameras: " << cameras.size() << std::endl;
    std::cout << "Anzahl der gelesenen Shapes: " << shapes.size() << std::endl;
    std::cout << "Anzahl der gelesenen Lichter: " << lights.size() << std::endl;

    std::cout << "Gelesene Materialien:" << std::endl;
    for (const auto& mat : materials) {
        std::cout << *mat << std::endl;
    }

    std::cout << "Gelesene Kameras:" << std::endl;
    for (const auto& cam : cameras) {
        //std::cout << *cam << std::endl;
    }

    std::cout << "Gelesene Shapes:" << std::endl;
    /*for (const auto& shape : shapes) {
        std::cout << *shape << std::endl;
    }*/

    std::cout << "Gelesene Lichter:" << std::endl;
    for (const auto& light : lights) {
        //std::cout << *light << std::endl;
    }

    if (cameras.empty()) {
        std::cerr << "Keine Kamera in der SDF-Datei definiert." << std::endl;
        return 1;
    }

    // Verwenden Sie die erste Kamera aus der SDF-Datei
    Camera& camera = *cameras[0];
    std::cout << "Kamera erfolgreich geladen." << std::endl;
    /*std::cout << "Kamera Position: " << camera.eye().x << ", " << camera.eye().y << ", " << camera.eye().z << std::endl;
    std::cout << "Kamera Richtung: " << camera.dir().x << ", " << camera.dir().y << ", " << camera.dir().z << std::endl;*/

    // Erstellen Sie einen Renderer mit der Kamera
    Renderer renderer{ image_width, image_height, filename, camera };
    std::cout << "Renderer erstellt." << std::endl;

    // Fügen Sie die Objekte und Lichtquellen zum Renderer hinzu
    renderer.add_shapes(shapes);
    renderer.add_lights(lights);
    std::cout << "Objekte und Lichtquellen zum Renderer hinzugefügt." << std::endl;

    // Rendern Sie die Szene
    std::cout << "Starte Rendering-Prozess..." << std::endl;
    renderer.render();
    std::cout << "Rendering abgeschlossen." << std::endl;

    // Zeigen Sie das gerenderte Bild in einem Fenster an
    std::cout << "Öffne Fenster..." << std::endl;
    Window window{ {image_width, image_height} };
    std::cout << "Fenster erstellt. Starte Render-Schleife..." << std::endl;

    while (!window.should_close()) {
        if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            window.close();
        }
        window.show(renderer.color_buffer());
    }

    renderer.add_shapes(shapes);
    renderer.add_lights(lights);

    std::cout << "Programm beendet." << std::endl;
    return 0;
}