#include "CoordTransform.h"

#include <iostream>


//#define PRINT_DEBUG


void screenToNormalizedScreenCoords(double xCoord, double yCoord, double * normalizedXCoord, double * normalizedYCoord, int width, int height)
{
	*normalizedXCoord = (xCoord / width) * 2 - 1;
	*normalizedYCoord = (height - yCoord) / height * 2 - 1;

#ifdef PRINT_DEBUG
	std::cout << xCoord << " : " << *normalizedXCoord << std::endl;
	std::cout << yCoord << " : " << *normalizedYCoord << std::endl;
#endif
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

void screenToTexCoords(double xCoord, double yCoord, double * texXCoord, double * texYCoord, int width, int height)
{
	*texXCoord = (xCoord / width);
	*texYCoord = (height - yCoord) / height;

#ifdef PRINT_DEBUG
	std::cout << xCoord << " : " << *texXCoord << std::endl;
	std::cout << yCoord << " : " << *texYCoord << std::endl;
#endif
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
