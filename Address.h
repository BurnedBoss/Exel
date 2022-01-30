#pragma once
#include<iostream>
#include<string>

using std::string;
using std::cin;
class Address {
	int row = 0;
	int col = 0;
	bool isAbsolute = true;
public:
	void read() {		
		string str;
		cin >> str;
		parse(str);
	}

	int getRow() const {
		return row;
	}

	int getCol() const {
		return col;
	}

	bool isAbsoluteAddress() const {
		return isAbsolute;
	}

	void parse(const string& str) {
		row = 0;
		col = 0;
		isAbsolute = true;

		bool weAreReadingRow = true;
		bool isNegative = false;
		for (int i = 0; i < str.size(); i++) {
			if (str[i] == 'C') {
				weAreReadingRow = false;
				if (isNegative) {
					row *= -1;
				}
				isNegative = false;
			}
			else if (str[i] == '[') {
				isAbsolute = false;
			}
			else if (str[i] == '-') {
				isNegative = true;
			}
			else if (str[i] >= '0' && str[i] <= '9') {
				if (weAreReadingRow) {
					row = row * 10 + str[i] - '0';
				}
				else {
					col = col * 10 + str[i] - '0';
				}
			}
		}
		if (isNegative) {
			col *= -1;
		}
	}

	void setRow(int row) {
		this->row = row;
	}

	void setCol(int col) {
		this->col = col;
	}

	void makeAbsoluteAddress() {
		this->isAbsolute = true;
	}

	void makeRelativeAddress() {
		this->isAbsolute = false;
	}
};
