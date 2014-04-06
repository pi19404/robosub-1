#!/usr/bin/env python

__author__ = 'Tyler Stubenvoll, Cameron Evans'
__License__= 'GPLv3 Robosub Club of the Palouse, 2014'

import cv2
import cv2.cv as cv
import numpy as np
from math import atan2
from time import sleep # TODO delete

__author__ = 'Cameron Evans, Tyler Stubenvoll'
__license__= 'GPLv3, Robosub Club of the Palouse 2014'

class FrameProcessor(object):
    """Image processing functions for Robosub VisionProcessor plugins."""

    # true hue constants
    hue_red = 178
    hue_green = 43
    hue_blue = 120
    hue_orange = 16
    hue_yellow = 30
    # TODO Do some interweb researching for the real duct tape hues.

    def __init__(self):
        """Initialize the FrameProcessor."""

        # Minimum height of peak on hue histogram before being considered
        # something of interest.
        # XXX Slope might also be a good way to detect these colors.
        # Investigate.
        self._hue_histogram_beta = 0

        # Hue midpoints will shift in different light. Midpoints for current
        # image should be calculated and stored here.
        self._hue_midpoints = {
            'b': self.hue_blue,
            'g': self.hue_green,
            'r': self.hue_red,
            'y': self.hue_yellow,
            'o': self.hue_orange
        }

        # Create the memoizable data structures.
        self._reset()

    def _reset(self, im=None):
        """Reset any memoized data and set new image.

        Args:

            im - New cv2 image to use. (default: None)

        """

        self.im = im
        self._hsv = None
        # Cool image for streaming.
        self._hud_im = None
        # Used for filtering noise.
        self._eroded_im = None
        # Use to detect objects of interest and hue shifts due to weather.
        self._histogram_hue = None

        # Use to detect presence of high saturation (duct tape).
        self._histogram_saturation = None

        # Use to detect presence of light emitters (LED buoys).
        self._histogram_value = None

        # Bitmasks of pixels with attributes unique to robosub obstacles.
        self._filtered_images = {
            # High saturation colors (colored duct tape).
            'b': None,  # blue
            'g': None,  # green
            'r': None,  # red
            'y': None,  # yellow
            'o': None,  # orange
            # High values colors (colored LED buoys).
            'buoy_b': None,  # blue
            'buoy_r': None,  # red
            'buoy_g': None   # green
        }

        # Channels of BGR and HSV images.
        self._channels = {
            'r': None,
            'b': None,
            'g': None,
            'h': None,
            's': None,
            'v': None
        }
        self.draw_dict = {
            "lines":   [],
            "circles": [],
            "text":    []
        }

        # Floodfilled images.
        self._floodfills = {
            'b': None,
            'g': None,
            'r': None,
            'y': None,
            'o': None
        }

    def load_im(self, im):
        """Load a new image to process.

        Args:

            im - A cv2 BGR image.

        """
        if im is None:
            raise Exception('No image given')

        # Reset the processor and clear out stale image data.
        self._reset(im)

    @staticmethod
    def lineAngle(line):
        theta = -atan2(int(line[2] - line[0]), int(line[3] - line[1]))
        # FIXME, make math good.
        if abs(theta) > cv2.cv.CV_PI / 2:
            theta = -atan2(int(line[0] - line[2]), int(line[1] - line[3]))
        return round(theta, 4)


    def translate_coordinates(self, x, y):
        """ Translates the coordinates into a relative format useful to the
        AI, regardless of the image size. e.g., center of image is
        (0.0,0.0), top center is (0.0,1.0), far left center is (-1.0, 0.0),
        ect.

        Args:

        x - x coord for pixel, where top left represents 0 and far right
            is x_dim max. int or float.

        y - y coord for pixel, where top represents 0 and bottom is y_dim
            max. int or float.

        returns -  a tuple of the new values, as floats.

        """

        x_dim, y_dim = self.im.shape[:2]
        try:
            t_x =  float( (-2.0 / x_dim) * x + 1)
            t_y =  float( (-2.0 / y_dim) * y + 1)
        except ZeroDivisionError:
            # account for empty images, though I doubt we'd get them
            return 0, 0

        return t_x, t_y

    def build_hud(self, img):
        """
        Function builds a "HUD" image that is good for streaming. Modules
        will have to append their values to the list of items they want printed.
        At the moment, it is assumed that the image to be written two is a three channel
        color image.

        Args:

        img - an image that the hud will be printed over. Currently not modified.

        """
        if img is None:
            # note; may not need to clone
            im = self.im.clone()
        else:
            im = img.clone()

        line_list = self.draw_dict.get("lines")
        if not line_list:
            pass
        else:
            for l in line_list:
                cv2.line(im, (l[0][0], l[0][1]), (l[0][2], l[0][3]),
                         l[1], 4, cv2.CV_AA)

        circle_list = self.draw_dict.get("circles")
        if not circle_list:
            pass
        else:
            for c in circle_list:
                cv2.circle(im, c[0], 8, c[1], 2)

        text_list = self.draw_dict.get("text")
        if not text_list:
            pass
        else:
            for t in text_list:
                cv2.putText(im, t[0], t[1], cv2.FONT_HERSHEY_PLAIN, 2, t[2])

        return im

    def erode_image(self, im=None, kernel_size=3, iterations=2):
        """ erodes the image. Useful for cleaning image.

        Args:

            im - the normal im.

            kernel_size - the size of kernel pixel, e.g. 3x3 pixels

            iterations - the number of iterations cv will do.

        """
        if im is None:
            im = self.im
        if self._eroded_im is None:
            kernel = np.ones((kernel_size, kernel_size), np.uint8)
            self._eroded_im = cv2.erode(im, kernel, iterations=iterations)
        return self._eroded_im

    def _get_hsv_channel(self, key):
        """Return hue, saturation, or value channel of current image.

        Args:

            key: 'h', 's', or 'v' for hue, saturation, or value.

        """

        if self._channels[key] is None:
            # Channels haven't been split yet. Memoize them.
            self._channels['h'], self._channels['s'], self._channels['v'] = \
                    cv2.split(self.hsv)
        return self._channels[key]

    def _get_bgr_channel(self, key):
        """Return blue, green, or red channel of current image.

        Args:

            key: 'b', 'g', or 'r' for blue, green, or red.

        """

        if self._channels[key] is None:
            # Channels haven't been split yet. Memoize them.
            self._channels['b'], self._channels['g'], self._channels['r'] = \
                    cv2.split(self.im)
        return self._channels[key]

    @property
    def hud(self):
        if self._hud is None:
            self._hud = self.build_hud(self.im)
        else:
            return self._hud

    @property
    def flooded_red(self):
        """Return mask of large, solid, high-saturation orange objects."""
        return self._memoized_flooded_hue_image('r')

    @property
    def flooded_orange(self):
        """Return mask of large, solid, high-saturation orange objects."""
        return self._memoized_flooded_hue_image('o')

    @property
    def flooded_blue(self):
        """Return mask of large, solid, high-saturation blue objects."""
        return self._memoized_flooded_hue_image('b')

    @property
    def flooded_green(self):
        """Return mask of large, solid, high-saturation green objects."""
        return self._memoized_flooded_hue_image('g')

    @property
    def flooded_yellow(self):
        """Return mask of large, solid, high-saturation yellow objects."""
        return self._memoized_flooded_hue_image('y')

    @property
    def hsv(self):
        """Return hsv of current image."""

        if self._hsv is None:
            self._hsv = cv2.cvtColor(self.im, cv2.COLOR_BGR2HSV)
        return self._hsv

    #ReviewQ: Does this need to have a @property key?
    def _memoized_filtered_hue_image(self, key):
        """Memoize a bitmask near given hue.

        Bitmask includes high-saturation areas that also closely match
        current hue midpoint stored in self._hue_midpoints[key].

        Args:

        key - One of the keys from self._filtered_images dict.

        """
        if self._filtered_images[key] is None:
            # The filtered image hasn't been created yet. Memoize it.
            self._filtered_images[key] = \
                    cv2.bitwise_and(
                        self._filter_hue(self._hue_midpoints[key]),
                        cv2.inRange(self.im_saturation,
                            np.array(170), np.array(255)))

        return self._filtered_images[key]

    def _validate_seed(self, row_i, col_i, key):
        row_offset = self.im.shape[0] / 64
        col_offset = self.im.shape[1] / 64
        pixel_offsets = [
                [0, 1 * col_offset],
                [0, -1 * col_offset],
                [1 * row_offset, 0],
                [-1 * row_offset, 0]]
        hue_pixel = self.hsv.item(row_i, col_i, 0)
        sat_pixel = self.hsv.item(row_i, col_i, 1)
        # Check the pixel to see if it is a worthy seed for flood fill.
        if sat_pixel > 100 and abs(hue_pixel - self._hue_midpoints[key]) < 5:
            print "found worthy seed"
            return True
            for offset in pixel_offsets:
                hue_pixel_neighbor = self.hsv.item(
                        row_i + offset[0], col_i + offset[1], 0)
                sat_pixel_neighbor = self.hsv.item(
                        row_i + offset[0], col_i + offset[1], 1)
                if abs(hue_pixel - hue_pixel_neighbor) > 5 or \
                        abs(sat_pixel - sat_pixel_neighbor) > 5:
                    break
            else:
                return True
        return False

    def _memoized_flooded_hue_image(self, key):
        """Memoize a bitmask near given hue.

        Pixels at a constant interval are checked to see if they make a good
        seed point for the flood fill algorithm. A good seed will be high
        saturation and near the target color range. If the flood fill generates
        a large, gemoetric object, it is accepted as a target object and
        included in the flooded hue image.

        """
        if self._floodfills[key] is None:
            self._floodfills[key] = np.zeros((self.hsv.shape[0], self.hsv.shape[1]),
                    np.uint8)
            # FIXME these two variables need better names. They're the distance
            # to check horizontally/vertically from a potential seed to make
            # sure they're close to the same as the seed.
            # 1/4 the distance between potential seed pixels.
            # Check pixels from (2**4) - 1 row intersections in the image.
            for row_i in range(0, self.im.shape[0], self.im.shape[0] / 16)[1:]:
                # Check pixels from (2**4) - 1 columns intersections in the image.
                for col_i in range(0, self.im.shape[1], self.im.shape[1] / 16)[1:]:
                    if self._validate_seed(row_i, col_i, key):
                        new_mask = np.zeros(
                                (self.im_hue.shape[0], self.im_hue.shape[1]),
                                np.uint8)
                        dummy_mask = np.zeros(
                                (self.im_hue.shape[0] + 2, self.im_hue.shape[1] + 2),
                                np.uint8)
                        write_to = cv2.merge([self.im_hue, new_mask, new_mask])
                        cv2.floodFill(
                                image=write_to,
                                mask=dummy_mask,
                                seedPoint=(col_i, row_i),
                                newVal=(255, 255, 255),
                                loDiff=5,
                                upDiff=5,
                                flags= 4 | cv2.FLOODFILL_FIXED_RANGE)
                        self._floodfills[key] = cv2.bitwise_or(
                                self._floodfills[key], cv2.split(write_to)[1])


        return self._floodfills[key]


    @property
    def filtered_blue(self):
        """Return single-channel image filtered for high-saturation blue."""

        return self._memoized_filtered_hue_image(key='b')

    @property
    def filtered_green(self):
        """Return single-channel image filtered for high-saturation green."""

        return self._memoized_filtered_hue_image(key='g')

    @property
    def filtered_red(self):
        """Return single-channel image filtered for high-saturation red."""

        return self._memoized_filtered_hue_image(key='r')

    @property
    def filtered_orange(self):
        """Return single-channel image filtered for high-saturation orange."""

        return self._memoized_filtered_hue_image(key='o')

    @property
    def filtered_yellow(self):
        """Return single-channel image filtered for high-saturation yellow."""

        return self._memoized_filtered_hue_image(key='y')

    @property
    def im_blue(self):
        """Return blue channel of original image."""

        return self._get_bgr_channel(key='b')

    @property
    def im_green(self):
        """Return green channel of original image."""

        return self._get_bgr_channel(key='g')

    @property
    def im_red(self):
        """Return red channel of original image."""

        return self._get_bgr_channel(key='r')

    @property
    def im_hue(self):
        """Return hue channel of hsv image."""

        return self._get_hsv_channel(key='h')

    @property
    def im_saturation(self):
        """Return saturation channel of hsv image."""

        return self._get_hsv_channel(key='s')

    @property
    def im_value(self):
        """Return value channel of hsv image."""

        return self._get_hsv_channel(key='v')

    def edge_detect(self, im, shift=1):
        """Return image filtered for hard color transitions.

        Args:

            im - Image to detect edges.

        Keyword Args:

            shift - Distance to shift image copies before calculating absolute
            difference with original. (default: 1)

        Note that this the (width, height) of returned image will be reduced by
        (shift, shift).

        """

        x_dim, y_dim = im.shape[:2]

        # Create three cropped images to simulate a pixel shift in two
        # different directions. The direction of the shifts is orthagonal so
        # that any direction of edges is detected.
        im_crop = im[0: x_dim - shift, 0: y_dim - shift]
        im_shift_up = im[0:x_dim - shift, shift: y_dim]
        im_shift_left = im[shift:x_dim, 0: y_dim - shift]
        vertical_detect = cv2.absdiff(im_crop, im_shift_up)
        horizontal_detect = cv2.absdiff(im_crop, im_shift_left)
        #ret = cv2.add(vertical_detect, horizontal_detect)
        #b = cv2.inRange(ret, (0, 0, 0), (255, 255, 255))
        return cv2.add(vertical_detect, horizontal_detect)

    def _filter_hue(self, mid, include_distance=5):
        """Return bitmask of image where hue is within include_distance of mid.

        Args:

            mid - Hue to target. Use (real hue / 2). Example, use 15 (real hue
            is 30) for orange.

            include_distance - Hue distance from mid that should be included in
            bitmask.


        """

        if mid - include_distance < 0:
            # We're calculating a value too near the 0 end of the hue circle.
            # Some values near 180 need to be included in this mask.
            low_side = cv2.inRange(self.im_hue,
                    np.array(0),
                    np.array(mid + include_distance))
            high_side = cv2.inRange(self.im_hue,
                    np.array(180 + (mid - include_distance)), np.array(180))
            return cv2.bitwise_or(low_side, high_side)
        elif mid + include_distance >= 180:
            # We're calculating a value too near the 180 end of the hue circle.
            # Some values near 0 need to be included in this mask.
            low_side = cv2.inRange(self.im_hue,
                    np.array(0 + (mid + include_distance - 180)),
                    np.array(180))
            high_side = cv2.inRange(self.im_hue,
                    np.array(mid - include_distance),
                    np.array(180))
            return cv2.bitwise_or(low_side, high_side)
        return cv2.inRange(self.im_hue,
                np.array(mid - include_distance),
                np.array(mid + include_distance))

    ###########################################################################
    # FUTURE: Implement functions requiring bitmasking edge detection images
    # with hue bitmasks. Denoise and dilate bitmasks to overlap nearby edges.
    # Bitwise_and the mask with the edges to filter for high saturation edges.
    ###########################################################################

    @property
    def features_blue(self):
        """Return mask of edges with of high-saturation blue objects."""

        pass

    @property
    def features_green(self):
        """Return mask of edges with of high-saturation green objects."""

        pass

    @property
    def features_red(self):
        """Return mask of edges with of high-saturation red objects."""

        pass

    @property
    def features_orange(self):
        """Return mask of edges with of high-saturation orange objects."""

        pass

    @property
    def features_yellow(self):
        """Return mask of edges with of high-saturation yellow objects."""

        pass

    ###########################################################################
    # FUTURE: Implement functions requiring use of histograms.
    ###########################################################################

    def _hist_peaks(self, hist):
        """Return list of values where peaks occur in hist.

        Args:

            hist - Histogram to search for peaks.

        """

        pass

    @property
    def hist_hue(self):
        """Return histogram of image hue.

        Only includes pixels that are high saturation.

        """

        # Generate histogram of hue after it is filtered for high saturation
        # pixels only. Also update hue midpoints?

        pass

    @property
    def hist_saturation(self):
        """Return histogram of image saturation."""

        pass

    @property
    def hist_value(self):
        """Return histogram of image value."""

        pass

    @property
    def adjusted_hue_mid_blue(self):
        """Return adjusted blue hue value for current image or None.

        If high saturation blue exists in the image, calculate the midpoint of
        the hue peak. Return None if there is no peak.

        """

        # TODO: Return midpoint of blue hue peak, if it exists.

        pass

    @property
    def adjusted_hue_mid_green(self):
        """Return adjusted green hue value for current image or None.

        If high saturation green exists in the image, calculate the midpoint of
        the hue peak. Return None if there is no peak.

        """

        # TODO: Return midpoint of green hue peak, if it exists.

        pass

    @property
    def adjusted_hue_mid_red(self):
        """Return adjusted red hue value for current image or None.

        If high saturation red exists in the image, calculate the midpoint of
        the hue peak. Return None if there is no peak.

        """

        # TODO: Return midpoint of red hue peak, if it exists.

        pass

    @property
    def adjusted_hue_mid_orange(self):
        """Return adjusted orange hue value for current image or None.

        If high saturation orange exists in the image, calculate the midpoint
        of the hue peak. Return None if there is no peak.

        """

        # TODO: Return midpoint of orange hue peak, if it exists.

        pass

    @property
    def adjusted_hue_mid_yellow(self):
        """Return adjusted yellow hue value for current image or None.

        If high saturation yellow exists in the image, calculate the midpoint
        of the hue peak. Return None if there is no peak.

        """

        # TODO: Return midpoint of yellow hue peak, if it exists.

        pass

    @property
    def has_high_saturation(self):
        """Return whether there is a lot of high saturation in image.

        Use to detect presence of duct tape obstacles.

        """

        # TODO: Detect a small, well-defined peak at the high end of saturation
        # histogram.

        pass

    @property
    def has_high_value(self):
        """Return whether there is a lot of high value in image.

        Use to detect presence of light-emitting (buoy) obstacles.

        """

        # TODO: Detect a small, well-defined peak at the high end of value
        # histogram.

        pass

    @property
    def has_high_saturation_blue(self):
        """Return whether there is a lot of high saturation blue in image."""

        # TODO: Detect a well-defined peak in blue range on hue histogram after
        # filtering for high saturation.

        pass

    @property
    def has_high_saturation_green(self):
        """Return whether there is a lot of high saturation green in image."""

        # TODO: Detect a well-defined peak in green range on hue histogram
        # after filtering for high saturation.

        pass

    @property
    def has_high_saturation_red(self):
        """Return whether there is a lot of high saturation red in image."""

        # TODO: Detect a well-defined peak in red range on hue histogram after
        # filtering for high saturation.

        pass

    @property
    def has_high_saturation_orange(self):
        """Return whether there is a lot of high saturation orange in image."""

        # TODO: Detect a well-defined peak in orange range on hue histogram
        # after filtering for high saturation.

        pass

    @property
    def has_high_saturation_yellow(self):
        """Return whether there is a lot of high saturation yellow in image."""

        # TODO: Detect a well-defined peak in yellow range on hue histogram
        # after filtering for high saturation.

        pass

###############################################################################
# Hack code to be removed.
###############################################################################

    def hacky_display(self):
        cv2.imshow("image", self.im)
        #cv2.imshow('saturation', self.im_saturation)
        #cv2.imshow('yellow', self.filtered_yellow)
        #cv2.imshow('green', self.filtered_green)
        #cv2.imshow('blue', self.filtered_blue)
        #cv2.imshow('red', self.filtered_red)
        #cv2.imshow('orange', self.filtered_orange)
        #cv2.imshow('r', cv2.subtract(self.im_red, self.im_saturation))
        #cv2.imshow('edge_detect', self.edge_detect(self.im, shift = 2))
        high_sat = cv2.inRange(self.im_saturation, np.array(170), np.array(255))
        cv2.imshow('r-b', cv2.inRange(cv2.subtract(self.im_red, self.im_blue), np.array(30), np.array(255)))
        #cv2.imshow('high_sat', high_sat)
        #cv2.imshow('hist_hue', hist_curve(cv2.bitwise_and(self.im, cv2.merge([high_sat, high_sat, high_sat]))))
        #cv2.imshow('hist_sat', hist_curve(self.im_saturation))
        cv2.waitKey(10)
        return

bins = np.arange(256).reshape(256, 1)


def hist_curve(im):
    h = np.zeros((300, 256, 3))
    if len(im.shape) == 2:
        color = [(255, 255, 255)]
    elif im.shape[2] == 3:
        color = [(255, 0, 0), (0, 255, 0), (0, 0, 255)]
    for ch, col in enumerate(color):
        hist_item = cv2.calcHist([im], [ch], None, [256], [0, 255])
        cv2.normalize(hist_item, hist_item, 0, 255, cv2.NORM_MINMAX)
        hist=np.int32(np.around(hist_item))
        pts = np.int32(np.column_stack((bins, hist)))
        cv2.polylines(h, [pts], False, col)
    y=np.flipud(h)
    return y
