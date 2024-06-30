#include <iomanip>
#include "Book.h"

//Constructors:
Book::Book() : title(""), author(""), release(0), id(""), borrowed(false), dueDate(chrono::year_month_day(chrono::year(0), chrono::month(0), chrono::day(0))), isOverdue(false), daysOverdue(0) {

}
//For adding new books
Book::Book(string title, string author, int release) : title(title), author(author), release(release) {
	stringstream SS;
	int idEnd = rand() % 10000;
	stringstream SSid;
	SSid << setfill('0') << setw(4) << idEnd;
	SS << *(author.begin()) << *(title.begin()) << '-' << to_string(release) << '-' << SSid.str();
	id = SS.str();
	borrowed = false;
	dueDate = chrono::year_month_day(chrono::year(0), chrono::month(0), chrono::day(0));
	isOverdue = false;
	daysOverdue = 0;
}
//For reading books from a file
Book::Book(string title, string author, int release, string id, bool borrowed, chrono::year_month_day dueDate, bool isOverdue, int daysOverdue): 
	title(title), author(author), release(release), id(id), borrowed(borrowed), dueDate(dueDate), isOverdue(isOverdue), daysOverdue(daysOverdue) {

}

//Getters:
string Book::getId() {
	return id;
}
string Book::getInfo() {
	stringstream SS;

	SS << "Title: " << title << " | Author: " << author << endl;
	SS << "ID: " << id << " / Release year: " << release << endl;
	SS << "Borrowed?: " << borrowed << " / Due Date: " << dueDate << endl;
	SS << endl;

	return SS.str();
}
bool Book::getBorrowed() {
	return borrowed;
}
bool Book::getOverdue() {
	return isOverdue;
}
int Book::getDaysOverdue() {
	return daysOverdue;
}

//Setters:
void Book::setLoaned(chrono::year_month_day current) {
	borrowed = true;
	dueDate = current + chrono::months(1);

}
void Book::setReturned() {
	borrowed = false;
	dueDate = chrono::year_month_day(chrono::year(0), chrono::month(0), chrono::day(0));
	isOverdue = false;
	daysOverdue = 0;
}
void Book::setNewId() {
	stringstream SS;
	int idEnd = rand() % 10000;
	stringstream SSid;
	SSid << setfill('0') << setw(4) << idEnd;
	SS << *(author.begin()) << *(title.begin()) << '-' << to_string(release) << '-' << SSid.str();
	id = SS.str();
}

//Misc:
string Book::saveInfo() {
	stringstream SS;

	SS << title << '/' << author << '/' << id << '/' << release << '/' << borrowed << '/' << dueDate << '/' << isOverdue << '/' << daysOverdue;

	return SS.str();
}
void Book::checkOverdue(chrono::year_month_day current) {
	if (borrowed && current > dueDate) {
		isOverdue = true;
		auto diff = chrono::sys_days(current) - chrono::sys_days(dueDate);
		daysOverdue = chrono::duration_cast<chrono::days>(diff).count();
	} else {
		isOverdue = false;
		daysOverdue = 0;
	}
}