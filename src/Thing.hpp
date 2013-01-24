/*
 * object.hpp
 *
 *  Created on: Jan 23, 2013
 *      Author: Travis Lewis
 */

#ifndef OBJECT_HPP_
#define OBJECT_HPP_

//std includes
#include <vector>

//other includes
#include "vec.h"

struct Thing
{
public:
	enum Type {LINE, TRIANGLE, CUBE, RGB};
	Type type;
	std::vector<vec4> points;
	float r,g,b;
};

#endif /* OBJECT_HPP_ */
