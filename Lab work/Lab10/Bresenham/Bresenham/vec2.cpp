#include <iostream>
#include <cmath>
#include "vec2.h"

namespace egc {

	vec2& vec2::operator =(const vec2 &srcVector) {
		this->x = srcVector.x;
		this->y = srcVector.y;
		return *this;
	}

	vec2 vec2::operator +(const vec2& srcVector) const {
		vec2 result;
		result.x = this->x + srcVector.x;
		result.y = this->y + srcVector.y;
		return result;
	}

	vec2& vec2::operator +=(const vec2& srcVector) {
		this->x = this->x + srcVector.x;
		this->y = this->y + srcVector.y;
		return *this;
	}

	vec2 vec2::operator *(float scalarValue) const {
		vec2 result;
		result.x = this->x * scalarValue;
		result.y = this->y * scalarValue;
		return result;
	}

	vec2 vec2::operator -(const vec2& srcVector) const {
		vec2 result;
		result.x = this->x - srcVector.x;
		result.y = this->y - srcVector.y;
		return result;
	}

	vec2& vec2::operator -=(const vec2& srcVector) {
		this->x = this->x - srcVector.x;
		this->y = this->y - srcVector.y;
		return *this;
	}

	vec2& vec2::operator -() {
		this->x = -this->x;
		this->y = -this->y;
		return *this;
	}

	float vec2::length() const {
		float result = sqrt((this->x * this->x) + (this->y * this->y));
		return result;
	}

	vec2& vec2::normalize() {
		this->x = this->x / this->length();
		this->y = this->y / this->length();
		return *this;
	}
}