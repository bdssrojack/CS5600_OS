#ifndef FUNC_DEF
#define FUNC_DEF

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

/* factor from mpg to kml*/
extern float factor_mpg2kml;
/* epsilon for float comparison*/
extern float epsilon;

/**
 * Convert miles per gallon to kilometers per liter
 * Param: float mpg - miles per gallon
 * Return: kilometers per liter of given mpg in float
*/
float mpg2kml(float mpg);


/**
 * Convert miles per gallon to liters per 100 kilometers
 * Param: float mpg - miles per gallon
 * Return: liters per 100 kilometers of given mpg in float. In case the input is zero (i.e. 0.0f) it would return 0.0f directly
*/
float mpg2lphm(float mpg);


/**
 * Convert liters per 100 kilometers to miles per gallon. 
 * Param: float lph - liters per 100 kilometers
 * Return: miles per gallon of given lph in float. In case the input is zero (i.e. 0.0f) it would return 0.0f directly
*/
float lph2mpg(float lph);

#endif