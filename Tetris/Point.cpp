#include "Point.h"
#include <sstream>

Point::Point() {}

Point::Point(int x, int y)
{
	this->x = x;
	this->y = y;
}

int Point::getX() const
{
	return x;
}

int Point::getY() const
{
	return y;
}

void Point::setX(int x) 
{
	this->x = x;
}

void Point::setY(int y)
{
	this->y = y;
}

void Point::setXY(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Point::swapXY()
{
	int swap{ y };
	y = x;
	x = swap;
}

void Point::multiplyX(int factor)
{
	x = x * factor;
}

void Point::multiplyY(int factor)
{
	y = y * factor;
}

std::string Point::toString() const
{
	std::stringstream str{};
	str << "[" << getX() << "," << getY() << "]";
	return str.str(); 
}