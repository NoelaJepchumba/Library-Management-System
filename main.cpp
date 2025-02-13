#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <algorithm>
using namespace std;

// Abstract Base Class
class LibraryEntity {
public:
    virtual void addEntity() = 0;
    virtual void removeEntity() = 0;
    virtual void displayDetails() const = 0;
    virtual ~LibraryEntity() {}
};

// Book Class
class Book : public LibraryEntity {
public:
    string title, author, ISBN;
    int year;
    bool isAvailable;

    Book(string t, string a, string i, int y) : title(t), author(a), ISBN(i), year(y), isAvailable(true) {}

    void addEntity() override {
        ofstream file("books.txt", ios::app);
        file << title << "," << author << "," << ISBN << "," << year << "," << isAvailable << endl;
        file.close();
    }

    void removeEntity() override {}

    void displayDetails() const override {
        cout << "Title: " << title << ", Author: " << author << ", ISBN: " << ISBN << ", Year: " << year << ", Available: " << (isAvailable ? "Yes" : "No") << endl;
    }
};

// Person Base Class
class Person {
public:
    string name;
    int ID;
    string contact;

    Person(string n, int i, string c) : name(n), ID(i), contact(c) {}
    virtual void displayDetails() const {
        cout << "Name: " << name << ", ID: " << ID << ", Contact: " << contact << endl;
    }
};

// Member Class (Inheritance from Person)
class Member : public Person, public LibraryEntity {
public:
    vector<string> borrowedBooks;

    Member(string n, int i, string c) : Person(n, i, c) {}

    void addEntity() override {
        ofstream file("members.txt", ios::app);
        file << name << "," << ID << "," << contact << endl;
        file.close();
    }

    void removeEntity() override {}

    void displayDetails() const override {
        Person::displayDetails();
        cout << "Borrowed Books: ";
        for (const string &book : borrowedBooks) cout << book << ", ";
        cout << endl;
    }
};

// Library System
class Library {
public:
    void borrowBook(Member &member, Book &book) {
        if (!book.isAvailable) {
            throw runtime_error("Book is not available!");
        }
        book.isAvailable = false;
        member.borrowedBooks.push_back(book.ISBN);
        logTransaction(member, book, "Borrowed");
    }

    void returnBook(Member &member, Book &book) {
        book.isAvailable = true;
        member.borrowedBooks.erase(remove(member.borrowedBooks.begin(), member.borrowedBooks.end(), book.ISBN), member.borrowedBooks.end());
        logTransaction(member, book, "Returned");
    }

    void logTransaction(Member &member, Book &book, string action) {
        ofstream file("transactions.txt", ios::app);
        file << member.ID << "," << book.ISBN << "," << action << endl;
        file.close();
    }
};

int main() {
    Book book1("C++ Programming", "Bjarne Stroustrup", "12345", 1998);
    Member member1("Alice", 1, "alice@email.com");

    Library library;
    try {
        library.borrowBook(member1, book1);
        member1.displayDetails();
        book1.displayDetails();

        library.returnBook(member1, book1);
        member1.displayDetails();
        book1.displayDetails();
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
