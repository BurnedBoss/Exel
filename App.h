#pragma once
#include<iostream>
#include<string>
#include"Table.h"
#include"Address.h"

using std::cin;
using std::cout;
using std::string;
using std::endl;

class App {
    Table table;

    void set() {     
        Address address;
        string expression;
        address.read();
        while (cin.peek() == ' ') {
            cin.ignore();
        }
        getline(cin, expression);
        table.setExpression(address.getRow(), address.getCol(), expression);
        cout << "Table was successfully updated" << endl;
    }

    void print() {
        string valOrExpr;
        string addressOrAll;
        cin >> valOrExpr;
        cin >> addressOrAll;
        if(valOrExpr == "VAL") {
            if (addressOrAll == "ALL") {
                table.printAllValues();
            }
            else {
                Address address;
                address.parse(addressOrAll);
                cout << table.getValue(address.getRow(), address.getCol()) << endl;
            }
        }
        else {
            if (addressOrAll == "ALL") {
                table.printAllExpressions();
            }
            else {
                Address address;
                address.parse(addressOrAll);
                cout << table.getExpression(address.getRow(), address.getCol()) << endl;
            }
        }
    }

    void save() {
        string fileName;
        cin >> fileName;
        if (fileName.substr(fileName.size() - 4) != ".csv") {
            fileName += ".csv";
        }
        table.saveInFile(fileName);
        cout << "Table was saved in " << fileName << endl;
    }

    // TO DO both ways
    void load() {
        string fileName;
        cin >> fileName;
        if (fileName.substr(fileName.size() - 4) != ".csv") {
            fileName += ".csv";
        }
        table.loadFromFile(fileName);
        cout << "Table was loaded from " << fileName << endl;
    }

    void increment() {
        Address address;
        address.read();
        table.incrementValue(address.getRow(),address.getCol());
        cout << "Table was successfully updated" << endl;
    }

    void decrement() {
        Address address;
        address.read();
        table.decrementValue(address.getRow(), address.getCol());
        cout << "Table was successfully updated" << endl;
    }
public:
    void run() {        
        bool running = true;
        while (running) {
            string command;
            cin >> command;
            if (command == "SET") {
                set();
            }
            else if (command == "PRINT") {
                print();
            }
            else if (command == "SAVE") {
                save();
            }
            else if (command == "LOAD") {
                load();
            }
            else if (command == "++") {
                increment();
            }
            else if (command == "--") {
                decrement();
            }
            else if (command == "QUIT" || command =="EXIT" || command == "STOP") {
                running = false;
            }
            else {
                cout << "Wrong command\n";
            }
            cout << endl;
        }
    }
};