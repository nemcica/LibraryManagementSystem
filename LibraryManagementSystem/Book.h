#pragma once
#include <iostream>
#include <chrono>
using namespace std;

class Book {
private:
	string title;
	string author;
	int release;
	string id;
	bool borrowed;
	chrono::year_month_day dueDate;
	bool isOverdue;
	int daysOverdue;

public:

	//Constructors:
	Book();
	//For adding new books
	Book(string title, string author, int release);
	//For reading books from a file
	Book(string title, string author, int release, string id, bool borrowed, chrono::year_month_day dueDate, bool isOverdue, int daysOverdue);

	//Getters:
	string getId() const;
	string getInfo() const;
	string getTitle() const;
	bool getBorrowed() const;
	bool getOverdue() const;
	int getDaysOverdue() const;

	//Setters:
	void setLoaned(chrono::year_month_day current);
	void setReturned();
	void setNewId();
	void extendLoan();

	//Misc:
	void checkOverdue(chrono::year_month_day current);
	string saveInfo();
	

	
};

