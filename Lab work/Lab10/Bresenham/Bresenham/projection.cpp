//
//  projection.cpp
//  Lab8_TODO
//
//  Copyright © 2016 CGIS. All rights reserved.
//

#include "projection.h"

namespace egc {
	// Mvp
	mat4 defineViewTransformMatrix(int startX, int startY, int width, int height)
	{
		mat4 viewtransformmatrix;

		float mvp[16] = { width / 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, -height / 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, width / 2.0 + startX, height / 2.0 + startY, 0.0f, 1.0f };
		viewtransformmatrix = mat4(mvp);

		return viewtransformmatrix;

	}

	// Mcam
	mat4 defineCameraMatrix(Camera myCamera)
	{
		mat4 cameraMatrix;

		vec3 e = myCamera.cameraPosition;
		vec3 g = myCamera.cameraTarget - e;
		vec3 t = myCamera.cameraUp;
		vec3 u, v, w;

		w = -g.normalize();
		u = (crossProduct(t, w)).normalize();
		v = crossProduct(w, u);

		float v1[16] = { u.x, v.x, w.x, 0.0f, u.y, v.y, w.y, 0.0f, u.z, v.z, w.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		float v2[16] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -e.x, -e.y, -e.z, 1.0f };

		mat4 m1 = mat4(v1);
		mat4 m2 = mat4(v2);

		cameraMatrix = m1 * m2;

		return cameraMatrix;


	}

	// Mper
	mat4 definePerspectiveProjectionMatrix(float fov, float aspect, float zNear, float zFar)
	{
		mat4 perspectiveMatrix;

		float tg = tan(fov / 2.0f);
		float v[16] = { 1.0f / (aspect * tg), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f / tg, 0.0f, 0.0f, 0.0f, 0.0f, (zFar + zNear) / (zNear - zFar), 1.0f, 0.0f, 0.0f,(2 * zFar*zNear) / (zFar - zNear), 0 };
		perspectiveMatrix = mat4(v);
		return perspectiveMatrix;

	}

	void perspectiveDivide(vec4 &inputVertex)
	{
		inputVertex = inputVertex * (1.0f / inputVertex.w);
	}
}