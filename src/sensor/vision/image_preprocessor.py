#!/usr/bin/env python

"""Detects color bounds of colored duct tape in an OpenCV BGR image."""
# TODO make that true.

import cv2
import cv2.cv as cv
import numpy as np
import matplotlib.pyplot as plt
from time import time

class Preprocessor(object):
    """Preprocess images for Robosub VisionProcessor plugins."""
    def __init__(self):
        print 'hi'

    def preprocess(self, im):
        splits = cv2.split(im)
        x_dim = splits[2].shape[0]
        y_dim = splits[2].shape[1]
        # subtract the red channel with itself shifted.
        #cropped1 = splits[2][0:0, x_dim - 40: y_dim - 40]
        cropped1 = splits[2][0: x_dim - 1, 0: y_dim - 1]
        cropped2 = splits[2][0:x_dim - 1, 1: y_dim]
        cropped3 = splits[2][1:x_dim, 0: y_dim - 1]
        #cv2.imshow("roi1", cropped1)
        #cv2.imshow("roi2", cropped2)
        sub1 = cv2.bitwise_or(cv2.subtract(cropped1, cropped2), cv2.subtract(cropped2, cropped1))
        sub2 = cv2.bitwise_or(cv2.subtract(cropped1, cropped3), cv2.subtract(cropped3, cropped1))
        tot = cv2.add(sub1, sub2)
        cv2.imshow("added", cv2.dilate(tot, None))
        #print(new_im.shape)
        cv2.imshow("image", im)
        cv2.waitKey(10)


#class ColorDetect(object):
    #def __init__(self):
#def plot1():
#    print 'hi'
#    pic = cv2.imread('test_im2.jpg')
#    ws = cv2.watershed(pic)
#    cv2.imwrite('watershed.jpg', ws)
#    exit(1)
#
#    # Separate into three arrays, one for each color channel.
#    channels = cv2.split(pic)
#
#    for i, c in enumerate(channels):
#        # Reshape array to be a single long row.
#        c.reshape(c.size)
#        #np.savetxt('channel{num}.csv'.format(num=i), c, fmt="%3d", delimiter=',')
#
#
#    # Make histograms, one for each color channel.
#    print len(channels)
#    histograms = [np.histogram(c, bins=256) for c in channels]
#
#    print len(histograms)
#    print len(channels)
#    c = np.histogram(channels[0], bins=256)
#
#    print c[0].shape
#
#    plt.plot(np.array(range(256)), histograms[0][0])
#    plt.show()
#    plt.plot(np.array(range(256)), histograms[1][0])
#    plt.show()
#    plt.plot(np.array(range(256)), histograms[2][0])
#    plt.show()
#
#
#if __name__ == '__main__':
#    pic = cv2.imread('test_im.jpg')
#    #pic = cv2.cvtColor(pic, cv2.COLOR_BGR2HSV)
#    channels = cv2.split(pic)
#    print len(channels)
#    rbcmp = cv2.compare(channels[2], channels[0], cv2.CMP_GT)
#    rbcmp.reshape(rbcmp.size)
#    print rbcmp.shape
#    rgcmp = cv2.compare(channels[2], channels[1], cv2.CMP_GT)
#    cv2.imshow('hi', rgcmp)
#    rgcmp.reshape(rgcmp.size)
#    print rgcmp.shape
#    result = cv2.bitwise_and(rbcmp, rgcmp)
#    result.reshape((pic.shape[0], pic.shape[1]))
#    print result.shape
#    cv2.imshow('hi', result)
#    cv2.waitKey(0)
#    
