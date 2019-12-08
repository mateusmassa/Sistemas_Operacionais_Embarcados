#include "check.hpp"
#include "LCD_I2C.h"
#include "MFRC522.h"
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

bool checkPassword() {
  string password;

  ClrLcd();
  lcdLoc(LINE1);
  typeln("Insira a senha:");

  cout << "Insira sua senha numérica: " << endl;
  getline (cin, password);
  
  ClrLcd();
  lcdLoc(LINE1);
  typeln("Validando Senha");
  lcdLoc(LINE2);
  typeln("       ...      ");
  sleep(2);
  
  if(is_in_database("password", password) == 1){
    ClrLcd();
    lcdLoc(LINE1);
    typeln("  Senha aceita! ");
    sleep(2);
    printf("Senha aceita!\n");
    return(true);
  }
  else{
    ClrLcd();
    lcdLoc(LINE1);
    typeln("Senha incorreta!");
    sleep(2);
    printf("Senha incorreta!\n\n");
    return(false);
  }  
}

bool checkTag() {
  MFRC522 mfrc;
  char tag_id[9];

  mfrc.PCD_Init();
  ClrLcd();
  lcdLoc(LINE1);
  typeln(" Passe sua tag: ");
  printf("\nPasse sua tag: \n");
  while(1)
  {
    while(!mfrc.PICC_IsNewCardPresent())
    {
      continue;
    }
    if(!mfrc.PICC_ReadCardSerial())
    {
      continue;
    }
    for(byte i = 0; i < mfrc.uid.size; ++i)
    {
      snprintf(&tag_id[2*i], sizeof(int), "%X", mfrc.uid.uidByte[i]);
    }
    printf("ID: %s\n", tag_id);
    
    ClrLcd();
    lcdLoc(LINE1);
    typeln("  Validando TAG ");
    lcdLoc(LINE2);
    typeln("       ...      ");
    sleep(2);
    
    if(is_tag(id, tag_id) == 1)
    {
      ClrLcd();
      lcdLoc(LINE1);
      typeln("   Tag aceita!  ");
      printf("Tag aceita!\n");
      sleep(2);
      return(true);
    }
    else{
      ClrLcd();
      lcdLoc(LINE1);
      typeln("  Tag recusada!");
      printf("Tag recusada!\n");
      sleep(2);
      return(false);
    }
  }
}

vector<string> get_column_info(const string column)
{
  unsigned int option = 0;

  ifstream csv_file(DATABASE_CSV, ifstream::in);
  if (!csv_file) {
	  cerr << "Arquivo CSV não encontrado!" << endl;
	  exit (1);
  }

  if (column == "id") option = 1;
  if (column == "name") option = 2;
  if (column == "password") option = 3;
  if (column == "tag") option = 4;
  if (column == "img_path") option = 5;

  stringstream row_stream;
  string row, token;
  vector<string> column_info;

  while (getline(csv_file, row)) {
    row_stream = stringstream(row);
    for (unsigned int i = 0; i < option; i++) {
      getline(row_stream, token, ',');
    }
    column_info.push_back(token);
  }
  csv_file.close();
  return (column_info);
}

int is_in_database(const string column, const string data)
{
  vector<string> column_info = get_column_info(column);
  for (unsigned int i = 0; i < column_info.size(); i++) {
    if (column_info.at(i) == data) {
      id = i;
      return (1);
    }
  }
  return (0);
}

int is_tag (int id_tag, string tag)
{
  ifstream csv_file(DATABASE_CSV, ifstream::in);
  if (!csv_file) {
	  cerr << "Arquivo CSV não encontrado!" << endl;
	  exit (1);
  }
  
  stringstream row_stream;
  string row, data;
  
  for (int i = 0; i < (id_tag+1); i++) {
    getline(csv_file, row);
    row_stream = stringstream(row);
    for (unsigned int i = 0; i < 4; i++) {
      getline(row_stream, data, ',');
    }
  }

  csv_file.close();

  if (tag == data) {
    return (1);
  }
  else {
    return (0);
  }
} 

int get_prediction(int id_pred) 
{
  
  ifstream csv_file(DATABASE_CSV, ifstream::in);
  if (!csv_file) {
    cerr << "Arquivo CSV não encontrado!" << endl;
    exit (1);
  }
  
  stringstream row_stream;
  string row, data;
  int prediction;
  
  for (int i = 0; i < (id_pred+1); i++) {
    getline(csv_file, row);
    row_stream = stringstream(row);
    getline(row_stream, data, ',');
  }
  csv_file.close();
  
  prediction = stoi(data);
  
  return prediction;
}
