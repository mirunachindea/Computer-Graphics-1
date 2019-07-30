//
//  main.cpp
//
//  Copyright � 2016 CGIS. All rights reserved.
//

#include <iostream>
#include "include/SDL.h"

#include "mat4.h"
#include "vec3.h"
#include "vec4.h"
#include "transform.h"
#include "camera.h"
#include "projection.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

//define window dimensions
const int WINDOW_WIDTH = 512;
const int WINDOW_HEIGHT = 512;

//The window
SDL_Window *window = NULL;
//The window renderer
SDL_Renderer* renderer = NULL;
SDL_Event currentEvent;

bool quit = false;

int mouseX, mouseY;
float rotationAngle = 0.0f;
float cameraZ = 3.0f;

// order: M V P VP
egc::mat4 viewTransformMatrix; // VP
egc::mat4 cameraMatrix; // V
egc::mat4 perspectiveMatrix; // P
egc::mat4 modelMatrix; // M
egc::Camera myCamera;

egc::vec2 viewportTopLeftCorner(30, 30);
egc::vec2 viewportDimensions(400, 400);

bool backFaceCulling = true;
bool displayNormals = false;

float buf[WINDOW_WIDTH + 1][WINDOW_HEIGHT + 1];

float zMin, zMax;
float yMin, yMax;
float xMin, xMax;

float depth[400][400];

void initializeDepth()
{
	for (int i = 0; i < 400; i++)
	{
		for (int j = 0; j < 400; j++)
		{
			depth[i][j] = -10.0f;
		}
	}

}

void findMinMax(std::vector<tinyobj::shape_t> shapes) {

	for (size_t i = 0; i < shapes.size(); i++) { // objects
		//for each triangle
		for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) { // iterates through every triangle of the object
			int vertexId1 = shapes[i].mesh.indices[3 * f + 0];
			int vertexId2 = shapes[i].mesh.indices[3 * f + 1];
			int vertexId3 = shapes[i].mesh.indices[3 * f + 2];
			// varful trebuie inmultit cu M, V, P, VP

			//update each of the three vertices that form a triangle with the ModelView matrix
			egc::vec4 tempVertex1 = egc::vec4(shapes[i].mesh.positions[3 * vertexId1 + 0], shapes[i].mesh.positions[3 * vertexId1 + 1], shapes[i].mesh.positions[3 * vertexId1 + 2], 1);
			egc::vec4 tempVertex2 = egc::vec4(shapes[i].mesh.positions[3 * vertexId2 + 0], shapes[i].mesh.positions[3 * vertexId2 + 1], shapes[i].mesh.positions[3 * vertexId2 + 2], 1);
			egc::vec4 tempVertex3 = egc::vec4(shapes[i].mesh.positions[3 * vertexId3 + 0], shapes[i].mesh.positions[3 * vertexId3 + 1], shapes[i].mesh.positions[3 * vertexId3 + 2], 1);

			xMin = std::min(xMin, std::min(tempVertex1.x, std::min(tempVertex2.x, tempVertex3.x)));
			xMax = std::max(xMax, std::max(tempVertex1.x, std::max(tempVertex2.x, tempVertex3.x)));

			yMin = std::min(yMin, std::min(tempVertex1.y, std::min(tempVertex2.y, tempVertex3.y)));
			yMax = std::max(yMax, std::max(tempVertex1.y, std::max(tempVertex2.y, tempVertex3.y)));

			zMin = std::min(zMin, std::min(tempVertex1.z, std::min(tempVertex2.z, tempVertex3.z)));
			zMax = std::max(zMax, std::max(tempVertex1.z, std::max(tempVertex2.z, tempVertex3.z)));
		}
	}
}

void drawWireframeTriangle(SDL_Renderer *renderer, egc::vec4 vertex1, egc::vec4 vertex2, egc::vec4 vertex3)
{
    int dy = 2 * viewportTopLeftCorner.y + viewportDimensions.y;
    
    SDL_RenderDrawLine(renderer, vertex1.x, -(vertex1.y - dy), vertex2.x, -(vertex2.y - dy));
    SDL_RenderDrawLine(renderer, vertex1.x, -(vertex1.y - dy), vertex3.x, -(vertex3.y - dy));
    SDL_RenderDrawLine(renderer, vertex2.x, -(vertex2.y - dy), vertex3.x, -(vertex3.y - dy));
}

egc::vec4 findBoundingBox(egc::vec4 vertex1, egc::vec4 vertex2, egc::vec4 vertex3)
{
	egc::vec4 box;
	
	float xmin, xmax, ymin, ymax;

	xmin = vertex1.x;
	xmax = vertex1.x;
	ymin = vertex1.y;
	ymax = vertex1.y;

	// find xmin
	if (vertex2.x < xmin)
		xmin = vertex2.x;
	if (vertex3.x < xmin)
		xmin = vertex3.x;

	// find xmax
	if (vertex2.x > xmax)
		xmax = vertex2.x;
	if (vertex3.x > xmax)
		xmax = vertex3.x;

	// find ymin
	if (vertex2.y < ymin)
		ymin = vertex2.y;
	if (vertex3.y < ymin)
		ymin = vertex3.y;

	// find ymax
	if (vertex2.y > ymax)
		ymax = vertex2.y;
	if (vertex3.y > ymax)
		ymax = vertex3.y;

	box.x = xmin;
	box.y = xmax;
	box.z = ymin;
	box.w = ymax;

	return box;

}

float F(egc::vec4 a, egc::vec4 b, float x, float y)
{
	float result;
	result = (a.y - b.y)*x + (b.x - a.x)*y + a.x * b.y - b.x * a.y;
	return result;
}

void triangleRasterization(SDL_Renderer *renderer, egc::vec4 a, egc::vec4 b, egc::vec4 c, egc::vec3 color_a, egc::vec3 color_b, egc::vec3 color_c)
{
	egc::vec4 box = findBoundingBox(a, b, c);

	float Fab, Fbc1, Fbc2, Fac1, Fac2;
	float alpha, beta, gamma;

	int dy = 2 * viewportTopLeftCorner.y + viewportDimensions.y;

	// from xmin to xmax
	for (int i = box.x; i <= box.y; i++)
	{
		// from ymin to ymax
		for (int j = box.z; j <= box.w; j++)
		{
			Fbc1 = F(b, c, i, j);
			Fbc2 = F(b, c, a.x, a.y);
			Fac1 = F(a, c, i, j);
			Fac2 = F(a, c, b.x, b.y);

			alpha = Fbc1 / Fbc2;
			beta = Fac1 / Fac2;
			gamma = 1 - alpha - beta;

			if (alpha > 0 && alpha < 1 && beta > 0 && beta < 1 && gamma > 0 && gamma < 1)
			{
					//egc::vec3 color_a = egc::vec3(85, 131, 204);
					//egc::vec3 color_b = egc::vec3(84, 204, 160);
					//egc::vec3 color_c = egc::vec3(96, 193, 104);

					//egc::vec3 color = color_a * alpha + color_b * beta + color_c * gamma;


					//SDL_SetRenderDrawColor(renderer, color.x, color.y, color.z, 0);

					//SDL_RenderDrawPoint(renderer, i, -(j - dy));

				//float depthCoeff_a = 255 * (1 - (a.z - zMin) / (zMax - zMin));
				//float depthCoeff_b = 255 * (1 - (b.z - zMin) / (zMax - zMin));
				//float depthCoeff_c = 255 * (1 - (c.z - zMin) / (zMax - zMin));

				//egc::vec3 color_a = egc::vec3(depthCoeff_a, depthCoeff_a, depthCoeff_a);
				//egc::vec3 color_b = egc::vec3(depthCoeff_b, depthCoeff_b, depthCoeff_b);
				//egc::vec3 color_c = egc::vec3(depthCoeff_c, depthCoeff_c, depthCoeff_c);

				//egc::vec3 color = color_a * alpha + color_b * beta + color_c * gamma;

				//float zp = alpha * a.z + beta * b.z + gamma * c.z;

				//SDL_SetRenderDrawColor(renderer, color.x, color.y, color.z, 0);

				//SDL_RenderDrawPoint(renderer, i, -(j - dy));

				egc::vec3 color = color_a * alpha + color_b * beta + color_c * gamma;

				float pixelX = a.x * alpha + b.x * beta + c.x * gamma;
				float pixelY = a.y * alpha + b.y * beta + c.y * gamma;
				float pixelZ = a.z * alpha + b.z * beta + c.z * gamma;

				if (pixelZ >= depth[i][j] && pixelX < 430 && pixelX > 31 &&
					pixelY < 430 && pixelY > 31)
				{
					depth[i][j] = pixelZ;
					SDL_SetRenderDrawColor(renderer, color.x, color.y, color.z, 0);

					SDL_RenderDrawPoint(renderer, i, -j + dy);
				}

			}
		}
	}



}

bool clipPointInHomogeneousCoordinate(egc::vec4 vertex)
{
	bool condition1 = (-abs(vertex.w) <= vertex.x) && (vertex.x <= abs(vertex.w));
	bool condition2 = (-abs(vertex.w) <= vertex.y) && (vertex.y <= abs(vertex.w));
	bool condition3 = (-abs(vertex.w) <= vertex.z) && (vertex.z <= abs(vertex.w));
	return !(condition1 && condition2 && condition3);
}

bool clipTriangleInHomegeneousCoordinates(egc::vec4 vertex1, egc::vec4 vertex2, egc::vec4 vertex3)
{
	return (clipPointInHomogeneousCoordinate(vertex1) || clipPointInHomogeneousCoordinate(vertex2) || clipPointInHomogeneousCoordinate(vertex3));
}

egc::vec3 findNormalVectorToTriangle(egc::vec4 vertex1, egc::vec4 vertex2, egc::vec4 vertex3)
{
    egc::vec3 n;
    
	egc::vec3 v1 = egc::vec3(vertex1);
	egc::vec3 v2 = egc::vec3(vertex2);
	egc::vec3 v3 = egc::vec3(vertex3);

	n = egc::crossProduct(v2 - v1, v3 - v1).normalize();

    return n;
}

egc::vec4 findCenterPointOfTriangle(egc::vec4 vertex1, egc::vec4 vertex2, egc::vec4 vertex3)
{
    egc::vec4 triangleCenter;

	triangleCenter.x = (vertex1.x + vertex2.x + vertex3.x) / 3.0f;
	triangleCenter.y = (vertex1.y + vertex2.y + vertex3.y) / 3.0f;
	triangleCenter.z = (vertex1.z + vertex2.z + vertex3.z) / 3.0f;
	triangleCenter.w = (vertex1.w + vertex2.w + vertex3.w) / 3.0f;

    return triangleCenter;
}

bool isTriangleVisible(egc::vec4 vertex1, egc::vec4 vertex2, egc::vec4 vertex3, egc::vec3 normalVector)
{
    return true;
}

void displayNormalVectors(SDL_Renderer *renderer, egc::vec3 normalVector, egc::vec4 triangleCenter)
{
	egc::vec3 p1 = egc::vec3(triangleCenter);
	egc::vec3 p2 = normalVector * 100 + p1;
	int dy = 2 * viewportTopLeftCorner.y + viewportDimensions.y;

	SDL_RenderDrawLine(renderer, p1.x, -(p1.y - dy), p2.x, -(p2.y - dy));
}

void renderMesh(SDL_Renderer *renderer, std::vector<tinyobj::shape_t> shapes)
{
	/*for (int i = 0; i < WINDOW_WIDTH; i++) {
		for (int j = 0; j < WINDOW_HEIGHT; j++) {
			buf[i][j] = -INFINITY;
		}
	}*/
    //for each mesh in the 3d model representation
    for (size_t i = 0; i < shapes.size(); i++) { // objects
        //for each triangle
        for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) { // iterates through every triangle of the object
            int vertexId1 = shapes[i].mesh.indices[3 * f + 0];
            int vertexId2 = shapes[i].mesh.indices[3 * f + 1];
            int vertexId3 = shapes[i].mesh.indices[3 * f + 2];
            // varful trebuie inmultit cu M, V, P, VP

            //update each of the three vertices that form a triangle with the ModelView matrix
            egc::vec4 tempVertex1 = egc::vec4(shapes[i].mesh.positions[3*vertexId1+0], shapes[i].mesh.positions[3*vertexId1+1], shapes[i].mesh.positions[3*vertexId1+2], 1);
            egc::vec4 tempVertex2 = egc::vec4(shapes[i].mesh.positions[3*vertexId2+0], shapes[i].mesh.positions[3*vertexId2+1], shapes[i].mesh.positions[3*vertexId2+2], 1);
            egc::vec4 tempVertex3 = egc::vec4(shapes[i].mesh.positions[3*vertexId3+0], shapes[i].mesh.positions[3*vertexId3+1], shapes[i].mesh.positions[3*vertexId3+2], 1);
           

			egc::vec3 color_a, color_b, color_c;
			float computeA = 255 * ((tempVertex1.x - xMin) / (xMax - xMin));
			float computeB = 255 * ((tempVertex1.y - yMin) / (yMax - yMin));
			float computeC = 255 * ((tempVertex1.z - zMin) / (zMax - zMin));

			color_a = egc::vec3(computeA, computeB, computeC);

			computeA = 255 * ((tempVertex2.x - xMin) / (xMax - xMin));
			computeB = 255 * ((tempVertex2.y - yMin) / (yMax - yMin));
			computeC = 255 * ((tempVertex2.z - zMin) / (zMax - zMin));

			color_b = egc::vec3(computeA, computeB, computeC);

			computeA = 255 * ((tempVertex3.x - xMin) / (xMax - xMin));
			computeB = 255 * ((tempVertex3.y - yMin) / (yMax - yMin));
			computeC = 255 * ((tempVertex3.z - zMin) / (zMax - zMin));

			color_c = egc::vec3(computeA, computeB, computeC);


			tempVertex1 = modelMatrix * tempVertex1;
			tempVertex1 = cameraMatrix * tempVertex1;
			tempVertex1 = perspectiveMatrix * tempVertex1;
			tempVertex1 = viewTransformMatrix *	tempVertex1;

			tempVertex2 = modelMatrix * tempVertex2;
			tempVertex2 = cameraMatrix * tempVertex2;
			tempVertex2 = perspectiveMatrix * tempVertex2;
			tempVertex2 = viewTransformMatrix * tempVertex2;

			tempVertex3 = modelMatrix * tempVertex3;
			tempVertex3 = cameraMatrix * tempVertex3;
			tempVertex3 = perspectiveMatrix * tempVertex3;
			tempVertex3 = viewTransformMatrix * tempVertex3;

			perspectiveDivide(tempVertex1);
			perspectiveDivide(tempVertex2);
			perspectiveDivide(tempVertex3);

            //SDL_SetRenderDrawColor(renderer, 96, 96, 96, 0);
            //drawWireframeTriangle(renderer, tempVertex1, tempVertex2, tempVertex3);
			egc::vec4 centerPoint = findCenterPointOfTriangle(tempVertex1, tempVertex2, tempVertex3);
			egc::vec3 normalVector = findNormalVectorToTriangle(tempVertex1, tempVertex2, tempVertex3);
			displayNormalVectors(renderer, normalVector, centerPoint);
			triangleRasterization(renderer, tempVertex1, tempVertex2, tempVertex3, color_a, color_b, color_c);
			
        
        }
    }
}

std::vector<tinyobj::shape_t> readOBJ()
{
    std::string inputfile = "bunny.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    
    std::string err;
    bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str());
    
    if (!err.empty()) { // `err` may contain warning message.
        std::cerr << err << std::endl;
    }
    
    if (!ret) {
        exit(1);
    }
    
#ifdef PRINTINFO
    std::cout << "# of shapes    : " << shapes.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;
    
    for (size_t i = 0; i < shapes.size(); i++) {
        printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
        printf("Size of shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());
        printf("Size of shape[%ld].material_ids: %ld\n", i, shapes[i].mesh.material_ids.size());
        
        assert((shapes[i].mesh.indices.size() % 3) == 0);
        for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
            printf("  idx[%ld] = %d, %d, %d. mat_id = %d\n", f, shapes[i].mesh.indices[3*f+0], shapes[i].mesh.indices[3*f+1], shapes[i].mesh.indices[3*f+2], shapes[i].mesh.material_ids[f]);
        }
        
        printf("shape[%ld].vertices: %ld\n", i, shapes[i].mesh.positions.size());
        assert((shapes[i].mesh.positions.size() % 3) == 0);
        for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
            printf("  v[%ld] = (%f, %f, %f)\n", v,
                   shapes[i].mesh.positions[3*v+0],
                   shapes[i].mesh.positions[3*v+1],
                   shapes[i].mesh.positions[3*v+2]);
        }
    }
#endif
    return shapes;
}

bool initWindow()
{
    bool success = true;
    
    //Try to initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL initialization failed" << std::endl;
        success = false;
    }
    else{
        //Try to create the window
        window = SDL_CreateWindow("SDL Hello World Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
        
        if(window == NULL)
        {
            std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
            success = false;
        }
        else
        {
            //Create renderer for window
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
            if (renderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }
    
    return success;
}

void destroyWindow()
{
    //Destroy window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;
    //Quit SDL
    SDL_Quit();
}

void handleMouseEvents()
{
    //Mouse event -> pressed button
    if (currentEvent.type == SDL_MOUSEBUTTONDOWN)
    {
        if (currentEvent.button.button == SDL_BUTTON_LEFT)
        {
            SDL_GetMouseState(&mouseX, &mouseY);
            
        }
        
        if (currentEvent.button.button == SDL_BUTTON_RIGHT)
        {
            SDL_GetMouseState(&mouseX, &mouseY);
            
        }
    }
    /*
    //Mouse event -> mouse movement
    if (currentEvent.type == SDL_MOUSEMOTION)
    {
        if (currentEvent.button.button & SDL_BUTTON_LMASK)
        {
            SDL_GetMouseState(&mouseX, &mouseY);
        }
        
        if (currentEvent.button.button & SDL_BUTTON_RMASK)
        {
            SDL_GetMouseState(&mouseX, &mouseY);
            
        }
    }
     */
}

void handleKeyboardEvents()
{
    //Keyboard event
    if (currentEvent.type == SDL_KEYDOWN)
    {
        switch (currentEvent.key.keysym.sym)
        {
            case SDLK_ESCAPE:
                quit = true;
                break;
            
            case SDLK_a:
                rotationAngle += 10.0f;
                break;
            
            case SDLK_d:
                rotationAngle -= 10.0f;
                break;
            
            case SDLK_w:
                cameraZ -= 0.5f;
                break;
                
            case SDLK_s:
                cameraZ += 0.5f;
                break;
                
            case SDLK_z:
                backFaceCulling = true;
                break;
                
            case SDLK_c:
                backFaceCulling = false;
                break;
                
            case SDLK_q:
                displayNormals = true;
                break;
                
            case SDLK_e:
                displayNormals = false;
                break;
                
            default:
                break;
        }
    }
}

int main(int argc, char * argv[]) {
    if(!initWindow())
    {
        std::cout << "Failed to initialize" << std::endl;
        return -1;
    }
    
    myCamera = egc::Camera(egc::vec3(-0.3f, 1.5f, cameraZ), egc::vec3(-0.3f, 1.5f, -10.0f), egc::vec3(0.0f, 1.0f, 0.0f));
    viewTransformMatrix = egc::defineViewTransformMatrix(viewportTopLeftCorner.x, viewportTopLeftCorner.y, viewportDimensions.x, viewportDimensions.y);
    cameraMatrix = egc::defineCameraMatrix(myCamera);
    perspectiveMatrix = egc::definePerspectiveProjectionMatrix(45.0f, 1.0, -0.1f, -10.0f);
    
    std::vector<tinyobj::shape_t> shapes = readOBJ();

	findMinMax(shapes);
    
    SDL_Rect viewportRectangle = {static_cast<int>(viewportTopLeftCorner.x), static_cast<int>(viewportTopLeftCorner.y), static_cast<int>(viewportDimensions.x), static_cast<int>(viewportDimensions.y)};
    
    while (!quit) {
        //Handle events on queue
        while (SDL_PollEvent(&currentEvent) != 0)
        {
            //User requests quit
            if(currentEvent.type == SDL_QUIT)
            {
                quit = true;
            }
            
            handleMouseEvents();
            handleKeyboardEvents();
            
            //Clear screen
            SDL_SetRenderDrawColor(renderer, 224, 224, 224, 0);
            SDL_RenderClear(renderer);
			initializeDepth();
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
            SDL_RenderDrawRect(renderer, &viewportRectangle);
            
            modelMatrix = egc::rotateY(rotationAngle) * egc::scale(15.0f, 15.0f, 15.0f);
            myCamera.cameraPosition.z = cameraZ;
            cameraMatrix = egc::defineCameraMatrix(myCamera);
            
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
            renderMesh(renderer, shapes);
            
            //Update screen
            SDL_RenderPresent(renderer);
        }
    }
    
    destroyWindow();
    return 0;
}
