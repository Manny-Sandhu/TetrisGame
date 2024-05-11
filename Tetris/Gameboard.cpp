#include <iostream>
#include <iomanip>
#include <cassert>
#include "Gameboard.h"

Gameboard::Gameboard()
{
	empty();
}

// fill the board with EMPTY_BLOCK 
//   (iterate through each rowIndex and fillRow() with EMPTY_BLOCK))
// - params: none
// - return: nothing
void Gameboard::empty()
{
	for (int i = 0; i < MAX_Y; i++)
	{
		for (int j = 0; j < MAX_X; j++)
		{
			grid[i][j] = -1;
		}
	}
}

// print the grid contents to the console (for debugging purposes)
//   use std::setw(2) to space the contents out 
//   (google it, you'll need to #include <iomanip>).
//   If the content == EMPTY_BLOCK, print a '.', otherwise print the content.
// - params: none
// - return: nothing
void Gameboard::printToConsole() const
{
	for (int i = 0; i < MAX_Y; i++)
	{
		for (int j = 0; j < MAX_X; j++)
		{
			if (grid[i][j] == -1)
			{
				std::cout << std::setw(2) << ".";
			}
			else 
			{
				std::cout << std::setw(2) << grid[i][j];
			}
		}
		std::cout << "\n";
	}
}

// get the content at a given point 
// assert the point is valid (use isValidPoint())
// - param 1: a Point (the XY of the content weant to retrieve)
// - return: an int, the content from the grid at the specified point 
int Gameboard::getContent(Point point) const
{
	return grid[point.getY()][point.getX()];
}

// get the content at a given XY location
// assert the point is valid (use isValidPoint())
// - param 1: an int for X (column)
// - param 2: an int for Y (row)
// - return: an int, the content from the grid at the specified XY
int Gameboard::getContent(int x, int y) const
{
	return grid[y][x];
}

// set the content at a given point (ignore invalid points)
// - param 1: a Point
// - param 2: an int representing the content we want to set at this location.
void Gameboard::setContent(const Point &point, int val)
{
	if (isValidPoint(point))
	{
		grid[point.getY()][point.getX()] = val;
	}
	else
	{
		std::cout << "Error not a valid location" << std::endl;
	}
}

// set the content at an x,y position (ignore invalid points)
// - param 1: an int for X (column)
// - param 2: an int for Y (row)
// - param 3: an int representing the content we want to set at this location	
// - return: nothing
void Gameboard::setContent(int  x, int  y, int val)
{
	if (isValidPoint(x, y))
	{
		grid[y][x] = val;
	}
	else
	{
		std::cout << "Error not a valid location" << std::endl;
	}
}

// set the content for a set of points (ignore invalid points)
// - param 1: a vector of Points representing locations
// - param 2: an int representing the content we want to set.
// - return: nothing
void Gameboard::setContent(std::vector<Point>  &blocks, int val)
{
	for (auto &block : blocks)
	{
		if (isValidPoint(block))
		{
			grid[block.getY()][block.getX()] = val;
		}
	}
}

// Determine if (valid) all points passed in are empty
// *** IMPORTANT: Assume invalid x,y values can be passed to this method.
// Invalid meaning outside the bounds of the grid.
// * ONLY TEST VALID POINTS (use isValidPoint()). Ignore invalid Points.
// Using invalid points to index into the grid would cause undefined behaviour. 
// - param 1: a vector of Points representing locations to test
// - return: true if the content at ALL VALID points is EMPTY_BLOCK, false otherwise
bool Gameboard::areAllLocsEmpty(std::vector<Point> &checks) const
{
	for (auto &check : checks)
	{
		if (isValidPoint(check))
		{
			if (getContent(check) != -1)
			{
				return false;
			}
		}
	}
	return true;
}

// Remove all completed rows from the board
//   use getCompletedRowIndices() and removeRows() 
// - params: none
// - return: the count of completed rows removed
int Gameboard::removeCompletedRows()
{
	std::vector<int> rmRows;
	for (int i = 0; i < MAX_Y; i++)
	{
		if (isRowCompleted(i))
		{
			rmRows.push_back(i);
		}
	}
	removeRows(rmRows);
	return rmRows.size();
}

// A getter for the spawn location
// - params: none
// - returns: a Point, representing our private spawnLoc
Point Gameboard::getSpawnLoc() const
{
	return spawnLoc;
}


// Determine if a given Point is a valid grid location
// - param 1: a Point object
// - return: true if the point is a valid grid location, false otherwise
bool Gameboard::isValidPoint(Point const &point) const
{
	if ((0 <= point.getX() && point.getX() <= MAX_X) && (0 <= point.getY() && point.getY() <= MAX_Y))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Determine if a given XY is a valid grid location
// - param 1: an int representing x
// - param 2: an int representing y
// - return: true if the x,y is a valid grid location, false otherwise
bool Gameboard::isValidPoint(int x, int y) const
{
	if ((0 <= x  && x <= MAX_X) && (0 <= y && y <= MAX_Y))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// return a bool indicating if a given row is full (no EMPTY_BLOCK in the row)
// assert the row index is valid
// - param 1: an int representing the row index we want to test
// - return: bool representing if the row is completed
bool Gameboard::isRowCompleted(int rowIndex) const
{
	bool check{ true };

	for (int i = 0; i < MAX_X; i++)
	{
		if (grid[rowIndex][i] == -1)
		{
			check = false;
		}
	}
	return check;
}

// fill a given grid row with specified content
// - param 1: an int representing a row index
// - param 2: an int representing content
// - return: nothing
void Gameboard::fillRow(int rowIndex, int content)
{
	for (int i = 0; i < MAX_X; i++)
	{
		grid[rowIndex][i] = content;
	}
}

// scan the board for completed rows.
// Iterate through grid rows and use isRowCompleted(rowIndex)
// - params: none
// - return: a vector of completed row indices (integers).
std::vector<int> Gameboard::getCompletedRowIndices() const
{
	std::vector<int> completed{};
	for (int i = 0; i < MAX_Y; i++)
	{
		if (isRowCompleted(i))
		{
			completed.push_back(i);
		}
		
	}
	return completed;
}

// copy a source row's contents into a target row.
// - param 1: an int representing the source row index
// - param 2: an int representing the target row index
// - return: nothing
void Gameboard::copyRowIntoRow(int source, int target)
{
	//Point sPoint{ MAX_X, source };
	//Point tPoint{ MAX_X, target };
	//if (isValidPoint(sPoint) && isValidPoint(tPoint))
	//{
		int rowCopy[1][MAX_X]{ 0 };
		for (int j = 0; j < MAX_X; j++)
		{
			grid[target][j] = grid[source][j];
		}
	//}
}

// In gameplay, when a full row is completed (filled with content)
// it gets "removed".  To be exact, the row itself is not removed
// but the content from the row above it is copied into it.
// This continues all the way up the grid until the first row is copied
// into the second row.  Finally, the first row is filled with EMPTY_BLOCK
// Given a row index:
//   1) Assert the row index is valid
//   2) Starting at rowIndex, copy each row above the removed
//     row "one-row-downwards" in the grid.
//     (loop from y=rowIndex-1 down to 0, and copyRowIntoRow(y, y+1)).
//   3) call fillRow() on the first row (and place EMPTY_BLOCKs in it).
// - param 1: an int representing a row index
// - return: nothing
void Gameboard::removeRow(int rowIndex)
{
	assert((rowIndex <= MAX_Y && 0 <= rowIndex) && "Row not on board");
	for (int i = rowIndex-1; i >= 0; i--)
	{
		copyRowIntoRow(i, i + 1);
	}
	fillRow(0, -1);
	
}

// given a vector of row indices, remove them. 
//   Iterate through the vector, call removeRow() on each row index. 
// - param 1: a vector of integers representing row indices we want to remove
// - return: nothing
void Gameboard::removeRows(std::vector<int> &rowIndices)
{
	for (auto &row : rowIndices)
	{
		removeRow(row);
	}
}