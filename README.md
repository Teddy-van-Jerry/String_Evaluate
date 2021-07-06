# String Evaluate
Use C++ to evaluate a string. Support complex numbers, function names and variables.

## Call the evaluation function
```cpp
string msg;
// eval with variables 'x' and 'y'
cout << eval("log(-2) * i + 2 $x ^ ($y - 1)", { "x", "y"} , { Comp(1, 2), Comp(3, 4) }, &msg) << endl;
cout << eval("-arcsin(2-i)") << endl;
cout << eval("abs(3 + 4i)") << endl;
```

- Variable Names are started with the symbol `$`.
- `+`, `-`, `*`, `/`, `^` and normal maths function names are supported.
- Imaginary unit can be `i`, `I`, `j` or `J`.
- Brackets can be `()`, `[]` or `{}`.