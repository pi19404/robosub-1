float getDistance(Rect r, int cameraMidpointX)
{
  float percentOfScreen = getPercentOfScreen(cameraMidpointX*2, r.width/2);
  *z_distance = approxDistance(percentOfScreen, r.width);
}

float getPercentOfScreen(float frameWidth, float objectWidth)
{
	return objectWidth/frameWidth;
}

float approxDistance(float percentOfScreen, float targetWidthCM)
{
	return ((1.0375870127 /
			(percentOfScreen * 0.2663492405)) *
			(30.48 / targetWidthCM / 10.7))*10;
}
