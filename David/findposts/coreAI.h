
#include <time.h>
#include <cstdio>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include <list>

// DEFINES
#define ABOVE_WATER false
#define NUM_OF_COLORS 6 //this includes "vC_error"
#define STATIC_DEBUG false
#define STATIC_IMAGE "/home/ian/Desktop/gate_1.jpg"
//#define STATIC_IMAGE "/home/ian/Desktop/greengate.JPG"
#define VISUAL_DEBUG false

#define TEXT_SCALE 1.5
#define TEXT_X 15
#define TEXT_Y 20
#define TEXT_SPACE 15
#define DILATE_ERODE_SCALE 3
#define MEDIAN_BLUR_ODD_SCALE 7
#define PRECONFIG_IS_AVAILABLE true
#define BGRA_ANTI_TOLERANCE 30
#define CHECKMARK_CH '@'
#define CIRCLE_CH '('
#define SQUARE_CH '['
#define GREEN_SCALAR Scalar(0,255,0)
#define RED_SCALAR Scalar(0,0,255)
#define YELLOW_SCALAR Scalar(0,255,255)
#define BLUE_SCALAR Scalar(255,0,0)
#define ORANGE_SCALAR Scalar(0,200,255)
#define WHITE_SCALAR Scalar(255,255,255)
#define OFF_SCALAR Scalar(100, 100, 100)

#define FOV 1.09 //radians
#define BUOY_WIDTH 7

using namespace cv;

//PRESET FUNCTIONS
typedef enum {red, green, blue, yellow, orange, vC_error} vColor; //for "verified color"
typedef enum {low, high} vLevel;						//for "varied level"
std::string vColorStr[NUM_OF_COLORS] = {"red", "green", "blue", "yellow", "orange", "vC_error"};
Scalar vColorBGR[NUM_OF_COLORS] = {RED_SCALAR, GREEN_SCALAR, BLUE_SCALAR, YELLOW_SCALAR, ORANGE_SCALAR, WHITE_SCALAR};
typedef struct HARDDATA {
	int hue[2][NUM_OF_COLORS];
	int sat[2][NUM_OF_COLORS];
	int value[2][NUM_OF_COLORS];
} hardDataStruct;
typedef struct TRACKINGOBJECT {
	float x_cm; //left/right of center
	float y_cm; //up/down of center
	float z_cm; //distance from sub
	char confirmation;
} objStruct;
