#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept>  // For std::runtime_error

using namespace std;

// Book class representing a book in the library system
class Book {
public:
    int id;            // Unique ID for each book
    string title;      // Title of the book
    string author;     // Author of the book
    bool isAvailable;  // Availability of the book (true if available, false if borrowed)

    // Constructor to initialize a Book object
    Book(int _id, string _title, string _author, bool _isAvailable)
        : id(_id), title(_title), author(_author), isAvailable(_isAvailable) {}

    // Method to display book details
    void displayDetails() const {
        cout << id << "\t" << title << "\t\t" << author << "\t\t" << (isAvailable ? "Available" : "Not Available") << endl;
    }
};

// Function to save books to a CSV file (Append mode)
void saveBooks(const vector<Book>& books) {
    try {
        ofstream file("books.csv", ios::app);  // Open file in append mode
        if (!file.is_open()) {
            throw runtime_error("Error opening file for saving books!");
        }

        file.seekp(0, ios::end);  // Move file pointer to the end
        if (file.tellp() == 0) {  // If file is empty, write header
            file << "ID,Title,Author,Availability\n";  // Write header
        }

        // Save each book's details to the file
        for (const auto& book : books) {
            file << book.id << "," << book.title << "," << book.author << "," << (book.isAvailable ? "Available" : "Not Available") << "\n";
        }

        file.close();
    } catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
    }
}

// Function to read books from the CSV file
vector<Book> loadBooks() {
    vector<Book> books;
    try {
        ifstream file("books.csv");
        if (!file.is_open()) {
            throw runtime_error("Error opening file to load books!");
        }

        string line;
        bool firstLine = true;  // Flag to skip the header line
        while (getline(file, line)) {
            if (firstLine) {  // Skip header line
                firstLine = false;
                continue;
            }
            stringstream ss(line);
            string id, title, author, availability;
            getline(ss, id, ',');
            getline(ss, title, ',');
            getline(ss, author, ',');
            getline(ss, availability, ',');
            bool isAvailable = (availability == "Available");
            books.push_back(Book(stoi(id), title, author, isAvailable));  // Add book to the list
        }
        file.close();
    } catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
    }
    return books;
}

// Function to display all books
void displayBooks(const vector<Book>& books) {
    cout << "\nBooks List:\n";
    cout << "ID\tTitle\t\t\tAuthor\t\t\tAvailability\n";
    for (const auto& book : books) {
        book.displayDetails();
    }
}

// Function to add a new book
void addNewBook(vector<Book>& books) {
    string title, author;
    cout << "Enter book title: ";
    cin.ignore();  // Ignore the leftover newline
    getline(cin, title);

    cout << "Enter author name: ";
    getline(cin, author);

    int id = books.size() + 1;  // New ID will be the next in sequence
    books.push_back(Book(id, title, author, true));  // New book is available by default

    saveBooks(books);  // Save updated books to file
    cout << "New book added successfully with ID: " << id << endl;
}

// Function to borrow a book (mark it as borrowed)
void borrowBook(vector<Book>& books, int bookID) {
    bool bookFound = false;
    for (auto& book : books) {
        if (book.id == bookID && book.isAvailable) {
            book.isAvailable = false;  // Mark the book as borrowed
            cout << "Book borrowed successfully!\n";
            saveBooks(books);  // Save the updated book list
            bookFound = true;
            return;
        }
    }
    if (!bookFound) {
        cout << "Error: Book not available or doesn't exist!\n";
    }
}

// Function to return a book (mark it as available again)
void returnBook(vector<Book>& books, int bookID) {
    bool bookFound = false;
    for (auto& book : books) {
        if (book.id == bookID && !book.isAvailable) {
            book.isAvailable = true;  // Mark the book as available again
            cout << "Book returned successfully!\n";
            saveBooks(books);  // Save the updated book list
            bookFound = true;
            return;
        }
    }
    if (!bookFound) {
        cout << "Error: Book not found or already available!\n";
    }
}

// Main program loop
int main() {
    vector<Book> books = loadBooks();  // Load books from file

    int choice;
    do {
        // Display menu options
        cout << "\nLibrary Menu:\n";
        cout << "1. Add New Book\n";
        cout << "2. View Books\n";
        cout << "3. Borrow Book\n";
        cout << "4. Return Book\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        try {
            if (choice == 1) {
                // Add a new book
                addNewBook(books);
            } else if (choice == 2) {
                // View all books
                displayBooks(books);
            } else if (choice == 3) {
                // Borrow a book
                int bookID;
                cout << "Enter book ID to borrow: ";
                cin >> bookID;
                borrowBook(books, bookID);  // Borrow the book
            } else if (choice == 4) {
                // Return a book
                int bookID;
                cout << "Enter book ID to return: ";
                cin >> bookID;
                returnBook(books, bookID);  // Return the book
            } else if (choice == 5) {
                cout << "Exiting the program. Goodbye!\n";
            } else {
                cout << "Invalid choice. Try again.\n";
            }
        } catch (const exception& e) {
            cout << "Exception: " << e.what() << endl;
        }
    } while (choice != 5);

    return 0;
}
