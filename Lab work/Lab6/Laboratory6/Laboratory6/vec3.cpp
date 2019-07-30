#include <iostream>
#include <cmath>
#include "vec3.h"

namespace egc {

	vec3& vec3::operator =(const vec3 &srcVector) {
		this->x = srcVector.x;
		this->y = srcVector.y;
		this->z = srcVector.z;
		return *this;
	}

	vec3 vec3::operator +(const vec3& srcVector) const {
		vec3 result;
		result.x = this->x + srcVector.x;
		result.y = this->y + srcVector.y;
		result.z = this->z + srcVector.z;
		return result;
	}

	vec3& vec3::operator +=(const vec3& srcVector) {
		this->x = this->x + srcVector.x;
		this->y = this->y + srcVector.y;
		this->z = this->z + srcVector.z;
		return *this;
	}

	vec3 vec3::operator *(float scalarValue) const {
		vec3 result;
		result.x = this->x * scalarValue;
		result.y = this->y * scalarValue;
		result.z = this->z * scalarValue;
		return result;
	}

	vec3 vec3::operator -(const vec3& srcVector) const {
		vec3 result;
		result.x = this->x - srcVector.x;
		result.y = this->y - srcVector.y;
		result.z = this->z - srcVector.z;
		return result;
	}

	vec3& vec3::operator -=(const vec3& srcVector) {
		this->x = this->x - srcVector.x;
		this->y = this->y - srcVector.y;
		this->z= this->z - srcVector.z;
		return *this;
	}

	vec3& vec3::operator -() {
		this->x = -this->x;
		this->y = -this->y;
		this->z = -this->z;
		return *this;
	}

	float vec3::length() const {
		float result = sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
		return result;
	}

	vec3& vec3::normalize() {
		float length = this->length();
		this->x = this->x / length;
		this->y = this->y / length;
		this->z = this->z / length;
		return *this;
	}

	float dotProduct(const vec3& v1, const vec3& v2) {
		float result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
		return result;
	}

	vec3 crossProduct(const vec3& v1, const vec3& v2) {
		vec3 result;
		result.x = v1.y*v2.z - v1.z*v2.y;
		result.y = v1.z*v2.x - v1.x*v2.z;
		result.z = v1.x*v2.y - v1.y*v2.x;
		return result;
	}
}