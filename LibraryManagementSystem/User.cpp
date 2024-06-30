#include "User.h"
#include "Book.h"

//Constructors:
User::User() {
	name = "";
	birthday = chrono::year_month_day(chrono::year(0), chrono::month(0), chrono::day(0));
	address = "";
	email = "";
	membership = chrono::year_month_day(chrono::year(0), chrono::month(0), chrono::day(0));
	id = "";
	borNum = 0;
	canBor = true;
	borBooks = "";
}
//For adding new users
User::User(string name, chrono::year_month_day birthday, string address, string email, chrono::year_month_day membership):
	name(name), birthday(birthday), address(address), email(email), membership(membership) {
	stringstream bdaySS;
	bdaySS << birthday.year();
	int idEnd = rand() & 10000;
	stringstream idEndSS;
	idEndSS << setfill('0') << setw(4) << idEnd;
	id = *(name.begin()) + name.substr(name.find(' ') + 1, 1) + '-' + bdaySS.str() + '-' + idEndSS.str();
	canBor = true;
	borNum = 0;
	borBooks = "";
}
//For reading users from a file
User::User(string name, string id, chrono::year_month_day birthday, string address, string email, chrono::year_month_day membership, bool canBor, int borNum, string borBooks) :
	name(name), id(id), birthday(birthday), address(address), email(email), membership(membership), canBor(canBor), borNum(borNum), borBooks(borBooks) {
}

//Getters:
string User::getInfo() const {

	stringstream SS;

	SS << "Name: " << name << " | User ID: " << id << endl;
	SS << "Birthday: " << birthday << " / Address: " << address << endl;
	SS << "Email: " << email << " / Membership expiry: " << membership << endl;
	SS << "Can Borrow?: " << canBor << " / Loaned Books: " << borBooks << endl;
	SS << endl;

	return SS.str();
}
string User::getId() const {
	return id;
}
bool User::getCanBor() const {
	return canBor;
}
string User::getBorBooks() const {
	return borBooks;
}

//Setters:
void User::setBorBooks(string ID) {
	borBooks += ID + ' ';
}
void User::setNewId() {
	stringstream bdaySS;
	bdaySS << birthday.year();
	int idEnd = rand() % 10000;
	stringstream idEndSS;
	idEndSS << setfill('0') << setw(4) << idEnd;
	id = *(name.begin()) + name.substr(name.find(' ') + 1, 1) + '-' + bdaySS.str() + '-' + idEndSS.str();
}
void User::changeBorNum(char sign) {
	if (sign == '+') borNum++;
	if (sign == '-') borNum--;

}
void User::removeBorBooks(string ID) {
	borBooks.erase(borBooks.find(ID), ID.length() + 1);
}

//Misc::
string User::saveInfo() {
	stringstream SS;

	SS << name << '/' << id << '/' << birthday << '/' << address << '/' << email << '/' << membership << '/' << canBor << '/' << borNum << '/' << borBooks;

	return SS.str();
}
void User::checkStatus(chrono::year_month_day in) {
	if (membership < in || borNum >= 3) canBor = false;
	else canBor = true;
}
void User::checkStatus(chrono::year_month_day in, Book inBook) {
	if (membership < in || borNum >= 3 || inBook.getOverdue()) canBor = false;
	else canBor = true;
}
bool User::operator<(const User& other) const {
	return name < other.name;
}