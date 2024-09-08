#include <GL/glew.h>         // Bibliothek zum Laden von OpenGL-Erweiterungen (Funktionen), die in modernen OpenGL-Versionen benötigt werden.
#include <GLFW/glfw3.h>      // Bibliothek zum Erstellen von Fenstern, Verwalten von Eingaben und Erstellen eines OpenGL-Kontextes.
#include <renderer.hpp>      // Benutzerdefinierte Header-Datei, die die `Renderer`-Klasse enthält, die für das Raytracing und das Speichern des gerenderten Bildes verantwortlich ist.
#include <window.hpp>        // Benutzerdefinierte Header-Datei, die die `Window`-Klasse enthält, um ein Fenster zu öffnen und das gerenderte Bild darin anzuzeigen.
#include <thread>            // Bibliothek für die Arbeit mit Threads, die parallele Ausführung ermöglicht.
#include <utility>           // Bibliothek für verschiedene Hilfsfunktionen, wie z. B. `std::move`, die zum effizienten Transfer von Ressourcen verwendet werden.
#include <cmath>             // Mathematische Bibliothek, die Funktionen wie Sinus, Kosinus, Quadratwurzeln usw. bereitstellt.
#include <iostream>          // Standardbibliothek für Ein- und Ausgabestreams, z. B. `std::cout` und `std::cerr`.
#include <vector>            // STL-Container, der dynamische Arrays bereitstellt, um mehrere Elemente desselben Typs zu speichern.
#include <memory>            // Bibliothek für Smart Pointers, z. B. `std::shared_ptr` und `std::unique_ptr`, die eine automatische Speicherverwaltung ermöglichen.
#include <filesystem>        // C++17-Bibliothek zur Arbeit mit dem Dateisystem, z. B. um Dateien und Verzeichnisse zu überprüfen und zu manipulieren.
#include "sphere.hpp"        // Benutzerdefinierte Header-Datei, die die Definition der `Sphere`-Klasse enthält, um Kugelobjekte zu erstellen.
#include "box.hpp"           // Benutzerdefinierte Header-Datei, die die Definition der `Box`-Klasse enthält, um Boxobjekte zu erstellen.
#include "camera.hpp"        // Benutzerdefinierte Header-Datei, die die Definition der `Camera`-Klasse enthält, um Kameraobjekte zu erstellen und zu verwalten.
#include "material.hpp"      // Benutzerdefinierte Header-Datei, die Materialdefinitionen für die Objekte in der Szene bereitstellt (z. B. Farbe, Reflexionseigenschaften).
#include "light.hpp"         // Benutzerdefinierte Header-Datei, die die Definition der `Light`-Klasse enthält, um Lichtquellen in der Szene zu erstellen.
#include "sdf-reader.cpp"    // Implementierung des SDF-Readers, der zum Einlesen von Szenen aus einer SDF-Datei verwendet wird.

int main(int argc, char* argv[])
{
    // Breite und Höhe des zu rendernden Bildes in Pixeln festlegen.
    unsigned const image_width = 2024;
    unsigned const image_height = 1024;

    // Name der Datei, in die das gerenderte Bild im PPM-Format gespeichert wird.
    std::string const filename = "./raytraced_scene.ppm";

    // Pfad zur SDF-Datei, die die Szene beschreibt (dieser Pfad sollte angepasst werden).
    std::string sdf_filename = "C:/Users/garud/Desktop/sdfreader/scene.sdf";

    // Ausgabe der Nachricht, dass versucht wird, die SDF-Datei zu öffnen, um den Ladevorgang zu visualisieren.
    std::cout << "Versuche, SDF-Datei zu öffnen: " << sdf_filename << std::endl;

    // Überprüfung, ob die angegebene SDF-Datei existiert.
    if (!std::filesystem::exists(sdf_filename)) {
        // Fehlermeldung ausgeben, wenn die Datei nicht gefunden wird, und das Programm beenden.
        std::cerr << "Die SDF-Datei existiert nicht: " << sdf_filename << std::endl;
        return 1; // Programm mit Fehlercode 1 beenden.
    }

    // Einlesen der SDF-Datei und Aufteilung des Inhalts in Materialien, Kameras, Shapes (Formen) und Lichtquellen.
    auto [materials, cameras, shapes, lights] = sdf_reader(sdf_filename);

    // Ausgabe der Anzahl der gefundenen Materialien.
    std::cout << "Anzahl der gelesenen Materialien: " << materials.size() << std::endl;
    // Ausgabe der Anzahl der gefundenen Kameras.
    std::cout << "Anzahl der gelesenen Kameras: " << cameras.size() << std::endl;
    // Ausgabe der Anzahl der gefundenen Shapes (Formen).
    std::cout << "Anzahl der gelesenen Shapes: " << shapes.size() << std::endl;
    // Ausgabe der Anzahl der gefundenen Lichtquellen.
    std::cout << "Anzahl der gelesenen Lichter: " << lights.size() << std::endl;

    // Ausgabe aller eingelesenen Materialien mit ihren Eigenschaften.
    std::cout << "Gelesene Materialien:" << std::endl;
    for (const auto& mat : materials) {
        std::cout << *mat << std::endl; // Die `*mat`-Dereferenzierung gibt das Materialobjekt aus.
    }

    // Ausgabe aller eingelesenen Kameras (derzeit auskommentiert).
    std::cout << "Gelesene Kameras:" << std::endl;
    for (const auto& cam : cameras) {
        //std::cout << *cam << std::endl; // Hier könnten Details der Kamera ausgegeben werden.
    }

    // Ausgabe aller eingelesenen Shapes (derzeit auskommentiert).
    std::cout << "Gelesene Shapes:" << std::endl;
    /*for (const auto& shape : shapes) {
        std::cout << *shape << std::endl;
    }*/

    // Ausgabe aller eingelesenen Lichtquellen (derzeit auskommentiert).
    std::cout << "Gelesene Lichter:" << std::endl;
    for (const auto& light : lights) {
        //std::cout << *light << std::endl;
    }

    // Überprüfen, ob mindestens eine Kamera definiert wurde.
    if (cameras.empty()) {
        // Fehlermeldung ausgeben, wenn keine Kamera in der Szene definiert ist, und das Programm beenden.
        std::cerr << "Keine Kamera in der SDF-Datei definiert." << std::endl;
        return 1; // Programm mit Fehlercode 1 beenden.
    }

    // Verwenden der ersten gefundenen Kamera aus der Szene.
    Camera& camera = *cameras[0];
    std::cout << "Kamera erfolgreich geladen." << std::endl;
    /*std::cout << "Kamera Position: " << camera.eye().x << ", " << camera.eye().y << ", " << camera.eye().z << std::endl;
    std::cout << "Kamera Richtung: " << camera.dir().x << ", " << camera.dir().y << ", " << camera.dir().z << std::endl;*/ // Optional: Ausgabe von Kamera-Details.

    // Erstellen eines Renderers mit den Bildabmessungen, dem Dateinamen und der Kamera.
    Renderer renderer{ image_width, image_height, filename, camera };
    std::cout << "Renderer erstellt." << std::endl;

    // Hinzufügen der eingelesenen Shapes (Formen) zum Renderer.
    renderer.add_shapes(shapes);
    // Hinzufügen der eingelesenen Lichtquellen zum Renderer.
    renderer.add_lights(lights);
    std::cout << "Objekte und Lichtquellen zum Renderer hinzugefügt." << std::endl;

    // Starten des Rendering-Prozesses (Raytracing).
    std::cout << "Starte Rendering-Prozess..." << std::endl;
    renderer.render(); // Das Bild wird berechnet und gerendert.
    std::cout << "Rendering abgeschlossen." << std::endl;

    // Erstellen eines Fensters, um das gerenderte Bild anzuzeigen.
    std::cout << "Öffne Fenster..." << std::endl;
    Window window{ {image_width, image_height} }; // Das Fenster wird mit der Bildgröße initialisiert.
    std::cout << "Fenster erstellt. Starte Render-Schleife..." << std::endl;

    // Hauptschleife des Fensters: Hier wird das Bild im Fenster angezeigt, bis das Fenster geschlossen wird.
    while (!window.should_close()) { // Die Schleife läuft, solange das Fenster nicht geschlossen werden soll.
        if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) { // Wenn die Escape-Taste gedrückt wird...
            window.close(); // ... wird das Fenster geschlossen.
        }
        window.show(renderer.color_buffer()); // Das gerenderte Bild wird im Fenster angezeigt.
    }

    // Das Programm gibt eine Erfolgsmeldung aus und beendet sich.
    std::cout << "Programm beendet." << std::endl;
    return 0; // Erfolgreiches Beenden des Programms mit Rückgabewert 0.
}


////Start für Animation
//int main(int argc, char* argv[])
//{
//    std::string input_directory;
//    std::string output_directory;
//
//    if (argc < 3) {
//        std::cout << "Bitte geben Sie das Eingabeverzeichnis ein: ";
//        std::getline(std::cin, input_directory);
//        std::cout << "Bitte geben Sie das Ausgabeverzeichnis ein: ";
//        std::getline(std::cin, output_directory);
//    }
//    else {
//        input_directory = argv[1];
//        output_directory = argv[2];
//    }
//
//    std::cout << "Input directory: " << input_directory << std::endl;
//    std::cout << "Output directory: " << output_directory << std::endl;
//
//    // Überprüfen Sie, ob die Verzeichnisse existieren
//    if (!std::filesystem::exists(input_directory)) {
//        std::cerr << "Eingabeverzeichnis existiert nicht: " << input_directory << std::endl;
//        return 1;
//    }
//    if (!std::filesystem::exists(output_directory)) {
//        std::cout << "Ausgabeverzeichnis existiert nicht. Soll es erstellt werden? (j/n): ";
//        char response;
//        std::cin >> response;
//        if (response == 'j' || response == 'J') {
//            std::filesystem::create_directories(output_directory);
//        }
//        else {
//            std::cerr << "Ausgabeverzeichnis existiert nicht und wurde nicht erstellt." << std::endl;
//            return 1;
//        }
//    }
//
//
//    unsigned const image_width = 2024;
//    unsigned const image_height = 1024;
//
//    for (const auto& entry : std::filesystem::directory_iterator(input_directory)) {
//        if (entry.path().extension() == ".sdf") {
//            std::string sdf_filename = entry.path().string();
//            std::cout << "Processing SDF file: " << sdf_filename << std::endl;
//
//            // SDF-Datei lesen
//            auto [materials, cameras, shapes, lights] = sdf_reader(sdf_filename);
//            std::cout << "Anzahl der gelesenen Materialien: " << materials.size() << std::endl;
//            std::cout << "Anzahl der gelesenen Kameras: " << cameras.size() << std::endl;
//            std::cout << "Anzahl der gelesenen Shapes: " << shapes.size() << std::endl;
//            std::cout << "Anzahl der gelesenen Lichter: " << lights.size() << std::endl;
//
//            if (cameras.empty()) {
//                std::cerr << "Keine Kamera in der SDF-Datei definiert: " << sdf_filename << std::endl;
//                continue;
//            }
//
//            // Verwenden Sie die erste Kamera aus der SDF-Datei
//            Camera& camera = *cameras[0];
//            std::cout << "Kamera erfolgreich geladen." << std::endl;
//
//            // Generate output filename
//            std::string output_filename = output_directory + "/" + entry.path().stem().string() + ".ppm";
//
//            // Erstellen Sie einen Renderer mit der Kamera
//            Renderer renderer{ image_width, image_height, output_filename, camera };
//            std::cout << "Renderer erstellt." << std::endl;
//
//            // Fügen Sie die Objekte und Lichtquellen zum Renderer hinzu
//            renderer.add_shapes(shapes);
//            renderer.add_lights(lights);
//            std::cout << "Objekte und Lichtquellen zum Renderer hinzugefügt." << std::endl;
//
//            // Rendern Sie die Szene
//            std::cout << "Starte Rendering-Prozess für " << sdf_filename << "..." << std::endl;
//            renderer.render();
//            std::cout << "Rendering abgeschlossen. Bild gespeichert als: " << output_filename << std::endl;
//        }
//    }
//
//    std::cout << "Alle SDF-Dateien wurden verarbeitet." << std::endl;
//    return 0;
//}