#ifndef BUW_SHADING_HPP
#define BUW_SHADING_HPP

#include "ray.hpp"
#include "shape.hpp"
#include "light.hpp"
#include "hitpoint.hpp"
#include "color.hpp"
#include <vector>
#include <memory>

class Shading {
public:
    Hitpoint compute_intersection(Ray const& ray, std::vector<std::shared_ptr<Shape>> const& shapes);
    Color compute_lighting(Hitpoint const& hitpoint, Ray const& ray, std::vector<std::shared_ptr<Shape>> const& shapes, std::vector<std::shared_ptr<Light>> const& lights, int depth);

private:
    Color compute_diffuse(Hitpoint const& hitpoint, Light const& light);
    Color compute_specular(Hitpoint const& hitpoint, Light const& light, Ray const& ray);
    Color blend_colors(Color c1, Color c2, float weight1, float weight2);
};

#endif // BUW_SHADING_HPP
