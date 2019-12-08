#include "saveFile.hpp"
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <vector>
#define DATABASE_CSV "/home/pi/Documents/github/3FA_Server/users_database.csv"

using namespace std;

void save_access (int id)
{
  ifstream csv_file;
  
  csv_file.open(DATABASE_CSV, ios::in);
	if (!csv_file) {
		cerr << "Arquivo CSV não encontrado!" << endl;
		exit (1);
	}
  
  stringstream row_stream;
	string row, date, hour, token;
  vector<string> data_user;
  struct tm *timeinfo;
  time_t seconds;
  
  time(&seconds);
  timeinfo = localtime(&seconds);
  hour = get_hour(timeinfo);
  date = get_date(timeinfo);
  for (unsigned int i = 0; i < (id+1); i++) {
    getline(csv_file, row);
    row_stream = stringstream(row);
  }
  for (unsigned int j = 0; j < 2; j++) {
    getline(row_stream, token, ',');
    data_user.push_back(token);
  }
  csv_file.close();
  fstream access("access_control.txt");
  if (access.is_open()) {
    access.seekg (0, ios::end);
    access << hour;
    access << date;
    access << "User: ";
    access << data_user.at(0);
    access << " - ";
    access << data_user.at(1);
    access << "\n----------------------------------------------------------------------------------------------------\n";
    access.close();
  }
  else cout << "Unable to open file";
}

string get_date (struct tm *timeinfo) 
{
  string  day, mon, year;
  stringstream temp_date;

  day = validate_value(timeinfo->tm_mday);
  mon = validate_value(timeinfo->tm_mon+1);
  year = validate_value(timeinfo->tm_year+1900);
  temp_date << "Data: " << day << "/" << mon << "/" << year << "\n";

  return temp_date.str();
}

string get_hour (struct tm *timeinfo) 
{
  string sec, min, hour;
  stringstream temp_hour;

  sec = validate_value(timeinfo->tm_sec);
  min = validate_value(timeinfo->tm_min);
  hour = validate_value(timeinfo->tm_hour);
  temp_hour << "Hora: " << hour << ":" << min << ":" << sec << "\n";
  return temp_hour.str();
}

string validate_value (int value) 
{
  stringstream svalue;

  if (value < 10) {
    svalue << "0" << value;
  }
  else {
    svalue << value;
  }
  return svalue.str();
}
