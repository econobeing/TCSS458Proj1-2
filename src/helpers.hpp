/*
 * Helpers.hpp
 *
 *  Created on: Jan 23, 2013
 *      Author: Travis Lewis
 */

#ifndef HELPERS_HPP_
#define HELPERS_HPP_

#include <vector>

#include "vec.h"

#include "Thing.hpp"


struct Point2D
{
	int x, y;
};

Thing createUnitCube()
{
	Thing cube;
	cube.type == Thing::CUBE;
	cube.points.push_back(vec4(-0.5, -0.5, -0.5, 1));
	cube.points.push_back(vec4(-0.5, -0.5, 0.5, 1));
	cube.points.push_back(vec4(-0.5, 0.5, -0.5, 1));
	cube.points.push_back(vec4(-0.5, 0.5, 0.5, 1));

	cube.points.push_back(vec4(0.5, -0.5, -0.5, 1));
	cube.points.push_back(vec4(0.5, -0.5, 0.5, 1));
	cube.points.push_back(vec4(0.5, 0.5, -0.5, 1));
	cube.points.push_back(vec4(0.5, 0.5, 0.5, 1));

	//which vectors pairs to draw as lines:
	// (0,1), (0,3), (0,5), (1,3), (1,5), (2,3), (2,6), (3,7), (4,5),
	// (4,6), (5,7), (6,7)
	//^those are the indices of the points array to use.

	return cube;
}

std::vector<Point2D> getPointsFromLine2D(int x1, int y1, int x2, int y2)
{
	std::vector<Point2D> points;

	//make sure x1 < x2
	if(x1 > x2)
	{
		int temp = x1;
		x1 = x2;
		x2 = temp;
		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	//get slope/distance information
	int dx = x2 - x1;
	int dy = y2 - y1;
	float m = ((float)dy) / ((float)dx);

	if(dx == 0 && dx != 0)
	{ //vertical line
		int inc = (y1 < y2) ? 1 : -1;
		for(int i = y1 ; i != y2 ; i += inc)
		{
			Point2D p;
			p.x = x1;
			p.y = i;
			points.push_back(p);
		}
	}
	else if(dy == 0 && dx != 0)
	{ // horizontal line
		//x1 < x2 so we don't need to check that
		for(int i = x1 ; i < x2 ; i++)
		{
			Point2D p;
			p.x = i;
			p.y = y1;
			points.push_back(p);
		}
	}
	else if(m <=1 && m >= -1)
	{ // non-steep line, iterate over x
		float ypos = (float)y1;
		for(int x = x1 ; x < x2 ; x++)
		{
			Point2D p;
			p.x = x;
			p.y = (int)ypos;
			points.push_back(p);
			ypos += m;
		}
	}
	else if(m > 1 || m < -1)
	{ // steep line, iterate over y
		m = 1.0/m; //invert slope for vertical use
		float xpos;
		if(y1 < y2)
		{
			xpos = (float)x1;
			for(int y = y1 ; y < y2 ; y++)
			{
				Point2D p;
				p.x = (int)xpos;
				p.y = y;
				points.push_back(p);
				xpos += m;
			}
		}
		else
		{
			xpos = (float)x2;
			for(int y = y2 ; y < y1 ; y++)
			{
				Point2D p;
				p.x = (int)xpos;
				p.y = y;
				points.push_back(p);
				xpos += m;
			}
		}
	}

	return points;
}

#endif /* HELPERS_HPP_ */
