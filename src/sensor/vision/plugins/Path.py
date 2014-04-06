#!/usr/bin/env python

__author__ = 'Tyler Stubenvoll, Cameron Evans'
__license__ = 'GPLv3, Robosub Club of the Palouse 2014'

import cv2
import cv2.cv as cv
from math import sqrt
from math import pow
import numpy as np

#uncomment these if you intend to run the module separately with main()
import os, sys
tool_path = os.path.abspath('..')
sys.path.append(tool_path)
from frame_processor import FrameProcessor


class Path(object):
    """
    This module finds line markers, and if they exists, returns 1 or 2 angles relative to the sub.
    """

    def __init__(self, tools, settings):
        self._tools = tools

    @staticmethod
    def get_longest_index(line_set):
        """
        Returns index to longest line in a set of lines. Calculates the longest
        line from the set of lines, returns index of line should be proceeded by
        a check to see if the set is empty.

        args
        line_set: a set of lines, in the form of line[0][set]
                  the set element being of form [x1, y1, x2, y2]

        """
        index = 0
        count = 0
        prev_length = 0
        next_length = 0

        if not len(line_set) == 0 or line_set == None:
            return -1

        # length = sqrt(x^2 + y^2)
        for line in line_set:
            x_dif = line[2] - line[0]
            y_dif = line[3] - line[1]
            next_length = sqrt( pow(x_dif, 2) + pow(y_dif, 2) )
            if next_length > prev_length:
                index = count
                prev_length = next_length
            count += 1

        return index

    @staticmethod
    def line_center(line):
        """
        Returns center point of a line using midpoint formula.

        args
        line: a line in form of [x1, y1, x2, y2]
        """

        return np.add(line[0], line[2]) / 2, np.add(line[1], line[3]) / 2


    # finds if two lines will intersect in the plane.
    @staticmethod
    def line_intersect(line1, line2):
        """
        Returns the intercept point line using slope-intercept
        formula.

        args
        line1: a line in form of [x1, y1, x2, y2]
        line2: a line in form of [x1, y1, x2, y2]
        """
        line1f = np.float32(line1)
        line2f = np.float32(line2)
        m1flag = False
        m2flag = False
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
            try:
                # FIXME this needs to be handled properly.
                x = int(x)
                y = int(y)
            except ValueError:
                # np NaN error.
                return 0, 0
            return x, y

    # draws out the entire lineset.
    @staticmethod
    def debug_draw_line_set(im, line_set):
        """
        Debug function that if there are lines, will draw them out on the original image

        args
        img: a numpy array form of an image
        line_set: a line set of lines
        """
        # clone so we don't modify the original image accidentally
        img = im.clone()
        count = 0
        for line in line_set:
            count += 1
            cv2.line(img, (line[0], line[1]), (line[2], line[3]), (0, 0, 255), 1, cv2.CV_AA)

        cv2.imshow("lines drawn.", img)
        cv2.waitKey()
        print "DEBUG: Number of lines is: ", count

    @staticmethod
    def divide_angle_set(line_set, angle_set, longest_index):
        """
        takes a list of lines and divides them into two
        two separate lists based on a 10 degree (pi/18) difference

        args
        line_set: a list of lines.
        angle_set: a list of angles, same indexing as line_set/
        longest_index: an int value of longest line in line_set.
                       the longest is used as the reference for dividing set.

        """
        longest = angle_set[longest_index]
        diff = 0
        angle_set1 = []
        angle_set2 = []
        line_set1  = []
        line_set2  = []

        # if the lines deviate, sort them into set 2, else 1.
        for i, rad in enumerate(angle_set):
            diff = abs(rad - longest)
            if diff > cv.CV_PI / 18:
                line_set2.append(line_set[i])
                angle_set2.append( round(rad, 5) )
            else:
                line_set1.append(line_set[i])
                angle_set1.append( round(rad, 5) )

        return angle_set1, angle_set2, line_set1, line_set2

    # main processor
    def process_image(self, packet):
        """
        calculates the angle of the sub relative to lines seen
        via the camera. If those angles exist, they are added to packet.

        args
        Packet: a dictionary where the two line angles will be output.
        """
        # crop the black that our camera seems to send so edge_detect doesn't bork.
        img = self._tools.im
        edges = self._tools.edge_detect(img)

        # remove all things without red, then smooth image for line detection
        edges = cv2.inRange(edges, (0, 0, 15), (255, 255, 255))
        # FIXME: cannot determine exactly the nature of this second inRange.
        #        potentially, this could accidentally remove out lines due to their
        #        low intensity. This should be tested and compared.
        #edges = cv2.inRange(edges, np.array(15), np.array(255))
        edges = cv2.GaussianBlur(edges, (5, 5), 0)
        edges = self._tools.erode_image(edges, 2, 2)

        # finds lines above certain length.
        line_hough = cv2.HoughLinesP(image=edges,
                                rho=5,
                                theta=cv.CV_PI/180,
                                threshold=40,
                                minLineLength=100,
                                maxLineGap=1)

        try:
            lines = line_hough[0]
            # lines is a ndnumpy array. "if not lines" will not work
            if len(lines) == 0 or lines == None:
                # FIXME: We should create a system of reporting errors, or behavior.
                print lines
                return
        except (AttributeError, TypeError):
            return

        # calculate angles of the entire set, and parse them into a new list.
        angle_set = []
        for l in lines:
            print l
            angle = self._tools.lineAngle(l)
            print angle
            angle_set.append(angle)

        # find the longest line in the set, which should be the most accurate
        longest_index = self.get_longest_index(lines)

        # divide into two sets
        angle_set1, angle_set2, line_set1, line_set2 = self.divide_angle_set(lines,
                                                                             angle_set,longest_index)

        if not angle_set2 or not angle_set1:
            # only a single line is found. Find the center of that line.
            center_x, center_y = self.line_center(lines[longest_index])
            center_x, center_y = self._tools.translate_coordinates(center_x, center_y)
            packet["VisionPath"] = {"angle1": self._tools.lineAngle(lines[longest_index]),
                                    "angle2": None, "center": (center_x, center_y)}

        else:
            longest_line1 = lines[longest_index]
            longest_index2 = self.get_longest_index(line_set2)
            longest_line2 = line_set2[longest_index2]
            center_x, center_y = self.line_intersect(longest_line1, longest_line2)
            center_x, center_y = self._tools.translate_coordinates(center_x, center_y)

            packet["VisionPath"] = {"angle1": self._tools.lineAngle(lines[longest_index]),
                                    "angle2": self._tools.lineAngle(line_set2[longest_index2]),
                                    "center": (center_x, center_y)}

def main():
    """
    This main is for debugging purposes. You will need to uncomment some of the includes
    up top to make this work, as well as have an image for testing in the local directory.
    """

    dimg  = cv2.imread("testimage.jpg", cv2.CV_LOAD_IMAGE_COLOR)
    toolset = FrameProcessor()
    toolset.load_im(dimg)
    vtest = Path(toolset, {})
    set = {}
    vtest.process_image(set)

    print set

if __name__ == "__main__":
    main()
