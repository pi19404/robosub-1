import cv2
import cv2.cv as cv


class VideoLogger(object):
	def __init__(self):
		self.Filename = "video_output"
		self.Fourcc = cv.CV_FOURCC('M', 'J', 'P', 'G')
		self.Fps = 10
		self.Width = 640
		self.Height = 480
		self.FrameSize = (Width, Height)
		self.videoWriter = cv2.VideoWriter().open(Filename = filename, fourcc = Fourcc, fps = Fps, frameSize = FrameSize)
		self.image = cv2.VideoCapture()
		self.camera = 0

		# status is a variable that can be used for error passing
		self.status = 0

	# Grabs an image from the camera
	def RecieveImage(self):
		# TODO: set the status variable depending on outcome of attempting to grab an image.
		self.image = cv2.VideoCapture(self.camera)

	# Writes the current image to file (the filename is set in __init__():)
	def WriteImage(self):
		# TODO: make this safer so we arent trying to write a bogus image.
		self.videoWriter.write(self.image)

if __name__ == '__main__':
	videoLogger = VideoLogger()
	while(1):
		videoLogger.RecieveImage()
		videoLogger.WriteImage()