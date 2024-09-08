#include <iostream>   // Einbinden der Bibliothek für die Ein- und Ausgabe
#include <fstream>    // Einbinden der Bibliothek für Dateioperationen
#include <sstream>    // Einbinden der Bibliothek für String-Streams
#include <cmath>      // Einbinden der Bibliothek für mathematische Funktionen wie Sinus und Cosinus
#include <vector>     // Einbinden der Bibliothek für den Datentyp Vektor
#include <string>     // Einbinden der Bibliothek für den Datentyp String

const int FRAMES = 120;   // Definieren einer Konstanten für die Anzahl der Frames (120)
const double PI = 3.14159265358979323846;   // Definieren des Wertes für Pi (ungefähr 15 Nachkommastellen)

std::string generateSDFContent(double angle) {   // Funktionsdefinition: Diese Funktion erzeugt den Inhalt der SDF-Datei basierend auf dem Winkel
    std::stringstream ss;   // Erstellen eines String-Streams, um den Textinhalt der Datei zu speichern

    // Füge hier den statischen Teil der SDF-Datei ein
    ss << R"(# Materialien definieren (unverändert)
define material red 0.1 0.1 0.1 0.7 0.0 0.0 0.3 0.3 0.3 20 1.0 0.0
define material glass 0.2 0.2 0.2 0.2 0.2 0.2 0.9 0.9 0.9 50 1.5 2 0.8 0.8 0.8
define material blue 0.1 0.1 0.2 0.0 0.0 0.7 0.2 0.2 0.8 10 1.0 0.0
define material green 0.1 0.2 0.1 0.0 0.7 0.0 0.2 0.8 0.2 15 1.0 0.0
define material gold 0.3 0.2 0.1 0.75 0.6 0.22 0.8 0.7 0.3 50 1.0 0.0
define material matte_purple 0.2 0.1 0.2 0.5 0.1 0.5 0.1 0.1 0.1 1 1.0 0.0
define material matte_sky_blue 0.2 0.4 0.6 0.5 0.7 0.9 0.0 0.0 0.0 1 1.0 0.0

# Metallische Materialien
define material silver 0.2 0.2 0.2 0.7 0.7 0.7 0.9 0.9 0.9 100 1.0 0.0
define material copper 0.3 0.1 0.1 0.7 0.3 0.2 0.8 0.5 0.3 80 1.0 0.0
define material bronze 0.2 0.1 0.06 0.6 0.3 0.1 0.7 0.5 0.2 70 1.0 0.0

# Edelstein-ähnliche Materialien
define material emerald 0.0 0.1 0.0 0.1 0.8 0.2 0.3 1.0 0.3 76 1.6 0.1
define material ruby 0.1 0.0 0.0 0.6 0.0 0.0 0.7 0.2 0.2 90 1.8 0.1
define material sapphire 0.0 0.0 0.1 0.0 0.2 0.8 0.2 0.3 1.0 80 1.7 0.1

# Matte Materialien
define material matte_orange 0.5 0.3 0.1 0.8 0.5 0.2 0.1 0.1 0.1 1 1.0 0.0
define material matte_lime 0.3 0.5 0.1 0.6 0.8 0.2 0.1 0.1 0.1 1 1.0 0.0
define material matte_pink 0.5 0.3 0.4 0.8 0.5 0.7 0.1 0.1 0.1 1 1.0 0.0

# Leuchtende Materialien
define material neon_green 0.1 0.8 0.1 0.2 1.0 0.2 0.8 1.0 0.8 50 1.0 0.0
define material neon_blue 0.1 0.1 0.8 0.2 0.2 1.0 0.8 0.8 1.0 50 1.0 0.0
define material neon_pink 0.8 0.1 0.5 1.0 0.2 0.7 1.0 0.8 0.9 50 1.0 0.0

# Perlmutt-ähnliche Materialien
define material pearl_white 0.8 0.8 0.8 0.9 0.9 0.9 1.0 1.0 1.0 30 1.2 0.2
define material pearl_pink 0.8 0.6 0.7 0.9 0.7 0.8 1.0 0.8 0.9 30 1.2 0.2

# Holz-ähnliche Materialien
define material light_wood 0.3 0.2 0.1 0.6 0.4 0.2 0.3 0.2 0.1 10 1.0 0.0
define material dark_wood 0.1 0.05 0.02 0.4 0.2 0.1 0.2 0.1 0.05 5 1.0 0.0

# Plastik-ähnliche Materialien
define material red_plastic 0.2 0.0 0.0 0.8 0.0 0.0 0.5 0.5 0.5 30 1.0 0.0
define material blue_plastic 0.0 0.0 0.2 0.0 0.0 0.8 0.5 0.5 0.5 30 1.0 0.0

# Wasser-ähnliches Material
define material water 0.0 0.1 0.2 0.0 0.2 0.4 0.8 0.8 1.0 50 1.33 0.6

define material olive_green 0.2 0.2 0.1 0.5 0.5 0.2 0.3 0.3 0.1 10 1.0 0.0
define material forest_green 0.1 0.4 0.1 0.0 0.6 0.0 0.3 0.5 0.2 15 1.0 0.0

# Neue Materialien für das Prisma
define material rainbow_red 0.1 0.0 0.0 0.8 0.0 0.0 0.9 0.1 0.1 100 1.5 0.7
define material rainbow_orange 0.1 0.05 0.0 0.8 0.4 0.0 0.9 0.5 0.1 100 1.5 0.7
define material rainbow_yellow 0.1 0.1 0.0 0.8 0.8 0.0 0.9 0.9 0.1 100 1.5 0.7
define material rainbow_green 0.0 0.1 0.0 0.0 0.8 0.0 0.1 0.9 0.1 100 1.5 0.7
define material rainbow_blue 0.0 0.0 0.1 0.0 0.0 0.8 0.1 0.1 0.9 100 1.5 0.7
define material rainbow_purple 0.05 0.0 0.1 0.4 0.0 0.8 0.5 0.1 0.9 100 1.5 0.7

# Angepasstes Material für die spiegelnde Fläche
define material mirror 0.1 0.1 0.1 0.1 0.1 0.1 0.9 0.9 0.9 150 1.0 0.0

)";

    // Berechne die neue Kameraposition basierend auf dem Winkel
    double radius = 10.0; // Radius des Kreises, auf dem sich die Kamera bewegt
    double x = radius * std::sin(angle);
    double z = radius * std::cos(angle);

    // Berechne die Blickrichtung der Kamera
    double lookX = -x;
    double lookZ = -z;

    // Normalisiere den Blickvektor
    double length = std::sqrt(lookX * lookX + lookZ * lookZ);
    lookX /= length;
    lookZ /= length;

    // Füge die aktualisierte Kameradefinition hinzu
    ss << "define camera main 45.0 " << x << " 2 " << z << " "
        << lookX << " -0.08 " << lookZ << " 0 -1 0\n";

    // Fügen den Rest der statischen SDF-Datei hinzu
    ss << R"(
define shape cylinder zylinder1 0.0 0.0 0.0 0.3 1.0 neon_green
define shape sphere sphere1 1 1 0 0.5 red
define shape cone konus1 -1.0 2 -2.0 0.5 1.0 rainbow_purple
define shape box box1 -0.5 -0.5 -0.5 0.5 0.5 0.5 blue
define shape box mirror_surface -3.5 -0.1 -4 3.5 0.1 1 light_wood
define shape triangle dreieck1 0 0 0 1 0 0 0 1 0 rainbow_blue

define shape composite rainbow_prism prism_red prism_orange prism_yellow prism_green prism_blue prism_purple

define light light1 5 5 5 1 1 1
define light light2 -3 2 0 0.6 0.6 0.6
define light light3 0 1 -0.5 0.8 0.8 0.8
define light light4 -1 3 -2 1.0 0.8 0.6

transform konus1 rotate -20 1 0 1
transform box1 translate 0 0 -1
transform box1 rotate 30 0 1 0
transform rainbow_prism translate 0.5 0.2 -2
transform rainbow_prism rotate -15 1 1 0
transform mirror_surface rotate 10 1 0 0
transform dreieck1 translate 0 0 0
transform dreieck1 rotate 45 1 1 1
transform zylinder1 translate 1 1 1
transform zylinder1 rotate -130 4 6 3

define shape composite root box1 sphere1 rainbow_prism
)";

    return ss.str();   // Die Funktion gibt den gesamten Textinhalt der SDF-Datei als String zurück
}

int main() {   // Hauptfunktion, die das Programm startet
    for (int frame = 0; frame < FRAMES; ++frame) {   // Schleife, die durch alle Frames (von 0 bis 119) iteriert
        double angle = 2 * PI * frame / FRAMES;   // Berechnung des Winkels für die aktuelle Frame-Nummer (verteilt über 2π für eine volle Drehung)
        std::string content = generateSDFContent(angle);   // Generieren des Inhalts der SDF-Datei für den aktuellen Frame basierend auf dem berechneten Winkel

        std::string filename = "frame_" + std::to_string(frame) + ".sdf";   // Erstellen des Dateinamens für den aktuellen Frame (z.B. "frame_0.sdf")
        std::ofstream outFile(filename);   // Öffnen der Datei zum Schreiben
        if (outFile.is_open()) {   // Überprüfen, ob die Datei erfolgreich geöffnet wurde
            outFile << content;   // Schreiben des Inhalts in die Datei
            outFile.close();   // Schließen der Datei
            std::cout << "Generated " << filename << std::endl;   // Ausgabe einer Bestätigungsmeldung in der Konsole
        }
        else {   // Falls die Datei nicht geöffnet werden konnte
            std::cerr << "Unable to open file " << filename << std::endl;   // Ausgabe einer Fehlermeldung in der Konsole
        }
    }

    return 0;   // Rückgabewert 0 zeigt an, dass das Programm erfolgreich beendet wurde
}