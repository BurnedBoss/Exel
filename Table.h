#pragma once
#include<iostream>
#include<vector>
#include<fstream>
#include"Cell.h"
#include<map>
#include<utility>

using std::vector;
using std::ofstream;
using std::ifstream;
using std::cout;
using std::endl;
using std::map;
using std::pair;


class Table {
	map<int,map<int,Cell>> cells;
	bool validCoordinates(int row, int col) const {
		return cells.count(row) && cells.at(row).count(col);
	}
public:
	void setExpression(int row, int col, const string& expression) {
		if (!cells.count(row)) {
			cells[row] = map<int, Cell>();
		}
		cells[row][col] = Cell(&cells, row, col, expression);
	}

	string getValue(int row, int col) const {
		if (validCoordinates(row, col)) {
			return cells.at(row).at(col).getValue();
		}
		return string();
	}

	string getExpression(int row, int col) const {
		if (validCoordinates(row, col)) {
			return cells.at(row).at(col).getExpression();
		}
		return string();
	}

	int numberOfRows() const {
		int max = 0;
		for (pair<int, map<int,Cell>> p : cells) {
			if (max < p.first) {
				max = p.first;
			}
		}
		return max+1;
	}

	int numberOfColumns() const {
		int max = 0;
		for (pair<int, map<int, Cell>> p : cells) {
			for (pair<int, Cell> p1 : p.second) {
				if (max < p1.first) {
					max = p1.first;
				}
			}
		}
		return max + 1;
	}

	void printAllValues() const {
		vector<int> maxWidths;
		int colSize = numberOfColumns();
		int rowSize = numberOfRows();
		for (int j = 0; j < colSize; j++) {
			int max = 0;
			for (int i = 0; i < rowSize; i++) {
				if (max < getValue(i, j).size()) {
					max = getValue(i, j).size();
				}
			}
			maxWidths.push_back(max);
		}

		for (int i = 0; i < rowSize; i++) {
			for (int j = 0; j < colSize; j++) {
				cout << "|";
				for (int k = getValue(i, j).size(); k < maxWidths[j]; k++) {
					cout << " ";
				}
				cout << getValue(i, j);
			}
			cout << "|" << endl;
		}
	}

	void printAllExpressions() const {
		vector<int> maxWidths;
		int colSize = numberOfColumns();
		int rowSize = numberOfRows();
		for (int j = 0; j < colSize; j++) {
			int max = 0;
			for (int i = 0; i < rowSize; i++) {
				if (max < getExpression(i, j).size()) {
					max = getExpression(i, j).size();
				}
			}
			maxWidths.push_back(max);
		}

		for (int i = 0; i < rowSize; i++) {
			for (int j = 0; j < colSize; j++) {
				cout << "|";
				for (int k = getExpression(i, j).size(); k < maxWidths[j]; k++) {
					cout << " ";
				}
				cout << getExpression(i, j);
			}
			cout << "|" << endl;
		}
	}

	void saveInFile(const string& fileName) const {
		ofstream out;
		out.open(fileName);
		int colSize = numberOfColumns();
		int rowSize = numberOfRows();
		for (int i = 0; i < rowSize; i++) {
			for (int j = 0; j < colSize; j++) {
				out << getExpression(i, j);
				if (j != colSize-1) {
					out << ";";
				}
				else {
					out << "\n";
				}
			}
		}
		out.close();
	}

	void loadFromFile(const string& fileName) {
		ifstream in;
		in.open(fileName);
		int row = 0;
		int col = 0;
		while (!in.eof()) {
			string expression;
			while (in.peek() != ';' && in.peek() != '\n' && in.peek() != -1) {
				expression.push_back(in.get());
			}			
			setExpression(row, col, expression);
			if (in.peek() == ';') {
				col++;
			}
			else {
				row++;
				col = 0;
			}
			in.ignore();
		}
		in.close();
	}

	void incrementValue(int row, int col) {
		if (validCoordinates(row, col)) {
			cells[row][col].incrementValue();
		}
	}

	void decrementValue(int row, int col) {
		if (validCoordinates(row, col)) {
			cells[row][col].decrementValue();
		}
	}
};