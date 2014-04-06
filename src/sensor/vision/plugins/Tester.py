import cv2
import cv2.cv as cv
from time import sleep

#TODO make a proper template out of this.
class Tester(object):
    def __init__(self, fp, settings):
        """
        Args:
            self - Standard python object reference.
            fp - Frame processor object.
            settings - Stream processor settings.

        """
        self._fp = fp
        self._settings = settings
        pass

    def process_image(self, packet):
        """
        Args:
            self - Standard python object reference.
            packet - Dict to add reply data to.

        """
        sleep(.1)
        #cv2.imshow('test window', self._fp.edge_detect(self._fp.im))
        #cv2.imshow('test window', self._fp.im)
        #cv2.waitKey(10)
