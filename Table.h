#pragma once
#include<iostream>
#include<vector>
#include<fstream>
#include"Cell.h"

using std::vector;
using std::ofstream;
using std::ifstream;
using std::cout;
using std::endl;

class Table {
	vector<vector<Cell>> cells;
	bool validCoordinates(int row, int col) {
		return row >= 0 && col >= 0 && row < cells.size() && col < numberOfColumns();
	}
public:
	void setExpression(int i, int j, const string& expression) {
		for (int k = cells.size(); k <= i; k++) {
			cells.push_back(vector<Cell>());
		}
		int newNumberOfColumns = j + 1;
		if (j + 1 < numberOfColumns()) {
			newNumberOfColumns = numberOfColumns();
		}
		for (int k = 0; k < cells.size(); k++) {
			for (int l = cells[k].size(); l < newNumberOfColumns; l++) {
				cells[k].push_back(Cell(&cells, k, l));
			}
		}
		cells[i][j].setExpression(expression);
	}

	string getValue(int i, int j) const {
		if (i >= cells.size() || j >= numberOfColumns()) {
			return string();
		}
		return cells[i][j].getValue();
	}

	string getExpression(int i, int j) const {
		if (i >= cells.size() || j >= numberOfColumns()) {
			return string();
		}
		return cells[i][j].getExpression();
	}

	int numberOfColumns() const {
		if (cells.size() == 0) {
			return 0;
		}
		return cells[0].size();
	}

	void printAllValues() const {
		vector<int> maxWidths;
		for (int j = 0; j < numberOfColumns(); j++) {
			int max = cells[0][j].getValue().size();
			for (int i = 1; i < cells.size(); i++) {
				if (max < cells[i][j].getValue().size()) {
					max = cells[i][j].getValue().size();
				}
			}
			maxWidths.push_back(max);
		}

		for (int i = 0; i < cells.size(); i++) {
			for (int j = 0; j < cells[i].size(); j++) {
				cout << "|";
				for (int k = cells[i][j].getValue().size(); k < maxWidths[j]; k++) {
					cout << " ";
				}
				cout << cells[i][j].getValue();
			}
			cout << "|" << endl;
		}
	}

	void printAllExpressions() const {
		vector<int> maxWidths;
		for (int j = 0; j < numberOfColumns(); j++) {
			int max = cells[0][j].getExpression().size();
			for (int i = 1; i < cells.size(); i++) {
				if (max < cells[i][j].getExpression().size()) {
					max = cells[i][j].getExpression().size();
				}
			}
			maxWidths.push_back(max);
		}

		for (int i = 0; i < cells.size(); i++) {
			for (int j = 0; j < cells[i].size(); j++) {
				cout << "|";
				for (int k = cells[i][j].getExpression().size(); k < maxWidths[j]; k++) {
					cout << " ";
				}
				cout << cells[i][j].getExpression();
			}
			cout << "|" << endl;
		}
	}

	void saveInFile(const string& fileName) {
		ofstream out;
		out.open(fileName);
		for (int row = 0; row < cells.size(); row++) {
			for (int col = 0; col < cells[row].size(); col++) {
				out << cells[row][col].getExpression();
				if (col != cells[row].size()-1) {
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