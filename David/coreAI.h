// Header file

// Find Posts:
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


// Function Definitions

//Core AI Functions
void checkDepth();
void wait_for_armed();
void mainLoop();

// Find Posts Functions
void initColorPresets (hardDataStruct * hardData);
vColor validifyColor (std::string inputColor);
bool compareRectArea (Rect first, Rect second);
std::list<Rect> getSortedRectangles(Mat &fromBinaryImage);
void getBestCircle(Mat &fromBinaryImage, Point * center, float * radius);
int isRectInCorner(Rect rectInQuestion, int midpointX, int midpointY);
float getPercentOfScreen(float frameWidth, float objectWidth);
float approxDistance(float percentOfScreen, float targetWidthCM);
float approxRadX(float fov, int midpointX, int objX);
float approxCoordX(float distance, float radX);
int findGate();

