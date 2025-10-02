#pragma once

// Local Headers
// Standard Headers
#include <memory>
#include <vector>
// External Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace IRG
{
	/**
	 * @class Transform
	 * @brief Represents the position, rotation, and scale of an object in 3D space.
	 *
	 * This class manages transformations such as translation, rotation, and scaling.
	 */
	class Transform
	{
		// The position of the object in world space.
		glm::vec3 _position;

		// Rotation vectors defining the object's orientation (local coordinate axis).
		glm::vec3 _front;
		glm::vec3 _up;
		glm::vec3 _right;

		// The scale of the object.
		glm::vec3 _scale;

		// Has the transform been changed since the last time a matrix has been generated for it?
		bool _dirty;

	protected:
		void CleanTransform();
		bool GetDirty() const;

	public:
		Transform();

		/**
		 * @brief Rotates the object using a given rotation matrix.
		 *
		 * This modifies the object's orientation (internal coordinate system).
		 *
		 * @param rotation - a 4x4 matrix representing the rotation transformation.
		 */
		void Rotate(const glm::mat4 &rotation);

		/**
		 * @brief Rotates the object using a given mouse offset and in case the pitch is done.
		 *
		 * This modifies the object's orientation (internal coordinate system).
		 *
		 * @param x_offset - movement of the mouse in the x axis.
		 * @param y_offset - movement of the mouse in the y axis.
		 * @param constrain_pitch - should the pitch be contained or not.
		 * @param sensitivity - used to decide on the sensitivity of the rotation.
		 */
		void Rotate(float x_offset, float y_offset, bool constrain_pitch, float sensitivity);

		/**
		 * @brief Moves the object relative to world orientation.
		 *
		 * This function applies a translation to the object's position in global space.
		 *
		 * @param delta - a vector representing the movement in world space.
		 */
		void MoveG(const glm::vec3 &delta);

		/**
		 * @brief Moves the object relative to its local orientation.
		 *
		 * The movement is applied based on the object's local coordinate system.
		 *
		 * @param delta - a vector representing the movement in local space.
		 */
		void MoveL(const glm::vec3 &delta);

		/**
		 * @brief Sets the position of the transform.
		 *
		 * @param position - the vector to change the position to.
		 */
		void SetPosition(const glm::vec3 &position);

		/**
		 * @brief Sets the orientation of the object according to the provided vectors.
		 *
		 * The function calculates the right vector from the provided two.
		 * It recalculates the up vector in case it's not perpendicular to the front one.
		 * It also normalizes all the vectors.
		 *
		 * @param front - the vector representing the front local axis;
		 * @param up - the vector representing the up local axis.
		 */
		void SetOrientation(const glm::vec3 &front, const glm::vec3 &up);

		/**
		 * @brief Sets the scale of the transform.
		 *
		 * @param scale - the vector to change the position to.
		 */
		void SetScale(const glm::vec3 &scale);

		glm::vec3 GetPosition() const { return _position; }
		glm::vec3 GetScale() const { return _scale; }

		glm::vec3 GetFront() const { return _front; }
		glm::vec3 GetUp() const { return _up; }
		glm::vec3 GetRight() const { return _right; }
	};
}
