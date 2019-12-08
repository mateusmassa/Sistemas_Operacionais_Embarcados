#ifndef VALIDATION_HPP_
#define VALIDATION_HPP_

#include <locale>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define ID_SIZE 5
#define PASSWORD_SIZE 6
#define TAG_SIZE 8
#define DATABASE_CSV "/tmp/users_database.csv"

int get_menu_option(void);
std::string get_id(void);
std::string get_name(void);
std::string format_name(std::string name);
std::string get_password(void);
std::string get_tag(void);
std::string format_tag(std::string tag);

#endif
