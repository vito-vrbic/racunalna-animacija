#pragma once

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
    };
}