/*
 * Project: String_Complex
 * File: Console_Calculator.cpp
 * Description: A console app for calculating complex number
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

/****************************************
 * NOTICE:
 * The app has not been fully developed,
 * currently it serves as the test file
 * of String_Complex.h.
 ****************************************/

#include <iostream>
#include "String_Complex.h"
using namespace std;

int main(int argc, char* argv[]) {

	// With arguments
	if (argc > 2) {
		cerr << "Invalid number of arguments." << endl;
		return -1;
	}
	else if (argc == 2) {
		string msg;
		auto ret = eval(argv[1], {}, {}, &msg);
		if (msg.empty()) {
			// correct string
			cout << ret << endl;
		}
		else {
			// invalid string with errors
			// print error info
			cerr << msg << endl;
		}
		return 0;
	}

	string msg;
	// eval with variables 'x' and 'y'
	cout << eval("log(-2) * i + 2 $x ^ ($y - 1)", { "x", "y" }, { Comp(1, 2), Comp(3, 4) }, &msg) << endl;
	cout << eval("-arcsin(2-i)") << endl;
	cout << eval("abs(3 + 4i)") << endl;
	return 0;
}