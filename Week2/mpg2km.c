#include "func.h"

float mpg2kml(float mpg) {
    float kml = mpg * factor_mpg2kml;
    return kml;
}

float mpg2lphm(float mpg) {
    float kml = mpg2kml(mpg);
    return fabs(mpg - 0.0f) < epsilon ? 0.0f : 100.0f/kml;
}

float lph2mpg(float lph) {
    if(fabs(lph - 0.0f) < epsilon) {
        return 0.0f;
    }
    float kml = 100.0f/lph;
    return kml/factor_mpg2kml;
}