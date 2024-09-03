// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#ifndef BUW_RENDERER_HPP
#define BUW_RENDERER_HPP

#include "color.hpp"
#include "shape.hpp"
#include "light.hpp"
#include "pixel.hpp"
#include "ppmwriter.hpp"
#include "ray.hpp"
#include "hitpoint.hpp"
#include "camera.hpp"
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Renderer
{
public:
    Renderer(unsigned w, unsigned h, std::string const& file, Camera const& cam);

    void render();
    void write(Pixel const& p);

    inline std::vector<Color> const& color_buffer() const {
        return color_buffer_;
    }

    void add_shapes(const std::vector<std::shared_ptr<Shape>>& new_shapes) {
        shapes_.insert(shapes_.end(), new_shapes.begin(), new_shapes.end());
    }

    void add_lights(const std::vector<std::shared_ptr<Light>>& new_lights) {
        lights_.insert(lights_.end(), new_lights.begin(), new_lights.end());
    }


private:
    unsigned width_;
    unsigned height_;
    std::vector<Color> color_buffer_;
    std::string filename_;
    PpmWriter ppm_;
    Camera camera_;

    // Methoden für Raytracing hinzugefügt
    Ray generate_primary_ray(unsigned x, unsigned y);
    Color trace_ray(Ray const& ray, int depth, std::vector<std::shared_ptr<Shape>> const& shapes, std::vector<std::shared_ptr<Light>> const& lights);
    Hitpoint compute_intersection(Ray const& ray, std::vector<std::shared_ptr<Shape>> const& shapes);
    Color compute_color(Hitpoint const& hitpoint, std::vector<std::shared_ptr<Light>> const& lights);
    Color blend_colors(Color c1, Color c2, float weight1, float weight2);

    std::vector<std::shared_ptr<Shape>> shapes_;
    std::vector<std::shared_ptr<Light>> lights_;

};

#endif // #ifndef BUW_RENDERER_HPP
