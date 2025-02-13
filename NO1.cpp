#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Book {
public:
    string title;
    string author;
    string ISBN;
    int year;
    string status;

    // Constructor to initialize the book details
    Book(string t, string a, string isbn, int y, string s)
        : title(t), author(a), ISBN(isbn), year(y), status(s) {}

    // Function to display the book details
    void display() const {
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "ISBN: " << ISBN<< endl;
        cout << "Year: " << year << endl;
        cout << "Status: " << status << endl;
    }
};

// Function to add a book
void addBook(vector<Book>& books) {
    string title, author, ISBN, status;
    int year;

    cout << "Enter book title: ";
    cin.ignore();
    getline(cin, title);
    cout << "Enter author name: ";
    getline(cin, author);
    cout << "Enter ISBN: ";
    getline(cin, ISBN);
    cout << "Enter publication year: ";
    cin >> year;
    cin.ignore();  // To clear the newline character
    cout << "Enter book status (Available/Borrowed): ";
    getline(cin, status);

    // Add the new book to the vector
    books.push_back(Book(title, author, ISBN, year, status));
    cout << "Book added successfully!\n";
}

// Function to view all books
void viewBooks(const vector<Book>& books) {
    if (books.empty()) {
        cout << "No books available.\n";
    } else {
        for (const auto& book : books) {
            book.display();
        }
    }
}

// Function to update a book
void updateBook(vector<Book>& books) {
    string ISBN;
    cout << "Enter ISBN of the book to update: ";
    cin.ignore();
    getline(cin, ISBN);

    bool found = false;
    for (auto& book : books) {
        if (book.ISBN == ISBN) {
            found = true;

            // Ask for new details to update
            cout << "Enter new title: ";
            getline(cin, book.title);
            cout << "Enter new author: ";
            getline(cin, book.author);
            cout << "Enter new year: ";
            cin >> book.year;
            cin.ignore();  
            cout << "Enter new status (Available/Borrowed): ";
            getline(cin, book.status);

            cout << "Book updated successfully!\n";
            break;
        }
    }

    if (!found) {
        cout << "Book with ISBN " << ISBN << " not found!\n";
    }
}

// Function to delete a book
void deleteBook(vector<Book>& books) {
    string ISBN;
    cout << "Enter ISBN of the book to delete: ";
    cin.ignore();
    getline(cin, ISBN);

    bool found = false;
    for (auto it = books.begin(); it != books.end(); ++it) {
        if (it->ISBN == ISBN) {
            books.erase(it);
            cout << "Book deleted successfully!\n";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Book with ISBN " << ISBN << " not found!\n";
    }
}

int main() {
    vector<Book> books; // Vector to store books
    int choice;

    // Menu-driven interface
    do {
        cout << "\nBook Management System\n";
        cout << "1. Add Book\n";
        cout << "2. View All Books\n";
        cout << "3. Update Book\n";
        cout << "4. Delete Book\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            addBook(books);  // Call the function to add a book
        }
        else if (choice == 2) {
            viewBooks(books);  // Call the function to view all books
        }
        else if (choice == 3) {
            updateBook(books);  // Call the function to update a book
        }
        else if (choice == 4) {
            deleteBook(books);  // Call the function to delete a book
        }
        else if (choice == 5) {
            cout << "Exiting the system...\n";
        }
        else {
            cout << "Invalid choice, please try again.\n";
        }

    } while (choice != 5);

    return 0;
}

