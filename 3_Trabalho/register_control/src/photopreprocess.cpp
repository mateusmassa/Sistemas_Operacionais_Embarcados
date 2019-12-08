#include "photopreprocess.hpp"

using namespace cv;
using namespace cv::face;
using namespace std;

void get_photo(const string id)
{
	VideoCapture camera(WEBCAM_ID);

	if(!camera.isOpened()) {
		cerr << "Capture device ID " << WEBCAM_ID;
		cerr << " cannot be opened!" << endl;
    	exit (1);
	}
	camera.set(3, RESOLUTION_WIDTH);
	camera.set(4, RESOLUTION_HEIGHT);

	unsigned int counter = 1;
	stringstream photo_path;

	photo_path << "mkdir " << PHOTOS_BASEPATH << id;
	system(photo_path.str().c_str());
	while (counter <= PHOTOS_QUANTITY) {
		Mat frame;
		camera >> frame;
		namedWindow("Registration", WINDOW_AUTOSIZE);
		imshow("Registration", frame);
		char keypress = waitKey(20);
		if(keypress == 13) {
			for (counter = 1; counter <= PHOTOS_QUANTITY; counter++) {
				camera >> frame;
				photo_path = stringstream();
				photo_path << PHOTOS_BASEPATH << id;
				photo_path << "/" << counter << ".jpg";
				imwrite(photo_path.str(), frame);
				preprocess(photo_path.str());
				usleep(1);
			}
		}
	}
	destroyWindow("Registration");
}

static void preprocess(const string photo_path)
{
	// Load image in grayscale and equalize histogram.
	Mat img = imread(photo_path, IMREAD_GRAYSCALE);
	equalizeHist(img, img);

	// Load xml classifiers for face and eye detection.
	CascadeClassifier frontal_face_detector;
	CascadeClassifier eye_detector;

	frontal_face_detector.load(FRONTAL_FACE_XML);
	eye_detector.load(EYE_XML);

	// Detect face and align eyes.
	vector<Rect> face;
	vector<Rect> eyes;

	frontal_face_detector.detectMultiScale(img, face, 1.1, 5);
	Point center(face[0].x + face[0].width/2, face[0].y + face[0].height/2);
	Mat face_ROI = img(face[0]);
	eye_detector.detectMultiScale(face_ROI, eyes, 1.1, 5);

	CvPoint left_eye;
	CvPoint right_eye;
	CvPoint eye_direction;

	for (unsigned int i = 0; i < 2; i++) {
		Point eye_center((face[0].x + eyes[1-i].x + eyes[1-i].width/2), (face[0].y + eyes[1-i].y + eyes[1-i].height/2));
		if (i == 0) {
			left_eye.x = eye_center.x;
			left_eye.y = eye_center.y;
		}
		if (i == 1) {
			right_eye.x = eye_center.x;
			right_eye.y = eye_center.y;
		}
	}
	// In case left eye and right eye got switched during detection.
	if(right_eye.x < left_eye.x) {
		int tmp_x = right_eye.x;
		int tmp_y = right_eye.y;
		right_eye.x = left_eye.x;
		right_eye.y = left_eye.y;
		left_eye.x = tmp_x;
		left_eye.y = tmp_y;
	}
	eye_direction.x = right_eye.x - left_eye.x;
	eye_direction.y = right_eye.y - left_eye.y;
	float rotation = atan2((float)(eye_direction.y), (float)(eye_direction.x));
	Point2f reference_center(left_eye.x, left_eye.y);
	double angle = (rotation * (180/3.141592));
	Mat rotation_matrix = getRotationMatrix2D(reference_center, angle, 1);
	Mat rotated_img(Size(img.size().height, img.size().width), img.type());
	warpAffine(img, rotated_img, rotation_matrix, img.size());
	// Crop face from image.
	Mat croped_img = rotated_img(face[0]);
	// Resize image without distortion
	Mat final_img;
	float scale = croped_img.cols / (float)RESOLUTION_HEIGHT;
	if(croped_img.cols < RESOLUTION_HEIGHT)
	{
		int scaled_height = cvRound(croped_img.rows / scale);
		resize(croped_img, final_img, Size(RESOLUTION_HEIGHT, scaled_height));
	}
	else
	{
		final_img = croped_img;
	}
	// Save preprocessed image.
	imwrite(photo_path, final_img);
}

static void read_photos_csv(const string photos_csv, vector<Mat>& photos, vector<int>& labels)
{
	ifstream csv_file;

	csv_file.open(photos_csv, ios::in);
	if (!csv_file) {
		cerr << "Arquivo CSV não encontrado!" << endl;
		exit (1);
	}
	string row, path, photo_label;
	while (getline(csv_file, row)) {
		stringstream row_stream(row);
		getline(row_stream, path, ',');
		getline(row_stream, photo_label);
		if (!path.empty() && !photo_label.empty()) {
			photos.push_back(imread(path, 0));
			labels.push_back(stoi(photo_label));
		}
	}
	csv_file.close();
}

void train_model(string photos_csv)
{
	vector<Mat> photos;
	vector<int> labels;

	try {
		read_photos_csv(photos_csv, photos, labels);
	}
	catch (cv::Exception& e) {
		cerr << "Error opening file \"" << photos_csv << "\". Reason: " << e.msg << endl;
		exit (1);
	}
	Ptr<FisherFaceRecognizer> model = FisherFaceRecognizer::create();
	model->train(photos, labels);
	model->save("/home/victor/Documents/github/3FA_Server/trained_model.xml");
}
