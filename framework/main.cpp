//// main.cpp
//#include "window.hpp"
//#include "renderer.hpp"
//#include "sdf-reader.cpp"
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <vector>
//#include <memory>
//
//int main(int argc, char* argv[]) {
//
//    std::ofstream logFile("debug_log.txt");
//    logFile << "Programm gestartet" << std::endl;
//
//    unsigned const width = 800;
//    unsigned const height = 600;
//    std::string const filename = "./rendered_scene.ppm";
//
//    // Fenster erstellen
//    Window window({ width, height });
//
//    // SDF-Datei lesen
//    std::string sdf_file = "scene.sdf";
//    auto [materials, cameras] = sdf_reader(sdf_file);
//
//    if (cameras.empty()) {
//        std::cerr << "Keine Kamera in der SDF-Datei gefunden." << std::endl;
//        return 1;
//    }
//
//    // Erste Kamera aus der SDF-Datei verwenden
//    auto& camera = cameras[0];
//
//    // Renderer mit der Kamera initialisieren
//    Renderer renderer(width, height, filename, *camera);
//
//    // Render-Schleife
//    while (!window.should_close()) {
//        if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//            window.close();
//        }
//
//        // Rendern
//        renderer.render();
//
//        // Bild anzeigen
//        window.show(renderer.color_buffer());
//    }
//
//
//    return 0;
//}