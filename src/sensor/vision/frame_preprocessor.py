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

    def rotate_hue(self, h, degrees):
        return cv2.subtract(h, np.array(degrees))

    def edge_detect(self, im, x_dim, y_dim):
        im_crop1 = im[0: x_dim - 1, 0: y_dim - 1]
        im_crop2 = im[0:x_dim - 1, 1: y_dim]
        im_crop3 = im[1:x_dim, 0: y_dim - 1]
        sub1 = cv2.absdiff(im_crop1, im_crop2)
        sub2 = cv2.absdiff(im_crop1, im_crop3)
        return cv2.add(sub1, sub2)

    def preprocess(self, im):
        cv2.imshow("image", im)
        x_dim, y_dim = im.shape[:2]
        im_crop1 = im[0: x_dim - 1, 0: y_dim - 1]
        im_crop2 = im[0:x_dim - 1, 1: y_dim]
        im_crop3 = im[1:x_dim, 0: y_dim - 1]
        b, g, r = cv2.split(im)
        hsv = cv2.cvtColor(im, cv2.COLOR_BGR2HSV)
        yuv = cv2.cvtColor(im, cv2.COLOR_BGR2YUV)
        y, u, vi = cv2.split(yuv)
        h, s, v = cv2.split(hsv)
        #splits = cv2.split(im)
        # subtract the red channel with itself shifted.
        crop1 = r[0: x_dim - 1, 0: y_dim - 1]
        crop2 = r[0:x_dim - 1, 1: y_dim]
        crop3 = r[1:x_dim, 0: y_dim - 1]

        edges = self.edge_detect(im, x_dim, y_dim)

        subbed_blue = cv2.subtract(r, b)
        subbed_green = cv2.subtract(r, g)
        r_gb = cv2.bitwise_or(cv2.subtract(r, b), cv2.subtract(r, g))
        #cv2.imshow('test', r_gb)
        orange = cv2.subtract(cv2.bitwise_and(cv2.subtract(r, b), cv2.subtract(r, g)), h)
        #orange = cv2.inRange(h, np.array(0), np.array(15))
        #blue = cv2.subtract(cv2.bitwise_and(cv2.subtract(b, r), cv2.subtract(b, g)), h - 105)
        blue = cv2.max(cv2.bitwise_and(cv2.subtract(b, r), cv2.subtract(b, g)), h + 75)
        blue = cv2.inRange(h, np.array(90), np.array(120))

        red = cv2.subtract(cv2.bitwise_and(cv2.subtract(r, b), cv2.subtract(r, g)), h + 15)
        red = cv2.bitwise_and(edges, cv2.merge([red, red, red])[0: x_dim - 1, 0: y_dim - 1])
        #red = cv2.max(cv2.bitwise_and(cv2.subtract(r, b), cv2.subtract(r, g)), h - 15)
        green = cv2.subtract(cv2.bitwise_and(cv2.subtract(g, b), cv2.subtract(g, r)), h - 45)
        yellow = cv2.subtract(cv2.bitwise_and(
                    cv2.bitwise_or(cv2.subtract(r, b), cv2.subtract(r, g)),
                    cv2.bitwise_or(cv2.subtract(g, b), cv2.subtract(g, r))), h - 15)


        #cv2.imshow('blue', blue)
        #cv2.imshow('orange', orange)
        #cv2.imshow('green', green)
        cv2.imshow('red', red)
        #cv2.imshow('yellow', yellow)
        #cv2.imshow('blue_low', h - 105)
        #cv2.imshow('orange_low', h)
        #cv2.imshow('green_low', h - 45)
        #cv2.imshow('red_low', h + 15)
        #cv2.imshow('yellow_low', h - 15)
        #cv2.imshow("subbed_green", subbed_green)
        sub1 = cv2.absdiff(im_crop1, im_crop2)
        sub2 = cv2.absdiff(im_crop1, im_crop3)
        tot = cv2.add(sub1, sub2)
        tot2 = cv2.split(tot)
        #cv2.imshow("added", cv2.dilate(tot2[2], None))
        red_minus_sat = cv2.subtract(r, s)
        #cv2.imshow("linedetect", tot)
        #cv2.imshow("orange_minus_hue", cv2.inRange(cv2.subtract(orange, cv2.add(cv2.add(h, b), g)), np.array(50), np.array(255)))
        #cv2.imshow("b", b)
        #cv2.imshow("g", g)
        #cv2.imshow("r", r)
        #cv2.imshow("h", h)
        #cv2.imshow("s", s)
        #cv2.imshow("v", v)
        #cv2.imshow("y", y)
        #cv2.imshow("u", u)
        #cv2.imshow("vi", vi)
        #cv2.imshow("red_minus_sat", red_minus_sat)
        #cv2.imshow("r_minus_b", subbed_blue)
        #hist = hist_curve(h)
        #cv2.imshow('hist', hist)
        #cv2.imshow('yuv', yuv)
        #cv2.imshow('bgr', im)

        cv2.waitKey(10)
        return {'yellow': yellow,
                'green': green,
                'blue': blue,
                'red': red,
                'orange': orange}

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
