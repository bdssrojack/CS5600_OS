/*
* LuoZ.CS5600.LearnC.c / Week2 Learn C
*
* Zhongshi Luo / CS5600 / Northeastern University
* Spring 2024 / Jan 22, 2024
*
*/
#include "func.h"

/* define the test function pointer type*/
typedef bool (*TestFunc)(void);

/* counter of tests*/
int test_cnt = 0;
float factor_mpg2kml = 0.42514;
float epsilon = 0.0001;

/************ test cases **************/
bool test_mpg2kml() {
    float mpg = 32.0f;
    float kml = mpg2kml(mpg);
    return fabs(kml - mpg * factor_mpg2kml) < epsilon;
}

bool test_mpg2lphm() {
    float mpg = 56.0f;
    float lphm = mpg2lphm(mpg);
    return fabs(lphm - 100.0f/(mpg*factor_mpg2kml)) < epsilon;
}

bool test_lph2mpg() {
    float lph = 6.8f;
    float mpg = lph2mpg(lph);
    return fabs(mpg - 100.0f/lph/factor_mpg2kml) < epsilon;
}

bool test_mpg2lphm_bad_input() {
    return mpg2lphm(0.0f) == 0.0f;
}

bool test_lph2mpg_bad_input() {
    return lph2mpg(0.0f) == 0.0f;
}

int test() {
    int res = 0;
    TestFunc tests[] = {test_mpg2kml, 
                        test_mpg2lphm, 
                        test_lph2mpg,
                        test_mpg2lphm_bad_input,
                        test_lph2mpg_bad_input};
    test_cnt = sizeof(tests)/sizeof(tests[0]);

    for(int i = 0; i < test_cnt; i++) {
        res += tests[i]();
    }
    return res;
}

int main() {
    printf("Learning C...\n");
    int res = test();
    printf("%d tests passed, %d failed.\n", res, test_cnt - res);
    return 0;
}