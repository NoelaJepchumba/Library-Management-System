#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>

using namespace std;

class Book {
public:
    int id;
    string title;
    string author;
    bool isAvailable;

    Book(int _id, string _title, string _author, bool _isAvailable)
        : id(_id), title(_title), author(_author), isAvailable(_isAvailable) {}
};

class Member {
public:
    int id;
    string name;
    vector<int> borrowedBooks;  // List of borrowed book IDs

    Member(int _id, string _name)
        : id(_id), name(_name) {}
};

// Function to save books to file (Append mode)
void saveBooks(const vector<Book>& books) {
    ofstream file("books.csv", ios::app);
    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    file.seekp(0, ios::end);  // Move the file pointer to the end
    if (file.tellp() == 0) {  // If the file is empty, write header
        file << "ID,Title,Author,Availability\n";  // Write header
    }

    for (const auto& book : books) {
        file << book.id << "," << book.title << "," << book.author << "," << (book.isAvailable ? "Available" : "Not Available") << "\n";
    }

    file.close();
}

// Function to read books from file (Ignore header, load only data)
vector<Book> loadBooks() {
    ifstream file("books.csv");
    vector<Book> books;
    string line;

    if (file.is_open()) {
        bool firstLine = true;  // Skip header line
        while (getline(file, line)) {
            if (firstLine) {  // Skip header
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
            books.push_back(Book(stoi(id), title, author, isAvailable));
        }
        file.close();
    } else {
        cout << "Error opening file to load books!" << endl;
    }
    return books;
}

// Function to save members to file (Append mode)
void saveMembers(const vector<Member>& members) {
    ofstream file("members.csv", ios::app);
    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    file.seekp(0, ios::end);  // Move the file pointer to the end
    if (file.tellp() == 0) {  // If the file is empty, write header
        file << "ID,Name,BorrowedBooks\n";  // Write header
    }

    for (const auto& member : members) {
        file << member.id << "," << member.name;
        for (const auto& bookID : member.borrowedBooks) {
            file << "," << bookID;
        }
        file << "\n";
    }

    file.close();
}

// Function to read members from file (Ignore header, load only data)
vector<Member> loadMembers() {
    ifstream file("members.csv");
    vector<Member> members;
    string line;

    if (file.is_open()) {
        bool firstLine = true;  // Skip header line
        while (getline(file, line)) {
            if (firstLine) {  // Skip header
                firstLine = false;
                continue;
            }
            stringstream ss(line);
            string id, name, bookID;
            vector<int> borrowedBooks;
            getline(ss, id, ',');
            getline(ss, name, ',');
            while (getline(ss, bookID, ',')) {
                borrowedBooks.push_back(stoi(bookID));
            }
            members.push_back(Member(stoi(id), name));
            members.back().borrowedBooks = borrowedBooks;
        }
        file.close();
    } else {
        cout << "Error opening file to load members!" << endl;
    }
    return members;
}

// Function to borrow a book
void borrowBook(vector<Book>& books, vector<Member>& members, int memberID, int bookID) {
    // Check if the book is available
    for (auto& book : books) {
        if (book.id == bookID && book.isAvailable) {
            book.isAvailable = false;  // Mark book as borrowed
            for (auto& member : members) {
                if (member.id == memberID) {
                    member.borrowedBooks.push_back(bookID);  // Add book to member's borrowed list
                    cout << "Book borrowed successfully!\n";
                    saveBooks(books);  // Save updated book list
                    saveMembers(members);  // Save updated member list
                    return;
                }
            }
        }
    }
    cout << "Error: Book is not available or Member not found!" << endl;
}

// Function to return a book
void returnBook(vector<Book>& books, vector<Member>& members, int memberID, int bookID) {
    for (auto& member : members) {
        if (member.id == memberID) {
            for (auto it = member.borrowedBooks.begin(); it != member.borrowedBooks.end(); ++it) {
                if (*it == bookID) {
                    member.borrowedBooks.erase(it);  // Remove book from member's borrowed list
                    for (auto& book : books) {
                        if (book.id == bookID) {
                            book.isAvailable = true;  // Mark book as available again
                            cout << "Book returned successfully!\n";
                            saveBooks(books);  // Save updated book list
                            saveMembers(members);  // Save updated member list
                            return;
                        }
                    }
                }
            }
        }
    }
    cout << "Error: Member not found or book was not borrowed!" << endl;
}

// Function to display books with headers
void displayBooks(const vector<Book>& books) {
    cout << "\nBooks List:\n";
    cout << "ID\tTitle\t\t\tAuthor\t\t\tAvailability\n";
    cout << "-----------------------------------------------------------\n";
    for (const auto& book : books) {
        cout << book.id << "\t" << book.title << "\t\t" << book.author << "\t\t" << (book.isAvailable ? "Available" : "Not Available") << "\n";
    }
}

// Function to display separation line
void displaySeparationLine() {
    cout << "-----------------------------\n";
}

// Main program
int main() {
    vector<Book> books = loadBooks();  // Load books from file
    vector<Member> members = loadMembers();  // Load members from file

    int choice;
    do {
        cout << "\nLibrary Menu:\n";
        cout << "1. Add New Book\n";
        cout << "2. Add New Member\n";
        cout << "3. View Books\n";
        cout << "4. Borrow Book\n";
        cout << "5. Return Book\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

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
            cout << "Enter member ID: ";
            cin >> memberID;
            cout << "Enter book ID: ";
            cin >> bookID;
            borrowBook(books, members, memberID, bookID);

        } else if (choice == 5) {
            // Return book
            int memberID, bookID;
            cout << "Enter member ID: ";
            cin >> memberID;
            cout << "Enter book ID: ";
            cin >> bookID;
            returnBook(books, members, memberID, bookID);
        }
    } while (choice != 6);

    return 0;
}
