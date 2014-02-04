#!/usr/bin/env python

__author__ = 'Tyler Stubenvoll, Cameron Evans'
__license__ = 'GPLv3, Robosub Club of the Palouse 2014'

import sys
sys.path.append("..")
import FrameProcessor as fp
import cv2
import cv2.cv as cv
from math import sqrt
from math import pow
import numpy as np


class Path(object):
    _debug = 0
    _tools = 0

    def __init__(self, debug):
        self._debug = debug
        self._tools = FrameProcessor()

    # Calculates the longest line from the set of lines, returns index of line
    # should be proceeded by a check to see if the set is empty.
    def get_longest_line(self, line_set):
        index = 0
        count = 0
        prev_length = 0
        next_length = 0

        # Review Q: perhaps change to a throw statement?
        #if len(line_set) == 0 or line_set == None:
        if not line_set:
            return -1

        # length = sqrt(x^2 + y^2)
        for line in line_set[0]:
            x_dif = line[2] - line[0]
            y_dif = line[3] - line[1]
            next_length = sqrt(pow(x_dif, 2) + pow(y_dif, 2))
            if next_length > prev_length:
                index = count
                prev_length = next_length
            count += 1

        return index

    # given a single line, finds the mid point using midpoint form
    # NOTE: This won't find the midpoint of the shape, but of the line
    def line_center(self, line):

        return np.add(line[0], line[2]) / 2, np.add(line[1], line[3])/2

    # finds if two lines will intersect in the plane.
    # REVIEWQ: Should this be moved into frame_processor?
    def line_intersect(self, line1, line2):
        line1f = np.float32(line1)
        line2f = np.float32(line2)
        m1flag, m2flag = 0
        b1 = 0
        b2 = 0

        try:
            m1 = (line1f[3] - line1f[1]) / (line1f[2] - line1f[0])
        except ZeroDivisionError:
            m1flag = True
            m1 = 0
        try:
            m2 = (line2f[3] - line2f[1]) / (line2f[2] - line2f[0])
        except ZeroDivisionError:
            m2flag = True
            m2 = 0

        # two vertical lines. They'll never intersect
        if m1flag and m2flag:
            return None
        # single vertical line.
        elif m1flag:
            b2 = line2f[1] - m2 * line2f[0]
            return line1f[0], line1f[0] * m2 + b2
        elif m2flag:
            b1 = line1f[1] - m1 * line1f[0]
            return line2f[0], line2f[0] * m1 + b1
        # parallel lines will never meet.
        elif m1 == m2:
            return None
        # two normal lines
        else:
            b1 = line1f[1] - m1 * line1f[0]
            b2 = line2f[1] - m2 * line2f[0]
            x = (b2 - b1) / (m1 - m2)
            y = m2 * x + b2
            return x, y

    # draws out the entire lineset.
    def debug_draw_line_set(self, img, line_set):
        count = 0
        for line in line_set[0]:
            count += 1
            cv2.line(img, (line[0], line[1]), (line[2], line[3]), (0, 0, 255), 1, cv2.CV_AA)

        cv2.imshow("lines drawn.", img)
        cv2.waitKey()

        print "DEBUG: Number of lines is: ", count

    def divide_angle_set(self, line_set, angle_set, longest_index):
        longest = angle_set[longest_index]
        diff = 0
        angle_set1 = []
        angle_set2 = []
        line_set1 = [[]]
        line_set2 = [[]]

        # if the lines deviate, sort them into set 2, else 1.
        for i, rad in enumerate(angle_set):
            diff = abs(rad - longest)
            if diff > cv.CV_PI / 18:
                line_set2[0].append((line_set[0][i]))
                angle_set2.append(round(rad, 5))
            else:
                line_set1[0].append(line_set[0][i])
                angle_set1.append(round(rad, 5))

            #print "DEBUG: iter: ", i, "angle: ", round(rad, 6)

        return angle_set1, angle_set2, line_set1, line_set2

    # main processor
    def task_path(self, in_img):
        # crop black bars.
        img = in_img[10:470, 90:550]

        # get the edges of every outline.
        edges = self._tools.simple_edge(img)

        # remove all things without red, then smooth image for line detection
        edges = cv2.inRange(edges, (0, 0, 15), (255, 255, 255))
        edges = cv2.GaussianBlur(edges, (5, 5), 0)

        # REVIEWQ: This might not be needed. Discuss validity?
        edges = self._tools.erode_image(edges, 2, 2)
        cv2.imshow("eroded drawn.", edges)
        cv2.waitKey()

        # finds lines above certain length.
        lines = cv2.HoughLinesP(image=edges, rho=5, theta=cv.CV_PI/180, threshold=40, minLineLength=100, maxLineGap=1)

        if not lines:
            print "no lines found"
            return None

        # calculate angles of the entire set.
        angle_set = []
        for l in lines[0]:
            angle = self._tools.lineAngle(l)
            angle_set.append(angle)

        # find the longest line in the set, which should be the most accurate
        longest_index = self.get_longest_line(lines)

        # divide into two sets
        angle_set1, angle_set2, line_set1, line_set2 = self.divide_angle_set(lines, angle_set, longest_index)

        # self.debug_draw_line_set(img, line_set1)
        # self.debug_draw_line_set(img, line_set2)
        if not angle_set2:
            # only a single line is found. Find the center of that line.
            center = self.line_center(lines[0][longest_index])
            return {"angle1": self._tools.lineAngle(lines[0][longest_index], 			"angle2": None, "center": center}
        else:
            longest_line1 = lines[0][longest_index]
            longest_line2 = self.get_longest_line(line_set2)
            center = self.line_intersect(longest_line1, longest_line2)
	    return {"angle1": self._tools.lineAngle(lines[0][longest_index],
		    "angle2": self._tools.lineAngle(lines_set2[0][longest_line2],
		    "center": center}
        # return dictionary of angle, location of image center on camera's image.
	
def main():
    print "this main is for debugging purposes only."
    dimg = cv2.imread("testimage.jpg", cv2.CV_LOAD_IMAGE_COLOR)
    cv2.imshow("test image", dimg)
    cv2.waitKey()
    vtest = Path(1)
    vtest.task_path(dimg)

if __name__ == "__main__":
    main()