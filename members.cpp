#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// Abstract Base Class
class LibraryEntity {
public:
    virtual void displayDetails() const = 0; // Pure virtual function
    virtual ~LibraryEntity() {}
};

// Book Class
class Book : public LibraryEntity {
public:
    string title, author, ISBN;
    int year;
    bool isAvailable;

    Book(string t, string a, string i, int y, bool avail = true)
        : title(t), author(a), ISBN(i), year(y), isAvailable(avail) {}

    void displayDetails() const override {
        cout << "Title: " << title << ", Author: " << author
             << ", ISBN: " << ISBN << ", Year: " << year
             << ", Status: " << (isAvailable ? "Available" : "Borrowed") << endl;
    }
};

// Base Class Person
class Person {
public:
    string name;
    int ID;

    Person(string n, int id) : name(n), ID(id) {}
    virtual void displayDetails() const {
        cout << "Name: " << name << ", ID: " << ID << endl;
    }
};

// Derived Class Member
class Member : public Person {
public:
    string contact;
    vector<Book> borrowedBooks;

    Member(string n, int id, string c) : Person(n, id), contact(c) {}

    void borrowBook(Book &book) {
        if (book.isAvailable) {
            book.isAvailable = false;
            borrowedBooks.push_back(book);
            cout << name << " borrowed " << book.title << endl;
        } else {
            cout << "Book is not available!" << endl;
        }
    }

    void returnBook(Book &book) {
        book.isAvailable = true;
        for (size_t i = 0; i < borrowedBooks.size(); i++) {
            if (borrowedBooks[i].ISBN == book.ISBN) {
                borrowedBooks.erase(borrowedBooks.begin() + i);
                cout << name << " returned " << book.title << endl;
                return;
            }
        }
    }

    void displayDetails() const override {
        Person::displayDetails();
        cout << "Contact: " << contact << endl;
        cout << "Borrowed Books: ";
        for (const auto &book : borrowedBooks) {
            cout << book.title << ", ";
        }
        cout << endl;
    }
};

// File Handling Functions
void saveBooksToFile(const vector<Book> &books) {
    ofstream file("books.txt");
    for (const auto &book : books) {
        file << book.title << "," << book.author << "," << book.ISBN << ","
             << book.year << "," << book.isAvailable << "\n";
    }
    file.close();
}

void loadBooksFromFile(vector<Book> &books) {
    ifstream file("books.txt");
    string line, title, author, ISBN;
    int year;
    bool isAvailable;
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, ISBN, ',');
        ss >> year >> isAvailable;
        books.push_back(Book(title, author, ISBN, year, isAvailable));
    }
    file.close();
}

int main() {
    vector<Book> books;
    vector<Member> members;
    loadBooksFromFile(books);

    // Sample Books and Members
    books.push_back(Book("The Great Gatsby", "F. Scott Fitzgerald", "12345", 1925));
    books.push_back(Book("To Kill a Mockingbird", "Harper Lee", "67890", 1960));
    members.push_back(Member("John Doe", 1, "123-456-7890"));

    // Test Borrowing
    members[0].borrowBook(books[0]);
    members[0].displayDetails();
    books[0].displayDetails();

    // Test Returning
    members[0].returnBook(books[0]);
    books[0].displayDetails();

    saveBooksToFile(books);
    return 0;
}
