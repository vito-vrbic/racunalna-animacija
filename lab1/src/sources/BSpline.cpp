#include "BSpline.hpp"

namespace RA
{
    BSpline::BSpline(float step)
        : _ControlPolygon(1.5f, glm::vec4(1, 0, 0, 1)),
          _CurveApproximation(2.5f, glm::vec4(0, 1, 0, 1)),
          _Step(step)
    {
    }

    void BSpline::SetControlPoints(const std::vector<glm::vec3> &points)
    {
        _ControlPoints = points;
        _ControlPolygon.SetPoints(points);
        _BuildCurveApproximation();
    }

    std::vector<glm::vec3> BSpline::GetControlPoints()
    {
        return _ControlPoints;
    }

    void BSpline::_BuildCurveApproximation()
    {
        std::vector<glm::vec3> curve_points;

        // We need at least 4 points for cubic B-spline
        if (_ControlPoints.size() < 4)
            return;

        // For each segment of 4 control points
        for (size_t i = 0; i + 3 < _ControlPoints.size(); i++)
        {
            // Sample along the segment from t=0 to t=1
            for (float t = 0.0f; t <= 1.0f; t += _Step)
            {
                glm::vec3 pt = GetPoint(static_cast<int>(i), t);
                curve_points.push_back(pt);
            }
        }

        // Set only the curve points, do not touch control polygon
        _CurveApproximation.SetPoints(curve_points);
    }

    glm::vec3 BSpline::GetPoint(int i, float t) const
    {
        if (i + 3 >= (int)_ControlPoints.size())
            return glm::vec3(0);

        // Different order than in .pdf
        glm::mat4 M(
            -1, 3, -3, 1, // column 0
            3, -6, 0, 4,  // column 1
            -3, 3, 3, 1,  // column 2
            1, 0, 0, 0    // column 3
        );

        glm::vec4 T(t * t * t, t * t, t, 1.0f);

        // Compute coefficients
        glm::vec4 coeff = T * M;

        // Multiply coefficients by control points component-wise
        glm::vec3 point =
            coeff.x * _ControlPoints[i] +
            coeff.y * _ControlPoints[i + 1] +
            coeff.z * _ControlPoints[i + 2] +
            coeff.w * _ControlPoints[i + 3];

        return point / 6.0f;
    }

    glm::vec3 BSpline::GetTangent(int i, float t) const
    {
        if (i + 3 >= (int)_ControlPoints.size())
            return glm::vec3(0);

        // Same B-spline basis matrix as GetPoint()
        glm::mat4 M(
            -1, 3, -3, 1, // column 0
            3, -6, 0, 4,  // column 1
            -3, 3, 3, 1,  // column 2
            1, 0, 0, 0    // column 3
        );

        // Derivative of T: [3 t^2, 2 t, 1, 0]
        glm::vec4 Tprime(3 * t * t, 2 * t, 1.0f, 0.0f);

        // Compute derivative coefficients
        glm::vec4 coeff = Tprime * M;

        // Multiply coefficients by control points component-wise
        glm::vec3 tangent =
            coeff.x * _ControlPoints[i] +
            coeff.y * _ControlPoints[i + 1] +
            coeff.z * _ControlPoints[i + 2] +
            coeff.w * _ControlPoints[i + 3];

        return tangent / 2.0f;
    }

    glm::vec3 BSpline::GetNormal(int i, float t) const
    {
        if (i + 3 >= (int)_ControlPoints.size())
            return glm::vec3(0);

        // Same B-spline basis matrix as GetPoint()
        glm::mat4 M(
            -1, 3, -3, 1, // column 0
            3, -6, 0, 4,  // column 1
            -3, 3, 3, 1,  // column 2
            1, 0, 0, 0    // column 3
        );

        glm::vec3 tangent = GetTangent(i, t);

        // Second derivative coefficients [6t, 2, 0, 0]
        glm::vec4 TdoublePrime(6 * t, 2.0f, 0.0f, 0.0f);
        glm::vec4 coeff2 = TdoublePrime * M;

        glm::vec3 second_derivative =
            coeff2.x * _ControlPoints[i] +
            coeff2.y * _ControlPoints[i + 1] +
            coeff2.z * _ControlPoints[i + 2] +
            coeff2.w * _ControlPoints[i + 3];
        second_derivative /= 6.0f;

        // Normal = cross(p''(t), p'(t))
        glm::vec3 normal = glm::cross(tangent, second_derivative);

        // Default normal
        if (glm::length(normal) < 1e-6f)
            normal = glm::vec3(0, 1, 0);

        return glm::normalize(normal);
    }

    void BSpline::Render(std::shared_ptr<Shader> shader, glm::mat4 view, glm::mat4 proj)
    {
        _ControlPolygon.Render(shader, view, proj);
        _CurveApproximation.Render(shader, view, proj);
    }
}