#include "shade.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <algorithm>

Hitpoint Shading::compute_intersection(Ray const& ray, std::vector<std::shared_ptr<Shape>> const& shapes) {
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

Color Shading::compute_lighting(const Hitpoint& hitpoint, const Ray& ray,
    const std::vector<std::shared_ptr<Shape>>& shapes,
    const std::vector<std::shared_ptr<Light>>& lights,
    int depth)
{
    if (depth <= 0) {
        return Color(0.0f, 0.0f, 0.0f);
    }

    Color color = hitpoint.color_obj->ka * Color(0.1f, 0.1f, 0.1f); // Ambient light

    for (const auto& light : lights) {
        glm::vec3 light_dir = glm::normalize(light->position - hitpoint.hit_p);
        float light_distance = glm::length(light->position - hitpoint.hit_p);

        // Shadow test
        Ray shadow_ray(hitpoint.hit_p + hitpoint.normal * 0.001f, light_dir);
        Hitpoint shadow_hit = compute_intersection(shadow_ray, shapes);

        if (!shadow_hit.hit || shadow_hit.distance > light_distance) {
            // Diffuse and Specular Lighting
            color += compute_diffuse(hitpoint, *light);
            color += compute_specular(hitpoint, *light, ray);
        }
    }

    // Reflection
    float reflectivity = (hitpoint.color_obj->ks.r + hitpoint.color_obj->ks.g + hitpoint.color_obj->ks.b) / 3.0f;
    if (reflectivity > 0.01f) {
        Ray reflected_ray;
        reflected_ray.origin = hitpoint.hit_p + hitpoint.normal * 0.001f;
        reflected_ray.direction = glm::reflect(ray.direction, hitpoint.normal);

        Hitpoint reflection_hit = compute_intersection(reflected_ray, shapes);
        if (reflection_hit.hit) {
            Color reflection_color = compute_lighting(reflection_hit, reflected_ray, shapes, lights, depth - 1);
            color = color * (1.0f - reflectivity) + reflection_color * reflectivity;
        }
    }

    return color;
}

Color Shading::compute_diffuse(Hitpoint const& hitpoint, Light const& light) {
    glm::vec3 light_dir = glm::normalize(light.position - hitpoint.hit_p);
    float diff = std::max(glm::dot(hitpoint.normal, light_dir), 0.0f);
    return hitpoint.color_obj->kd * light.color * diff;
}

Color Shading::compute_specular(Hitpoint const& hitpoint, Light const& light, Ray const& ray) {
    glm::vec3 view_dir = glm::normalize(-ray.direction);
    glm::vec3 reflect_dir = glm::reflect(-light.position, hitpoint.normal);
    float spec = std::pow(std::max(glm::dot(view_dir, reflect_dir), 0.0f), hitpoint.color_obj->m);
    return hitpoint.color_obj->ks * light.color * spec;
}

Color Shading::blend_colors(Color c1, Color c2, float weight1, float weight2) {
    return Color(
        weight1 * c1.r + weight2 * c2.r,
        weight1 * c1.g + weight2 * c2.g,
        weight1 * c1.b + weight2 * c2.b
    );
}
