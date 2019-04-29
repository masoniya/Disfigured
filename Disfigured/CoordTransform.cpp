#include "CoordTransform.h"

#include <iostream>


//#define PRINT_DEBUG


void screenToNormalizedScreenCoords(double xCoord, double yCoord, float * normalizedXCoord, float * normalizedYCoord, int width, int height)
{
	double x, y;
	screenToNormalizedScreenCoords(xCoord, yCoord, &x, &y, width, height);
	*normalizedXCoord = (float)x;
	*normalizedYCoord = (float)y;
}

void screenToNormalizedScreenCoords(double xCoord, double yCoord, double * normalizedXCoord, double * normalizedYCoord, int width, int height)
{
	*normalizedXCoord = (xCoord / width) * 2 - 1;
	*normalizedYCoord = (height - yCoord) / height * 2 - 1;

#ifdef PRINT_DEBUG
	std::cout << xCoord << " : " << *normalizedXCoord << std::endl;
	std::cout << yCoord << " : " << *normalizedYCoord << std::endl;
#endif
}

void normalizedScreenToScreenCoords(double normalizedXCoord, double normalizedYCoord, int * xCoord, int * yCoord, int width, int height)
{
	double x, y;
	normalizedScreenToScreenCoords(normalizedXCoord, normalizedYCoord, &x, &y, width, height);
	*xCoord = (int)x;
	*yCoord = (int)y;
}

void normalizedScreenToScreenCoords(double normalizedXCoord, double normalizedYCoord, float * xCoord, float * yCoord, int width, int height)
{
	double x, y;
	normalizedScreenToScreenCoords(normalizedXCoord, normalizedYCoord, &x, &y, width, height);
	*xCoord = (float)x;
	*yCoord = (float)y;
}

void normalizedScreenToScreenCoords(double normalizedXCoord, double normalizedYCoord, double * xCoord, double * yCoord, int width, int height)
{
	*xCoord = (normalizedXCoord + 1) / 2 * width;
	*yCoord = height - (normalizedYCoord + 1) / 2 * height;

#ifdef PRINT_DEBUG
	std::cout << normalizedXCoord << " : " << *xCoord << std::endl;
	std::cout << normalizedYCoord << " : " << *yCoord << std::endl;
#endif
}

void normalizedScreenToFlippedScreenCoords(double normalizedXCoord, double normalizedYCoord, int * xCoord, int * yCoord, int width, int height)
{
	double x, y;
	normalizedScreenToFlippedScreenCoords(normalizedXCoord, normalizedYCoord, &x, &y, width, height);
	*xCoord = (int)x;
	*yCoord = (int)y;
}

void normalizedScreenToFlippedScreenCoords(double normalizedXCoord, double normalizedYCoord, float * xCoord, float * yCoord, int width, int height)
{
	double x, y;
	normalizedScreenToFlippedScreenCoords(normalizedXCoord, normalizedYCoord, &x, &y, width, height);
	*xCoord = (float)x;
	*yCoord = (float)y;
}

void normalizedScreenToFlippedScreenCoords(double normalizedXCoord, double normalizedYCoord, double * xCoord, double * yCoord, int width, int height)
{
	double x, y;
	normalizedScreenToScreenCoords(normalizedXCoord, normalizedYCoord, &x, &y, width, height);
	*xCoord = x;
	*yCoord = height - y;
}

void screenToTexCoords(double xCoord, double yCoord, double * texXCoord, double * texYCoord, int width, int height)
{
	*texXCoord = (xCoord / width);
	*texYCoord = (height - yCoord) / height;

#ifdef PRINT_DEBUG
	std::cout << xCoord << " : " << *texXCoord << std::endl;
	std::cout << yCoord << " : " << *texYCoord << std::endl;
#endif
}

void normalizedScreenToTexCoords(double xCoord, double yCoord, float * texXCoord, float * texYCoord)
{
	double x, y;
	normalizedScreenToTexCoords(xCoord, yCoord, &x, &y);
	*texXCoord = (float)x;
	*texYCoord = (float)y;
}

void normalizedScreenToTexCoords(double xCoord, double yCoord, double * texXCoord, double * texYCoord)
{
	*texXCoord = (xCoord + 1) / 2;
	*texYCoord = (yCoord + 1) / 2;

#ifdef PRINT_DEBUG
	std::cout << xCoord << " : " << *texXCoord << std::endl;
	std::cout << yCoord << " : " << *texYCoord << std::endl;
#endif
}
