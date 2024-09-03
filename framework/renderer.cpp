// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#include "renderer.hpp"

#include "shape.hpp"
#include "camera.hpp"
#include "shape.cpp"
#include "ray.hpp"
#include "hitpoint.hpp"
#include "light.hpp"
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

/*Renderer::Renderer(unsigned w, unsigned h, std::string const& file)
  : width_(w)
  , height_(h)
  , color_buffer_(w*h, Color{0.0, 0.0, 0.0})
  , filename_(file)
  , ppm_(width_, height_)
{}*/


Renderer::Renderer(unsigned w, unsigned h, std::string const& file, Camera const& cam)
    : width_(w)
    , height_(h)
    , color_buffer_(w* h, Color{ 0.0, 0.0, 0.0 })
    , filename_(file)
    , ppm_(w, h)
    , camera_(cam)
{}

/*void Renderer::render()
{
  std::size_t const checker_pattern_size = 20;

  for (unsigned y = 0; y < height_; ++y) {
    for (unsigned x = 0; x < width_; ++x) {
      Pixel p(x,y);
      if ( ((x/checker_pattern_size)%2) != ((y/checker_pattern_size)%2)) {
        p.color = Color{0.0f, 1.0f, float(x)/height_};
      } else {
        p.color = Color{1.0f, 0.0f, float(y)/width_};
      }

      write(p);
    }
  }
  ppm_.save(filename_);
}

void Renderer::write(Pixel const& p)
{
  // flip pixels, because of opengl glDrawPixels
  size_t buf_pos = (width_*p.y + p.x);
  if (buf_pos >= color_buffer_.size() || (int)buf_pos < 0) {
    std::cerr << "Fatal Error Renderer::write(Pixel p) : "
      << "pixel out of ppm_ : "
      << (int)p.x << "," << (int)p.y
      << std::endl;
  } else {
    color_buffer_[buf_pos] = p.color;
  }

  ppm_.write(p);
}*/

//überarbeitet und erweiter für Aufgabe 7

void Renderer::render() {
    std::vector<std::shared_ptr<Shape>> shapes; // Placeholder, sollte aus der Szene geladen werden
    std::vector<std::shared_ptr<Light>> lights; // Placeholder, sollte aus der Szene geladen werden

    for (unsigned y = 0; y < height_; ++y) {
        for (unsigned x = 0; x < width_; ++x) {
            Ray ray = generate_primary_ray(x, y);
            Color pixel_color = trace_ray(ray, 5, shapes_, lights_); // Max depth 5
            Pixel p(x, y);
            p.color = pixel_color;
            write(p);
        }
    }
    ppm_.save(filename_);
}

/*Ray Renderer::generate_primary_ray(unsigned x, unsigned y) {
    // Erzeugt einen Primärstrahl basierend auf den Pixelkoordinaten
    // und der Kameraeinstellung
    //Ray ray;
    // Berechnung der Ray-Richtung basierend auf x und y
    //return ray;
    float aspect_ratio = static_cast<float>(width_) / static_cast<float>(height_);
    float fov_x_rad = glm::radians(camera_.fov_x);
    float fov_y_rad = 2.0f * std::atan(std::tan(fov_x_rad * 0.5f) / aspect_ratio);

    float pixel_x = (2.0f * ((x + 0.5f) / width_) - 1.0f) * std::tan(fov_x_rad * 0.5f);
    float pixel_y = (1.0f - 2.0f * ((y + 0.5f) / height_)) * std::tan(fov_y_rad * 0.5f);

    glm::vec3 ray_direction = glm::normalize(camera_.dir + pixel_x * glm::cross(camera_.dir, camera_.up) + pixel_y * camera_.up);

    return Ray{ camera_.eye, ray_direction };
}*/

Ray Renderer::generate_primary_ray(unsigned x, unsigned y) {
    float aspect_ratio = static_cast<float>(width_) / static_cast<float>(height_);
    float fov_x_rad = glm::radians(camera_.fov_x);
    float fov_y_rad = 2.0f * std::atan(std::tan(fov_x_rad * 0.5f) / aspect_ratio);

    float pixel_x = (2.0f * ((x + 0.5f) / width_) - 1.0f) * std::tan(fov_x_rad * 0.5f);
    float pixel_y = (1.0f - 2.0f * ((y + 0.5f) / height_)) * std::tan(fov_y_rad * 0.5f);

    glm::vec3 ray_direction = glm::normalize(camera_.dir + pixel_x * glm::cross(camera_.dir, camera_.up) + pixel_y * camera_.up);

    return Ray{ camera_.eye, ray_direction };
}

/*Color Renderer::trace_ray(Ray const& ray, int depth, std::vector<std::shared_ptr<Shape>> const& shapes, std::vector<std::shared_ptr<Light>> const& lights) {
    if (depth <= 0) {
        return Color{ 0.0f, 0.0f, 0.0f }; // Base case: Kein Beitrag zur Farbe
    }

    Hitpoint hitpoint = compute_intersection(ray, shapes);

    if (hitpoint.hit) {
        Color local_color = compute_color(hitpoint, lights); // Direkte Beleuchtung

        // Berechnung des reflektierten Strahls
        if (hitpoint.color_obj->ks != Color{ 0.0f, 0.0f, 0.0f }) {
            Ray reflected_ray;
            reflected_ray.origin = hitpoint.hit_p;
            reflected_ray.direction = glm::reflect(ray.direction, hitpoint.normal);

            Color reflection_color = trace_ray(reflected_ray, depth - 1, shapes, lights);
            local_color = blend_colors(local_color, reflection_color, 1.0f, hitpoint.color_obj->ks.r); // Gewichtung durch den Reflexionskoeffizienten
        }

        return local_color;
    }

    return Color{ 0.0f, 0.0f, 0.0f }; // Hintergrundfarbe
}*/

/*Color Renderer::trace_ray(Ray const& ray, int depth, std::vector<std::shared_ptr<Shape>> const& shapes, std::vector<std::shared_ptr<Light>> const& lights) {
    if (depth <= 0) {
        return Color{ 0.0f, 0.0f, 0.0f };
    }

    Hitpoint hitpoint = compute_intersection(ray, shapes);

    if (hitpoint.hit) {
        Color local_color = compute_color(hitpoint, lights);

        // Berechnung des reflektierten Strahls
        float reflectivity = (hitpoint.color_obj->ks.r + hitpoint.color_obj->ks.g + hitpoint.color_obj->ks.b) / 3.0f;
        if (reflectivity > 0.0f) {
            Ray reflected_ray;
            reflected_ray.origin = hitpoint.hit_p + hitpoint.normal * 0.001f; // Leichter Offset zur Vermeidung von Self-Intersection
            reflected_ray.direction = glm::reflect(ray.direction, hitpoint.normal);

            Color reflection_color = trace_ray(reflected_ray, depth - 1, shapes, lights);
            local_color = blend_colors(local_color, reflection_color, 1.0f - reflectivity, reflectivity);
        }

        return local_color;
    }

    return Color{ 0.0f, 0.0f, 0.0f }; // Hintergrundfarbe
}*/

/*Color Renderer::trace_ray(Ray const& ray, int depth, std::vector<std::shared_ptr<Shape>> const& shapes, std::vector<std::shared_ptr<Light>> const& lights) {
    if (depth <= 0) {
        return Color{ 0.0f, 0.0f, 0.0f };
    }

    Hitpoint hitpoint = compute_intersection(ray, shapes);

    if (hitpoint.hit) {
        Color local_color = compute_color(hitpoint, lights);

        // Berechnung des reflektierten Strahls
        float reflectivity = (hitpoint.color_obj->ks.r + hitpoint.color_obj->ks.g + hitpoint.color_obj->ks.b) / 3.0f;
        if (reflectivity > 0.01f) {
            Ray reflected_ray;
            reflected_ray.origin = hitpoint.hit_p + hitpoint.normal * 0.001f;
            reflected_ray.direction = glm::reflect(ray.direction, hitpoint.normal);

            Color reflection_color = trace_ray(reflected_ray, depth - 1, shapes, lights);
            local_color = blend_colors(local_color, reflection_color, 1.0f - reflectivity, reflectivity);
        }

        return local_color;
    }

    // Hintergrundfarbe (z.B. Himmel)
    float t = 0.5f * (glm::normalize(ray.direction).y + 1.0f);
    return Color{ (1.0f - t) * 1.0f + t * 0.5f, (1.0f - t) * 1.0f + t * 0.7f, (1.0f - t) * 1.0f + t * 1.0f };
}*/

Color Renderer::trace_ray(Ray const& ray, int depth, std::vector<std::shared_ptr<Shape>> const& shapes, std::vector<std::shared_ptr<Light>> const& lights) {
    if (depth <= 0) {
        return Color{ 0.0f, 0.0f, 0.0f };
    }

    Hitpoint hitpoint = compute_intersection(ray, shapes);

    if (hitpoint.hit) {
        Color local_color = compute_color(hitpoint, lights);

        // Reflection
        float reflectivity = (hitpoint.color_obj->ks.r + hitpoint.color_obj->ks.g + hitpoint.color_obj->ks.b) / 3.0f;
        if (reflectivity > 0.01f) {
            Ray reflected_ray;
            reflected_ray.origin = hitpoint.hit_p + hitpoint.normal * 0.001f;
            reflected_ray.direction = glm::reflect(ray.direction, hitpoint.normal);

            Color reflection_color = trace_ray(reflected_ray, depth - 1, shapes, lights);
            local_color = blend_colors(local_color, reflection_color, 1.0f - reflectivity, reflectivity);
        }

        // Refraction
        if (hitpoint.color_obj->refraction_index > 1.0f) {
            float eta = 1.0f / hitpoint.color_obj->refraction_index;
            glm::vec3 refraction_dir = glm::refract(ray.direction, hitpoint.normal, eta);

            if (glm::length(refraction_dir) > 0.0f) {
                Ray refracted_ray;
                refracted_ray.origin = hitpoint.hit_p - hitpoint.normal * 0.001f;
                refracted_ray.direction = refraction_dir;

                Color refraction_color = trace_ray(refracted_ray, depth - 1, shapes, lights);
                local_color = blend_colors(local_color, refraction_color, 0.5f, 0.5f); // Adjust blend weights as needed
            }
        }

        return local_color;
    }

    // Background color (e.g., sky)
    float t = 0.5f * (glm::normalize(ray.direction).y + 1.0f);
    return Color{ (1.0f - t) * 1.0f + t * 0.5f, (1.0f - t) * 1.0f + t * 0.7f, (1.0f - t) * 1.0f + t * 1.0f };
}

Hitpoint Renderer::compute_intersection(Ray const& ray, std::vector<std::shared_ptr<Shape>> const& shapes) {
    Hitpoint closest_hitpoint;
    closest_hitpoint.hit = false;
    closest_hitpoint.distance = std::numeric_limits<float>::max();

    for (auto const& shape : shapes) {
        Hitpoint hitpoint = shape->intersect(ray);
        if (hitpoint.hit && hitpoint.distance < closest_hitpoint.distance) {
            closest_hitpoint = hitpoint;
        }
    }

    return closest_hitpoint;
}

/*Color Renderer::compute_color(Hitpoint const& hitpoint, std::vector<std::shared_ptr<Light>> const& lights) {
    Color color = hitpoint.color_obj->ka; // Ambient color

    for (auto const& light : lights) {
        // Berechnung der Beleuchtungskomponenten wie Diffus und Spekular
        glm::vec3 light_dir = glm::normalize(light->position - hitpoint.hit_p);
        glm::vec3 view_dir = glm::normalize(-hitpoint.ray_dir);
        glm::vec3 reflect_dir = glm::reflect(-light_dir, hitpoint.normal);

        float diff = std::max(glm::dot(hitpoint.normal, light_dir), 0.0f);
        float spec = std::pow(std::max(glm::dot(view_dir, reflect_dir), 0.0f), hitpoint.color_obj->m);

        Color diffuse = hitpoint.color_obj->kd * light->color * diff;
        Color specular = hitpoint.color_obj->ks * light->color * spec;

        color += diffuse + specular;
    }

    return color;
}

Color Renderer::compute_color(Hitpoint const& hitpoint, std::vector<std::shared_ptr<Light>> const& lights) {
    Color color = hitpoint.color_obj->ka; // Ambient color

    for (auto const& light : lights) {
        glm::vec3 light_dir = glm::normalize(light->position - hitpoint.hit_p);
        float light_distance = glm::length(light->position - hitpoint.hit_p);

        // Schatten-Test
        Ray shadow_ray(hitpoint.hit_p + hitpoint.normal * 0.001f, light_dir);
        Hitpoint shadow_hit = compute_intersection(shadow_ray, shapes_);

        if (!shadow_hit.hit || shadow_hit.distance > light_distance) {
            glm::vec3 view_dir = glm::normalize(-hitpoint.ray_dir);
            glm::vec3 reflect_dir = glm::reflect(-light_dir, hitpoint.normal);

            float diff = std::max(glm::dot(hitpoint.normal, light_dir), 0.0f);
            float spec = std::pow(std::max(glm::dot(view_dir, reflect_dir), 0.0f), hitpoint.color_obj->m);

            Color diffuse = hitpoint.color_obj->kd * light->color * diff;
            Color specular = hitpoint.color_obj->ks * light->color * spec;

            color += diffuse + specular;
        }
    }

    return color;
}*/

Color Renderer::compute_color(Hitpoint const& hitpoint, std::vector<std::shared_ptr<Light>> const& lights) {
    Color color = hitpoint.color_obj->ka * Color{ 0.1f, 0.1f, 0.1f }; // Ambient light

    for (auto const& light : lights) {
        glm::vec3 light_dir = light->position - hitpoint.hit_p;
        float light_distance = glm::length(light_dir);
        light_dir = glm::normalize(light_dir);

        // Shadow test
        Ray shadow_ray(hitpoint.hit_p + hitpoint.normal * 0.001f, light_dir);
        Hitpoint shadow_hit = compute_intersection(shadow_ray, shapes_);

        if (!shadow_hit.hit || shadow_hit.distance > light_distance) {
            float diff = std::max(glm::dot(hitpoint.normal, light_dir), 0.0f);

            glm::vec3 view_dir = glm::normalize(-hitpoint.ray_dir);
            glm::vec3 reflect_dir = glm::reflect(-light_dir, hitpoint.normal);

            float spec = std::pow(std::max(glm::dot(view_dir, reflect_dir), 0.0f), hitpoint.color_obj->m);

            // Light attenuation
            float attenuation = 1.0f / (1.0f + 0.09f * light_distance + 0.032f * light_distance * light_distance);

            Color diffuse = hitpoint.color_obj->kd * light->color * diff * attenuation;
            Color specular = hitpoint.color_obj->ks * light->color * spec * attenuation;

            color += diffuse + specular;
        }
    }

    // Clamp color values to [0, 1]
    color.r = std::min(1.0f, std::max(0.0f, color.r));
    color.g = std::min(1.0f, std::max(0.0f, color.g));
    color.b = std::min(1.0f, std::max(0.0f, color.b));

    return color;
}

Color Renderer::blend_colors(Color c1, Color c2, float weight1, float weight2) {
    return Color(
        weight1 * c1.r + weight2 * c2.r,
        weight1 * c1.g + weight2 * c2.g,
        weight1 * c1.b + weight2 * c2.b
    );
}

void Renderer::write(Pixel const& p) {
    // flip pixels, because of opengl glDrawPixels
    size_t buf_pos = (width_ * p.y + p.x);
    if (buf_pos >= color_buffer_.size() || (int)buf_pos < 0) {
        std::cerr << "Fatal Error Renderer::write(Pixel p) : "
            << "pixel out of ppm_ : "
            << (int)p.x << "," << (int)p.y
            << std::endl;
    }
    else {
        color_buffer_[buf_pos] = p.color;
    }

    ppm_.write(p);
}