#include <stdio.h>
#include <mysql.h>
#include "validation.hpp"
#include "photopreprocess.hpp"

#define PHOTOS_CSV "/home/victor/Desktop/register_control/face_database.csv"
#define UNKNOWN_QUANTITY 59
#define UNKNOWN_PHOTOPATH "/home/victor/Desktop/register_control/face_database/00000/"
#define UNKNOWN_ID "00000"
#define UPDATE_REPO_SCRIPT "cd /home/victor/Desktop/register_control/ ; ./push_to_repo.sh"

using namespace std;

struct info {
	string id;
	string name;
	string password;
	string tag;
	stringstream photos_path;
};

void create_users_csv(MYSQL *con);
void create_photos_csv(void);
void add_user(MYSQL *con, info &user);
void remove_user(MYSQL *con);
vector<string> get_column_info(const string column);
bool is_in_database(const string column, const string data);

int main(int argc, char *argv[])
{
	MYSQL *con = mysql_init(NULL);
	info user;
	unsigned int menu_option = -1;

	if (con == NULL) {
		cerr << mysql_error(con) << endl;
		exit(1);
	}
	if (mysql_real_connect(con, "localhost", "root", "accesscontrol", "users", 3306, "/var/run/mysqld/mysqld.sock", 0) == NULL) {
		cerr << mysql_error(con) << endl;
		exit (1);
	}
	create_users_csv(con);
	do {
		switch(menu_option) {
		case 1: {
			system("clear");
			add_user(con, user);
			menu_option = -1;
		} break;
		case 2: {
			system("clear");
			remove_user(con);
			menu_option = -1;
		} break;
		case 3: {
			vector<string> tmp = get_column_info("id");
			/*
			Face recognizer class using fisher faces needs at least two faces registered to use in training.
			*/
			if (tmp.size() != 0) {
				system("clear");
				train_model(PHOTOS_CSV);
				system(UPDATE_REPO_SCRIPT);
				cout << "\nPressione ENTER para voltar ao menu inicial..." << endl;
				cin.get();
			}
			else {
				system("clear");
				cerr << "Cadastre pelo menos um usuário no programa ";
				cerr << "para atualizar os dados na nuvem." << endl;
				cout << "\nPressione ENTER para voltar ao menu inicial..." << endl;
				cin.get();
			}
			menu_option = -1;
		} break;
		default: {
			system("clear");
			cout << "========== Menu de registros ==========" << endl;
			cout << "1. Adicionar novo usuário" << endl;
			cout << "2. Remover usuário existente" << endl;
			cout << "3. Salvar modificações" << endl;
			cout << "0. Encerrar programa" << endl;
			cout << "\nInsira sua opção: ";
			menu_option = get_menu_option();
		}
		}
	} while(menu_option != 0);
	mysql_close(con);
	return (0);
}

void create_users_csv(MYSQL *con)
{
	stringstream export_csv_command;

	remove(DATABASE_CSV);
	export_csv_command << "SELECT * FROM info INTO OUTFILE '";
	export_csv_command << DATABASE_CSV << "' FIELDS ENCLOSED BY '' ";
	export_csv_command << "TERMINATED BY ',' LINES TERMINATED BY '\n';";
	if (mysql_query(con, export_csv_command.str().c_str())) {
		cerr << mysql_error(con) << endl;
		exit (1);
	}
}

void create_photos_csv(void)
{
	vector<string> id = get_column_info("id");
	vector<string> photos_path = get_column_info("photos_path");
	ofstream csv_file;

	remove(PHOTOS_CSV);
	csv_file.open(PHOTOS_CSV, ios::out);
	for (unsigned int i = 0; i < photos_path.size(); i++) {
		for (unsigned int counter = 1; counter <= PHOTOS_QUANTITY; counter++) {
			csv_file << photos_path.at(i) << counter << ".jpg";
			csv_file << "," << id.at(i) << endl;
		}
	}
	for (unsigned int i = 1; i <= UNKNOWN_QUANTITY; i++) {
		csv_file << UNKNOWN_PHOTOPATH << i << ".jpg";
		csv_file << "," << UNKNOWN_ID << endl;
	}
	csv_file.close();
}

void add_user(MYSQL *con, info &user)
{
	stringstream add_user_command;

	user.photos_path = stringstream();
	system("clear");
	do {
		user.id = get_id();
		if (is_in_database("id", user.id) == true) {
			cerr << "\nIdentificação já cadastrada no banco de dados, ";
			cerr << "tente novamente." << endl;
		}
	} while (is_in_database("id", user.id) == true);
	system("clear");
	do {
		user.name = get_name();
		if (is_in_database("name", user.name) == true) {
			cerr << "\nNome já cadastrado no banco de dados, ";
			cerr << "tente novamente." << endl;
		}
	} while (is_in_database("name", user.name) == true);
	system("clear");
	do {
		user.password = get_password();
		if (is_in_database("password", user.password) == true) {
			cerr << "\nSenha já cadastrada no banco de dados, ";
			cerr << "tente novamente." << endl;
		}
	} while (is_in_database("password", user.password) == true);
	system("clear");
	do {
		user.tag = get_tag();
		if (is_in_database("tag", user.tag) == true) {
			cerr << "\nTag já cadastrada no banco de dados, ";
			cerr << "tente novamente." << endl;
		}
	} while (is_in_database("tag", user.tag) == true);
	user.photos_path << PHOTOS_BASEPATH << user.id << "/";
	add_user_command << "INSERT INTO info(id, name, password, ";
	add_user_command << "tag, photos_path) VALUES('" << user.id << "','";
	add_user_command << user.name << "','" << user.password << "','";
	add_user_command << user.tag << "','" << user.photos_path.str() << "');";
	if (mysql_query(con, add_user_command.str().c_str())) {
		cerr << mysql_error(con) << endl;
		exit (1);
	}
	get_photo(user.id);
	create_users_csv(con);
	create_photos_csv();
	system("clear");
	cout << "Cadastrado com sucesso usuário com informações:" << endl;
	cout << "ID: " << user.id << endl;
	cout << "Nome: " << user.name << endl;
	cout << "\nPressione ENTER para voltar ao menu inicial..." << endl;
	cin.get();
}

void remove_user(MYSQL *con)
{
	stringstream remove_user_command;
	stringstream user_photo_path;

	string id = get_id();
	if (is_in_database("id", id) == true) {
		remove_user_command << "DELETE FROM info WHERE id = '" << id << "';";
		if (mysql_query(con, remove_user_command.str().c_str())) {
			cerr << mysql_error(con) << endl;
			exit (1);
		}
		for (unsigned int i = 1; i <= PHOTOS_QUANTITY; i++) {
			user_photo_path << PHOTOS_BASEPATH << id << "/";
			user_photo_path << i << ".jpg";
			remove(user_photo_path.str().c_str());
			user_photo_path = stringstream();
		}
		user_photo_path << PHOTOS_BASEPATH << id << "/";
		remove(user_photo_path.str().c_str());
		create_users_csv(con);
		create_photos_csv();
		vector<string> tmp = get_column_info("id");
		if (tmp.size() < 1) {
			remove(TRAINED_MODEL_XML);
		}
		system("clear");
		cout << "Usuário de identificação " << id;
		cout << " removido com sucesso!" << endl;
		cout << "\nPressione ENTER para voltar ao menu inicial..." << endl;
		cin.get();
	}
	else {
		system("clear");
		cerr << "Usuário não encontrado no banco de dados." << endl;
		cout << "\nPressione ENTER para voltar ao menu inicial..." << endl;
		cin.get();
	}
}

// General function to see if a given data is already present at the database,
// to avoid equal information between users.
vector<string> get_column_info(const string column)
{
	unsigned int option = 0;
	ifstream csv_file;

	csv_file.open(DATABASE_CSV, ios::in);
	if (!csv_file) {
		cerr << "Arquivo CSV não encontrado!" << endl;
		exit (1);
	}
	if (column == "id") option = 1;
	if (column == "name") option = 2;
	if (column == "password") option = 3;
	if (column == "tag") option = 4;
	if (column == "photos_path") option = 5;

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

bool is_in_database(const string column, const string data)
{
	vector<string> column_info = get_column_info(column);
	for (unsigned int i = 0; i < column_info.size(); i++) {
		if (column_info.at(i) == data) return (true);
	}
	return (false);
}
