#pragma once

// Local Headers
#include "Polyline.hpp"

// Standard Headers
#include <iostream>
#include <memory>
#include <vector>

// External Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace RA
{
    class BSpline
    {
    public:
        /// @brief Constructor
        /// @param step Sampling step for curve approximation
        BSpline(float step = 0.05f);

        /// @brief Set control points of the B-spline
        /// @param points Vector of 3D points
        void SetControlPoints(const std::vector<glm::vec3> &points);

        /// @brief Get control points of the B-spline
        /// @return Vector of 3D control points
        std::vector<glm::vec3> GetControlPoints();

        /// @brief Render both the control polygon and the curve
        /// @param shader Shader used for rendering
        /// @param view View matrix
        /// @param proj Projection matrix
        void Render(std::shared_ptr<Shader> shader, glm::mat4 view, glm::mat4 proj);

        /// @brief Compute a point on the curve
        /// @param i Segment index
        /// @param t Parameter along the segment [0,1]
        /// @return 3D point on the curve
        glm::vec3 GetPoint(int i, float t) const;

        /// @brief Compute the tangent vector at a point on the curve
        /// @param i Segment index
        /// @param t Parameter along the segment [0,1]
        /// @return Tangent vector (normalized)
        glm::vec3 GetTangent(int i, float t) const;

        /// @brief Compute the normal vector at a point on the curve (Frenet frame)
        /// @param i Segment index
        /// @param t Parameter along the segment [0,1]
        /// @return Normal vector (normalized)
        glm::vec3 GetNormal(int i, float t) const;

    private:
        Polyline _ControlPolygon;              ///< Visual representation of the control polygon
        Polyline _CurveApproximation;          ///< Visual representation of the curve (sampled points)
        std::vector<glm::vec3> _ControlPoints; ///< List of control points
        float _Step;                           ///< Sampling step for curve approximation

        /// @brief Recalculate the sampled points for rendering the curve
        void _BuildCurveApproximation();
    };
}