#pragma once
#include <vector>
#include "Point.h"


// all the possible colors for a tetris piece
enum TetColor
{
	RED,
	ORANGE,
	YELLOW,
	GREEN,
	BLUE_LIGHT,
	BLUE_DARK,
	PURPLE
};

// all the possible shapes for a tetris piece
enum TetShape
{
	S,
	Z,
	L,
	J,
	O,
	I,
	T,
	COUNT
};

class Tetromino
{
	friend class TestSuite;// (allows TestSuite access to private members for testing)
	private:
		TetColor color;
		TetShape shape;
	protected:
		std::vector<Point> blockLocs;
	public:
		// constructor for the Tetromino class
		Tetromino();


		// returns the current shape of the Tetromino
		TetShape getShape() const;


		// returns the current color of the Tetromino
		TetColor getColor() const;


		// returns a random tetshape
		static TetShape getRandomShape();

		// - set the shape
		// - set the blockLocs for the shape
		// - set the color for the shape
		void setShape(TetShape shape);


		// rotate the shape 90 degrees around [0,0] (clockwise)
		// to do this:
		// - iterate through blockLocs
		// - rotate each Point 90 degrees clockwise around [0,0]
		// hint: as you rotate the point [1,2] clockwise around [0,0], note
		// how the x,y values change. Once you find the pattern you can use
		// a couple existing methods in the Point class to accomplish this.
		// make it so that the TetShape::O doesn’t rotate
		void rotateClockwise();


		// print a grid to display the current shape
		// to do this: print out a “grid” of text to represent a co-ordinate
		// system. Start at top left [-3,3] go to bottom right [3,-3]
		// (use nested for loops)
		// for each [x,y] point, loop through the blockLocs and if the point exists
		// in the list, print an 'x' instead of a '.'. You should end up with something
		// like this: (results will vary depending on shape and rotation, eg: this
		// one shows a T shape rotated clockwise once)
		// .......
		// .......
		// ...x...
		// ..xx...
		// ...x...
		// .......
		void printToConsole() const;
};