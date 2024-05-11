#include "Tetromino.h"
#include <iostream>


// default constructor sets the shape to O and color to Oranage
Tetromino::Tetromino()
{
	setShape(TetShape::O);
}


// returns the current Color of the Tetromino
TetColor Tetromino::getColor() const
{
	return color;
}


// returns the current Shape of the Tetromino
TetShape Tetromino::getShape() const
{
	return shape;
}


TetShape Tetromino::getRandomShape()
{
	int rand_num = rand() % static_cast<int>(TetShape::COUNT);
	return static_cast<TetShape>(rand_num);
}

// sets the shape for the Tetromino
//color and blocks are based on the shape so those are set as well
void Tetromino::setShape(TetShape shape)
{
	this->shape = shape;
	switch (shape) 
	{
		case O:
			color = TetColor::YELLOW;
			blockLocs = { Point(0,0), Point(0,1), Point(1,0), Point(1,1) };
			break;
		case I:
			color = TetColor::BLUE_LIGHT;
			blockLocs = { Point(0,0), Point(0,-1), Point(0,1), Point(0,2) };
			break;
		case S:
			color = TetColor::RED;
			blockLocs = { Point(0,0), Point(-1,0), Point(0,1), Point(1,1) };
			break;
		case Z:
			color = TetColor::GREEN;
			blockLocs = { Point(0,0), Point(0,1), Point(1,0), Point(-1,1) };
			break;
		case L:
			color = TetColor::ORANGE;
			blockLocs = { Point(0,0), Point(1,-1), Point(0,-1), Point(0,1) };
			break;
		case J:
			color = TetColor::BLUE_DARK;
			blockLocs = { Point(0,0), Point(0,1), Point(-1,-1), Point(0,-1) };
			break;
		case T:
			color = TetColor::PURPLE;
			blockLocs = { Point(0,0), Point(-1,0), Point(1,0), Point(0,-1) };
			break;
	}

}

// rotates the tetromino 90 degrees 
void Tetromino::rotateClockwise()
{
	if (shape == TetShape::O)
	{
	}
	else 
	{
		for (auto& blockloc : blockLocs)
		{
			blockloc.multiplyX(-1);
			blockloc.swapXY();
		}
	}
}


// prints the tetromino to a 7/7 grid
void Tetromino::printToConsole() const
{
	Point current_point = Point(-3, 3);
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (current_point.getY() == blockLocs[0].getY() && current_point.getX() == blockLocs[0].getX())
			{
				std::cout << "X";
			}
			else if (current_point.getY() == blockLocs[1].getY() && current_point.getX() == blockLocs[1].getX())
			{
				std::cout << "X";
			}
			else if (current_point.getY() == blockLocs[2].getY() && current_point.getX() == blockLocs[2].getX())
			{
				std::cout << "X";
			}
			else if (current_point.getY() == blockLocs[3].getY() && current_point.getX() == blockLocs[3].getX())
			{
				std::cout << "X";
			}
			else
			{
				std::cout << ".";
			}
			current_point.setX(current_point.getX() + 1);
		}
		std::cout << "\n";
		current_point.setXY(-3, current_point.getY() - 1);
	}
	std::cout << "\n\n";
}
