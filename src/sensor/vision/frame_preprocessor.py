#!/usr/bin/env python

"""Detects color bounds of colored duct tape in an OpenCV BGR image."""
# TODO make that true.

import cv2
import cv2.cv as cv
import numpy as np

class FramePreprocessor(object):
    """Preprocess images for Robosub VisionProcessor plugins."""
    def __init__(self):
        pass

    def preprocess(self, im):
        x_dim, y_dim = im.shape[:2]
        im_crop1 = im[0: x_dim - 1, 0: y_dim - 1]
        im_crop2 = im[0:x_dim - 1, 1: y_dim]
        im_crop3 = im[1:x_dim, 0: y_dim - 1]
        b, g, r = cv2.split(im)
        #splits = cv2.split(im)
        # subtract the red channel with itself shifted.
        #crop1 = r[0: x_dim - 1, 0: y_dim - 1]
        #crop2 = r[0:x_dim - 1, 1: y_dim]
        #crop3 = r[1:x_dim, 0: y_dim - 1]

        subbed_blue = cv2.subtract(r, b)
        subbed_green = cv2.subtract(r, g)
        #cv2.imshow("subbed_blue", subbed_blue)
        #cv2.imshow("subbed_green", subbed_green)
        sub1 = cv2.bitwise_or(cv2.subtract(crop1, crop2), cv2.subtract(crop2, crop1))
        sub2 = cv2.bitwise_or(cv2.subtract(crop1, crop3), cv2.subtract(crop3, crop1))
        tot = cv2.add(sub1, sub2)
        cv2.imshow("added", cv2.dilate(tot, None))
        #print(new_im.shape)
        cv2.imshow("image", im)
        cv2.waitKey(10)


