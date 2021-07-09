/*
 * Project: String_Evaluate
 * File: Console_Calculator.cpp
 * Description: A console app for calculating complex number
 * ---------------------------
 *
 * @author: Teddy van Jerry, Pulkit Mittal
 * @licence: MIT Licence
 * @compiler: At least C++11
 *
 * @version 1.0: 2021/07/05
 * - initial version
 *
 * @version 1.1: 2021/07/08
 * - add bracket check
 * 
 * @version 1.2: 2021/07/09
 * - break the function eval_postorder
 * - break into header and source files
 * 
 */

#include <iostream>
#include "String_Evaluate.h"
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
	cout << eval("log(-2) * i + 2 $x ^ ($y - 1)", { "x", "y" }, { { 1, 2 }, { 3, 4 } }, &msg) << endl;
	cout << eval("$x + $y", { "x", "y" }, { { 1, 2 }, { 2, 3 } }) << endl;
	cout << eval("-arcsin(2 - i)") << endl;
	// Mismatch of brackets error example
	cout << eval("3 * ( 2 + 3 ]") << endl;
	return 0;
}