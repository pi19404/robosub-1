/********************************************************************
*   These functions are used to sort the rectangles found in a given
*   image, and return a vector of the rectangles in order of largest
*   to smallest.
********************************************************************/    

// Utility function, defines how to sort the rectangles
bool compareRectArea (Rect first, Rect second)
{
	if ((first.width * first.height) >
		(second.width*second.height)) return true;
	return false;
}

// This function accepts an image, and returns a vector of
// rectangles found in the image. The rectangles are sorted using
// the above function, causing them to be sorted from largest to smallest.
std::list<Rect> getSortedRectangles(Mat &fromBinaryImage)
{
	std::list<Rect> rectangles;
	Rect tempRect;
    vector<vector<Point> > contours;
    
    Mat imgCopy;
    fromBinaryImage.copyTo(imgCopy);

    findContours(imgCopy, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

    for (unsigned int i=0; i<contours.size(); i++) {
		tempRect = boundingRect(Mat(contours[i]));
		rectangles.push_front(tempRect);
    }
	rectangles.sort(compareRectArea);
	return rectangles;    
}
