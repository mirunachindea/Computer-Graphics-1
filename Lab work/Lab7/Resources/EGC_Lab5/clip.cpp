#pragma once
#include "clip.h"
#include <vector>

namespace egc {

	int lineClip_CyrusBeck(std::vector<vec3> clipWindow, vec3& p1, vec3& p2) {

		std::vector<vec3> N, PEi;

		for (int i = 0; i < clipWindow.size() - 1; i++) {
			vec3 vect = clipWindow.at(i + 1) - clipWindow.at(i);
			vec3 normala = vec3(vect.y, -vect.x, 0);
			normala.normalize();
			N.push_back(normala);
		}

		vec3 vect = clipWindow.at(0) - clipWindow.at(clipWindow.size() - 1);
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
			float tL = 1;
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

	std::vector<int> computeCSCode(std::vector<vec3> clipWindow, const vec3 p)
	{
		std::vector<int> code;
		code.resize(4);
		float maxX = clipWindow[0].x;
		float minX = clipWindow[0].x;
		float maxY = clipWindow[0].y;
		float minY = clipWindow[0].y;

		//find max x and min x
		for (int i = 0; i < clipWindow.size(); i++)
		{
			if (clipWindow[i].x < minX)
				minX = clipWindow[i].x;
			if (clipWindow[i].x > maxX)
				maxX = clipWindow[i].x;
			if (clipWindow[i].y < minY)
				minY = clipWindow[i].y;
			if (clipWindow[i].y > maxY)
				maxY = clipWindow[i].y;
		}

		code.at(0) = 0; //U
		code.at(1) = 0; //D
		code.at(2) = 0; //R
		code.at(3) = 0; //L

		if (p.y < minY)
			code.at(0) = 1;
		if (p.y > maxY)
			code.at(1) = 1;
		if (p.x < minX)
			code.at(3) = 1;
		if (p.x > maxX)
			code.at(2) = 1;
		return code;
	}

	bool simpleRejection(std::vector<int> cod1, std::vector<int> cod2)
	{
		if ((cod1.at(0) & cod2.at(0) != 0) || (cod1.at(1) & cod2.at(1) != 0) || (cod1.at(2) & cod2.at(2) != 0) || (cod1.at(3) & cod2.at(3) != 0))
		{
			return true;
		}
		return false;
	}

	bool simpleAcceptance(std::vector<int> cod1, std::vector<int> cod2)
	{
		if (cod1.at(0) == 0 && cod1.at(1) == 0 && cod1.at(2) == 0 && cod1.at(3) == 0 && cod2.at(0) == 0 && cod2.at(1) == 0 && cod2.at(2) == 0 && cod2.at(3) == 0)
			return true;
		return false;
	}

	//function returns -1 if the line segment cannot be clipped
	int lineClip_CohenSutherland(std::vector<vec3> clipWindow, vec3& p1, vec3& p2)
	{
		bool finished = false;
		bool display = false;
		bool rejected;

		float maxX;
		float minX;
		float maxY;
		float minY;

		maxX = clipWindow.at(0).x;
		minX = clipWindow.at(0).x;
		maxY = clipWindow.at(0).y;
		minY = clipWindow.at(0).y;

		for (int i = 0; i < clipWindow.size(); i++)
		{
			if (clipWindow.at(i).x < minX)
				minX = clipWindow.at(i).x;
			if (clipWindow.at(i).x > maxX)
				maxX = clipWindow.at(i).x;
			if (clipWindow.at(i).y < minY)
				minY = clipWindow.at(i).y;
			if (clipWindow.at(i).y > maxY)
				maxY = clipWindow.at(i).y;
		}

		printf("%.2f %.2f %.2f %.2f\n", maxX, maxY, minX, minY);

		egc::vec3 aux;
		std::vector<int> aux2;

		std::vector<int> code1;
		std::vector<int> code2;

		code1 = computeCSCode(clipWindow, p1);
		code2 = computeCSCode(clipWindow, p2);
		rejected = simpleRejection(code1, code2);
		if (rejected)
			return -1;

		while (!finished) {

			code1 = computeCSCode(clipWindow, p1);
			code2 = computeCSCode(clipWindow, p2);
			rejected = simpleRejection(code1, code2);

			printf("code1: %d%d%d%d %.2f %.2f\ncode2: %d%d%d%d %.2f %.2f\n", code1[0], code1[1], code1[2], code1[3], p1.x, p1.y, code2[0], code2[1], code2[2], code2[3], p2.x, p2.y);

			if (rejected == true)
			{
				finished = true;
				printf("done r");
				break;
			}
			else
			{
				display = simpleAcceptance(code1, code2);
				if (display == true)
				{
					finished = true;
					printf("done a");
					break;
				}

				else
				{
					if (code1.at(0) == 0 && code1.at(1) == 0 && code1.at(2) == 0 && code1.at(3) == 0)
					{
						aux = p1;
						p1 = p2;
						p2 = aux;

						aux2.clear();
						aux2.resize(4);
						aux2.assign(code1.begin(), code1.end());
						code1.assign(code2.begin(), code2.end());
						code2.assign(aux2.begin(), aux2.end());

						printf("0 %.2f %.2f\n", p1.x, p1.y);
						printf("0 %.2f %.2f\n", p2.x, p2.y);
					}

					if (code1.at(0) == 1 && (p2.y != p1.y))
					{
						p1.x = p1.x + (p2.x - p1.x)*(minY - p1.y) / (p2.y - p1.y);
						p1.y = minY;

						printf("1 %.2f %.2f\n", p1.x, p1.y);
						printf("1 %.2f %.2f\n", p2.x, p2.y);
					}
					else if (code1.at(1) == 1 && (p2.y != p1.y))
					{
						p1.x = p1.x + (p2.x - p1.x)*(maxY - p1.y) / (p2.y - p1.y);
						p1.y = maxY;

						printf("2 %.2f %.2f\n", p1.x, p1.y);
						printf("2 %.2f %.2f\n", p2.x, p2.y);
					}
					else if (code1.at(2) == 1 && (p1.x != p2.x))
					{
						p1.y = p1.y + (p2.y - p1.y)*(maxX - p1.x) / (p2.x - p1.x);
						p1.x = maxX;

						printf("3 %.2f %.2f\n", p1.x, p1.y);
						printf("3 %.2f %.2f\n", p2.x, p2.y);
					}
					else if (code1.at(3) == 1 && (p1.x != p2.x))
					{
						p1.y = p1.y + (p2.y - p1.y)*(minX - p1.x) / (p2.x - p1.x);
						p1.x = minX;

						printf("4 %.2f %.2f\n", p1.x, p1.y);
						printf("4 %.2f %.2f\n", p2.x, p2.y);
					}
				}
			}

		}

		return 0;

	}



}