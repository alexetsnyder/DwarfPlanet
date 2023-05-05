export module Camera;

import <cmath>;
import glmModule;

export enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

export class Camera
{
public:
	Camera(Vec3 position = Vec3(0.0f, 0.0f, 0.0f), Vec3 up = Vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
		: speed(SPEED), sensitivity(SENSITIVITY), _zoom(ZOOM)
	{
		cameraPos = position;
		worldUp = up;
		this->yaw = YAW;
		this->pitch = PITCH;
		calculateCameraVectors();
	}

	Mat4 getViewMatrix()
	{
		Vec3 eye = cameraPos;
		Vec3 center = cameraPos + cameraFront;
		Vec3 up = cameraUp;

		return lookAt(eye, center, up);
	}

	float getZoom()
	{
		return _zoom;
	}

	void move(CameraMovement direction, float deltaTime)
	{
		float velocity = speed * deltaTime;
		switch (direction)
		{
		case FORWARD:
			cameraPos += cameraFront * velocity;
			break;
		case BACKWARD:
			cameraPos -= cameraFront * velocity;
			break;
		case LEFT:
			cameraPos -= cameraRight * velocity;
			break;
		case RIGHT:
			cameraPos += cameraRight * velocity;
			break;
		}
	}

	void rotate(float xOffset, float yOffset)
	{
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		yaw += xOffset;
		pitch += yOffset;

		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}
		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}

		calculateCameraVectors();
	}

	void zoom(float yOffset)
	{
		_zoom -= yOffset;
		if (_zoom < 1.0f)
		{
			_zoom = 1.0f;
		}
		if (_zoom > 45.0f)
		{
			_zoom = 45.0f;
		}
	}

private:
	void calculateCameraVectors()
	{
		float x = cos(radians(yaw)) * cos(radians(pitch));
		float y = sin(radians(pitch));
		float z = sin(radians(yaw)) * cos(radians(pitch));
		cameraFront = Vec3(x, y, z).normalize();

		cameraRight = cameraFront.cross(worldUp).normalize();
		cameraUp = cameraRight.cross(cameraFront).normalize();
	}

	Vec3 cameraPos;
	Vec3 cameraFront;
	Vec3 cameraUp;

	Vec3 cameraRight;
	Vec3 worldUp;

	float yaw, pitch;

	float speed;
	float sensitivity;
	float _zoom;
};
