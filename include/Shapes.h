#ifndef SHAPES_H_INCLUDED
#define SHAPES_H_INCLUDED

#include "Math.h"

namespace be
{

    class shapes
    {
    protected:
        int x, y;
    public:
        shapes(int x, int y): x(x), y(y) {};
        virtual double Area() = 0;
        virtual double Perimeter() = 0;
        virtual double get(std::string key) = 0;
        virtual double set(std::string key, double value) = 0;
    };


    class circle : shapes
    {
        double radius;
    public:
        circle(int x, int y, double radius)
        : shapes(x,y), radius(radius) {}

        double Area()
        {
            return PI * radius * radius;
        }

        double Perimeter()
        {
            return 2 * PI * radius;
        }

        double get(std::string key)
        {
            if(key == "x")
                return x;
            else if(key == "y")
                return y;
            else if(key == "r" || key == "radius" || key == "R")
                return radius;
        }

        double set(std::string key, double value)
        {
            if(key == "x")
                x = value;
            else if(key == "y")
                y = value;
            else if(key == "r" || key == "radius" || key == "R")
                radius = value;
        }
    };
}

#endif // SHAPES_H_INCLUDED
