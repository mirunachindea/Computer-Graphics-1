#pragma once
#include "clip.h"
#include <vector>

namespace egc {

	int max(int a, int b) {
		if (a >= b) return a;
		return b;
	}

	int min(int a, int b) {
		if (a <= b) return a;
		return b;
	}

	int lineClip_CyrusBeck(std::vector<vec3> clipWindow, vec3& p1, vec3& p2) {

		// calculate Ni
		std::vector<vec3> N, PEi;

		for (int i = 0; i < clipWindow.size() - 1; i++) {
			vec3 vect = clipWindow.at(i + 1) - clipWindow.at(i);
			vec3 normala = vec3(vect.y, -vect.x, 0);
			normala.normalize();
			N.push_back(normala);
		}
		vec3 vect = clipWindow.at(clipWindow.size() - 2) - clipWindow.at(clipWindow.size() - 1);
		vec3 normala = vec3(vect.y, -vect.x, 0);
		normala.normalize();
		N.push_back(normala);

		for (int i = 0; i < clipWindow.size(); i++) {
			PEi.push_back(vec3(clipWindow[i].x, clipWindow[i].y, 0));
		}

		if (p1 == p2) {
			return 0;
		}
		else {

			float t;
			float tE = 0;
			float tL = 0;
			int PE, PL;
			vec3 D = p2 - p1;

			for (int i = 0; i < clipWindow.size(); i++) {

				PE = 0;
				PL = 0;

				float product = dotProduct(N[i], D);

				if (product != 0) {

					t = -dotProduct(N[i], p1 - PEi[i]) / dotProduct(N[i], D);

					if (product < 0)
						PE = 1;
					else if (product >= 0)
						PL = 1;

					if (t >= 0 && t <= 1) {
						if (PE)
							if (tE < t)
								tE = t;
						if (PL)
							if (tL > t)
								tL = t;
					}

				}
			}
			if (tE > tL)
				return -1;
			else {
				vec3 PtE = p1 + (p2 - p1) * tE;
				vec3 PtL = p1 + (p2 - p1) * tL;
				p1 = PtE;
				p2 = PtL;
				return 1;
			}
		}
	}





}