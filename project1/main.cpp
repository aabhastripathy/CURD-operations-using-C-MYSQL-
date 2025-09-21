#include <iostream>
#include <windows.h>
#include <mysql.h>
#include <string>
#include <sstream>

using namespace std;

// Global variables for database connection
const char* HOST = "localhost";
const char* USER = "root";
const char* PASSWORD = "08520"; // <-- IMPORTANT: Change this to your MySQL password
const char* DB = "my_db";

int main() {
    MYSQL* conn;
    conn = mysql_init(0);

    // Establish connection to the database
    if (mysql_real_connect(conn, HOST, USER, PASSWORD, DB, 0, NULL, 0)) {
        cout << "Connected to the database successfully." << endl;
        Sleep(2000);
    } else {
        cout << "Failed to connect to the database. Error: " << mysql_error(conn) << endl;
        return -1; // Exit if connection fails
    }

    int choice;
    bool running = true;

    while (running) {
        system("cls"); // Clear the console screen

        cout << "====== DATABASE MENU ======" << endl;
        cout << "1. Insert Data" << endl;
        cout << "2. Read/View Data" << endl;
        cout << "3. Update Data" << endl;
        cout << "4. Delete Data" << endl;
        cout << "0. Exit" << endl;
        cout << "===========================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: { // INSERT DATA
                system("cls");
                int id;
                string name;
                float grade;

                cout << "Enter Student ID: ";
                cin >> id;
                cout << "Enter Student Name: ";
                cin.ignore(); // Clears the buffer before getline
                getline(cin, name);
                cout << "Enter Student Grade: ";
                cin >> grade;

                stringstream ss;
                ss << "INSERT INTO student (ID, Name, Grade) VALUES (" << id << ", '" << name << "', " << grade << ")";
                string query = ss.str();

                if (mysql_query(conn, query.c_str())) {
                    cout << "Query Error: " << mysql_error(conn) << endl;
                } else {
                    cout << "Data inserted successfully!" << endl;
                }
                Sleep(3000);
                break;
            }
            case 2: { // READ DATA
                system("cls");
                if (mysql_query(conn, "SELECT * FROM student")) {
                    cout << "Query Error: " << mysql_error(conn) << endl;
                } else {
                    MYSQL_RES* result = mysql_store_result(conn);
                    if (result) {
                        MYSQL_ROW row;
                        cout << "ID\tName\t\tGrade" << endl;
                        cout << "---------------------------------" << endl;
                        while ((row = mysql_fetch_row(result))) {
                            cout << row[0] << "\t" << row[1] << "\t\t" << row[2] << endl;
                        }
                        mysql_free_result(result);
                    }
                }
                cout << "\nPress any key to continue...";
                cin.ignore();
                cin.get();
                break;
            }
            case 3: { // UPDATE DATA
                system("cls");
                int id;
                float newGrade;

                cout << "Enter Student ID to update: ";
                cin >> id;
                cout << "Enter new Grade: ";
                cin >> newGrade;

                stringstream ss;
                ss << "UPDATE student SET Grade = " << newGrade << " WHERE ID = " << id;
                string query = ss.str();

                if (mysql_query(conn, query.c_str())) {
                    cout << "Query Error: " << mysql_error(conn) << endl;
                } else {
                    if(mysql_affected_rows(conn) > 0){
                        cout << "Data updated successfully!" << endl;
                    } else {
                        cout << "Update failed. No student found with that ID." << endl;
                    }
                }
                Sleep(3000);
                break;
            }
            case 4: { // DELETE DATA
                system("cls");
                int id;
                cout << "Enter Student ID to delete: ";
                cin >> id;

                stringstream ss;
                ss << "DELETE FROM student WHERE ID = " << id;
                string query = ss.str();

                if (mysql_query(conn, query.c_str())) {
                    cout << "Query Error: " << mysql_error(conn) << endl;
                } else {
                     if(mysql_affected_rows(conn) > 0){
                        cout << "Data deleted successfully!" << endl;
                    } else {
                        cout << "Delete failed. No student found with that ID." << endl;
                    }
                }
                Sleep(3000);
                break;
            }
            case 0: { // EXIT
                running = false;
                cout << "Exiting the program. Goodbye!" << endl;
                Sleep(2000);
                break;
            }
            default: {
                cout << "Invalid choice. Please try again." << endl;
                Sleep(2000);
                break;
            }
        }
    }

    // Close the connection before exiting
    mysql_close(conn);
    return 0;
}