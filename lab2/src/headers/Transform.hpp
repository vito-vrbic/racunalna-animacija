#pragma once

// External headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace RA
{
	class Transform
	{
	protected:
		glm::vec3 _position;
		glm::vec3 _front;
		glm::vec3 _up;
		glm::vec3 _right;
		glm::vec3 _scale;
		bool _dirty;

		glm::mat4 _modelMatrix;

		void _RecalculateModelMatrix();

	public:
		Transform();

		// Transform operations
		void Rotate(const glm::mat4 &rotation);
		void Rotate(float x_offset, float y_offset, bool constrain_pitch, float sensitivity);
		void MoveG(const glm::vec3 &delta);
		void MoveL(const glm::vec3 &delta);
		void SetPosition(const glm::vec3 &position);
		void SetOrientation(const glm::vec3 &front, const glm::vec3 &up);
		void SetOrientation(const glm::vec3 &front, const glm::vec3 &up, const glm::vec3 &right);
		void SetScale(const glm::vec3 &scale);

		glm::vec3 GetPosition() const { return _position; }
		glm::vec3 GetScale() const { return _scale; }
		glm::vec3 GetFront() const { return _front; }
		glm::vec3 GetUp() const { return _up; }
		glm::vec3 GetRight() const { return _right; }

		glm::mat4 GetModelMatrix();
	};
}