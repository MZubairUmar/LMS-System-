#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <vector>
#include <iomanip>

using namespace std;

// Utility function for Console Text Color
void Print(string str, int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    cout << str;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

// Utility function to get console width
int ConsoleWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

// Utility function to center-align text
string CenterText(string str)
{
    return string((ConsoleWidth() - str.length()) / 2, ' ') + str;
}

// Base class for common attributes and methods
class Person
{
protected:
    string name;
    int id;

public:
    Person(string name = "", int id = 0) : name(name), id(id) {}
    virtual void Display() const = 0;
};

// Book class
class Book
{
private:
    string name;
    int isbn;
    int year;
    int price;
    int quantity;

public:
    Book(string name = "", int isbn = 0, int year = 0, int price = 0, int quantity = 0)
        : name(name), isbn(isbn), year(year), price(price), quantity(quantity) {}

    int GetISBN() const { return isbn; }
    int GetQuantity() const { return quantity; }
    void IncrementQuantity(int amount) { quantity += amount; }
    void DecrementQuantity()
    {
        if (quantity > 0)
            quantity--;
    }

    void Display() const
    {
        cout << isbn << "\t" << quantity << "\t" << price << "\t" << year << "\t" << name << endl;
    }

    friend istream &operator>>(istream &in, Book &b);
    friend ostream &operator<<(ostream &out, const Book &b);
};

istream &operator>>(istream &in, Book &b)
{
    in >> b.isbn >> b.quantity >> b.price >> b.year;
    in.ignore();
    getline(in, b.name);
    return in;
}

ostream &operator<<(ostream &out, const Book &b)
{
    out << b.isbn << " " << b.quantity << " " << b.price << " " << b.year << " " << b.name;
    return out;
}

// Student class derived from Person
class Student : public Person
{
private:
    int age;
    char gender;
    int issuedBook;

public:
    Student(string name = "", int id = 0, int age = 0, char gender = 'M', int issuedBook = 0)
        : Person(name, id), age(age), gender(gender), issuedBook(issuedBook) {}

    int GetIssuedBook() const { return issuedBook; }
    void SetIssuedBook(int isbn) { issuedBook = isbn; }
    void ReturnBook() { issuedBook = 0; }

    void Display() const override
    {
        cout << id << "\t" << issuedBook << "\t" << age << "\t" << gender << "\t" << name << endl;
    }

    friend istream &operator>>(istream &in, Student &s);
    friend ostream &operator<<(ostream &out, const Student &s);
};

istream &operator>>(istream &in, Student &s)
{
    in >> s.id >> s.issuedBook >> s.age >> s.gender;
    in.ignore();
    getline(in, s.name);
    return in;
}

ostream &operator<<(ostream &out, const Student &s)
{
    out << s.id << " " << s.issuedBook << " " << s.age << " " << s.gender << " " << s.name;
    return out;
}

// Library Management System class
class LibraryManagementSystem
{
private:
    vector<Book> books;
    vector<Student> students;
    const string username = "zubair";
    const string password = "112233";

    void DisplayHeader() const
    {
        system("cls");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
        cout << string(ConsoleWidth(), '_') << "\n\n";
        cout << CenterText("Project By") << endl;
        cout << CenterText("M Zubair Umar (F2021065286)") << endl;
        cout << string(ConsoleWidth(), '_') << "\n\n";
        cout << CenterText("Welcome") << endl;
        cout << CenterText("Library Management System") << endl;
        cout << string(ConsoleWidth(), '_') << "\n\n";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    }

    void DisplayFooter(string message, int color) const
    {
        Print(CenterText(message) + "\n\n", color);
        system("pause");
    }

    int Authenticate()
    {
        while (true)
        {
            DisplayHeader();
            string inputUsername, inputPassword;
            cout << "Enter your username: ";
            cin >> inputUsername;
            if (inputUsername == username)
            {
                cout << "Enter your password: ";
                cin >> inputPassword;
                if (inputPassword == password)
                    return 1;
                else
                    DisplayFooter("Wrong password", 12);
            }
            else
                DisplayFooter("Username not found", 12);
        }
    }

    int GetOption()
    {
        cout << "1) Display all books in library\n";
        cout << "2) Add new book in library\n";
        cout << "3) Issue book to student\n";
        cout << "4) Display students who borrowed books\n";
        cout << "5) Return book from student\n";
        Print("6) Exit library\n\n", 8);
        cout << "Select option: ";
        int option;
        cin >> option;
        return option;
    }

    void DisplayBooks() const
    {
        Print("ISBN\tQuantity\tPrice\tYear\tName\n", 11);
        for (const auto &book : books)
            book.Display();
    }

    void AddBook()
    {
        Book newBook;
        cout << "Enter Book ISBN: ";
        cin >> newBook;
        auto it = find_if(books.begin(), books.end(), [&newBook](const Book &b)
                          { return b.GetISBN() == newBook.GetISBN(); });

        if (it != books.end())
        {
            it->IncrementQuantity(newBook.GetQuantity());
            DisplayFooter("Book quantity updated successfully", 10);
        }
        else
        {
            books.push_back(newBook);
            DisplayFooter("Book added successfully", 10);
        }
    }

    void IssueBook()
    {
        DisplayBooks();
        cout << "\nSelect Book ISBN: ";
        int isbn;
        cin >> isbn;

        auto bookIt = find_if(books.begin(), books.end(), [isbn](const Book &b)
                              { return b.GetISBN() == isbn; });
        if (bookIt == books.end() || bookIt->GetQuantity() < 1)
        {
            DisplayFooter("Book not available or out of stock", 12);
            return;
        }

        Student newStudent;
        cout << "Enter student details (ID, Name, Age, Gender): ";
        cin >> newStudent;

        if (any_of(students.begin(), students.end(), [&newStudent](const Student &s)
                   { return s.GetIssuedBook() == newStudent.GetIssuedBook(); }))
        {
            DisplayFooter("Student already has an issued book", 12);
            return;
        }

        newStudent.SetIssuedBook(isbn);
        bookIt->DecrementQuantity();
        students.push_back(newStudent);
        DisplayFooter("Book issued successfully", 10);
    }

    void DisplayStudents() const
    {
        Print("ID\tBook\tAge\tGender\tName\n", 11);
        for (const auto &student : students)
            student.Display();
    }

    void ReturnBook()
    {
        DisplayStudents();
        cout << "\nEnter Student ID to return book: ";
        int studentId;
        cin >> studentId;

        auto studentIt = find_if(students.begin(), students.end(), [studentId](const Student &s)
                                 { return s.id == studentId; });
        if (studentIt == students.end() || studentIt->GetIssuedBook() == 0)
        {
            DisplayFooter("No book to return for this student", 12);
            return;
        }

        auto bookIt = find_if(books.begin(), books.end(), [&studentIt](const Book &b)
                              { return b.GetISBN() == studentIt->GetIssuedBook(); });
        if (bookIt != books.end())
            bookIt->IncrementQuantity(1);

        studentIt->ReturnBook();
        DisplayFooter("Book returned successfully", 10);
    }

    void LoadData()
    {
        ifstream bookFile("Books.txt"), studentFile("Students.txt");
        Book tempBook;
        Student tempStudent;

        while (bookFile >> tempBook)
            books.push_back(tempBook);

        while (studentFile >> tempStudent)
            students.push_back(tempStudent);
    }

    void SaveData()
    {
        ofstream bookFile("Books.txt"), studentFile("Students.txt");

        for (const auto &book : books)
            bookFile << book << endl;

        for (const auto &student : students)
            studentFile << student << endl;
    }

public:
    void Run()
    {
        Authenticate();
        LoadData();

        while (true)
        {
            DisplayHeader();
            int option = GetOption();
            DisplayHeader();

            switch (option)
            {
            case 1:
                DisplayBooks();
                DisplayFooter("", 10);
                break;
            case 2:
                AddBook();
                break;
            case 3:
                IssueBook();
                break;
            case 4:
                DisplayStudents();
                DisplayFooter("", 10);
                break;
            case 5:
                ReturnBook();
                break;
            case 6:
                SaveData();
                DisplayFooter("Thanks for using our LMS", 10);
                return;
            default:
                DisplayFooter("Invalid option, try again", 12);
                break;
            }
        }
    }
};

int main()
{
    LibraryManagementSystem lms;
    lms.Run();
    return 0;
}
