#include "validation.hpp"

using namespace std;

int get_menu_option(void)
{
	string menu_option;

	getline(cin, menu_option);
	if (menu_option == "0") return (0);
	if (menu_option == "1") return (1);
	if (menu_option == "2") return (2);
	if (menu_option == "3") return (3);
	return (-1);
}

string get_id(void)
{
	bool id_is_validated = false;
	unsigned int counter;
	string id;

	while (id_is_validated == false) {
		counter = 0;
		cout << "Insira o número de identificação do usuário:" << endl;
		getline(cin, id);
		if (id.size() != ID_SIZE) {
			cerr << "\nA identificação deve ter " << ID_SIZE;
			cerr << " dígitos, tente novamente." << endl;
			id_is_validated = false;
		}
		else {
			for (unsigned int i = 0; i < ID_SIZE; i++) {
				if (isdigit(id.at(i))) {
					counter++;
				}
			}
			if (counter != ID_SIZE) {
				cerr << "\nA identificação deve conter apenas números, ";
				cerr << "tente novamente." << endl;
				id_is_validated = false;
			}
			else {
				id_is_validated = true;
			}
		}
	}
	return (id);
}

string get_name(void)
{
	bool name_is_validated = false;
	unsigned int counter;
	string name;

	while (name_is_validated == false) {
		counter = 0;
		cout << "Insira o nome completo do usuário:" << endl;
		getline(cin, name);
		if ((name.size() == 0) || (name.at(0) == ' ')) {
			cerr << "O nome deve ser iniciado com letra, ";
			cerr << "tente novamente." << endl;
			name_is_validated = false;
		}
		else {
			for (unsigned int i = 0; i < name.size(); i++) {
				if (isalpha(name.at(i)) || name.at(i) == ' ') {
					counter++;
				}
			}
			if (counter != name.size()) {
				cerr << "\nO nome deve conter apenas letras, ";
				cerr << "tente novamente." << endl;
				name_is_validated = false;
			}
			else {
				name = format_name(name);
				name_is_validated = true;
			}
		}
	}
	return (name);
}

string format_name(string name)
{
	string formatted_name;

	for (unsigned int i = 0; i < name.size(); i++) {
		formatted_name.push_back(toupper(name.at(i)));
	}
	return (formatted_name);
}

string get_password(void)
{
	bool password_is_validated = false;
	unsigned int counter;
	string password;

	while (password_is_validated == false) {
		counter = 0;
		cout << "Insira a senha do usuário:" << endl;
		getline(cin, password);
		if (password.size() != PASSWORD_SIZE) {
			cerr << "\nA senha deve ter " << PASSWORD_SIZE;
			cerr << " dígitos, tente novamente." << endl;
			password_is_validated = false;
		}
		else {
			for (unsigned int i = 0; i < PASSWORD_SIZE; i++) {
				if (isdigit(password.at(i))) {
					counter++;
				}
			}
			if (counter != PASSWORD_SIZE) {
				cerr << "\nA senha deve conter apenas números, ";
				cerr << "tente novamente." << endl;
				password_is_validated = false;
			}
			else {
				password_is_validated = true;
			}
		}
	}
	return (password);
}

string get_tag(void)
{
	bool tag_is_validated = false;
	unsigned int counter;
	string tag;

	while (tag_is_validated == false) {
		counter = 0;
		cout << "Insira o número da tag do usuário:" << endl;
		getline(cin, tag);
		if (tag.size() != TAG_SIZE) {
			cerr << "\nO número da tag deve ter " << TAG_SIZE;
			cerr << " dígitos, tente novamente." << endl;
			tag_is_validated = false;
		}
		else {
			for (unsigned int i = 0; i < TAG_SIZE; i++) {
				if (isalnum(tag.at(i))) {
					counter++;
				}
			}
			if (counter != TAG_SIZE) {
				cerr << "\nO número da tag deve conter apenas letras ";
				cerr << "maiúsculas e números, tente novamente." << endl;
				tag_is_validated = false;
			}
			else {
				tag = format_tag(tag);
				tag_is_validated = true;
			}
		}
	}
	return (tag);
}

string format_tag(string tag)
{
	string formatted_tag;

	for (unsigned int i = 0; i < tag.size(); i++) {
		formatted_tag.push_back(toupper(tag.at(i)));
	}
	return (formatted_tag);
}
