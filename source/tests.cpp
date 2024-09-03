/*#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include "shape.hpp"
#include "sphere.hpp"
#include "box.hpp"
#include <ostream>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <memory>
#include "sdf-reader.cpp"


TEST_CASE("sdf-reader-test", "[sdf-r-test]")
{
	//std::string sdf_path = "C:\\Users\\garud\\Desktop\\sdfreader\\raytracer-sdf.sdf";
	//std::vector<std::shared_ptr<Material>> scene = sdf_reader(sdf_path);
	auto[material, camera] = sdf_reader("C:\\Users\\garud\\Desktop\\sdfreader\\raytracer-sdf.sdf");
}



TEST_CASE(" box-test ", "[box-test]")
{
	Box b1 = {};
	glm::vec3 min = { 0.0f, 0.0f, 1.0f };
	glm::vec3 max = { -10.0f, 100.0f, 100.0f };
	Box b2 = { min, max };
	REQUIRE(Approx(b2.volume()) == 99000.0f);
	REQUIRE(Approx(b1.volume()) == 1000000.0f);

	REQUIRE(Approx(b2.area()) == (2000.0f + 1980.0f + 2.0f * 9900.0f));
	REQUIRE(Approx(b1.area()) == 60000.0f);
}

TEST_CASE(" sphere-test ", "[sp-test]") {
	Sphere s1 = {};
	float r = -10.0f;
	glm::vec3 p = { -10.0f, 100.0f, 100.0f };
	Sphere s2 = { r, p };
	Sphere s3 = { 0.0f, p };

	REQUIRE(Approx(s2.volume()).margin(10e-5) == 4188.7902f);
	REQUIRE(Approx(s1.volume()).margin(10e-5) == 4188.7902f);
	REQUIRE(Approx(s3.volume()).margin(10e-5) == 0.0f);

	REQUIRE(Approx(s2.area()).margin(10e-5) == 1256.637f);
	REQUIRE(Approx(s1.area()).margin(10e-5) == 1256.637f);
	REQUIRE(Approx(s3.area()).margin(10e-5) == 0.0f);
}


TEST_CASE("Outstream-test", "[os-test]") {
	Sphere s = {};
	Box b = {};
	std::cout << b << " " << s << std::endl;
}*/
/*
TEST_CASE("intersect_ray_sphere", "[intersect]")
{
	glm::vec3 ray_origin = { 0.0f, 0.0f, 0.0f };

	glm::vec3 ray_direction = { 0.0f, 0.0f, 1.0f };

	glm::vec3 sphere_center = { 0.0f, 0.0f, 5.0f };
	float sphere_radius = 1.0f;

	float distance = 0.0f;
	auto result = glm::intersectRaySphere(ray_origin, ray_direction, sphere_center, sphere_radius * sphere_radius, distance);
	REQUIRE(distance == Approx(4.0f));

	Sphere s1{ sphere_radius, sphere_center };
	Ray r1{ ray_origin, ray_direction };
	Hitpoint h1 = s1.intersect(r1);
	REQUIRE(h1.distance == distance);
	REQUIRE(h1.hit == result);


	glm::vec3 ray_origin1 =  { 0.0f, 0.0f, 0.0f };

	glm::vec3 ray_direction1 = { 0.0f, 1.0f, 0.0f };

	glm::vec3 sphere_center1 = { 0.0f, 0.0f, 5.0f };
	float sphere_radius1 =  1.0f;

	float distance1 = 0.0f;
	Sphere s2{ sphere_radius1, sphere_center1 };
	Ray r2{ ray_origin1, ray_direction1 };
	Hitpoint h2 = s2.intersect(r2);

	REQUIRE(h2.hit == false);

	glm::vec3 ray_origin2 = { 0.0f, 1.0f, 0.0f };

	glm::vec3 ray_direction2 = { 0.0f, 0.0f, 1.0f };

	glm::vec3 sphere_center2 = { 0.0f, 0.0f, 5.0f };
	float sphere_radius2 =  1.0f;

	float distance2 = 0.0f;
	Sphere s3{ sphere_radius2, sphere_center2 };
	Ray r3{ ray_origin2, ray_direction2 };
	Hitpoint h3 = s3.intersect(r3);

	float dist = 0.0f;
	auto v = glm::normalize(r3.direction);
	glm::intersectRaySphere(ray_origin2, v, sphere_center2, sphere_radius2 * sphere_radius2, dist);
	glm::vec3 ergeb = { 0, 1, 5 };

	REQUIRE(h3.hit == true);
	REQUIRE(h3.color_obj->name == s3.color_->name);
	REQUIRE(h3.name_obj == s3.name_);
	std::cout << h3.hit_p.x << h3.hit_p.y << h3.hit_p.z << std::endl;
	REQUIRE(h3.hit_p == ergeb);
	REQUIRE(h3.distance == dist);
	REQUIRE(h3.ray_dir == ray_direction2);

}
*/
/*TEST_CASE("destruktor-test", "[dtor-test]")
{
	std::cout << " Beginn " << std::endl;
	Material mat = {};
	auto mat_zeig = std::make_shared<Material>(mat);
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	std::string name1 = "sphere0";
	std::string name2 = "sphere1";
	Sphere* s1 = new Sphere{ 1.2f, position,  name1, mat_zeig };
	Shape* s2 = new Sphere{ 1.2f, position,  name2, mat_zeig };

	s1->print(std::cout);
	s2->print(std::cout);

	delete s1;
	delete s2;
}

TEST_CASE("box-intersect-test", "[b-inter-test]")
{
	Box b = {};
	Ray r1 = {};
	Ray r2 = {};
	Ray r3 = {};
	r1.direction = { 1, 0, 0 };
	r2.direction = { 0, 1, 0 };
	r3.direction = { 0, 0, 1 };
	r1.origin = { 0, 0 ,0 };
	r2.origin = { 110, 110, 110 };
	r3.origin = { 50, 50, 150 };
	Hitpoint h1 = b.intersect(r1);
	Hitpoint h2 = b.intersect(r2);
	Hitpoint h3 = b.intersect(r3);
	REQUIRE(h1.hit == true);
	REQUIRE(h1.hit_p == glm::vec3 {0, 0, 0});
	REQUIRE(Approx(h1.distance) == 0.0f);
	REQUIRE(h2.hit == false);
	REQUIRE(h3.hit == false);
	REQUIRE(Approx(h3.distance) == 0.0f);


}

int main(int argc, char *argv[])
{ 
  return Catch::Session().run(argc, argv);
}*/

// main.cpp
#include "window.hpp"
#include "renderer.hpp"
#include "sdf-reader.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

int main(int argc, char* argv[]) {

	std::ofstream logFile("debug_log.txt");
	logFile << "Programm gestartet" << std::endl;

	unsigned const width = 800;
	unsigned const height = 600;
	std::string const filename = "./rendered_scene.ppm";

	// Fenster erstellen
	Window window({ width, height });

	// SDF-Datei lesen
	std::string sdf_file = "scene.sdf";
	auto [materials, cameras] = sdf_reader(sdf_file);

	if (cameras.empty()) {
		std::cerr << "Keine Kamera in der SDF-Datei gefunden." << std::endl;
		return 1;
	}

	// Erste Kamera aus der SDF-Datei verwenden
	auto& camera = cameras[0];

	// Renderer mit der Kamera initialisieren
	Renderer renderer(width, height, filename, *camera);

	// Render-Schleife
	while (!window.should_close()) {
		if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			window.close();
		}

		// Rendern
		renderer.render();

		// Bild anzeigen
		window.show(renderer.color_buffer());
	}


	return 0;
}