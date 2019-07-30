#include <iostream>
#include <cmath>
#include "vec4.h"

namespace egc {

	vec4& vec4::operator =(const vec4 &srcVector) {
		this->x = srcVector.x;
		this->y = srcVector.y;
		this->z = srcVector.z;
		this->w = srcVector.w;
		return *this;
	}

	vec4 vec4::operator +(const vec4& srcVector) const {
		vec4 result;
		result.x = this->x + srcVector.x;
		result.y = this->y + srcVector.y;
		result.z = this->z + srcVector.z;
		result.w = this->w + srcVector.w;
		return result;
	}

	vec4& vec4::operator +=(const vec4& srcVector) {
		this->x = this->x + srcVector.x;
		this->y = this->y + srcVector.y;
		this->z = this->z + srcVector.z;
		this->w = this->w + srcVector.w;
		return *this;
	}

	vec4 vec4::operator *(float scalarValue) const {
		vec4 result;
		result.x = this->x * scalarValue;
		result.y = this->y * scalarValue;
		result.z = this->z * scalarValue;
		result.w = this->w * scalarValue;
		return result;
	}

	vec4 vec4::operator -(const vec4& srcVector) const {
		vec4 result;
		result.x = this->x - srcVector.x;
		result.y = this->y - srcVector.y;
		result.z = this->z - srcVector.z;
		result.w = this->w - srcVector.w;
		return result;
	}

	vec4& vec4::operator -=(const vec4& srcVector) {
		this->x = this->x - srcVector.x;
		this->y = this->y - srcVector.y;
		this->z = this->z - srcVector.z;
		this->w = this->w - srcVector.w;
		return *this;
	}

	vec4& vec4::operator -() {
		this->x = -this->x;
		this->y = -this->y;
		this->z = -this->z;
		this->w = -this->w;
		return *this;
	}

	float vec4::length() const {
		float result = sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z) + (this->w * this->w));
		return result;
	}

	vec4& vec4::normalize() {
		float length = this->length();
		this->x = this->x / length;
		this->y = this->y / length;
		this->z = this->z / length;
		this->w = this->w / length;
		return *this;
	}
}