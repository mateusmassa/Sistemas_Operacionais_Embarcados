#ifndef CHECK_HPP_
#define CHECK_HPP_

#include "LCD_I2C.h"
#include "MFRC522.h"
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <vector>

extern int id;

bool checkPassword();
bool checkTag();
vector<string> get_column_info(const string column);
int is_in_database(const string column, const string data);
int is_tag(int id_tag, string tag);
int get_prediction(int id_pred);

#endif
