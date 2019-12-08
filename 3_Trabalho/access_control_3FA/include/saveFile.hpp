#ifndef SAVEFILE_HPP_
#define SACEFILE_HPP_

#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <vector>

void save_access (int id);
std::string get_date (struct tm *timeinfo);
std::string get_hour (struct tm *timeinfo);
std::string validate_value (int value);

#endif

