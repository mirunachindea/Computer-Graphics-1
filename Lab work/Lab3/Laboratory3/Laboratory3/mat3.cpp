#include <algorithm>
#include <iostream>
#include <cmath>
#include "mat3.h"

namespace egc {

	mat3& mat3::operator =(const mat3& srcMatrix) {
		for (int i = 0; i <= 8; i++) {
			this->matrixData[i] = srcMatrix.matrixData[i];
		}
		return *this;
	}
	mat3 mat3::operator *(float scalarValue) const {
		mat3 newMat;
		for (int i = 0; i <= 8; i++) {
			newMat.matrixData[i] = this->matrixData[i] * scalarValue;
		}
		return newMat;
	}
	float& mat3::at(int i, int j) {
		return this->matrixData[i + 3 * j];
	}

	const float& mat3::at(int i, int j) const {
		return this->matrixData[i + 3 * j];
	}

	mat3 mat3::operator +(const mat3& srcMatrix) const {
		mat3 result;
		for (int i = 0; i <= 8; i++) {
			result.matrixData[i] = this->matrixData[i] + srcMatrix.matrixData[i];
		}
		return result;
	}
	vec3 mat3::operator *(const vec3& srcVector) const {
		vec3 result;
		result.x = this->at(0, 0) * srcVector.x + this->at(0, 1) * srcVector.y + this->at(0, 2) * srcVector.z;
		result.y = this->at(1, 0) * srcVector.x + this->at(1, 1) * srcVector.y + this->at(1, 2) * srcVector.z;
		result.z = this->at(2, 0) * srcVector.x + this->at(2, 1) * srcVector.y + this->at(2, 2) * srcVector.z;
		return result;
	}
	mat3 mat3::operator *(const mat3& srcMatrix) const {
		mat3 result;
		float sum;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				sum = 0;
				for (int k = 0; k < 3; k++) {
					sum += matrixData[i + k * 3] * srcMatrix.matrixData[k + j * 3];
				}
				result.matrixData[i + j * 3] = sum;
			}
		}
		return result;
	}

	float mat3::determinant() const {
		return matrixData[0] * matrixData[4] * matrixData[8] + matrixData[3] * matrixData[7] * matrixData[2] + matrixData[1] * matrixData[5] * matrixData[6] - matrixData[2] * matrixData[4] * matrixData[6] - matrixData[1] * matrixData[3] * matrixData[8] - matrixData[0] * matrixData[5] * matrixData[7];
	}

	mat3 mat3::transpose() const {
		mat3 result;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				result.matrixData[i * 3 + j] = matrixData[i + j * 3];
			}
		}
		return result;
	}

	mat3 mat3::inverse() const {
		mat3 result;
		mat3 transpose = this->transpose();
		float det = 1 / this->determinant();
		result.matrixData[0] = transpose.matrixData[4] * transpose.matrixData[8] - transpose.matrixData[7] * transpose.matrixData[5];
		result.matrixData[1] = transpose.matrixData[5] * transpose.matrixData[6] - transpose.matrixData[3] * transpose.matrixData[8];
		result.matrixData[2] = transpose.matrixData[3] * transpose.matrixData[7] - transpose.matrixData[4] * transpose.matrixData[6];
		result.matrixData[3] = transpose.matrixData[2] * transpose.matrixData[7] - transpose.matrixData[1] * transpose.matrixData[8];
		result.matrixData[4] = transpose.matrixData[0] * transpose.matrixData[8] - transpose.matrixData[2] * transpose.matrixData[6];
		result.matrixData[5] = transpose.matrixData[1] * transpose.matrixData[6] - transpose.matrixData[0] * transpose.matrixData[7];
		result.matrixData[6] = transpose.matrixData[1] * transpose.matrixData[5] - transpose.matrixData[2] * transpose.matrixData[4];
		result.matrixData[7] = transpose.matrixData[2] * transpose.matrixData[3] - transpose.matrixData[0] * transpose.matrixData[5];
		result.matrixData[8] = transpose.matrixData[0] * transpose.matrixData[4] - transpose.matrixData[1] * transpose.matrixData[3];
		result = result * det;
		return result;
	}
}
