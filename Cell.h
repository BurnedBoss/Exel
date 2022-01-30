#pragma once
#include<string>
#include<stack>
#include<vector>
#include"Address.h"

using std::vector;
using std::stack;
using std::string;
using std::stoi;
using std::to_string;

#include<iostream>
using std::cout;

class Cell {
	string expression;
	vector<vector<Cell>>* cells;
	int row;
	int col;

	string getValueByAddress (const Address& address) const {
		if (address.isAbsoluteAddress()) {
			return (*cells)[address.getRow()][address.getCol()].getValue();
		}
		return (*cells)[address.getRow()+row][address.getCol()+col].getValue();
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


	bool isInteger(const string& str) const {
		if (str[0] != '-' && (str[0] < '0' || str[0] > '9')) {
			return false;
		}
		for (int i = 1; i < str.size(); i++) {
			if (str[i] <= '0' || str[i] >= '9') {
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


	int calculate(int x, char op, int y) const {
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

	bool compareNumbers(int first, const string& op,int second) const {
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
		if (isInteger(first) && isInteger(second)) {
			return compareNumbers(stoi(first), op, stoi(second));
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
public:

	Cell(vector<vector<Cell>>* cells = nullptr, int row = 0,int col = 0) {
		this->cells = cells;
		this->row = row;
		this->col = col;
	}

	string getExpression() const {
		return expression;
	}

	void setExpression(const string& expression) {
		this->expression = expression;
	}


	void putNumberInStack(bool operationIsWaitingForCurrentNumber, stack<int>& numbers, stack<char>& operations, int num)  const{
		if (operationIsWaitingForCurrentNumber) {
			int waitingNumber = numbers.top();
			int waitingOperation = operations.top();
			numbers.pop();
			operations.pop();
			numbers.push(calculate(waitingNumber, waitingOperation, num));
		}
		else {
			numbers.push(num);
		}
		
	}

	// (12+34-(3*5
	string tryToParseAsArithmeticFormula(const string& expression) const {
		stack<int> numbers;
		stack<char> operations;
		int num = 0;
		bool operationIsWaitingForCurrentNumber = false;
		for(int i = 0; i < expression.size(); i++) {			
			if(isDigit(expression[i])) {
				num = num * 10 + expression[i] - '0';
				if (!isDigit(expression[i + 1])) {
					putNumberInStack(operationIsWaitingForCurrentNumber, numbers, operations, num);
					num = 0;
				}
			}
			else if (expression[i] == 'R' ) {
				Address address;
				if (expression[i + 1] == '[') {
					i += 2;
					string  rowNumber = captureSubstring(expression, i, ']');
					if (expression[i] == '\0' || !isInteger(rowNumber)) {
						return expression;
					}
					i++;
					if (expression[i] != 'C' || expression[i+1] != '[') {
						return expression;
					}
					i += 2;
					string colNumber = captureSubstring(expression, i, ']');
					if (expression[i] == '\0' || !isInteger(colNumber)) {
						return expression;
					}
					i++;
					address.setRow(stoi(rowNumber));
					address.setCol(stoi(colNumber));
					address.makeRelativeAddress();
					string otherCellValue = getValueByAddress(address);
					if (!isInteger(otherCellValue)) {
						return expression;
					}
					putNumberInStack(operationIsWaitingForCurrentNumber, numbers, operations, stoi(otherCellValue));
				}
				else if (isDigit(expression[i + 1])) {
					i++;
					string  rowNumber = captureSubstring(expression, i, 'C');
					if (expression[i] == '\0' || !isInteger(rowNumber)) {
						return expression;
					}
					i++;
					if (!isDigit(expression[i])) {
						return expression;
					}					
					string colNumber = captureSubstring(expression, i, ' ');
					if (expression[i] == '\0' || !isInteger(colNumber)) {
						return expression;
					}
					address.setRow(stoi(rowNumber));
					address.setCol(stoi(colNumber));
					address.makeAbsoluteAddress();
					string otherCellValue = getValueByAddress(address);
					if (!isInteger(otherCellValue)) {
						return expression;
					}
					putNumberInStack(operationIsWaitingForCurrentNumber, numbers, operations, stoi(otherCellValue));
				}
				else {
					return expression;
				}				
			}
			else if (expression[i] == '(') {
				operationIsWaitingForCurrentNumber = false;
			}
			else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
				operationIsWaitingForCurrentNumber = true;
				operations.push(expression[i]);
			}
		}
		if (numbers.size() == 0) {
			return expression;
		}
		while (numbers.size() != 1) {
			int num1 = numbers.top();
			numbers.pop();
			int num2 = numbers.top();
			numbers.pop();
			numbers.push(calculate(num2, operations.top(), num1));
			operations.pop();
		}
		return to_string(numbers.top());
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


	string getValue() const {
		return evaluateSubexpression(expression);
	}

	void incrementValue() {
		string value = getValue();
		if(isInteger(value)) {
			setExpression(to_string(stoi(value) + 1));
		}
	}

	void decrementValue() {
		string value = getValue();
		if (isInteger(value)) {
			setExpression(to_string(stoi(value) - 1));
		}
	}
};