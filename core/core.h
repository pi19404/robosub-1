/* Core Header File */

#ifndef __CORE__
#define __CORE__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "kb.h"
#include "image_kb.h"
#include "../AIFunctionCall/checkImage.h"


int aiMainLoop(KB *kb, IMAGE_KB *im);
int StartGate(KB *kb, IMAGE_KB *im);
int Paths(KB *kb, IMAGE_KB *im);
int Buoys(KB *kb, IMAGE_KB *im);
int ObstacleCourse(KB *kb, IMAGE_KB *im);
int Torpedos(KB *kb, IMAGE_KB *im);
int Bins(KB *kb, IMAGE_KB *im);

bool move( int x, int y, int32_t depth, int32_t heading, 
           bool t1, bool t2, bool m1, bool m2, bool claw );

extern KB *kb;
extern IMAGE_KB *im;

ofstream myFile;

#endif
