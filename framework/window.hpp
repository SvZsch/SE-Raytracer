// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Window
// -----------------------------------------------------------------------------
#ifndef BUW_WINDOW_HPP             // Beginn der Include-Guard: Verhindert mehrfaches Einbinden dieser Datei
#define BUW_WINDOW_HPP             // Definition des Include-Guard-Makros

#include "color.hpp"               // Inkludiert die Farbdefinitionen
#include <GL/glew.h>               // Inkludiert OpenGL Extension Wrangler Library
#include <GLFW/glfw3.h>            // Inkludiert GLFW für Fensterverwaltung
#include <vector>                  // Inkludiert die Standard-Vektor-Bibliothek
#define GLM_FORCE_RADIANS          // Erzwingt die Verwendung von Radiant in GLM
#include <glm/vec2.hpp>            // Inkludiert GLM 2D-Vektoren
#include <glm/vec3.hpp>            // Inkludiert GLM 3D-Vektoren
#include <glm/gtx/multiple.hpp>    // Inkludiert GLM-Erweiterungen
#include <array>                   // Inkludiert die Standard-Array-Bibliothek
#include <string>                  // Inkludiert die Standard-String-Bibliothek

struct GLFWwindow;                 // Vorwärtsdeklaration von GLFWwindow

class Window                       // Beginn der Window-Klassen-Definition
{
public:
    Window(glm::ivec2 const& window_size = { 640, 480 }); // Konstruktor mit Standardfenstergröße
    ~Window();                       // Destruktor

    enum MouseButton                 // Aufzählung für Maustasten
    {
        MOUSE_BUTTON_NONE = 0,       // Keine Maustaste
        MOUSE_BUTTON_LEFT = (1 << 0),// Linke Maustaste
        MOUSE_BUTTON_RIGHT = (1 << 1),// Rechte Maustaste
        MOUSE_BUTTON_MIDDLE = (1 << 2) // Mittlere Maustaste
    };

    enum KeyAction                   // Aufzählung für Tastenaktionen
    {
        KEY_PRESS = GLFW_PRESS,      // Taste gedrückt
        KEY_RELEASE = GLFW_RELEASE,    // Taste losgelassen
        KEY_REPEAT = GLFW_REPEAT      // Taste gehalten
    };

    glm::vec2 mouse_position() const; // Gibt die aktuelle Mausposition zurück
    bool should_close() const;       // Prüft, ob das Fenster geschlossen werden soll
    void close();                    // Schließt das Fenster
    void update();                   // Aktualisiert das Fenster
    int get_key(int key) const;      // Prüft den Status einer bestimmten Taste
    int get_mouse_button(int button) const; // Prüft den Status einer bestimmten Maustaste
    glm::ivec2 window_size() const;  // Gibt die aktuelle Fenstergröße zurück
    float get_time() const;          // Gibt die verstrichene Zeit seit der GLFW-Initialisierung zurück
    void show(std::vector<Color> const& color_buffer); // Zeigt den Farbpuffer im Fenster an

private:
    GLFWwindow* window_;       // Pointer auf das GLFW-Fenster
    glm::ivec2        window_size_;  // Fenstergröße
    glm::ivec2        frame_buffer_size_; // Größe des Framebuffers
    std::string const title_;        // Fenstertitel
    GLuint            vao_ = 0;  // Vertex Array Object
    GLuint            program_ = 0;  // Shader-Programm
    GLuint            texture_ = 0;  // Textur-Objekt
};

#endif // define BUW_WINDOW_HPP    // Ende der Include-Guard