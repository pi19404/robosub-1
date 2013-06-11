/* Core Header File */

#ifndef __CORE__
#define __CORE__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "kb.h"
#include "image_kb.h"


int aiMainLoop(KB *kb, IMAGE_KB *im);
int StartGate(KB *kb, IMAGE_KB *im);
int Paths(KB *kb, IMAGE_KB *im);
int Buoys(KB *kb, IMAGE_KB *im);
int ObstacleCourse(KB *kb);
int Torpedos(KB *kb);
int Bins(KB *kb, IMAGE_KB *im);

bool move(int x, int y, int z, double heading);

#endif
