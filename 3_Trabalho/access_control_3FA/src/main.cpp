#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wiringPi.h>
#include "LCD_I2C.h"
#include "MFRC522.h"
#include "face_recognition.hpp"
#include "check.hpp"
#include "saveFile.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/face/facerec.hpp"
#define PIN 1
#define FILEPATH "/home/pi/Desktop/access_control_3FA/alerta/invasor.jpg"

using namespace cv;
using namespace cv::face;
using namespace std;


void get_intruder (string path);
static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator=';');

int id = 0;

int main()
{
  unsigned int counter = 0;
  vector<Mat> images;
  vector<int> labels;
  
  wiringPiSetup();    
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, HIGH);

  lcd_init();

  while(1)
  {
    system("clear");
    counter = 0;

    // Password authentication.
    stringstream command_senha;
    command_senha << "cd email; sudo -u pi ./email 1 1";

    while(checkPassword() == false)
    {
      if(counter == 2)
      {
				get_intruder(FILEPATH);
        system(command_senha.str().c_str());
        printf("\n3 senhas falhas! Acesso negado!\n");
				ClrLcd();
	      lcdLoc(LINE1);
	      typeln("3 senhas falhas!");
        lcdLoc(LINE2);
	      typeln(" Acesso negado! ");
        sleep(1);
        system("clear");
        continue;
      }
      else
      {
        counter++;
      }
    }

    // RFID authentication.
    counter = 0;
    stringstream command_tag;
    command_tag << "cd email; sudo -u pi ./email 1 2";

    while(checkTag() == false)
    {
      if(counter == 2)
      {
				get_intruder(FILEPATH);
        system(command_tag.str().c_str());
        printf("\n3 tags falhas! Acesso negado!\n");
				ClrLcd();
	      lcdLoc(LINE1);
	      typeln(" 3 tags falhas! ");
        lcdLoc(LINE2);
	      typeln(" Acesso negado! ");
        sleep(1);
        system("clear");
        break;
      }
      else
      {
        counter++;
      }
    }
    
    if (counter < 2)
    {
      // Facial authentication.
      bool checkFace = true;
      stringstream command_rec;
      command_rec << "cd email; sudo -u pi ./email 1 3";

      int prediction = 0;
      prediction = get_prediction(id);
      
      ClrLcd();
      lcdLoc(LINE1);
      typeln(" Aproxime-se da ");
      lcdLoc(LINE2);
      typeln("     WebCam     ");
      sleep(2);

      checkFace = face_recognition(prediction);
      
      ClrLcd();
      lcdLoc(LINE1);
      typeln(" Validando Face ");
      lcdLoc(LINE2);
      typeln("       ...      ");
      sleep(2);
      
      if(checkFace == false)
      {
        get_intruder(FILEPATH);
        system(command_rec.str().c_str());
        ClrLcd();
        lcdLoc(LINE1);
        typeln(" Face invalida! ");
        lcdLoc(LINE2);
        typeln(" Acesso negado! ");
        printf("Face não reconhecida!\nAcesso negado!\n");
      }
      else
      {
        ClrLcd();
        lcdLoc(LINE1);
        typeln("Acesso permitido");
        lcdLoc(LINE2);
        typeln("Seja bem vindo!");
        printf("\nAcesso permitido. Seja bem vindo!\n");
        
        //Controling access.
        digitalWrite(PIN, LOW);
        sleep(10);
        digitalWrite(PIN, HIGH);
        
        //Save access.
        save_access(id);
      }
    }
    else
    {
      continue;
    }
    
    sleep(2);
  }
  return(0);
}

static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator)
{
  separator = ';';
  ifstream file(filename.c_str(), ifstream::in);
  if (!file)
  {
    string error_message = "No valid input file was given, please check the given filename.";
    CV_Error(Error::StsBadArg, error_message);
  }
  string line, path, classlabel;
  while (getline(file, line))
  {
    stringstream liness(line);
    getline(liness, path, separator);
    getline(liness, classlabel);
    if(!path.empty() && !classlabel.empty())
    {
      images.push_back(imread(path, 0));
      labels.push_back(atoi(classlabel.c_str()));
    }
  }
}

void get_intruder(string path) {

	//definindo imagem de anexo
	VideoCapture camera;
	stringstream filename;

	camera.open(0);
	camera.set(3, 640);
	camera.set(4, 480);

	Mat cameraFrame;

	camera >> cameraFrame;
	if (cameraFrame.empty())
	{
		std::cerr << "ERROR: Couldn't grab a camera frame." << std::endl;
		exit(1);
	}

	if(sleep(2) == 0){
		camera >> cameraFrame;
		imwrite(path, cameraFrame);
  }
}

