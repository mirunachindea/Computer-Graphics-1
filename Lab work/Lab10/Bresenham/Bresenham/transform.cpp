#include <iostream>
#include <cmath>
#include "transform.h"

namespace egc {

	mat3 egc::translate(const vec2 translateArray) {
		float t[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, translateArray.x, translateArray.y, 1.0f };
		mat3 result(t);
		return result;
	}

	mat3 translate(float tx, float ty) {
		float t[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, tx, ty, 1.0f };
		mat3 result(t);
		return result;
	}

	mat3 scale(const vec2 scaleArray) {
		float s[] = { scaleArray.x, 0.0f, 0.0f, 0.0f, scaleArray.y, 0.0f, 0.0f, 0.0f, 1.0f };
		mat3 result(s);
		return result;
	}

	mat3 scale(float sx, float sy) {
		float s[] = { sx, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 1.0f };
		mat3 result(s);
		return result;
	}

	mat3 rotate(float angle) {
		float rad = angle * PI / 180;
		float r[] = { cos(rad), sin(rad), 0.0f, -sin(rad), cos(rad), 0.0f, 0.0f, 0.0f, 1.0f };
		mat3 result(r);
		return result;
	}

	mat4 translate(const vec3 translateArray) {
		float t[] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, translateArray.x, translateArray.y, translateArray.z, 1.0f };
		mat4 result(t);
		return result;
	}

	mat4 translate(float tx, float ty, float tz) {
		float t[] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, tx, ty, tz, 1.0f };
		mat4 result(t);
		return result;
	}

	mat4 scale(const vec3 scaleArray) {
		float s[] = { scaleArray.x, 0.0f, 0.0f, 0.0f, 0.0f, scaleArray.y, 0.0f, 0.0f, 0.0f, 0.0f, scaleArray.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		mat4 result(s);
		return result;
	}

	mat4 scale(float sx, float sy, float sz) {
		float s[] = { sx, 0.0f, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 0.0f, sz, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		mat4 result(s);
		return result;
	}

	mat4 rotateX(float angle) {
		float rad = angle * PI / 180;
		float r[] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cos(rad), sin(rad), 0.0f, 0.0f, -sin(rad), cos(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		mat4 result(r);
		return result;
	}

	mat4 rotateY(float angle) {
		float rad = angle * PI / 180;
		float r[] = { cos(rad), 0.0f, -sin(rad), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, sin(rad), 0.0f, cos(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		mat4 result(r);
		return result;
	}

	mat4 rotateZ(float angle) {
		float rad = angle * PI / 180;
		float r[] = { cos(rad), sin(rad), 0.0f, 0.0f, -sin(rad), cos(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		mat4 result(r);
		return result;
	}










}