#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <algorithm>
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

// Member class representing a library member
class Member {
public:
    int id;                // Unique ID for each member
    string name;           // Name of the member
    vector<int> borrowedBooks;  // List of borrowed book IDs

    // Constructor to initialize a Member object
    Member(int _id, string _name) : id(_id), name(_name) {}

    // Method to display member details
    void displayDetails() const {
        cout << "ID: " << id << ", Name: " << name << ", Borrowed Books: ";
        for (const int &bookID : borrowedBooks) {
            cout << bookID << " ";  // Display each borrowed book's ID
        }
        cout << endl;
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

// Function to save members to a CSV file (Append mode)
void saveMembers(const vector<Member>& members) {
    try {
        ofstream file("members.csv", ios::app);  // Open file in append mode
        if (!file.is_open()) {
            throw runtime_error("Error opening file for saving members!");
        }

        file.seekp(0, ios::end);  // Move file pointer to the end
        if (file.tellp() == 0) {  // If file is empty, write header
            file << "ID,Name,BorrowedBooks\n";  // Write header
        }

        // Save each member's details to the file
        for (const auto& member : members) {
            file << member.id << "," << member.name;
            for (const auto& bookID : member.borrowedBooks) {
                file << "," << bookID;  // Add each borrowed book's ID
            }
            file << "\n";
        }

        file.close();
    } catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
    }
}

// Function to read members from the CSV file
vector<Member> loadMembers() {
    vector<Member> members;
    try {
        ifstream file("members.csv");
        if (!file.is_open()) {
            throw runtime_error("Error opening file to load members!");
        }

        string line;
        bool firstLine = true;  // Flag to skip the header line
        while (getline(file, line)) {
            if (firstLine) {  // Skip header line
                firstLine = false;
                continue;
            }
            stringstream ss(line);
            string id, name, bookID;
            vector<int> borrowedBooks;
            getline(ss, id, ',');
            getline(ss, name, ',');
            while (getline(ss, bookID, ',')) {
                borrowedBooks.push_back(stoi(bookID));  // Add borrowed book IDs to the list
            }
            members.push_back(Member(stoi(id), name));  // Add member to the list
            members.back().borrowedBooks = borrowedBooks;  // Assign borrowed books to the member
        }
        file.close();
    } catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
    }
    return members;
}

// Function to borrow a book (mark it as borrowed)
void borrowBook(vector<Book>& books, vector<Member>& members, int memberID, int bookID) {
    try {
        // Find the book by its ID and check if it's available
        bool bookFound = false;
        for (auto& book : books) {
            if (book.id == bookID && book.isAvailable) {
                book.isAvailable = false;  // Mark the book as borrowed
                for (auto& member : members) {
                    if (member.id == memberID) {
                        member.borrowedBooks.push_back(bookID);  // Add the book to the member's borrowed list
                        cout << "Book borrowed successfully!\n";
                        saveBooks(books);  // Save the updated book list
                        saveMembers(members);  // Save the updated member list
                        bookFound = true;
                        return;
                    }
                }
            }
        }
        if (!bookFound) {
            throw runtime_error("Error: Book is not available or Member not found!");
        }
    } catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
    }
}

// Function to return a book (mark it as available again)
void returnBook(vector<Book>& books, vector<Member>& members, int memberID, int bookID) {
    try {
        bool memberFound = false;
        for (auto& member : members) {
            if (member.id == memberID) {
                memberFound = true;
                bool bookFound = false;
                for (auto it = member.borrowedBooks.begin(); it != member.borrowedBooks.end(); ++it) {
                    if (*it == bookID) {
                        member.borrowedBooks.erase(it);  // Remove the book from the member's borrowed list
                        for (auto& book : books) {
                            if (book.id == bookID) {
                                book.isAvailable = true;  // Mark the book as available again
                                cout << "Book returned successfully!\n";
                                saveBooks(books);  // Save the updated book list
                                saveMembers(members);  // Save the updated member list
                                bookFound = true;
                                return;
                            }
                        }
                    }
                }
                if (!bookFound) {
                    throw runtime_error("Error: Book not found in the member's borrowed list!");
                }
            }
        }
        if (!memberFound) {
            throw runtime_error("Error: Member not found!");
        }
    } catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
    }
}

// Function to display books
void displayBooks(const vector<Book>& books) {
    cout << "\nBooks List:\n";
    cout << "ID\tTitle\t\t\tAuthor\t\t\tAvailability\n";
    cout << "-----------------------------------------------------------\n";
    for (const auto& book : books) {
        book.displayDetails();  // Display each book's details
    }
}

// Function to display separation line
void displaySeparationLine() {
    cout << "-----------------------------\n";
}

// Main program loop
int main() {
    vector<Book> books = loadBooks();  // Load books from file
    vector<Member> members = loadMembers();  // Load members from file

    int choice;
    do {
        // Display menu options
        cout << "\nLibrary Menu:\n";
        cout << "1. Add New Book\n";
        cout << "2. Add New Member\n";
        cout << "3. View Books\n";
        cout << "4. Borrow Book\n";
        cout << "5. Return Book\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        try {
            if (choice == 1) {
                // Add a new book
                char addMore = 'y';
                while (addMore == 'y' || addMore == 'Y') {
                    string title, author;
                    cout << "Enter book title: ";
                    cin.ignore();  // Ignore the leftover newline
                    getline(cin, title);

                    cout << "Enter author name: ";
                    getline(cin, author);

                    int id = books.size() + 1;
                    books.push_back(Book(id, title, author, true));  // New book is available
                    saveBooks(books);  // Save books to file

                    cout << "Do you want to add another book? (y/n): ";
                    cin >> addMore;
                }
                displaySeparationLine();  // Add separation line after adding books

            } else if (choice == 2) {
                // Add a new member
                string name;
                cout << "Enter member name: ";
                cin.ignore();
                getline(cin, name);

                int id = members.size() + 1;
                members.push_back(Member(id, name));
                saveMembers(members);  // Save members to file
                displaySeparationLine();  // Add separation line after adding member

            } else if (choice == 3) {
                // View books
                displayBooks(books);
            } else if (choice == 4) {
                // Borrow book
                int memberID, bookID;
                cout << "Enter Member ID: ";
                cin >> memberID;
                cout << "Enter Book ID: ";
                cin >> bookID;

                borrowBook(books, members, memberID, bookID);
                displaySeparationLine();  // Add separation line after borrowing book

            } else if (choice == 5) {
                // Return book
                int memberID, bookID;
                cout << "Enter Member ID: ";
                cin >> memberID;
                cout << "Enter Book ID: ";
                cin >> bookID;

                returnBook(books, members, memberID, bookID);
                displaySeparationLine();  // Add separation line after returning book

            } else if (choice == 6) {
                cout << "Exiting the program. Goodbye!\n";
            } else {
                cout << "Invalid choice. Try again.\n";
            }
        } catch (const exception& e) {
            cout << "Exception: " << e.what() << endl;
        }
    } while (choice != 6);

    return 0;
}
