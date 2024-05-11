#pragma once
#include <string>

class Point {
    friend class TestSuite;// (allows TestSuite access to private members for testing)
    // the x and y values of this class should not be accessible without the get methods
    private:
        int x{0};
        int y{0};
    public:
        // the default constructor for the point class
        Point();

        // Constructor for Point class that is given an x and y value
        Point(int x, int y);

        // getter for the x value of the Point class
        // returns the current x value 
        int getX() const;

        // getter for the y value of the Point class
        // returns the current y value 
        int getY() const;

        // setter for the x value of the Point class
        // -param 1 is the new x value
        // returns nothing
        void setX(int x);

        // setter for the y value of the Point class
        // -param 1 is the new y value
        // returns nothing
        void setY(int y);

        // setter for the x and y values of the Point class
        // -param 1 is the new x value
        // -param 2 is the new y value
        // returns nothing
        void setXY(int x, int y);

        // swaps the x and y value of the Point class
        // returns nothing
        void swapXY();

        // multiply the x value by the given factor
        // -param 1 the factor to multiply the value by
        // returns nothing
        void multiplyX(int factor);

        // multiply the y value by the given factor
        // -param 1 the factor to multiply the value by
        // returns nothing
        void multiplyY(int factor);

        // returns a string in the form of [x,y]
        std::string toString() const;
};