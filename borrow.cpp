#include <iostream>
#include <string>
using namespace std;

class Book {
private:
    string title;
    bool isAvailable;

public:
    // Constructor to initialize the book
    Book(string bookTitle) {
        title = bookTitle;
        isAvailable = true;  // Book is initially available
    }

    // Method to borrow the book
    void borrow() {
        if (isAvailable) {
            isAvailable = false;
            cout << title << " borrowed." << endl;
        } else {
            cout << title << " is already borrowed." << endl;
        }
    }

    // Method to return the book
    void returnBook() {
        if (!isAvailable) {
            isAvailable = true;
            cout << title << " returned." << endl;
        } else {
            cout << title << " was not borrowed." << endl;
        }
    }

    // Method to check if the book is available
    void checkAvailability() {
        if (isAvailable) {
            cout << title << " is available." << endl;
        } else {
            cout << title << " is not available." << endl;
        }
    }
};

int main() {
    // Create a book object
    Book book1("The Great Gatsby");

    // Check availability
    book1.checkAvailability();

    // Borrow the book
    book1.borrow();

    // Try to borrow the book again (should show that it's already borrowed)
    book1.borrow();

    // Return the book
    book1.returnBook();

    // Try to return the book again (should show that it's not borrowed)
    book1.returnBook();

    return 0;
}
