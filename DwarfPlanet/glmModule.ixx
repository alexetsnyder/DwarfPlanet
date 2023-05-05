export module glmModule;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

export class Vec3
{
	public:
		Vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f)
		{
			innerVec3 = glm::vec3(x, y, z);
		}

		Vec3(glm::vec3 vector)
		{
			innerVec3 = vector;
		}

		float x()
		{
			return innerVec3.x;
		}

		float y()
		{
			return innerVec3.y;
		}

		float z()
		{
			return innerVec3.z;
		}

		glm::vec3& getInnerVec3()
		{
			return innerVec3;
		}

		Vec3 cross(const Vec3& rhs)
		{
			glm::vec3 retVec3 = glm::cross(innerVec3, rhs.innerVec3);

			return Vec3(retVec3);
		}

		Vec3 normalize()
		{
			return Vec3(glm::normalize(innerVec3));
		}

		Vec3 operator*(const float s)
		{
			return Vec3(innerVec3 * s);
		}

		Vec3& operator+=(const Vec3& rhs)
		{
			innerVec3 += rhs.innerVec3;
			return *this;
		}

		Vec3& operator-=(const Vec3& rhs)
		{
			innerVec3 -= rhs.innerVec3;
			return *this;
		}

		Vec3 operator+(const Vec3& rhs) const
		{
			return Vec3(innerVec3 + rhs.innerVec3);
		}

	private:
		glm::vec3 innerVec3;
};

export class Vec4
{
public:
	Vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f)
	{
		innerVec4 = glm::vec4(x, y, z, w);
	}

	Vec4(glm::vec4 vector)
	{
		innerVec4 = vector;
	}

	glm::vec4& getInnerVec4()
	{
		return innerVec4;
	}

	Vec4 normalize()
	{
		return Vec4(glm::normalize(innerVec4));
	}

	Vec4 operator*(const float s)
	{
		return Vec4(innerVec4 * s);
	}

	Vec4& operator+=(const Vec4& rhs)
	{
		innerVec4 += rhs.innerVec4;
		return *this;
	}

	Vec4& operator-=(const Vec4& rhs)
	{
		innerVec4 -= rhs.innerVec4;
		return *this;
	}

	Vec4 operator+(const Vec4& rhs) const
	{
		return Vec4(innerVec4 + rhs.innerVec4);
	}

private:
	glm::vec4 innerVec4;
};


export class Mat4
{
	public:
		Mat4(float d = 1.0f)
		{
			innerMat4 = glm::mat4(d);
		}

		Mat4(glm::mat4 matrix)
		{
			innerMat4 = matrix;
		}

		glm::mat4& getInnerMat4()
		{
			return innerMat4;
		}

		Mat4 translate(Vec3& v)
		{
			glm::mat4 retMat4(innerMat4);
			retMat4 = glm::translate(retMat4, v.getInnerVec3());

			return Mat4(retMat4);
		}

		Mat4 rotate(float angle, Vec3& v)
		{
			glm::mat4 retMat4(innerMat4);
			retMat4 = glm::rotate(retMat4, angle, v.getInnerVec3());

			return Mat4(retMat4);
		}

		Mat4 scale(Vec3& v)
		{
			glm::mat4 retMat4(innerMat4);
			retMat4 = glm::scale(retMat4, v.getInnerVec3());

			return Mat4(retMat4);
		}

	private:
		glm::mat4 innerMat4;
};

export float radians(float f)
{
	return glm::radians(f);
}

export Mat4 lookAt(Vec3& eye, Vec3& center, Vec3& up)
{
	glm::mat4 retMat4 = glm::lookAt(eye.getInnerVec3(), center.getInnerVec3(), up.getInnerVec3());
	return Mat4(retMat4);
}

export Mat4 perspective(float fov, float aspect, float near, float far)
{
	glm::mat4 retMat4 = glm::perspective(fov, aspect, near, far);
	return Mat4(retMat4);
}

export const float* value_ptr(Vec3& v)
{
	return glm::value_ptr(v.getInnerVec3());
}

export const float* value_ptr(Vec4& v)
{
	return glm::value_ptr(v.getInnerVec4());
}

export const float* value_ptr(Mat4& m)
{
	return glm::value_ptr(m.getInnerMat4());
}