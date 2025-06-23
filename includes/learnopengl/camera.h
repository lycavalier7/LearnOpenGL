#pragma once

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr float PITCH = 0.0f;
constexpr float YAW = -90.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

enum class CameraMovement
{
	FORWARD = 0,
    BACK,
	LEFT,
	RIGHT
};

class Camera
{
public:
	Camera(glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f), float Yaw = YAW, float Pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
	{
		position = Position;
		worldUp = Up;
		yaw = Yaw;
		pitch = Pitch;
		UpdateCameraVectors();
	}

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw_ = YAW, float pitch_ = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f))
	{
		position = glm::vec3(posX, posY, posZ);
		worldUp = glm::vec3(upX, upY, upZ);
		yaw = yaw_;
		pitch = pitch_;

		UpdateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(position, front + position, up);
	}

	glm::vec3 GetPosition()
	{
		return position;
	}

	glm::vec3 GetFront()
	{
		return front;
	}

	void ProcessKeyBorad(CameraMovement moveDirection, float deltaTime)
	{
		float velocity = movementSpeed * deltaTime;

		if (moveDirection == CameraMovement::FORWARD)
		{
			position += front * velocity;
		}
		else if (moveDirection == CameraMovement::BACK)
		{
			position -= front * velocity;
		}
		else if (moveDirection == CameraMovement::LEFT)
		{
			position -= right * velocity;
		}
		else if (moveDirection == CameraMovement::RIGHT)
		{
			position += right * velocity;
		}

		position.y = 0.0f;
	}

	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true)
	{
		xOffset *= mouseSensitivity;
		yOffset *= mouseSensitivity;

		yaw += xOffset;
		pitch += yOffset;

		if (constrainPitch)
		{
			if (pitch > 89.0f)
			{
				pitch = 89.0f;
			}

			if (pitch < -89.0f)
			{
				pitch = -89.0f;
			}
		}

		UpdateCameraVectors();
	}

	void ProcessZoom(float yoffset)
	{
		zoom -= yoffset;

		if (zoom < 1.0f)
		{
			zoom = 1.0f;
		}

		if (zoom > 45.0f)
		{
			zoom = 45.0f;
		}
	}

	float GetZoom()
	{
		return zoom;
	}

private:
	glm::vec3 position;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 worldUp;

	float pitch;
	float yaw;

	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	void UpdateCameraVectors()
	{
		glm::vec3 Front;
		Front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		Front.y = sin(glm::radians(pitch));
		Front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

		front = glm::normalize(Front);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}
};