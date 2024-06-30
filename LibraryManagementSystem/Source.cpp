#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <time.h>
#include <ctime>
#include "User.h"
#include "Book.h"

using namespace std;

User createUser() {
	cout << "Enter user name: " << flush;
	string name;
	getline(cin >> ws, name);

	cout << "Enter user birthday (using format dd.mm.yyyy): " << flush;
	string bday;
	getline(cin >> ws, bday);

	int bdd, bdm, bdy;
	char dot;

	istringstream bdayss(bday);
	bdayss >> bdd >> dot >> bdm >> dot >> bdy;

	cout << "Enter user address: " << flush;
	string address;
	getline(cin >> ws, address);

	cout << "Enter user e-mail: " << flush;
	string email;
	getline(cin >> ws, email);

	cout << "Enter user membership expiry date (using format dd.mm.yyyy): " << flush;
	string membership;
	getline(cin >> ws, membership);

	int mbd, mbm, mby;
	
	istringstream mbss(membership);
	mbss >> mbd >> dot >> mbm >> dot >> mby;

	return User(name, chrono::year_month_day(chrono::year(bdy), chrono::month(bdm), chrono::day(bdd)), address, email, chrono::year_month_day(chrono::year(mby), chrono::month(mbm), chrono::day(mbd)));
}

Book createBook() {
	cout << "Enter book title: " << flush;
	string title;
	getline(cin >> ws, title);
	cout << "Enter book author: " << flush;
	string author;
	getline(cin >> ws, author);
	cout << "Enter release year: " << flush;
	int releaseYear;
	cin >> releaseYear;
	return Book(title, author, releaseYear);
}

string readUsers(fstream& file, vector<User>& users) {
	cout << "Loading user registry... " << flush;
	file.open("users.txt", ios::in);
	if (file.is_open()) {
		while (file) {
			string read, name, id, address, email, loanedBooks;
			bool canBor;
			int borNum;
			getline(file, read);
			if (read.find('/') == string::npos) break;
			istringstream SS(read);
			int bdy, bdm, bdd, mby, mbm, mbd;
			char sign;
			getline(SS, name, '/');
			getline(SS, id, '/');
			SS >> bdy >> sign >> bdm >> sign >> bdd >> sign;
			getline(SS, address, '/');
			getline(SS, email, '/');
			SS >> mby >> sign >> mbm >> sign >> mbd >> sign >> canBor >> sign >> borNum >> sign;
			getline(SS, loanedBooks);
			users.push_back(User(name, id, chrono::year_month_day(chrono::year(bdy), chrono::month(bdm), chrono::day(bdd)), address, email, chrono::year_month_day(chrono::year(mby), chrono::month(mbm), chrono::day(mbd)), canBor, borNum, loanedBooks));

		}
		
	} else return "FAILED";

	file.close();
	return "SUCCESSFUL";
}

string readBooks(fstream& file, vector<Book>& books) {
	cout << "Loading book registry... " << flush;
	file.open("books.txt", ios::in);
	if (file.is_open()) {
		while (file) {
			string read, title, author, bookId;
			bool borrowed, isOverdue;
			int ddy, ddm, ddd, release, daysOverdue;
			char sign;
			getline(file, read);
			if (read.find('/') == string::npos) break;
			istringstream SS(read);
			getline(SS, title, '/');
			getline(SS, author, '/');
			getline(SS, bookId, '/');
			SS >> release >> sign >> borrowed >> sign >> ddy >> sign >> ddm >> sign >> ddd >> sign >> isOverdue >> sign >>daysOverdue;
			books.push_back(Book(title, author, release, bookId, borrowed, chrono::year_month_day(chrono::year(ddy), chrono::month(ddm), chrono::day(ddd)), isOverdue, daysOverdue));
		}
	} else return "FAILED";

	file.close();
	return "SUCCESSFUL";
}

string writeUsers(fstream& file, vector<User>& users) {
	cout << "Saving user changes... " << flush;
	file.open("users.txt", ios::out);
	if (file.is_open()) {
			for (auto it = users.begin(); it != users.end(); ++it) {
				string write = it->saveInfo() + '\n';
				file << write;
			}
	} else return "FAILED";

	file.close();
	return "SUCCESSFUL";
}

string writeBooks(fstream& file, vector<Book>& books) {
	cout << "Saving book changes... " << flush;
	file.open("books.txt", ios::out);
	if (file.is_open()) {
		for (auto it = books.begin(); it != books.end(); ++it) {
			string write = it->saveInfo() + '\n';
			file << write;
		}
	} else return "FAILED";

	file.close();
	return "SUCCESSFUL";
}

void update(vector<User>& users, vector<Book>& books, chrono::year_month_day current) {
	for (auto it = users.begin(); it != users.end(); ++it) {
		it->checkStatus(current);
	}
	for (auto it = books.begin(); it != books.end(); ++it) {
		it->checkOverdue(current);
	}
	for (auto it = users.begin(); it != users.end(); ++it) {
		for (auto itb = books.begin(); itb != books.end(); ++itb) {
			if (it->getBorBooks().find(itb->getId()) != string::npos) {
				it->checkStatus(current, *itb);
			}
		}
	}
	cout << "Updating Library Database." << endl;
}

int main() {

	srand(time(NULL));

	fstream file;

	vector<User> Users;
	vector<Book> Books;
	vector<string> userIDs;
	vector<string> bookIDs;

	auto now = chrono::system_clock::now();
	time_t now_c = chrono::system_clock::to_time_t(now);
	struct tm now_tm;
	localtime_s(&now_tm, &now_c);

	chrono::year_month_day current = chrono::year_month_day(chrono::year(now_tm.tm_year+1900), chrono::month(now_tm.tm_mon + 1), chrono::day(now_tm.tm_mday));

	cout << current << endl;

	cout << readUsers(file, Users) << endl;
	for (auto it = Users.begin(); it != Users.end(); ++it) {
		it->checkStatus(current);
		userIDs.push_back(it->getId());
	}
	
	cout << readBooks(file, Books) << endl;
	for (auto it = Books.begin(); it != Books.end(); ++it) {
		bookIDs.push_back(it->getId());
	}

	while (true) {
		update(Users, Books, current);

		cout << "Menu:" << endl;
		cout << "-----------------------" << endl;
		cout << "1. Manage users" << endl;
		cout << "2. Manage books" << endl;
		cout << "3. Manage loans" << endl;
		cout << "4. Save and exit" << endl;

		int choice;
		cin >> choice;

		string userID, bookID;
		User newUser;
		Book newBook;

		switch (choice) {
		case 1: 
			cout << "Menu:" << endl;
			cout << "-----------------------" << endl;
			cout << "1. Add user" << endl;
			cout << "2. Remover user" << endl;
			cout << "3. List all users" << endl;
			cout << "4. Search" << endl;
			cin >> choice;
			switch (choice) {
			case 1:
				newUser = createUser();
				newUser.checkStatus(current);
				for (auto it = userIDs.begin(); it != userIDs.end(); ++it) {
					while (newUser.getId() == *it) {
						newUser.setNewId();
					}
				}
				Users.push_back(newUser);
				cout << "Completed." << endl;
				break;

			case 2:
				cout << "Enter user ID: " << flush;
				getline(cin >> ws, userID);
				for (auto it = Users.begin(); it != Users.end(); ++it) {
					if (it->getId().find(userID) != string::npos) {
						for (auto itb = Books.begin(); itb != Books.end(); ++itb) {
							if (it->getBorBooks().find(itb->getId()) != string::npos) {
								itb->setReturned();
							}
						}
						Users.erase(it);
						cout << "Completed." << endl;
						break;
					}
				}
				break;

			case 3:
				if (Users.empty()) {
					cout << "User registry is empty." << endl;
					break;
				}
				for (auto it = Users.begin(); it != Users.end(); ++it) {
					cout << it->getInfo();
				}
				break;
				s
			case 4:
				cout << "Enter user name or ID: " << flush;
				getline(cin >> ws, userID);
				for (auto it = Users.begin(); it != Users.end(); ++it) {
					if (it->getInfo().find(userID) != string::npos) {
						cout << it->getInfo();
					}
				}
			}
			break;

		case 2:
			cout << "Menu:" << endl;
			cout << "-----------------------" << endl;
			cout << "1. Add book" << endl;
			cout << "2. Remove book" << endl;
			cout << "3. List all books" << endl;
			cin >> choice;
			switch (choice) {
			case 1:
				newBook = createBook();
				for (auto it = bookIDs.begin(); it != bookIDs.end(); ++it) {
					while (newBook.getId() == *it) {
						newBook.setNewId();
					}
				}
				Books.push_back(newBook);
				cout << "Completed" << endl;
				break;

			case 2:
				cout << "Enter book ID: " << flush;
				getline(cin >> ws, bookID);

				for (auto itb = Books.begin(); itb != Books.end(); ++itb) {
					if (itb->getId().find(bookID) != string::npos) {
						for (auto it = Users.begin(); it != Users.end(); ++it) {
							if (it->getBorBooks().find(itb->getId()) != string::npos) {
								it->removeBorBooks(itb->getId());
								it->changeBorNum('-');
								break;
							}
						}
						Books.erase(itb);
						cout << "Completed" << endl;
						break;
					}
				}

				break;

			case 3:
				if (Books.empty()) {
					cout << "Book registry is empty." << endl;
					break;
				}
				for (auto it = Books.begin(); it != Books.end(); ++it) {
					cout << it->getInfo();
				}
				break;
			}
			break;

		case 3:
			cout << "Menu:" << endl;
			cout << "-----------------------" << endl;
			cout << "1. Loan book" << endl;
			cout << "2. Return book" << endl;
			cout << "3. Preview Loans" << endl;
			cin >> choice;
			switch (choice) {
			case 1:
				cout << "Enter user ID: " << flush;
				getline(cin >> ws, userID);
				cout << "Enter book ID: " << flush;
				getline(cin >> ws, bookID);
				for (auto it = Users.begin(); it != Users.end(); ++it) {
					if (it->getId().find(userID) != string::npos) {
						for (auto itb = Books.begin(); itb != Books.end(); ++itb) {
							if (itb->getId().find(bookID) != string::npos) {

								if (it->getCanBor() && !(itb->getBorrowed())) {
									it->changeBorNum('+');
									it->setBorBooks(itb->getId());
									itb->setLoaned(current);
									cout << "Book loaned" << endl;
								}
								else {
									cout << "Loan failed" << endl;
								}
								break;
							}
						}
						break;
					}
				}
				break;

			case 2:
				cout << "Enter user ID: " << flush;
				getline(cin >> ws, userID);
				cout << "Enter book ID: " << flush;
				getline(cin >> ws, bookID);
				for (auto it = Users.begin(); it != Users.end(); ++it) {
					if (it->getId().find(userID) != string::npos) {
						for (auto itb = Books.begin(); itb != Books.end(); ++itb) {
							if (itb->getId().find(bookID) != string::npos) {
								it->changeBorNum('-');
								it->removeBorBooks(itb->getId());
								itb->setReturned();

								break;
							}
						}
						break;
					}
				}

				break;

			case 3:
				for (auto it = Users.begin(); it != Users.end(); ++it) {
					for (auto itb = Books.begin(); itb != Books.end(); ++itb) {
						if (it->getBorBooks().find(itb->getId()) != string::npos) {
							cout << it->getId() << " | " << itb->getId() << endl;
						}
					}
				}

				break;
			}
			break;

		case 4:
			cout << writeUsers(file, Users) << endl;
			cout << writeBooks(file, Books) << endl;
			return 0;
		}
	}

	return 0;
}

