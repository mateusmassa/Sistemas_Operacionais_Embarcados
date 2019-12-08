#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

void send_alert(int id, int id_step);
string define_subject (int i);
string define_step (int id_step); 
string define_message (int i, string date, string current_hour, string step);
string define_file (int i); 
string validate_value (int value);
string get_date (struct tm *timeinfo);
string get_hour (struct tm *timeinfo);

int main(int argc, char *argv[])
{
    send_alert(stoi(argv[1]), stoi(argv[2]));
    exit(0);
}

void send_alert(int id, int id_step)
{
    string email = "'accesscontrolthreestep@gmail.com'";
    string subject = define_subject(id);
    stringstream step, message, command, file;

    //Get date and hour.
    struct tm *timeinfo;
    time_t seconds;

    time(&seconds);
    timeinfo = localtime(&seconds);
    string current_hour, date;

    current_hour = get_hour(timeinfo);
    date = get_date(timeinfo);

    //Define message.
    step << define_step(id_step);
    message << define_message(id, date, current_hour, step.str());

    //Define command.
    file << define_file(id);
    command << "sudo -u pi echo " << message.str() << "| mutt -s " << subject << " " << email << " -a " << file.str();

    //Send message.
    system(command.str().c_str());

}

string define_subject (int i)
{
  stringstream subject;
  
  switch(i) {
    case 1: 
      subject << "'Alerta de invasão'";
      return subject.str();
      break;
    case 2:
      subject << "'Histórico de Acesso'";
      return subject.str();
      break;
    default:
      subject << "";
      return subject.str();
  }
}

string define_step (int id_step) 
{
  stringstream step;
  
  switch(id_step) {
    case 1:
      step << "Etapa de verificação: Senha";
      return step.str();
      break;
    case 2:
      step << "Etapa de verificação: RFID";
      return step.str();
      break;
    case 3:
      step << "Etapa de verificação: Reconhecimento Facial";
      return step.str();
      break;
    default:
      step << "";
      return step.str();
  }
}

string define_message (int i, string date, string current_hour, string step)
{
  
  stringstream message;
  
  switch(i) {
    case 1:
      message << "'Alerta de invasão!\n" << date << current_hour << step << "'";
      return message.str();
      break;
    case 2:
      message << "'Histórico de acesso:\n" << "Segue abaixo o arquivo com o histórico de acesso dos últimos 10 dias.\n" << "'";
      return message.str();
      break;
    default:
      message << "";
      return message.str();
  }
}

string define_file (int i) 
{
  
  stringstream file;
  
  switch(i) {
    case 1:
      file << "/home/pi/Desktop/access_control_3FA/alerta/invasor.jpg";
      return file.str();
      break;
    case 2:
      file << "/home/pi/Desktop/access_control_3FA/access_control.txt";
      return file.str();
      break;
    default:
      file << "";
      return file.str();
  }
}

string get_date (struct tm *timeinfo) {

  string  day, mon, year;
  stringstream temp_date;

  day = validate_value(timeinfo->tm_mday);
  mon = validate_value(timeinfo->tm_mon+1);
  year = validate_value(timeinfo->tm_year+1900);

  temp_date << "Data: " << day << "/" << mon << "/" << year << "\n";

  return temp_date.str();
}

string get_hour (struct tm *timeinfo) {

  string sec, min, hour;
  stringstream temp_hour;

  sec = validate_value(timeinfo->tm_sec);
  min = validate_value(timeinfo->tm_min);
  hour = validate_value(timeinfo->tm_hour);

  temp_hour << "Hora: " << hour << ":" << min << ":" << sec << "\n";

  return temp_hour.str();
}

string validate_value (int value){

  stringstream svalue;

  if (value < 10) {
      svalue << "0" << value;
  }
  else {
    svalue << value;
  }

  return svalue.str();
}

