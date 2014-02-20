import cv2
import numpy as np
from random import randint
from math import *

# test image: http://imgur.com/4pWOTKq.jpg

# The high and low HSV values for orange
ORANGE_LO  = np.array([0, 100, 200], np.uint8)
ORANGE_HI  = np.array([30, 255, 255], np.uint8)
#ORANGE_HI  = np.array([255, 255, 255], np.uint8)
#ORANGE_LO  = np.array([30, 50, 50], np.uint8)

class PathDetection(object):
    def __init__(self):
        pass

    def process_image(self, image):
        lines = self.get_lines(image)

        # If it's empty, return bool and empty.
        if (lines is None):
            #print 'Lines not detected'
            return (None, None)

        longest = self.get_longest_line(lines)
        angle   = self.get_line_angle(longest)

        # now modify an image to return
        c_image   = image[10:470, 90:550]
        hsv_img = cv2.cvtColor(c_image, cv2.COLOR_BGR2HSV)

        lines_detected = 0
        for l in lines:
            lines_detected = lines_detected + 1
            line_color     = (180, 0, 0)
            cv2.line(hsv_img, (l[0], l[1]), (l[2], l[3]), line_color, 4, cv2.CV_AA)
            cv2.circle(hsv_img, (l[2], l[3]), 10,  line_color, 2, cv2.CV_AA)
        #print 'angle is:',angle, 'rads, degrees:', degrees(angle)

        # draw last line.
        cv2.line(hsv_img, (longest[0], longest[1]), (longest[2], longest[3]), (180, 180, 180), 6, cv2.CV_AA)
        cv2.circle(hsv_img, (longest[2], longest[3]), 20,  (180, 180, 180), 2, cv2.CV_AA)
        cv2.imwrite("path_out.jpg", hsv_img)

        return ({"angle": angle}, hsv_img)

    def get_line_angle(self, line):

        theta = -atan2(int(line[2]-line[0]), int(line[3] - line[1]))

        # Fix me, make math good.
        if(abs(theta) > pi/2):
            theta = -atan2(int(line[0]-line[2]), int(line[1]-line[3]))
        return theta

    def get_longest_line(self, lines):
        longest_line = []
        x_len   = 0
        y_len   = 0
        mag     = 0
        new_mag = 0
        l_index = 0

        for (x1, y1, x2, y2) in lines:
            # compute the length of the wire, compare, then update if it's the longest line.
            # TODO: this might not be the correct calulation. Maybe just care about endpoint angle?
            x_len = int(x2 - x1)
            y_len = int(y2 - y1)

            new_mag = sqrt(x_len * x_len + y_len * y_len)
            if mag < (abs(new_mag)):
                mag = abs(new_mag)
                longest_line = [x1, y1, x2, y2]
            l_index = l_index+1
        #print 'longest vector is', longest_line
        return longest_line



    def get_lines(self, img):

        # crop the image to remove edges ends of the image
        image = img[10:470, 90:550]

        # convert to hsv
        hsv_img = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

        # remove noise by applying gaussian blur. the (5,5) specifies a 5x5
        # kernel, and the 0 sets the sigmaX to the same value.
        gau_hsv_img = cv2.GaussianBlur(hsv_img, (5,5), 0)

        # now threshold anything NOT of this color.
        threshold_hsv = cv2.inRange(gau_hsv_img, ORANGE_LO, ORANGE_HI)

        # TODO: Check for a blank image here. Return empty if so.
        min_e = 50
        max_e = 100
        Canny_edges = cv2.Canny(threshold_hsv, min_e, max_e,apertureSize=5, L2gradient=True)
        g_Canny_edges = cv2.GaussianBlur(Canny_edges, (5,5), 0)
        #cv2.imshow("brotime", g_Canny_edges)
        #cv2.waitKey(20)

        # returns beginning points and  endpoints of all lines.
        minLineLength = 160
        maxLineGap    = 50
        rho           = 1
        theta         = pi/180
        h_threshold   = 20
        #print type(g_Canny_edges)
        lines = cv2.HoughLinesP(
                    image=g_Canny_edges, 
                    rho=rho, 
                    theta=theta, 
                    threshold=h_threshold, 
                    minLineLength=minLineLength, 
                    maxLineGap=maxLineGap)

        # if nothing is detected.
        if(lines is None):
            return None

        return lines[0]


if (__name__ == "__main__"):
    main()
