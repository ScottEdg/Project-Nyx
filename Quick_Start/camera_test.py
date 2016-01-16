import time
import picamera

with picamera.PiCamera() as camera: #this line implies thae close() is called at the end
	camera.start_preview()
	time.sleep(10)
	camera.stop_preview()

