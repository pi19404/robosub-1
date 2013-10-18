#!/usr/bin/env python
import cv2
import cv2.cv as cv


class VideoLogger(object):
	def __init__(self):
		self.Filename = "video_output.avi"
		self.Fourcc = cv.CV_FOURCC('M', 'J', 'P', 'G')
		self.Fps = 10
		self.Width = 640
		self.Height = 480
		self.FrameSize = (self.Width, self.Height)
		self.videoWriter = cv2.VideoWriter()
		self.videoWriter.open(filename = self.Filename, 
				      fourcc = self.Fourcc, 
				      fps = self.Fps, 
				      frameSize = self.FrameSize)
		self.camera = 0
		self.cap = cv2.VideoCapture(self.camera)
		self.image = None

		# status is a variable that can be used for error passing
		self.status = 0

	# Grabs an image from the camera
	def RecieveImage(self):
		# TODO: set the status variable depending on outcome of attempting to grab an image.
		_, self.image = self.cap.read()

	# Writes the current image to file (the filename is set in __init__():)
	def WriteImage(self):
		# TODO: make this safer so we arent trying to write a bogus image.
		self.videoWriter.write(self.image)

if __name__ == '__main__':
	videoLogger = VideoLogger()
	while(1):
		videoLogger.RecieveImage()
		videoLogger.WriteImage()
		cv2.imshow('video test', videoLogger.image)
		key = cv2.waitKey(10)
		if key == 27:
			break	
