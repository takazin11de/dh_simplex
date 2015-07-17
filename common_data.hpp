#include "user_std.hpp"
#ifndef COMMON_DATA
#define COMMON_DATA

#define STEP (0.001)
#define T (1.0)
#define K (2.0)
extern double SIM_TIME;

#define Rho (1000)

#define MAX_MACHINE_SPEED (2.0)
#define MAX_MACHINE_ACCELERATION (1.0)

#define Pmax (1500)
#define Au1 (pow(0.197,2)*M_PI/4)
#define Au2 (pow(0.291,2)*M_PI/4 - 12*pow(0.008,2)*M_PI/4)
#define Ab1 (pow(0.220,2)*M_PI/4)
#define Ab2 (pow(0.292,2)*M_PI/4)
#define Bu (0.0485)
#define Bb (0.0400)
#define VOL (850.0/1000/1000)
#define g (9.80665)

#define SIMPLEX_SIZE (10)

struct xydata{
	double x;
	double y;
};


#endif

