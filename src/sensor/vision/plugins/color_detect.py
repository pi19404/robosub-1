#!/usr/bin/env python

"""Detects color bounds of colored duct tape in an OpenCV BGR image."""
# TODO make that true.

import cv2
import cv2.cv as cv
import numpy as np
import matplotlib.pyplot as plt
import rof
from time import time

#class ColorDetect(object):
    #def __init__(self):
def plot1():
    print 'hi'
    pic = cv2.imread('test_im2.jpg')
    ws = cv2.watershed(pic)
    cv2.imwrite('watershed.jpg', ws)
    exit(1)

    # Separate into three arrays, one for each color channel.
    channels = cv2.split(pic)

    for i, c in enumerate(channels):
        # Reshape array to be a single long row.
        c.reshape(c.size)
        #np.savetxt('channel{num}.csv'.format(num=i), c, fmt="%3d", delimiter=',')


    # Make histograms, one for each color channel.
    print len(channels)
    histograms = [np.histogram(c, bins=256) for c in channels]

    print len(histograms)
    print len(channels)
    c = np.histogram(channels[0], bins=256)

    print c[0].shape

    plt.plot(np.array(range(256)), histograms[0][0])
    plt.show()
    plt.plot(np.array(range(256)), histograms[1][0])
    plt.show()
    plt.plot(np.array(range(256)), histograms[2][0])
    plt.show()

if __name__ == '__main__':
    begin = time()
    pic = cv2.imread('test_im.jpg')
    pic = cv2.cvtColor(pic, cv2.COLOR_BGR2HSV)
    pic = cv2.resize(pic, (120, 80))
    channels = cv2.split(pic)
    for i, c in enumerate(channels):
        U,T = rof.denoise(c, c, tolerance=0.1)
        t = 0.4
        #import scipy.misc
        #cv2.imwrite('result2{index}.jpg'.format(index=i), U)
        #scipy.misc.imsave('result{index}.jpg'.format(index=i), U < t*U.max())
    print time() - begin



