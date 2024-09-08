#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "shade.hpp"
#include "Material.hpp"
#include "Hitpoint.hpp"

#include "Ray.hpp"
#include "Shape.hpp"
#include <memory>

#include <algorithm>
#include <iostream>

//const float EPSILON = 0.0001f;

Hitpoint Shading::compute_intersection(Ray const& ray, const std::shared_ptr<Shape>& root_shape) const {
    return root_shape->intersect(ray);
}

//Hitpoint Shading::compute_intersection(Ray const& ray, const std::shared_ptr<Shape>& root_shape) const {
//    //std::cout << "Computing intersection for ray: origin = (" << ray.origin.x << ", " << ray.origin.y << ", " << ray.origin.z
//    //    << "), direction = (" << ray.direction.x << ", " << ray.direction.y << ", " << ray.direction.z << ")" << std::endl;
//
//    Hitpoint hitpoint = root_shape->intersect(ray);
//
//    if (hitpoint.hit) {
//        //std::cout << "Intersection found at point (" << hitpoint.hit_p.x << ", " << hitpoint.hit_p.y << ", " << hitpoint.hit_p.z
//        //    << "), distance = " << hitpoint.distance << std::endl;
//    }
//    else {
//        //std::cout << "No intersection found" << std::endl;
//    }
//
//    return hitpoint;
//}



//Hitpoint Shading::compute_intersection(Ray const& ray, const std::shared_ptr<Shape>& root_shape) const {
//    /*Hitpoint closest_hitpoint;
//    closest_hitpoint.hit = false;
//    closest_hitpoint.distance = std::numeric_limits<float>::max();*/
//
//    //for (auto const& shape : root_shape) {
//    //    Hitpoint hitpoint = shape->intersect(ray);
//    //    if (hitpoint.hit && hitpoint.distance < closest_hitpoint.distance) {
//    //        closest_hitpoint = hitpoint;
//    //        // Früher Abbruch, wenn ein sehr naher Schnittpunkt gefunden wurde
//    //        if (closest_hitpoint.distance < EPSILON) break;
//    //    }
//    //}
//
//    //return closest_hitpoint;
//    return root_shape->intersect(ray);
//}

Color Shading::compute_lighting(const Hitpoint& hitpoint, const Ray& ray,   
    const std::shared_ptr<Shape>& root_shape,
    const std::vector<std::shared_ptr<Light>>& lights,
    int depth) const
{

    //std::cout << "Computing lighting for " << hitpoint.name_obj << std::endl;
    //std::cout << "Material properties: ka = (" << hitpoint.color_obj->ka.r << ", " << hitpoint.color_obj->ka.g << ", " << hitpoint.color_obj->ka.b << ")"
    //    << ", kd = (" << hitpoint.color_obj->kd.r << ", " << hitpoint.color_obj->kd.g << ", " << hitpoint.color_obj->kd.b << ")"
    //    << ", ks = (" << hitpoint.color_obj->ks.r << ", " << hitpoint.color_obj->ks.g << ", " << hitpoint.color_obj->ks.b << ")"
    //    << ", m = " << hitpoint.color_obj->m << std::endl;

    const float EPSILON = 0.001f;
    if (depth <= 0) return Color(0.0f, 0.0f, 0.0f);

    if (!hitpoint.color_obj) {
        return Color(0.0f, 0.0f, 0.0f);  // oder eine Fehlerfarbe
    }

    Color color(0.0f, 0.0f, 0.0f);

    // Ambient component
    Color ambient = hitpoint.color_obj->ka * Color(0.1f, 0.1f, 0.1f);
    color += ambient;

    // Emission component
    color += hitpoint.color_obj->emission;

    // Diffuse and Specular components
    for (const auto& light : lights) {
        glm::vec3 light_dir = glm::normalize(light->position - hitpoint.hit_p);
        float light_distance = glm::length(light->position - hitpoint.hit_p);

        // Shadow check
        Ray shadow_ray(hitpoint.hit_p + hitpoint.normal * EPSILON, light_dir);
        Hitpoint shadow_hit = compute_intersection(shadow_ray, root_shape);

        float shadow_factor = 1.0f;
        if (shadow_hit.hit && shadow_hit.distance < light_distance) {
            shadow_factor = 0.1f;  // Nicht komplett schwarz, sondern leicht beleuchtet
        }

        // Diffuse component
        float diff = std::max(glm::dot(hitpoint.normal, light_dir), 0.0f);
        Color diffuse = shadow_factor * hitpoint.color_obj->kd * light->color * diff;

        // Specular component
        glm::vec3 view_dir = glm::normalize(-ray.direction);
        glm::vec3 reflect_dir = glm::reflect(-light_dir, hitpoint.normal);
        float spec = std::pow(std::max(glm::dot(view_dir, reflect_dir), 0.0f), hitpoint.color_obj->m);
        Color specular = shadow_factor * hitpoint.color_obj->ks * light->color * spec;

        color += diffuse + specular;
    }

    // Reflection
    if (glm::length(glm::vec3(hitpoint.color_obj->ks.r, hitpoint.color_obj->ks.g, hitpoint.color_obj->ks.b)) > 0.0f) {
        Ray reflect_ray(hitpoint.hit_p + hitpoint.normal * EPSILON, hitpoint.reflection_dir);
        Hitpoint reflect_hit = compute_intersection(reflect_ray, root_shape);
        if (reflect_hit.hit) {
            Color reflect_color = compute_lighting(reflect_hit, reflect_ray, root_shape, lights, depth - 1);
            color += hitpoint.color_obj->ks * reflect_color;
        }
    }

    if (hitpoint.color_obj->transparency > 0.0f && depth > 0) {
        float n1 = 1.0f;  // Brechungsindex von Luft
        float n2 = hitpoint.color_obj->refraction_index;

        glm::vec3 normal = hitpoint.normal;
        float cos_i = glm::dot(-ray.direction, normal);
        bool entering = cos_i > 0;

        if (!entering) {
            std::swap(n1, n2);
            normal = -normal;
            cos_i = -cos_i;
        }

        float n = n1 / n2;
        float sin_t2 = n * n * (1.0f - cos_i * cos_i);

        Color refracted_color;
        Color reflected_color;

        if (sin_t2 <= 1.0f) {
            float cos_t = std::sqrt(1.0f - sin_t2);
            glm::vec3 refracted_dir = glm::normalize(n * ray.direction + (n * cos_i - cos_t) * normal);

            Ray refracted_ray(hitpoint.hit_p - normal * EPSILON, refracted_dir);
            Hitpoint refracted_hit = compute_intersection(refracted_ray, root_shape);
            if (refracted_hit.hit) {
                refracted_color = compute_lighting(refracted_hit, refracted_ray, root_shape, lights, depth - 1);

                // Apply Beer's law
                float distance = glm::length(refracted_hit.hit_p - hitpoint.hit_p);
                Color absorption = Color(
                    std::exp(-hitpoint.color_obj->kd.r * distance * 0.15f),
                    std::exp(-hitpoint.color_obj->kd.g * distance * 0.15f),
                    std::exp(-hitpoint.color_obj->kd.b * distance * 0.15f)
                );
                refracted_color = refracted_color * absorption;
            }
            else {
                Color background_color(0.6f, 0.8f, 1.0f);
                refracted_color = background_color * 1.2f;
            }
        }

        // Berechne immer die Reflexionsfarbe
        glm::vec3 reflected_dir = glm::normalize(glm::reflect(ray.direction, normal));
        Ray reflected_ray(hitpoint.hit_p + normal * EPSILON, reflected_dir);
        Hitpoint reflected_hit = compute_intersection(reflected_ray, root_shape);
        if (reflected_hit.hit) {
            reflected_color = compute_lighting(reflected_hit, reflected_ray, root_shape, lights, depth - 1);
        }
        else {
            Color background_color(0.6f, 0.8f, 1.0f);
            reflected_color = background_color;
        }

        // Fresnel-Gleichungen für realistischere Reflexion/Brechung
        float r0 = ((n1 - n2) / (n1 + n2)) * ((n1 - n2) / (n1 + n2));
        float fresnel = r0 + (1 - r0) * std::pow(1 - cos_i, 5);

        // Mische Reflexion und Brechung basierend auf Fresnel
        Color mixed_color = Shading::blend_colors(reflected_color, refracted_color, fresnel, 1.0f - fresnel);

        // Füge einen Teil der lokalen Beleuchtung hinzu
        color = Shading::blend_colors(mixed_color, color, 0.8f, 0.2f);
    }

    //std::cout << "Final color: (" << color.r << ", " << color.g << ", " << color.b << ")" << std::endl;

    return color;
}

// In shade.cpp

//Color Shading::compute_lighting(const Hitpoint& hitpoint, const Ray& ray,
//    const std::vector<std::shared_ptr<Shape>>& shapes,
//    const std::vector<std::shared_ptr<Light>>& lights,
//    int depth)
//{
//    const float EPSILON = 0.0001f;
//    if (depth <= 0) return Color(0.0f, 0.0f, 0.0f);
//
//    if (!hitpoint.color_obj) {
//        return Color(0.0f, 0.0f, 0.0f);
//    }
//
//    Color local_color = hitpoint.color_obj->ka * Color(0.1f, 0.1f, 0.1f);
//
//    // Local illumination
//    for (const auto& light : lights) {
//        glm::vec3 light_dir = glm::normalize(light->position - hitpoint.hit_p);
//        float light_distance = glm::length(light->position - hitpoint.hit_p);
//
//        Ray shadow_ray(hitpoint.hit_p + hitpoint.normal * EPSILON, light_dir);
//        Hitpoint shadow_hit = compute_intersection(shadow_ray, shapes);
//
//        if (!shadow_hit.hit || shadow_hit.distance > light_distance) {
//            float diff = std::max(glm::dot(hitpoint.normal, light_dir), 0.0f);
//            Color diffuse = hitpoint.color_obj->kd * light->color * diff;
//
//            glm::vec3 view_dir = glm::normalize(-ray.direction);
//            glm::vec3 half_dir = glm::normalize(light_dir + view_dir);
//            float spec = std::pow(std::max(glm::dot(hitpoint.normal, half_dir), 0.0f), hitpoint.color_obj->m);
//            Color specular = hitpoint.color_obj->ks * light->color * spec;
//
//            local_color += diffuse + specular;
//        }
//    }
//
//    Color reflected_color(0.0f);
//    Color refracted_color(0.0f);
//
//    if (hitpoint.color_obj->transparency > 0.0f && depth > 0) {
//        float n1 = 1.0f;
//        float n2 = hitpoint.color_obj->refraction_index;
//        glm::vec3 normal = hitpoint.normal;
//        float cos_i = glm::dot(-ray.direction, normal);
//        bool entering = cos_i > 0;
//
//        if (!entering) {
//            std::swap(n1, n2);
//            normal = -normal;
//            cos_i = -cos_i;
//        }
//
//        float n = n1 / n2;
//        float sin_t2 = n * n * (1.0f - cos_i * cos_i);
//
//        // Compute Fresnel terms
//        float cos_t = std::sqrt(1.0f - sin_t2);
//        float r_parallel = ((n2 * cos_i) - (n1 * cos_t)) / ((n2 * cos_i) + (n1 * cos_t));
//        float r_perpendicular = ((n1 * cos_i) - (n2 * cos_t)) / ((n1 * cos_i) + (n2 * cos_t));
//        float fresnel = (r_parallel * r_parallel + r_perpendicular * r_perpendicular) / 2.0f;
//
//        // Reflection
//        glm::vec3 reflect_dir = glm::reflect(ray.direction, normal);
//        Ray reflect_ray(hitpoint.hit_p + normal * EPSILON, reflect_dir);
//        Hitpoint reflect_hit = compute_intersection(reflect_ray, shapes);
//        if (reflect_hit.hit) {
//            reflected_color = compute_lighting(reflect_hit, reflect_ray, shapes, lights, depth - 1);
//        }
//
//        // Refraction
//        if (sin_t2 <= 1.0f) {
//            glm::vec3 refract_dir = glm::refract(ray.direction, normal, n1 / n2);
//            Ray refract_ray(hitpoint.hit_p - normal * EPSILON, refract_dir);
//            Hitpoint refract_hit = compute_intersection(refract_ray, shapes);
//            if (refract_hit.hit) {
//                refracted_color = compute_lighting(refract_hit, refract_ray, shapes, lights, depth - 1);
//
//                // Apply Beer's law for absorption
//                float distance = glm::length(refract_hit.hit_p - hitpoint.hit_p);
//                Color absorption = Color(std::exp(-hitpoint.color_obj->kd.r * distance*0.1f),
//                    std::exp(-hitpoint.color_obj->kd.g * distance * 0.1f),
//                    std::exp(-hitpoint.color_obj->kd.b * distance * 0.1f));
//                refracted_color = refracted_color * absorption;
//            }
//        }
//
//        // Blend reflected and refracted colors
//        Color final_color = fresnel * reflected_color + (1 - fresnel) * refracted_color;
//        final_color += hitpoint.color_obj->emission;
//
//        // Mix with local color
//        return blend_colors(final_color, local_color, hitpoint.color_obj->transparency, 1 - hitpoint.color_obj->transparency);
//    }
//    else {
//        // For non-transparent materials, add reflection
//        float reflectivity = (hitpoint.color_obj->ks.r + hitpoint.color_obj->ks.g + hitpoint.color_obj->ks.b) / 3.0f;
//        if (reflectivity > 0.0f && depth > 0) {
//            glm::vec3 reflect_dir = glm::reflect(ray.direction, hitpoint.normal);
//            Ray reflect_ray(hitpoint.hit_p + hitpoint.normal * EPSILON, reflect_dir);
//            Hitpoint reflect_hit = compute_intersection(reflect_ray, shapes);
//            if (reflect_hit.hit) {
//                reflected_color = compute_lighting(reflect_hit, reflect_ray, shapes, lights, depth - 1);
//                local_color = blend_colors(local_color, reflected_color, 1 - reflectivity, reflectivity);
//            }
//        }
//    }
//
//    return local_color;
//}

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
    const float MIN_COLOR_VALUE = 1e-6f;
    float total_weight = weight1 + weight2;
    float normalized_weight1 = weight1 / total_weight;
    float normalized_weight2 = weight2 / total_weight;
    return Color(
        std::clamp(normalized_weight1 * c1.r + normalized_weight2 * c2.r, MIN_COLOR_VALUE, 1.0f),
        std::clamp(normalized_weight1 * c1.g + normalized_weight2 * c2.g, MIN_COLOR_VALUE, 1.0f),
        std::clamp(normalized_weight1 * c1.b + normalized_weight2 * c2.b, MIN_COLOR_VALUE, 1.0f)
    );
}