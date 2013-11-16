import cv2
import numpy as np
# test image: http://imgur.com/4pWOTKq.jpg


# The number of min and max "hits" needed to detect an edge.
MIN_EDGE_VAL = 50
MAX_EDGE_VAL = 300

# The high and low HSV values for orange
ORANGE_HI  = np.array([180, 200, 255], np.uint8)
ORANGE_LO  = np.array([30, 100, 80], np.uint8)

def main():
    img   = cv2.imread("imagefiletest.jpg")
    #debug code
    cv2.imshow("Initial image", img)
    cv2.waitKey(0)

    lines = get_lines(img)
    return True #debug

    # If it's empty, return bool and empty.
    if (not lines):
        print 'Lines not detected'
        return (False, [])

    longest_line = get_longest_line(lines)
    angle  = get_line_angle(longest_line)

    # return that we've found something, and
    return (True, angle)

def get_line_angle(line):
    """ Simple geometric function to get the angle of the previously calculated lines relative to the submarine (which is just to the line (0, 1). Will return an array of angles of the line. """

    return (np.arctan2((line[0] - line[1]), (line[3] - line[2])))

def get_longest_line(lines):
    longest_line = []
    x_len   = 0
    y_len   = 0
    mag     = 0
    new_mag = 0

    for (x1, y1, x2, y2) in lines:
        # compute the length of the wire, compare, then update if it's the longest line.
        # TODO: this might not be the correct calulation. Maybe just care about endpoint angle?
        x_len = x2 - x1
        y_len = y2 - y1
        new_mag = np.absolute[x_len, y_len]
        if mag < (new_mag):
            mag = new_mag
            longest_line = [x1, y1, x2, y2]

    print 'longest vector is', longest_line
    return longest_line



def get_lines(img):

    # crop the image to remove edges ends of the image
    image = img[10:470, 90:550]
    cv2.imshow("cropped image", image)

    # convert to hsv
    hsv_img = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

    # remove noise by applying gaussian blur. the (5,5) specifies a 5x5 kernel,
    # and the 0 sets the sigmaX to the same value.
    gau_hsv_img = cv2.GaussianBlur(hsv_img, (5,5), 0)

    # now threshold anything NOT of this color.
    threshold_hsv = cv2.inRange(gau_hsv_img, ORANGE_LO, ORANGE_HI)
    cv2.imshow("Image orange threshold", threshold_hsv)
    cv2.waitKey(0)

    # TODO: Check for a blank image here. Return empty if so.
    old_hsv = threshold_hsv.copy()

    # get the 'edge' points, the True selects more accurate algorithm.    while(1):
    threshold_hsv = old_hsv.copy()

    min_e = 30  #int(raw_input("min value:"))
    max_e = 300  # int(raw_input("max value:"))
    Canny_edges = cv2.Canny(threshold_hsv, min_e, max_e)

    # returns beginning points and  endpoints of all lines.
    lines = cv2.HoughLinesP(Canny_edges, 1, np.pi/180, 40, 100, 10)
    print lines
    for l in lines[0]:
        cv2.line(threshold_hsv,(l[0], l[1]), (l[2], l[3]), (100,255,0), cv2.CV_AA)

        cv2.imshow("can", threshold_hsv)
        cv2.waitKey(0)

    return lines


if (__name__ == "__main__"):
    main()
