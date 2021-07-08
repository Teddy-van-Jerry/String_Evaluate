/* 
 * Project: String_Complex
 * File: String_Complex.h
 * Description: calculate std::string and return std::complex
 * ---------------------------
 * 
 * @author: Teddy van Jerry
 * @licence: MIT Licence
 * @compiler: At least C++11
 * 
 * @version 1.0: 2021/07/05
 * - initial version
 * 
 */

#pragma once
#include <istream>
#include <complex>
#include <string>
#include <stack>
#include <vector>

static const std::vector<std::string> empty_string_list_;           // an empty string list
static const std::vector<std::complex<double>> empty_complex_list_; // an empty complex list
static constexpr std::complex<double> ERROR_COMPLEX { 0, 0 };       // complex of error result
static constexpr std::complex<double> IMAG_I{ 0, 1 };               // the imaginary unit

bool bracket(std:: string s);										//Checking for anomalies

// If it is defined, error information will be printed.
#define COMPLEX_CERR_ERROR_

/**
 * \brief Complex number with argument 'real' and 'imag'.
 */
static inline std::complex<double> Comp(double real, double imag) {
	return real + imag * IMAG_I;
}

/**
 * \brief type 'var' is used in stack
 *
 * if 'str' is empty, then it is 'num'
 */
struct var {
	std::string str;
	std::complex<double> num;

	// constructors
	var(std::string s) : str(s) {}
	var(std::complex<double> n) : num(n) {}
};

/**
 * \brief Change 'c' with 'from' to the corresponding letter in 'to'
 */
inline void char_replace(char& c, const std::string& from, const std::string& to) {
	for (auto i = from.begin(); i != from.end(); i++) {
		if (c == *i) {
			c = to[i - from.begin()];
			break;
		}
	}
}

/**
 * \brief Change char of 'str' with 'from' to the corresponding letter in 'to'
 */
inline void string_replace(std::string& str, const std::string& from, const std::string& to) {
	bool continue_ = true;
	for (auto& c : str) {
		char_replace(c, from, to);
	}
}

/**
 * \brief give error message
 */
inline void error_msg_(std::string* msg, const std::string& word, const char* file_, size_t line_) {
	if (msg) {
		*msg = word;
	}
#ifdef COMPLEX_CERR_ERROR_
	std::cerr << "ERROR: " << word << "\nAt \"" << file_ << ' ' << "\" Line " << line_ << std::endl;
#endif
}

/**
 * \brief give error message
 */
#ifndef error_msg
#define error_msg(msg, word) error_msg_(msg, word, __FILE__, __LINE__)
#endif

inline int operator_weight_index(char c) {
	switch (c) {
	case '+': return 0;
	case '-': return 1;
	case '*': return 2;
	case '/': return 3;
	case '^': return 4;
	case '(': return 5;
	case ')': return 6;
	case '&': return 7; // '&' is for function names
	default: return -1; // indicate error of undefined operator
	}
}

inline int operator_weight_compare(char c1, char c2) {
	int compare[8][8] {
		//        +   -   *   /   ^   (   )   &
		/* + */ { 1,  1, -1, -1, -1, -1,  1, -1 },
		/* - */ { 1,  1, -1, -1, -1, -1,  1, -1 },
		/* * */ { 1,  1,  1,  1, -1, -1,  1, -1 },
		/* / */ { 1,  1,  1,  1, -1, -1,  1, -1 },
		/* ^ */ { 1,  1,  1,  1,  1, -1,  1, -1 },
		/* ( */ {-1, -1, -1, -1, -1, -1,  0, -1 },
		/* ) */ { 1,  1,  1,  1,  1, 10,  1,  1 },
		/* & */ { 1,  1,  1,  1,  1, -1,  1,  1 }
		// The code 10 here indicates error
		// 1: pop the stack
		// -1: insert directly
	};
	if (operator_weight_index(c1) == -1 || operator_weight_index(c2) == -1) { // error
		return 11;
	}
	else {
		return compare[operator_weight_index(c1)][operator_weight_index(c2)];
	}
}

inline bool isoperator(char c) {
	std::string oper = "+-*/^()";
	for (const char& c_ : oper) {
		if (c == c_) {
			return true;
		}
	}
	return false;
}

inline bool isplus_minus_times_divide(char c) {
	std::string oper = "+-*/";
	for (const char& c_ : oper) {
		if (c == c_) {
			return true;
		}
	}
	return false;
}

inline void unary_to_binary(const std::string& str, std::string& new_str, typename std::string::iterator iter) {
	if (iter == str.begin()) {
		if (*iter == '+' || *iter == '-') {
			new_str.append("0");
		}
	}
	if (((iter != str.begin() && *(iter - 1) == '(' ) || iter == str.begin()) && (*iter == '+' || *iter == '-')) {
		new_str.append("0");
	}
	if (*iter == '(') {
		if (iter != str.begin() && isdigit(*(iter - 1))) {
			new_str.append("*");
		}
	}
	else if (*iter == '$' && iter != str.begin() && !isoperator(*(iter - 1))) {
		new_str.append("*");
	}
}

/**
 * \brief Pre-edit the 
 *
 * The initial string after pre-editing is str,
 * the final postorder result will be stored in vec.
 */
bool pre_edit(std::string& str) {
	// PART 1: change into standard form
	string_replace(str, "[{}]IJj", "(())iii");
	std::string new_str;
	if (str.empty()) {
		// empty string
		return true;
	}

	size_t valid_size = 0; // length without space
	for (auto iter = str.begin(); iter != str.end(); iter++) {
		// omit space
		if (!isspace(*iter)) {
			str[valid_size++] = *iter;
		}
	}
	str.erase(valid_size);
	
	for (auto iter = str.begin(); iter != str.end(); iter++) {

		bool add_self = true;

		// PART 2: change unary operators (+, -) into binary operators
		// Notice thay unary operators can only be seen
		// at the front or after '('.
		unary_to_binary(str, new_str, iter);
	
		//PART 3: change the surrounding of i
		if (*iter == 'i' &&
			(iter == str.begin() || isoperator(*(iter - 1)) || isdigit(*(iter - 1))) &&
			(iter + 1 == str.end() || isoperator(*(iter + 1)))) {
			// 'i' here is the imaginary unit
			if (iter != str.begin() && isoperator(*(iter - 1))) {
				if (*(iter - 1) == ')') {
					new_str.append("*@");
				}
				else {
					new_str.append("@");
				}
			}
			else if (iter != str.begin() && isdigit(*(iter - 1))) {
				if (iter + 1 != str.end() && *(iter + 1) != '^') {
					new_str.append("*@");
				}
				else {
					new_str.append("*(@");
					int br_cnt = 0; // bracket count
					while (iter != str.end() && !isplus_minus_times_divide(*iter) && !br_cnt) {
						if (*iter == '(') {
							br_cnt++;
						}
						else if (*iter == ')') {
							br_cnt--;
						}
						unary_to_binary(str, new_str, iter);
						iter++;
					}
					iter--;
					new_str.append(")");
				}
			}
			else {
				new_str.append("@");
			}
			add_self = false;
		}
		if (add_self) {
			new_str += *iter;
		}
	}

	// assign back
	str = new_str;
	return true;
}

/**
 * \brief Add a new operator
 *
 * compare the priority of operators,
 * decide to push directly or pop and then compares
 */
inline bool add_new_oper(const std::string& oper, std::string* msg, std::stack<std::string>& s, std::vector<var>& vec) {
	if (s.empty()) {
		if (oper != ")") {
			s.push(oper);
		}
		return true;
	}
	else {
		char c1 = s.top().length() > 1 ? '&' : s.top()[0];
		char c2 = oper.length() > 1 ? '&' : oper[0];
		int compare_ = operator_weight_compare(c1, c2);
		switch (compare_) {
		case -1:
			// push to stack directly
			if (oper != ")") {
				s.push(oper);
			}
			return true;
			break;
		case 1:
			// keep poping
			vec.push_back(s.top());
			s.pop();
			add_new_oper(oper, msg, s, vec);
			break;
		case 0:
			// ')' meets '('
			s.pop();
			break;
		default:
			error_msg(msg, "Mismatch of brackets");
			return false;
		}
	}
	return false; // normally it won't get here.
}

/**
 * \brief Change the inorder str to postorder
 *
 * The initial string after pre-editing is str,
 * the final postorder result will be stored in vec.
 */
bool to_postorder(const std::string& str, std::string* msg, std::vector<var>& vec, 
	std::vector<std::string> var_list, std::vector<std::complex<double>> num_list) {
	// temporary stack to store operator
	std::stack<std::string> s;
	
	for (auto iter = str.begin(); iter != str.end(); iter++) {
		if (*iter == '$') {
			// starting sign of variable
			// discard '$' which is not a part of the var name
			std::string var_name;
			while (iter != str.end() && ++iter != str.end() && !isoperator(*iter)) {
				var_name += *iter; // append char at the end
			}
			size_t i_ = 0;
			while (i_ != var_list.size()) {
				if (var_list[i_] == var_name) {
					if (i_ >= num_list.size()) {
						// overflow
						error_msg(msg, "Some variables values are not given");
						return false;
					}
					vec.push_back(num_list[i_]);
					break;
				}
				else {
					// check next variable name
					i_++;
				}
			}
			if (i_ == var_list.size()) {
				std::string msg_ = "Undefined variable: ";
				msg_.append(var_name);
				error_msg(msg, msg_);
				vec.clear();
				return false;
			}
			// no next element or the next element is an operator
			// end of the variable name, go back one
			iter--;
		}
		else if (*iter == '@') {
			// the imaginary unit i
			vec.push_back(IMAG_I);
		}
		else if (isoperator(*iter)) {
			// operator
			std::string oper(1, *iter);
			add_new_oper(oper, msg, s, vec);
		}
		else if (isdigit(*iter) || *iter == '.') {
			// number
			std::string num_str(1, *iter);
			while (iter != str.end() && ++iter != str.end() && (isdigit(*iter) || *iter == '.')) {
				num_str += *iter;
			}
			iter--;
			std::istringstream is(num_str);
			double number;
			// convert string number into double
			is >> number;
			if (is.fail()) {
				error_msg(msg, "Illegal input of number");
				vec.clear();
				return false;
			}
			vec.push_back(static_cast<std::complex<double>>(number));
		}
		else {
			// fun name
			std::string fun_name(1, *iter);
			while (iter != str.end() && ++iter != str.end() && !isoperator(*iter)) {
				fun_name += *iter; // append char at the end
			}
			// no next element or the next element is an operator
			// end of the function name, go back one
			iter--;
			add_new_oper(fun_name, msg, s, vec);
		}
	}
	while (!s.empty()) {
		vec.push_back(s.top());
		s.pop();
	}
	return true;
}

/**
 * \brief implement func in std::string
 */
inline std::complex<double> func(std::string fun_name, std::complex<double> num, std::string* msg) {
	// change fun_name to lower case
	for (auto& c : fun_name) {
		c = tolower(c);
	}
	auto result = ERROR_COMPLEX;
	if (fun_name == "exp") {
		result = exp(num);
	}
	else if (fun_name == "sqrt") {
		result = sqrt(num);
	}
	else if (fun_name == "square") {
		result = num * num;
	}
	else if (fun_name == "log" || fun_name == "ln") {
		result = log(num);
	}
	else if (fun_name == "sin") {
		result = sin(num);
	}
	else if (fun_name == "cos") {
		result = cos(num);
	}
	else if (fun_name == "tan") {
		result = tan(num);
	}
	else if (fun_name == "asin" || fun_name == "arcsin") {
		result = asin(num);
	}
	else if (fun_name == "acos" || fun_name == "arccos") {
		result = acos(num);
	}
	else if (fun_name == "atan" || fun_name == "arctan") {
		result = atan(num);
	}
	else if (fun_name == "arg" || fun_name == "argz") {
		result = atan2(num.real(), num.imag());
	}
	else if (fun_name == "abs" || fun_name == "fabs") {
		result = abs(num);
	}
	else {
		std::string msg_ = "Unknown function name: ";
		msg_.append(fun_name);
		error_msg(msg, msg_);
	}
	return result;
}

/**
 * \brief Evaluate the complex number from the string
 * 
 * The second argument msg is optional, return the status.
 * if it is empty, the process is correct;
 * Otherwise, it displays error information.
 */
std::complex<double> eval(std::string str,
	std::vector<std::string> var_list = empty_string_list_,
	std::vector<std::complex<double>> num_list = empty_complex_list_,
	std::string* msg = nullptr) {
	if (str.empty()) {
		error_msg(msg, "Empty string");
		return ERROR_COMPLEX;
	}
	for (const auto& c : str) {
		// check validity
		if (c < 0 || c > 255) {
			error_msg(msg, "Not ASCII characters");
			return ERROR_COMPLEX;
		}
	}
	if (!pre_edit(str)) {
		// error handling
		return ERROR_COMPLEX;
	}
	std::vector<var> post_order;
	if (!to_postorder(str, msg, post_order, var_list, num_list)) {
		// error handling
		return ERROR_COMPLEX;
	}
	std::stack<var> s;
	auto iter = post_order.begin();
	do {
		if ((*iter).str.empty()) {
			// num
			s.push((*iter).num);
		}
		else {
			// str
			if ((*iter).str.length() > 1) {
				// function
				if (s.size() < 1) {
					error_msg(msg, "More operators or functions, or less numbers than expected");
					return ERROR_COMPLEX;
				}
				auto num = s.top().num;
				s.pop();
				std::complex<double> result = func((*iter).str, num, msg);
				s.push(result);
			}
			else {
				// operator
				if (s.size() < 2) {
					error_msg(msg, "More operators or functions, or less numbers than expected");
					return ERROR_COMPLEX;
				}
				auto num2 = s.top().num;
				s.pop();
				auto num1 = s.top().num;
				s.pop();
				std::complex<double> result = ERROR_COMPLEX;
				switch ((*iter).str[0]) {
				case '+':
					result = num1 + num2;
					break;
				case '-':
					result = num1 - num2;
					break;
				case '*':
					result = num1 * num2;
					break;
				case '/':
					result = num1 / num2;
					break;
				case '^':
					result = pow(num1, num2);
					break;
				default:	
					error_msg(msg, std::string("Undefined operator: ").append((*iter).str));
					return ERROR_COMPLEX;
				}
				s.push(result);
			}
		}
	} while (!s.empty() && ++iter != post_order.end());

	if (s.size() == 1) {
		return s.top().num;
	}
	else if (s.size() == 0) {
		error_msg(msg, "More operators or functions, or less numbers than expected");
		return ERROR_COMPLEX;
	}
	else {
		error_msg(msg, "Less operators or functions, or more numbers than expected");
		return ERROR_COMPLEX;
	}
}
//Function definition
bool bracket(std::string expr)
{ 
    std::stack<char> s;
    char x;
 
    for (int i = 0; i < expr.length(); i++)
    {
        if (expr[i] == '(' || expr[i] == '['
            || expr[i] == '{')
        {
            s.push(expr[i]);
            continue;
        }
		if (s.empty())
            return false;
 
        switch (expr[i]) {
        case ')':
             
            
            x = s.top();
            s.pop();
            if (x == '{' || x == '[')
                return false;
            break;
 
        case '}':
 
            
            x = s.top();
            s.pop();
            if (x == '(' || x == '[')
                return false;
            break;
 
        case ']':
 
            
            x = s.top();
            s.pop();
            if (x == '(' || x == '{')
                return false;
            break;
        }
    }
 
    
    return (s.empty());
}