#include <algorithm>
#include <iostream>
#include <cmath>
#include "mat4.h"

namespace egc {
	mat4& mat4::operator =(const mat4& srcMatrix) {
		for (int i = 0; i < 16; i++) {
			matrixData[i] = srcMatrix.matrixData[i];
		}
		return *this;
	}

	mat4 mat4::operator *(float scalarValue) const {
		mat4 result;
		for (int i = 0; i < 16; i++) {
			result.matrixData[i] = matrixData[i] * scalarValue;
		}
		return result;
	}

	float& mat4::at(int i, int j) {
		return matrixData[i + 4 * j];
	}

	const float& mat4::at(int i, int j) const {
		return matrixData[i + 4 * j];
	}

	mat4 mat4::operator +(const mat4& srcMatrix) const {
		mat4 result;
		for (int i = 0; i < 16; i++) {
			result.matrixData[i] = this->matrixData[i] + srcMatrix.matrixData[i];
		}
		return result;
	}

	vec4 mat4::operator *(const vec4& srcVector) const {
		vec4 result;
		result.x = this->at(0, 0) * srcVector.x + this->at(0, 1) * srcVector.y + this->at(0, 2) * srcVector.z + this->at(0, 3) * srcVector.w;
		result.y = this->at(1, 0) * srcVector.x + this->at(1, 1) * srcVector.y + this->at(1, 2) * srcVector.z + this->at(1, 3) * srcVector.w;
		result.z = this->at(2, 0) * srcVector.x + this->at(2, 1) * srcVector.y + this->at(2, 2) * srcVector.z + this->at(2, 3) * srcVector.w;
		result.w = this->at(3, 0) * srcVector.x + this->at(3, 1) * srcVector.y + this->at(3, 2) * srcVector.z + this->at(3, 3) * srcVector.w;
		return result;
	}

	mat4 mat4::operator *(const mat4& srcMatrix) const {
		mat4 result;
		float sum;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				sum = 0;
				for (int k = 0; k < 4; k++) {
					sum += matrixData[i + k * 4] * srcMatrix.matrixData[k + j * 4];
				}
				result.matrixData[i + j * 4] = sum;
			}
		}
		return result;
	}

	float mat4::determinant() const {
		return
			at(0, 3) * at(1, 2) * at(2, 1) * at(3, 0) - at(0, 2) * at(1, 3) * at(2, 1) * at(3, 0) -
			at(0, 3) * at(1, 1) * at(2, 2) * at(3, 0) + at(0, 1) * at(1, 3) * at(2, 2) * at(3, 0) +
			at(0, 2) * at(1, 1) * at(2, 3) * at(3, 0) - at(0, 1) * at(1, 2) * at(2, 3) * at(3, 0) -
			at(0, 3) * at(1, 2) * at(2, 0) * at(3, 1) + at(0, 2) * at(1, 3) * at(2, 0) * at(3, 1) +
			at(0, 3) * at(1, 0) * at(2, 2) * at(3, 1) - at(0, 0) * at(1, 3) * at(2, 2) * at(3, 1) -
			at(0, 2) * at(1, 0) * at(2, 3) * at(3, 1) + at(0, 0) * at(1, 2) * at(2, 3) * at(3, 1) +
			at(0, 3) * at(1, 1) * at(2, 0) * at(3, 2) - at(0, 1) * at(1, 3) * at(2, 0) * at(3, 2) -
			at(0, 3) * at(1, 0) * at(2, 1) * at(3, 2) + at(0, 0) * at(1, 3) * at(2, 1) * at(3, 2) +
			at(0, 1) * at(1, 0) * at(2, 3) * at(3, 2) - at(0, 0) * at(1, 1) * at(2, 3) * at(3, 2) -
			at(0, 2) * at(1, 1) * at(2, 0) * at(3, 3) + at(0, 1) * at(1, 2) * at(2, 0) * at(3, 3) +
			at(0, 2) * at(1, 0) * at(2, 1) * at(3, 3) - at(0, 0) * at(1, 2) * at(2, 1) * at(3, 3) -
			at(0, 1) * at(1, 0) * at(2, 2) * at(3, 3) + at(0, 0) * at(1, 1) * at(2, 2) * at(3, 3);
	}

	mat4 mat4::transpose() const {
		mat4 result;
		result.matrixData[0] = matrixData[0];
		result.matrixData[1] = matrixData[4];
		result.matrixData[2] = matrixData[8];
		result.matrixData[3] = matrixData[12];
		result.matrixData[4] = matrixData[1];
		result.matrixData[5] = matrixData[5];
		result.matrixData[6] = matrixData[9];
		result.matrixData[7] = matrixData[13];
		result.matrixData[8] = matrixData[2];
		result.matrixData[9] = matrixData[6];
		result.matrixData[10] = matrixData[10];
		result.matrixData[11] = matrixData[14];
		result.matrixData[12] = matrixData[3];
		result.matrixData[13] = matrixData[7];
		result.matrixData[14] = matrixData[11];
		result.matrixData[15] = matrixData[15];

		return result;
	}

	mat4 mat4::inverse() const {
		mat4 result;
		mat4 t = transpose();
		float det = this->determinant();
		result.matrixData[0] = t.matrixData[5] * t.matrixData[10] * matrixData[15] - matrixData[5] * matrixData[11] * matrixData[14] - matrixData[9] * matrixData[6] * matrixData[15] + matrixData[9] * matrixData[7] * matrixData[14] + matrixData[13] * matrixData[6] * matrixData[11] - matrixData[13] * matrixData[7] * matrixData[10];
		result.matrixData[4] = -matrixData[4] * matrixData[10] * matrixData[15] + matrixData[4] * matrixData[11] * matrixData[14] + matrixData[8] * matrixData[6] * matrixData[15] - matrixData[8] * matrixData[7] * matrixData[14] - matrixData[12] * matrixData[6] * matrixData[11] + matrixData[12] * matrixData[7] * matrixData[10];
		result.matrixData[8] = matrixData[4] * matrixData[9] * matrixData[15] - matrixData[4] * matrixData[11] * matrixData[13] - matrixData[8] * matrixData[5] * matrixData[15] + matrixData[8] * matrixData[7] * matrixData[13] + matrixData[12] * matrixData[5] * matrixData[11] - matrixData[12] * matrixData[7] * matrixData[9];
		result.matrixData[12] = -matrixData[4] * matrixData[9] * matrixData[14] + matrixData[4] * matrixData[10] * matrixData[13] + matrixData[8] * matrixData[5] * matrixData[14] - matrixData[8] * matrixData[6] * matrixData[13] - matrixData[12] * matrixData[5] * matrixData[10] + matrixData[12] * matrixData[6] * matrixData[9];
		result.matrixData[1] = -matrixData[1] * matrixData[10] * matrixData[15] + matrixData[1] * matrixData[11] * matrixData[14] + matrixData[9] * matrixData[2] * matrixData[15] - matrixData[9] * matrixData[3] * matrixData[14] - matrixData[13] * matrixData[2] * matrixData[11] + matrixData[13] * matrixData[3] * matrixData[10];
		result.matrixData[5] = matrixData[0] * matrixData[10] * matrixData[15] - matrixData[0] * matrixData[11] * matrixData[14] - matrixData[8] * matrixData[2] * matrixData[15] + matrixData[8] * matrixData[3] * matrixData[14] + matrixData[12] * matrixData[2] * matrixData[11] - matrixData[12] * matrixData[3] * matrixData[10];
		result.matrixData[9] = -matrixData[0] * matrixData[9] * matrixData[15] + matrixData[0] * matrixData[11] * matrixData[13] + matrixData[8] * matrixData[1] * matrixData[15] - matrixData[8] * matrixData[3] * matrixData[13] - matrixData[12] * matrixData[1] * matrixData[11] + matrixData[12] * matrixData[3] * matrixData[9];
		result.matrixData[13] = matrixData[0] * matrixData[9] * matrixData[14] - matrixData[0] * matrixData[10] * matrixData[13] - matrixData[8] * matrixData[1] * matrixData[14] + matrixData[8] * matrixData[2] * matrixData[13] + matrixData[12] * matrixData[1] * matrixData[10] - matrixData[12] * matrixData[2] * matrixData[9];
		result.matrixData[2] = matrixData[1] * matrixData[6] * matrixData[15] - matrixData[1] * matrixData[7] * matrixData[14] - matrixData[5] * matrixData[2] * matrixData[15] + matrixData[5] * matrixData[3] * matrixData[14] + matrixData[13] * matrixData[2] * matrixData[7] - matrixData[13] * matrixData[3] * matrixData[6];
		result.matrixData[6] = -matrixData[0] * matrixData[6] * matrixData[15] + matrixData[0] * matrixData[7] * matrixData[14] + matrixData[4] * matrixData[2] * matrixData[15] - matrixData[4] * matrixData[3] * matrixData[14] - matrixData[12] * matrixData[2] * matrixData[7] + matrixData[12] * matrixData[3] * matrixData[6];
		result.matrixData[10] = matrixData[0] * matrixData[5] * matrixData[15] - matrixData[0] * matrixData[7] * matrixData[13] - matrixData[4] * matrixData[1] * matrixData[15] + matrixData[4] * matrixData[3] * matrixData[13] + matrixData[12] * matrixData[1] * matrixData[7] - matrixData[12] * matrixData[3] * matrixData[5];
		result.matrixData[14] = -matrixData[0] * matrixData[5] * matrixData[14] + matrixData[0] * matrixData[6] * matrixData[13] + matrixData[4] * matrixData[1] * matrixData[14] - matrixData[4] * matrixData[2] * matrixData[13] - matrixData[12] * matrixData[1] * matrixData[6] + matrixData[12] * matrixData[2] * matrixData[5];
		result.matrixData[3] = -matrixData[1] * matrixData[6] * matrixData[11] + matrixData[1] * matrixData[7] * matrixData[10] + matrixData[5] * matrixData[2] * matrixData[11] - matrixData[5] * matrixData[3] * matrixData[10] - matrixData[9] * matrixData[2] * matrixData[7] + matrixData[9] * matrixData[3] * matrixData[6];
		result.matrixData[7] = matrixData[0] * matrixData[6] * matrixData[11] - matrixData[0] * matrixData[7] * matrixData[10] - matrixData[4] * matrixData[2] * matrixData[11] + matrixData[4] * matrixData[3] * matrixData[10] + matrixData[8] * matrixData[2] * matrixData[7] - matrixData[8] * matrixData[3] * matrixData[6];
		result.matrixData[11] = -matrixData[0] * matrixData[5] * matrixData[11] + matrixData[0] * matrixData[7] * matrixData[9] + matrixData[4] * matrixData[1] * matrixData[11] - matrixData[4] * matrixData[3] * matrixData[9] - matrixData[8] * matrixData[1] * matrixData[7] + matrixData[8] * matrixData[3] * matrixData[5];
		result.matrixData[15] = matrixData[0] * matrixData[5] * matrixData[10] - matrixData[0] * matrixData[6] * matrixData[9] - matrixData[4] * matrixData[1] * matrixData[10] + matrixData[4] * matrixData[2] * matrixData[9] + matrixData[8] * matrixData[1] * matrixData[6] - matrixData[8] * matrixData[2] * matrixData[5];
		result = result * (1 / det);
		return result;
	}






}