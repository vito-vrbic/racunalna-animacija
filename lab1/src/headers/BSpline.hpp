#pragma once

#include "Polyline.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace RA
{
    class BSpline
    {
    public:
        BSpline(float step = 0.05f);

        // Function to set the control points
        void SetControlPoints(const std::vector<glm::vec3> &points);

        // Function to get the control points
        std::vector<glm::vec3> GetControlPoints();

        // Function to render both the control polygon AND the curve
        void Render(std::shared_ptr<Shader> shader, glm::mat4 view, glm::mat4 proj);

        // Get a point on the curve; on segment i, with t between 0 and 1 (inclusive)
        glm::vec3 GetPoint(int i, float t) const;

        // Get a tangent at a point on the curve; on segment i, with t between 0 and 1 (inclusive)
        glm::vec3 GetTangent(int i, float t) const;

    private:
        Polyline _ControlPolygon;
        Polyline _CurveApproximation;

        std::vector<glm::vec3> _ControlPoints;
        float _Step;

        void _BuildCurveApproximation();
    };
}