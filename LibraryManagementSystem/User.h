#pragma once
#include <iostream>
#include <iomanip>
#include <chrono>
#include "Book.h"

using namespace std;


class User {
private:
	string name;
	chrono::year_month_day birthday;
	string address;
	string email;
	chrono::year_month_day membership;
	string id;
	bool canBor;
	int borNum;
	string borBooks;

public:
	//Constructors:
	User();
	//For adding new users
	User(string name, chrono::year_month_day birthday, string address, string email, chrono::year_month_day membership);
	//For reading users from a file
	User(string name, string id, chrono::year_month_day birthday, string address, string email, 
		chrono::year_month_day membership, bool canBor, int borNum, string borBooks);

	//Getters:
	string getInfo() const;
	string getId() const;
	bool getCanBor() const;
	string getBorBooks() const;

	//Setters:
	void setBorBooks(string ID);
	void setNewId();
	void changeBorNum(char sign);
	void removeBorBooks(string ID);

	//Misc:
	string saveInfo();
	void checkStatus(chrono::year_month_day in);
	void checkStatus(chrono::year_month_day in, Book inBook);
	bool operator<(const User& other) const;

};

