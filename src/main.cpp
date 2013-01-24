/*
 * main.cpp
 *
 *  Created on: Jan 23, 2013
 *      Author: Travis Lewis
 */

//helper stuff, needs to be at the top for some reason.
#include "mat.h"
#include "vec.h"

//C includes
#include <stdio.h>
#include <string.h>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glut.h>
#endif  // __APPLE__

#include "math.h"

//C++ includes
#include <vector>
#include <iostream>



//my includes
#include "helpers.hpp"
#include "TriLines.hpp"
#include "Thing.hpp"

using namespace std;

//defines to make understanding the code easier
#define TRUE 1
#define FALSE 0
//#define LINE 1
//#define TRIANGLE 2
//#define RGB 3

unsigned int window_width,// = 512,
             window_height;// = 512;

//the number of pixels the window contains
//(took away const modifier)
int size;

float* pixels;
float red, green, blue;

std::vector<Thing> things;

mat4 CTM;

//FUNCTION PROTOTYPES
int objToPix(float f, int pixels);
void drawLine(int x1, int y1, int x2, int y2);
void drawTriangle(Thing* t);


void putPixel(int x, int y, float r, float g, float b) {
    if (0 <= x && x < (int)window_width &&
        0 <= y && y < (int)window_height) {
        pixels[y*window_width*3+x*3] = r;  // red
        pixels[y*window_width*3+x*3+1] = g;  // green
        pixels[y*window_width*3+x*3+2] = b;  // bluee
    } else {
        printf("Pixel out of bounds: %d %d", x, y);
    }
}

void display()
{
	for(unsigned int y = 0 ; y < window_height ; y++)
	{
		for(unsigned int x = 0 ; x < window_width ; x++)
			putPixel(x, y, 1.0, 1.0, 1.0); //clear to white...
			//putPixel(x,y, 1.0*y/window_height, 1.0*x/window_width,0);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//this is where the stuff gets drawn.
	for(std::vector<Thing>::iterator it = things.begin(), end = things.end();
		it != end ; ++it)
	{
		switch(it->type)
		{
		case Thing::LINE:
		{
//			drawLine(objToPix(it->x1, window_width),
//					objToPix(it->y1, window_height),
//					objToPix(it->x2, window_width),
//					objToPix(it->y2, window_height));
			//TODO: apparently vec4 has no such thing as .xyz(), so
			// i'll probably have to make a helper method for it...
			std::vector<vec2> points = vec3Tovec2(vec4Tovec3(it->points));
			int x1 = objToPix(points[0].x, window_width);
			int y1 = objToPix(points[0].y, window_height);
			int x2 = objToPix(points[1].x, window_width);
			int y2 = objToPix(points[1].y, window_height);
			drawLine(x1, y1, x2, y2);
			break;
		}
		case Thing::TRIANGLE:
			drawTriangle(&*it);
			break;
		case Thing::RGB:
			red = it->r;
			green = it->g;
			blue = it->b;
			break;
		case Thing::CUBE:
			break;
		}

	}

	//glDrawPixels writes a block of pixels to the framebuffer.
	glDrawPixels(window_width,window_height,GL_RGB,GL_FLOAT,pixels);

	glutSwapBuffers();
}

void drawTriangle(Thing* t)
{
//	int x1 = objToPix(p->x1,window_width);
//	int x2 = objToPix(p->x2,window_width);
//	int x3 = objToPix(p->x3,window_width);
//	int y1 = objToPix(p->y1,window_height);
//	int y2 = objToPix(p->y2,window_height);
//	int y3 = objToPix(p->y3,window_height);

//	TriLines tri;
//	tri.addLine(x1,y1,x2,y2);
//	tri.addLine(x1,y1,x3,y3);
//	tri.addLine(x2,y2,x3,y3);
//
//	for(std::vector<HorizLine>::iterator it = tri.lines.begin(),
//			end = tri.lines.end() ; it != end ; ++it)
//	{
//		drawLine(it->left, it->y, it->right, it->y);
//	}
}

void drawLine(int x1, int y1, int x2, int y2)
{
	std::vector<Point2D> points = getPointsFromLine2D(x1, y1, x2, y2);
	for(std::vector<Point2D>::iterator it = points.begin(),
			end = points.end() ; it != end ; ++it)
	{
		putPixel(it->x, it->y, red, green, blue);
	}
}

/**
 * \brief Parses an input data file
 */
void readData()
{
	FILE* input;
	printf("Enter name of input file (no blanks): ");
	char filename[50];
	gets(filename);

	char s[30];

	input = fopen(filename, "r+");
	if(input == NULL)
		perror("Error opening file");
	else
	{
		while(feof(input) == FALSE)
		{
			fscanf(input,"%s",s);

			//TODO: create an if statement that will read until the end of
			// the line is encountered whenever a // is read
			if(strcmp(s, "DIM") == 0)
				fscanf(input, "%d %d", &window_width, &window_height);
			if(strcmp(s, "LINE") == 0)
			{
				Thing t;
				t.type = Thing::LINE;
				for(int i = 0 ; i < 2 ; i++)
				{
					vec4 p;
					fscanf(input, "%f %f %f", &p.x, &p.y, &p.z);
					p.w = 1.0;
					t.points.push_back(p);
				}

				for(std::vector<vec4>::iterator it = t.points.begin(),
						end = t.points.end() ; it != end ; ++it)
				{
					*it = CTM * (*it);
				}

				things.push_back(t);
			}
			if(strcmp(s, "RGB") == 0)
			{
				//read R, G, B
				Thing t;
				t.type = Thing::RGB;
				fscanf(input, "%f %f %f", &t.r, &t.g, &t.b);
				things.push_back(t);
			}
			if(strcmp(s, "TRI") == 0)
			{
				//read x1, y1, x2, y2, x3, y3
				Thing t;
				t.type = Thing::TRIANGLE;
				for(int i = 0 ; i < 3 ; i++)
				{
					vec4 p;
					fscanf(input, "%f %f %f", &p.x, &p.y, &p.z);
					p.w = 1.0;
					t.points.push_back(p);
				}

				for(std::vector<vec4>::iterator it = t.points.begin(),
						end = t.points.end() ; it != end ; ++it)
				{
					*it = CTM * (*it);
				}

				things.push_back(t);
			}
			if(strcmp(s, "WIREFRAME_CUBE") == 0)
			{
				//TODO: create a wireframe cube and apply CTM to it.
				Thing t = createUnitCube();
				for(std::vector<vec4>::iterator it = t.points.begin(),
						end = t.points.end() ; it != end ; ++it)
				{
					*it = CTM * (*it);
				}
				things.push_back(t);
			}
			if(strcmp(s, "LOAD_IDENTITY_MATRIX") == 0)
			{
				CTM = mat4();
			}
			if(strcmp(s, "ROTATEX") == 0)
			{
				float angle;
				fscanf(input, "%f", &angle);
				CTM = RotateX(angle) * CTM;
			}
			if(strcmp(s, "ROTATEY") == 0)
			{
				float angle;
				fscanf(input, "%f", &angle);
				CTM = RotateY(angle) * CTM;
			}
			if(strcmp(s, "ROTATEZ") == 0)
			{
				float angle;
				fscanf(input, "%f", &angle);
				CTM = RotateZ(angle) * CTM;
			}
			if(strcmp(s, "SCALE") == 0)
			{
				float sx, sy, sz;
				fscanf(input, "%f %f %f", &sx, &sy, &sz);
				CTM = Scale(sx, sy, sz) * CTM;
			}
			if(strcmp(s, "TRANSLATE") == 0)
			{
				float tx, ty, tz;
				fscanf(input, "%f %f %f", &tx, &ty, &tz);
				CTM = Translate(tx, ty, tz) * CTM;
			}
		}
	}
	fclose(input);
}

/**
 * Converts object coordinates (-1 <= f <= 1) to pixel coordinates.
 * @param f The object coordinate to convert.
 * @param pixels The number of pixels in the dimension to convert.
 */
int objToPix(float f, int pixels)
{
	float result = (float)pixels * ((f+1.0)/2.0);
	int num = (int)(result*10) % 10;
	if(num >= 5)
		return ((int) result) + 1; // round up
	else
		return (int) result; // round down
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	//read data from file
	readData();
	size = window_width * window_height;
	pixels = new float[size*3];

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("OpenGL glDrawPixels demo - simplified by JM");

	glutDisplayFunc(display);

	glClearColor(0.0, 0.0, 0.0, 1.0);

	glutMainLoop();
}
