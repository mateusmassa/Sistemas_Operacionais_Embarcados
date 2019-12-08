#include "face_recognition.hpp"

using namespace cv;
using namespace cv::face;
using namespace std;

bool face_recognition(int id)
{
  VideoCapture camera(WEBCAM_ID);
  
  if(!camera.isOpened()) {
    cerr << "Capture device ID " << WEBCAM_ID;
    cerr << " cannot be opened!" << endl;
    exit(1);
  }
  
  Mat frame;
  unsigned int attempts = 0;
  unsigned int sucessful_attempts = 0;
  CascadeClassifier frontal_face_detector;
  Ptr<FisherFaceRecognizer> model = FisherFaceRecognizer::create();
  
  model->read(TRAINED_MODEL_XML);
  frontal_face_detector.load(FRONTAL_FACE_XML);
  while(attempts < ATTEMPTS_THRESHOLD) {
    vector<Rect_<int>> faces;
    Mat gray_img;
    
    camera >> frame;
    Mat original_img = frame.clone();
    cvtColor(original_img, gray_img, COLOR_BGR2GRAY);
    frontal_face_detector.detectMultiScale(gray_img, faces);
    for (unsigned int i = 0; i < faces.size(); i++) {
      Rect face_i = faces[i];
      Mat face = gray_img(face_i);
      Mat face_resized;
      
      resize(face, face_resized, Size(IMG_WIDTH, IMG_HEIGTH), 1.0, 1.0, INTER_CUBIC);
      
      int prediction = -1;
      double confidence = 0;
      const double confidence_setpoint = 3000;
      
      model->predict(face_resized, prediction, confidence);
      confidence = ((confidence_setpoint * 100) / confidence);
      if((confidence > 30) && (prediction == id)) {
        sucessful_attempts++;
      }
      attempts++;
      
      // Remove later.
      rectangle(gray_img, face_i, CV_RGB(255, 0,0), 1);
      string box_text = format("Prediction = %d", prediction);
      int pos_x = std::max(face_i.tl().x - 10, 0);
      int pos_y = std::max(face_i.tl().y - 10, 0);
      putText(gray_img, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
      string box_text_2 = format("Confidence = %g %%", confidence);
      int pos_x_2 = std::max(face_i.tl().x - 25, 0);
      int pos_y_2 = std::max(face_i.tl().y - 25, 0);
      putText(gray_img, box_text_2, Point(pos_x_2, pos_y_2), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
    }
    // Remove later.
    namedWindow ("face_recognizer", WINDOW_AUTOSIZE);
    imshow("face_recognizer", gray_img);
    char key = (char)waitKey(20);
    if(key == 27) {
      break;
    }
    if(sucessful_attempts >= SUCESSFUL_ATTEMPTS_THRESHOLD) {
      destroyWindow("face_recognizer");
      return (true);
    }
  }
  destroyWindow("face_recognizer");
  printf("Attempts: %d\nSucessfull: %d\n", attempts, sucessful_attempts);
  return (false);
}
