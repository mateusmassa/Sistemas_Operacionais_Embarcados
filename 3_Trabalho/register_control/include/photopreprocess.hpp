#ifndef PHOTO_PREPROCESS_HPP_
#define PHOTO_PREPROCESS_HPP_

#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/face/facerec.hpp"
#include "opencv2/core/types_c.h"

#define WEBCAM_ID 0
#define RESOLUTION_WIDTH 640
#define RESOLUTION_HEIGHT 480
#define PHOTOS_QUANTITY 20
#define PHOTOS_BASEPATH "/home/victor/Desktop/register_control/face_database/"
#define TRAINED_MODEL_XML "/home/victor/Documents/github/3FA_Server/trained_model.xml"
#define FRONTAL_FACE_XML "/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_default.xml"
#define EYE_XML "/usr/local/share/opencv4/haarcascades/haarcascade_eye.xml"

void get_photo(std::string id);
static void preprocess(const std::string photo_path);
static void read_photos_csv(const std::string photos_csv, std::vector<cv::Mat>& photos, std::vector<int>& labels);
void train_model(std::string photos_csv);

#endif
