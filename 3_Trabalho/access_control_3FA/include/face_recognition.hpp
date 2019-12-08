#ifndef FACE_RECOGNITION_HPP_
#define FACE_RECOGNITION_HPP_

#include <iostream>
#include <string>
#include <algorithm>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/face/facerec.hpp"

#define WEBCAM_ID 0
#define IMG_WIDTH 480
#define IMG_HEIGTH 480
#define ATTEMPTS_THRESHOLD 30
#define SUCESSFUL_ATTEMPTS_THRESHOLD 15
#define FRONTAL_FACE_XML "/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_default.xml"
#define TRAINED_MODEL_XML "/home/pi/Documents/github/3FA_Server/trained_model.xml"

bool face_recognition(int id);

#endif
