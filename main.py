from scipy.spatial import distance as dist
from imutils import face_utils
import numpy as np
import imutils
import time
import dlib
import cv2

import socket             #socket programming   
sock = socket.socket()
host = "192.168.43.19" #ESP32 IP in local network
port = 5555          #ESP32 Server Port    
sock.connect((host, port))
message = "drowsy"

def eye_ratio(eye):
	A = dist.euclidean(eye[1], eye[5])
	B = dist.euclidean(eye[2], eye[4])
	C = dist.euclidean(eye[0], eye[3])
	ratio = (A + B) / (2.0 * C)
	return ratio
	
Eye_ratio_threshold = 0.30
Eye_ratio_frames = 50
counter = 0

detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor("dlib_facial_dots.dat")

(lStart, lEnd) = face_utils.FACIAL_LANDMARKS_IDXS["left_eye"]
(rStart, rEnd) = face_utils.FACIAL_LANDMARKS_IDXS["right_eye"]

video = cv2.VideoCapture(0)

while True:
	ret, frame = video.read()
	frame = imutils.resize(frame, width=1000)
	#frame = imutils.rotate(frame, 270)
	gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

	rects = detector(gray, 0)

	for rect in rects:
		
		shape = predictor(gray, rect)
		shape = face_utils.shape_to_np(shape)

		leftEye = shape[lStart:lEnd]
		rightEye = shape[rStart:rEnd]
		left_ratio = eye_ratio(leftEye)
		right_ratio = eye_ratio(rightEye)
		
		current_ratio = (left_ratio + right_ratio) / 2.0
		
		leftEyeHull = cv2.convexHull(leftEye)
		rightEyeHull = cv2.convexHull(rightEye)
		cv2.drawContours(frame, [leftEyeHull], -1, (0, 255, 0), 1)
		cv2.drawContours(frame, [rightEyeHull], -1, (0, 255, 0), 1)
		
		if current_ratio < Eye_ratio_threshold:
			counter = counter + 1
			
			if counter >= Eye_ratio_frames:
				cv2.putText(frame, "Fatigue Detected!", (10, 30),cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
				print("OYEEE") #changes
				sock.send(message.encode()) #changes
				
		
		else:
			counter = 0
			#sendRequest(root_url+"/CLOSE_LED")
	
		cv2.putText(frame, "RATIO: {:.3f}".format(current_ratio), (300, 30),cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 0, 0), 2)

	cv2.imshow("Frame", frame)
	key = cv2.waitKey(1) & 0xFF
	if key == ord("q"):
		break

cv2.destroyAllWindows()
video.stop()

