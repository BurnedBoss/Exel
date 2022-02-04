#pragma once
#include<string>
#include<stack>
#include<map>
#include"Address.h"

using std::stack;
using std::string;
using std::stoi;
using std::stod;
using std::to_string;
using std::map;

class Cell {
	string expression;
	map<int,map<int,Cell>>* cells;
	int row;
	int col;

	string getValueByAddress(const Address& address) const {
		int otherRow = address.getRow();
		int otherCol = address.getCol();
		if (!address.isAbsoluteAddress()) {
			otherRow += row;
			otherCol += col;
		}
		if (validCoordinates(otherRow, otherCol)) {
			return cells->at(otherRow).at(otherCol).getValue();
		}
		return string();
	}

	bool validCoordinates(int row, int col) const {
		return cells->count(row) && cells->at(row).count(col);
	}

	string trim(const string& s) const {
		int i = 0;
		skipIntervals(s, i);
		int end = s.length() - 1;
		while(s[end] == ' ') {
			end--;
		}
		return s.substr(i, end-i+1);
	}

	bool isNumber(const string& str) const {
		if (str[0] != '-' && (str[0] < '0' || str[0] > '9')) {
			return false;
		}
		bool dotFound = false;
		for (int i = 1; i < str.size(); i++) {
			if (str[i] == '.') {
				if (dotFound || !isDigit(str[i-1]) || !isDigit(str[i+1])) {
					return false;
				}
				dotFound = true;
			}
			else if (str[i] < '0' || str[i] > '9') {
				return false;
			}
		}
		return true;
	}

	bool isInteger(const string& str) const {
		if (str[0] != '-' && (str[0] < '0' || str[0] > '9')) {
			return false;
		}
		for (int i = 1; i < str.size(); i++) {
			if (str[i] < '0' || str[i] > '9') {
				return false;
			}
		}
		return true;
	}

	bool isDigit(char symbol) const {
		return symbol >= '0' && symbol <= '9';
	}

	bool isComparisonSymbol(char symbol) const {
		return symbol == '>' || symbol == '<' || symbol == '=' || symbol == '!';
	}


	void skipIntervals(const string& expression, int& i) const {
		while (expression[i] == ' ') {
			i++;
		}
	}

	string captureSubstring(const string& expression, int& i, char end) const {
		string result;
		while (expression[i] != end && expression[i] != '\0') {
			result.push_back(expression[i]);
			i++;
		}
		return trim(result);
	}

	string captureBalancedSubstring(const string& expression, int& i, char end) const {
		string result;
		int balance = 0;
		while (expression[i] != '\0' && (expression[i] != end || balance > 0)) {
			if(expression[i] == '(') {
				balance++;
			}
			else if(expression[i] == ')') {
				balance--;
			}
			result.push_back(expression[i]);
			i++;
		}
		return trim(result);
	}

	string captureComparisonOperator(const string& expression, int& i) const {
		string result;
		while (expression[i] != '\0' && isComparisonSymbol(expression[i])) {
			result.push_back(expression[i]);
			i++;
		}
		return trim(result);
	}

	string captureComparisonOperand1(const string& expression, int& i) const {
		string result;
		while (expression[i] != '\0' && !isComparisonSymbol(expression[i])) {
			result.push_back(expression[i]);
			i++;
		}
		return trim(result);
	}

	string captureComparisonOperand2(const string& expression, int& i) const {
		string result;
		while (!(expression[i] == 'a' && expression[i+1] == 'n' && expression[i+2] == 'd')
			&& !(expression[i] == 'o' && expression[i+1] == 'r')
			&&   expression[i] != '\0') {
			result.push_back(expression[i]);
			i++;
		}
		return trim(result);
	}

	
	double calculate(double x, char op, double y) const {
		if (op == '+') {
			return x + y;
		}
	    if (op == '-') {
			return x - y;
		}
		if (op == '*') {
			return x * y;
		}
		if (op == '/') {
			return x / y;
		}
		return 0;
	}

	bool calculateLogicOperation(bool first, const string& op, bool second) const {
		if (op == "or") {
			return first || second;
		}
		if (op == "and") {
			return first && second;
		}
		return false;
	}

	bool compareNumbers(double first, const string& op,double second) const {
		if (op == ">") {
			return first > second;
		}
		if (op == "<") {
			return first < second;
		}
		if (op == ">=") {
			return first >= second;
		}
		if (op == "<=") {
			return first <= second;
		}
		if (op == "!=") {
			return first != second;
		}
		if (op == "==") {
			return first == second;
		}
		return false;
	}

	bool compare(const string& first, const string& op, const string& second) const {
		if (isNumber(first) && isNumber(second)) {
			return compareNumbers(stod(first), op, stod(second));
		}
		if (op == ">") {
			return first > second;
		}
		if (op == "<") {
			return first < second;
		}
		if (op == ">=") {
			return first >= second;
		}
		if (op == "<=") {
			return first <= second;
		}
		if (op == "!=") {
			return first != second;
		}
		if (op == "==") {
			return first == second;
		}
		return false;
	}

	bool toBool(const string& str) const {
		return str == "true";
	}

	string stringFromBool(bool b) const {
		if(b == true) {
			return "true";
		}
		return "false";
	}

	Address invalidAddress() const {
		return Address(-1, -1, true);
	}

	Address captureAddress(const string& expression, int& i) const {
		if (expression[i + 1] != '[' && !isDigit(expression[i+1])) {
			return invalidAddress();
		}
		Address address;
		string rowNumber;
		string colNumber;
		if (expression[i + 1] == '[') {
			i += 2;
			rowNumber = captureSubstring(expression, i, ']');
			if (expression[i] == '\0' || !isInteger(rowNumber)) {
				return invalidAddress();
			}
			i++;
			if (expression[i] != 'C' || expression[i + 1] != '[') {
				return invalidAddress();
			}
			i += 2;
			colNumber = captureSubstring(expression, i, ']');
			if (expression[i] == '\0' || !isInteger(colNumber)) {
				return invalidAddress();
			}
			i++;
			address.makeRelativeAddress();
		}
		else {
			i++;
			rowNumber = captureSubstring(expression, i, 'C');
			if (expression[i] == '\0' || !isInteger(rowNumber)) {
				return invalidAddress();
			}
			i++;
			while (isDigit(expression[i])) {
				colNumber.push_back(expression[i]);
				i++;
			}
			if (expression[i] == '\0' || !isInteger(colNumber)) {
				return invalidAddress();
			}
			address.makeAbsoluteAddress();
		}
		address.setRow(stoi(rowNumber));
		address.setCol(stoi(colNumber));
		return address;
	}

	bool captureAndHandleAddress(const string& expression,int& i, bool& operationIsWaitingForCurrentNumber,stack<double>& numbers, stack<char>& operations) const {
		Address address = captureAddress(expression, i);
		if (!address.isValid()) {
			return false;
		}
		string otherCellValue = getValueByAddress(address);
		if (!isNumber(otherCellValue)) {
			return false;
		}
		putNumberInStack(operationIsWaitingForCurrentNumber, numbers, operations, stod(otherCellValue));
		return true;
	}

	bool captureAndHandleSum(const string& expression, int& i, bool& operationIsWaitingForCurrentNumber, stack<double>& numbers, stack<char>& operations) const {
		if (expression[i + 1] != 'u' || expression[i + 2] != 'm') {
			return false;
		}
		i += 3;
		skipIntervals(expression, i);
		if (expression[i] != '(') {
			return false;
		}
		i++;
		skipIntervals(expression, i);
		Address firstAddress = captureAddress(expression, i);
		if (!firstAddress.isValid()) {
			return false;
		}
		skipIntervals(expression, i);
		if (expression[i] != ',') {
			return false;
		}
		i++;
		skipIntervals(expression, i);
		Address secondAddress = captureAddress(expression, i);
		if (!secondAddress.isValid()) {
			return false;
		}
		skipIntervals(expression, i);
		if (expression[i] != ')') {
			return false;
		}
		i++;
		int startRow = firstAddress.getRow();
		int startCol = firstAddress.getCol();
		int endRow = secondAddress.getRow();
		int endCol = secondAddress.getCol();
		if (!firstAddress.isAbsoluteAddress()) {
			startRow += row;
			startCol += col;
		}
		if (!secondAddress.isAbsoluteAddress()) {
			endRow += row;
			endCol += col;
		}
		double sum = 0;
		for (int r = startRow; r <= endRow; r++) {
			for (int c = startCol; c <= endCol; c++) {
				string value = getValueByAddress(Address(r, c));
				if (isNumber(value)) {
					sum += stod(value);
				}
			}
		}
		putNumberInStack(operationIsWaitingForCurrentNumber, numbers, operations, sum);
		return true;
	}

	bool captureAndHandleCount(const string& expression, int& i, bool& operationIsWaitingForCurrentNumber, stack<double>& numbers, stack<char>& operations) const {
		if (expression[i+1] != 'o' || expression[i + 2] != 'u' || expression[i + 3] != 'n' || expression[i+4] != 't') {
			return false;
		}
		i += 5;
		skipIntervals(expression, i);
		if (expression[i] != '(') {
			return false;
		}
		i++;
		skipIntervals(expression, i);
		Address firstAddress = captureAddress(expression, i);
		if (!firstAddress.isValid()) {
			return false;
		}
		skipIntervals(expression, i);
		if (expression[i] != ',') {
			return false;
		}
		i++;
		skipIntervals(expression, i);
		Address secondAddress = captureAddress(expression, i);
		if (!secondAddress.isValid()) {
			return false;
		}
		skipIntervals(expression, i);
		if (expression[i] != ')') {
			return false;
		}
		i++;
		int startRow = firstAddress.getRow();
		int startCol = firstAddress.getCol();
		int endRow = secondAddress.getRow();
		int endCol = secondAddress.getCol();
		if (!firstAddress.isAbsoluteAddress()) {
			startRow += row;
			startCol += col;
		}
		if (!secondAddress.isAbsoluteAddress()) {
			endRow += row;
			endCol += col;
		}
		double count = 0;
		for (int r = startRow; r <= endRow; r++) {
			for (int c = startCol; c <= endCol; c++) {
				string value = getValueByAddress(Address(r, c));
				if (!value.empty()) {
					count++;
				}
			}
		}
		putNumberInStack(operationIsWaitingForCurrentNumber, numbers, operations, count);
		return true;
	}

	void putNumberInStack(bool operationIsWaitingForCurrentNumber, stack<double>& numbers, stack<char>& operations, double num)  const{
		if (operationIsWaitingForCurrentNumber) {
			double waitingNumber = numbers.top();
			char waitingOperation = operations.top();
			numbers.pop();
			operations.pop();
			numbers.push(calculate(waitingNumber, waitingOperation, num));
		}
		else {
			numbers.push(num);
		}		
	}

	string tryToParseAsArithmeticFormula(const string& expression) const {
		stack<double> numbers;
		stack<char> operations;
		string num;
		bool operationIsWaitingForCurrentNumber = false;
		int i = 0;
		skipIntervals(expression, i);
		if (expression[i] == '-') {
			i++;
			skipIntervals(expression, i);
			if (expression[i] == ')' || expression[i] == '+' || expression[i] == '*' || expression[i] == '/' || expression[i] == '-') {
				return expression;
			}
			i--;
			numbers.push(0);
			operationIsWaitingForCurrentNumber = true;
			operations.push('-');
		}
		for(; i < expression.size(); i++) {			
			if(isDigit(expression[i]) || expression[i] == '.') {
				num.push_back(expression[i]);
				if (!isDigit(expression[i + 1]) && expression[i + 1] != '.') {
					if (!isNumber(num)) {
						return expression;
					}
					putNumberInStack(operationIsWaitingForCurrentNumber, numbers, operations, stod(num));
					num = string();
				}
			}
			else if (expression[i] =='s') {
				if (!captureAndHandleSum(expression, i, operationIsWaitingForCurrentNumber, numbers, operations)) {
					return expression;
				}
				i--;
			}
			else if (expression[i] == 'c') {
				if (!captureAndHandleCount(expression, i, operationIsWaitingForCurrentNumber, numbers, operations)) {
					return expression;
				}
				i--;
			}
			else if (expression[i] == 'R') {
				if (!captureAndHandleAddress(expression, i, operationIsWaitingForCurrentNumber, numbers, operations)) {
					return expression;
				}
				i--;
			}
			else if (expression[i] == '(') {
				operationIsWaitingForCurrentNumber = false;
				i++;
				skipIntervals(expression, i);
				if (expression[i] == '-') {
					i++;
					skipIntervals(expression, i);
					if (expression[i] == ')' || expression[i] == '+' || expression[i] == '*' || expression[i] == '/' || expression[i] == '-') {
						return expression;
					}
					i--;
					numbers.push(0);
					operationIsWaitingForCurrentNumber = true;
					operations.push('-');					
				}
				else if (expression[i] == ')' || expression[i] == '+' || expression[i] == '*' || expression[i] == '/') {
					return expression;
				}
				else {
					i--;
				}
			}
			else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
				operationIsWaitingForCurrentNumber = true;
				operations.push(expression[i]);
				i++;
				skipIntervals(expression, i);
				if (expression[i] == ')' || expression[i] == '+' || expression[i] == '*' || expression[i] == '/'  || expression[i] == '-') {
					return expression;
				}
				i--;
			}
			else if(expression[i] != ')' && expression[i] != ' ') {
				return expression;
			}
		}
		if (numbers.size() == 0) {
			return expression;
		}
		while (numbers.size() != 1) {
			double num1 = numbers.top();
			numbers.pop();
			double num2 = numbers.top();
			numbers.pop();
			numbers.push(calculate(num2, operations.top(), num1));
			operations.pop();
		}
		string result = to_string(numbers.top());
		result.erase(result.find_last_not_of('0') + 1, string::npos);
		if (result.back() == '.') {
			result.pop_back();
		}
		return result;
	}
    
	string tryToParseAsLogicFormula(const string& expression) const {
		int i = 0;
		skipIntervals(expression, i);
		if (expression[i] != 'i' && expression[i] != 'I') {
			return expression;
		}
		
		i++;
		if (expression[i] != 'f' && expression[i] != 'F') {
			return expression;
		}

		i++;
		skipIntervals(expression, i);
		if (expression[i] != '(') {
			return expression;
		}
		
		i++;
		string condition = captureBalancedSubstring(expression, i, ',');
		string valueOfCondition = evaluateCondition(condition);
		if (expression[i] == '\0' || valueOfCondition == "error") {
			return expression;
		}

		i++;
		string valueIfTrue = captureBalancedSubstring(expression, i, ',');
		if (expression[i] == '\0') {
			return expression;
		}
		
		i++;
		string valueIfFalse = captureBalancedSubstring(expression, i, ')');
		if (expression[i] == '\0') {
			return expression;
		}
		
		i++;
		skipIntervals(expression, i);
		if (expression[i] != '\0') {
			return expression;
		}
		
		if (valueOfCondition == "true") {
			return evaluateSubexpression(trim(valueIfTrue));
		}
		return evaluateSubexpression(trim(valueIfFalse));
	}


	string evaluateConditionStartingWithBracket(const string& condition, int i) const {
		string operand1 = captureSubstring(expression, i, ')');
		if (condition[i] == '\0') {
			return "error";
		}
		string value1 = evaluateCondition(operand1);
		if (value1 == "true" || value1 == "false") {			
			i++;
			skipIntervals(condition, i);
			string logicOperator = captureSubstring(condition, i, ' ');
			if (logicOperator == "or" || logicOperator == "and") {
				string operand2 = captureSubstring(condition, i, '\0');
				string value2 = evaluateCondition(operand2);
				if (value2 == "error") {
					return "error";
				}					
				return stringFromBool(calculateLogicOperation(toBool(value1), operand2, toBool(value2)));				
			}
			return "error";
		}			
		value1 = evaluateSubexpression(operand1);
		i++;
		skipIntervals(condition, i);
		string comparisonOperator = captureSubstring(condition, i, ' ');
		if (comparisonOperator == ">" || comparisonOperator == "<" || comparisonOperator == ">=" || comparisonOperator == "<=" || comparisonOperator == "!=" || comparisonOperator == "==") {
			string operand2 = captureSubstring(condition, i, '\0');
			return stringFromBool(compare(operand1, comparisonOperator,operand2));
		}
		return "error";
	}

	string evaluateCondition(const string& condition) const {
		int i = 0;
		skipIntervals(condition, i);
		
		if (condition[i] == '(') {
			return evaluateConditionStartingWithBracket(condition, i);
		}
		
		string comparisonOperand1 = captureComparisonOperand1(condition, i);
		if (condition[i] == '\0') {
			return "error";
		}
		string comparisonOperator = captureComparisonOperator(condition, i);
		if (condition[i] == '\0') {
			return "error";
		}
		string comparisonOperand2 = captureComparisonOperand2(condition, i);
		bool resultFromComparison = compare(comparisonOperand1, comparisonOperator, comparisonOperand2);

		if (condition[i] == '\0') {
			return stringFromBool(resultFromComparison);
		}
		
		string logicOperator = captureSubstring(condition, i, ' ');
		string logicOperand2 = evaluateCondition(captureSubstring(condition, i, '\0'));
		if (logicOperand2 == "error") {
			return "error";
		}
		return stringFromBool(calculateLogicOperation(resultFromComparison, logicOperator, toBool(logicOperand2)));
	}

	bool isValidAddress(const string& str) const {
		if (str[0] != 'R') {
			return false;
		}
		if (str[1] != '[' || !isDigit(str[1])) {
			return false;
		}
		if (str[1] == '[') {
			int i = 2;
			string rowNumber = captureSubstring(str, i, ']');
			if (str[i] == '\0'|| !isInteger(rowNumber)) {
				return false;
			}
			i++;
			if (str[i] != 'C' || str[i+1] != '[') {
				return false;
			}
			i += 2;
			string colNumber = captureSubstring(str, i, ']');
			if (str[i] == '\0' || !isInteger(colNumber)) {
				return false;
			}
			i++;
			return str[i] == '\0';
		}
		int i = 1;
		string rowNumber = captureSubstring(str, i, 'C');
		if (str[i] == '\0' || !isInteger(rowNumber)) {
		    return false;
		}
		i++;
		string colNumber = captureSubstring(str, i, '\0');
		return isInteger(colNumber);
	}

	string evaluateSubexpression(const string& expression) const {
		if (isValidAddress(expression)) {
			Address address;
			address.parse(expression);
			return getValueByAddress(address);
		}
		string valueAsLogicFormula = tryToParseAsLogicFormula(expression);
		if (valueAsLogicFormula == expression) {
			return tryToParseAsArithmeticFormula(expression);
		}
		return valueAsLogicFormula;
	}

public:
	Cell(map<int, map<int, Cell>>* cells = nullptr, int row = 0, int col = 0, string expression = string()) {
		this->cells = cells;
		this->row = row;
		this->col = col;
		this->expression = expression;
	}

	string getExpression() const {
		return expression;
	}

	void setExpression(const string& expression) {
		this->expression = expression;
	}


	string getValue() const {
		return evaluateSubexpression(expression);
	}

	void incrementValue() {
		string value = getValue();
		if(isInteger(value)) {
			setExpression(to_string(stod(value) + 1));
		}
	}

	void decrementValue() {
		string value = getValue();
		if (isInteger(value)) {
			setExpression(to_string(stod(value) - 1));
		}
	}
};