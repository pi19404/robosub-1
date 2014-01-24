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
        hsv = cv2.cvtColor(im, cv.CV_BGR2HSV)
        h, s, v = cv2.split(hsv)
        #splits = cv2.split(im)
        # subtract the red channel with itself shifted.
        crop1 = r[0: x_dim - 1, 0: y_dim - 1]
        crop2 = r[0:x_dim - 1, 1: y_dim]
        crop3 = r[1:x_dim, 0: y_dim - 1]

        subbed_blue = cv2.subtract(r, b)
        subbed_green = cv2.subtract(r, g)
        #cv2.imshow("subbed_blue", subbed_blue)
        #cv2.imshow("subbed_green", subbed_green)
        sub1 = cv2.bitwise_or(cv2.subtract(im_crop1, im_crop2), cv2.subtract(im_crop2, im_crop1))
        sub2 = cv2.bitwise_or(cv2.subtract(im_crop1, im_crop3), cv2.subtract(im_crop3, im_crop1))
        tot = cv2.add(sub1, sub2)
        #cv2.imshow("added", cv2.dilate(tot, None))
        #print(new_im.shape)
        #cv2.imshow("linedetect", tot)
        #cv2.imshow("image", im)
        #cv2.imshow("b", b)
        #cv2.imshow("g", g)
        #cv2.imshow("r", r)
        #cv2.imshow("h", h)
        hist = hist_curve(im)
        cv2.imshow('bgr', im)
        cv2.imshow('hist', hist)
        #cv2.imshow("s", s)
        #cv2.imshow("v", v)

        cv2.waitKey(10)

bins = np.arange(256).reshape(256, 1)
def hist_curve(im):
    h = np.zeros((300,256,3))
    if len(im.shape) == 2:
        color = [(255,255,255)]
    elif im.shape[2] == 3:
        color = [ (255,0,0),(0,255,0),(0,0,255) ]
    for ch, col in enumerate(color):
        hist_item = cv2.calcHist([im],[ch],None,[256],[0,255])
        cv2.normalize(hist_item,hist_item,0,255,cv2.NORM_MINMAX)
        hist=np.int32(np.around(hist_item))
        pts = np.int32(np.column_stack((bins,hist)))
        cv2.polylines(h,[pts],False,col)
    y=np.flipud(h)
    return y
