#pragma once

//coordinate space transformation functions

void screenToNormalizedScreenCoords(double xCoord, double yCoord, float *normalizedXCoord, float *normalizedYCoord, int width, int height);
void screenToNormalizedScreenCoords(double xCoord, double yCoord, double *normalizedXCoord, double *normalizedYCoord, int width, int height);

void normalizedScreenToScreenCoords(double normalizedXCoord, double normalizedYCoord, int *xCoord, int *yCoord, int width, int height);
void normalizedScreenToScreenCoords(double normalizedXCoord, double normalizedYCoord, float *xCoord, float *yCoord, int width, int height);
void normalizedScreenToScreenCoords(double normalizedXCoord, double normalizedYCoord, double *xCoord, double *yCoord, int width, int height);

void normalizedScreenToFlippedScreenCoords(double normalizedXCoord, double normalizedYCoord, int *xCoord, int *yCoord, int width, int height);
void normalizedScreenToFlippedScreenCoords(double normalizedXCoord, double normalizedYCoord, float *xCoord, float *yCoord, int width, int height);
void normalizedScreenToFlippedScreenCoords(double normalizedXCoord, double normalizedYCoord, double *xCoord, double *yCoord, int width, int height);

void screenToTexCoords(double xCoord, double yCoord, double *texXCoord, double *texYCoord, int width, int height);

void normalizedScreenToTexCoords(double xCoord, double yCoord, float *texXCoord, float *texYCoord);
void normalizedScreenToTexCoords(double xCoord, double yCoord, double *texXCoord, double *texYCoord);
