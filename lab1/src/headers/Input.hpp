#pragma once

// Local headers
#include "Transform.hpp"
#include "Mesh.hpp"
#include "BSpline.hpp"

namespace RA
{
    class Input
    {
    public:
        // Camera movement
        static void ProcessCameraInput(GLFWwindow *window, float deltaTime, RA::Transform &camera);

        // Follow a B-spline
        static void ProcessBSplineFollow(GLFWwindow *window, float delta_time, std::shared_ptr<Mesh> mesh, std::shared_ptr<Polyline> front_vec, std::shared_ptr<BSpline> spline);

        // Helper function
    private:
        static void _ComputeRotationFromTo(const glm::vec3 &s, const glm::vec3 &e, glm::vec3 &out_axis, float &out_angle);
        static void _PrintAxisDifference(const glm::vec3 &current_front, float delta_time);
    };
}