# String Evaluate
Use C++ to evaluate a string. Support complex numbers, function names and variables.

## Call the evaluation function
```cpp
string msg;
// eval with variables 'x' and 'y'
cout << eval("log(-2) * i + 2 $x ^ ($y - 1)", { "x", "y" }, { { 1, 2 }, { 3, 4 } }, &msg) << endl;
cout << eval("$x + $y", { "x", "y" }, { { 1, 2 }, { 2, 3 } }) << endl;
cout << eval("-arcsin(2 - i)") << endl;
// Mismatch of brackets error example
cout << eval("3 * ( 2 + 3 ]") << endl;
```

- Variable Names are started with the symbol `$`.
- `+`, `-`, `*`, `/`, `^` and normal maths function names are supported.
- Imaginary unit can be `i`, `I`, `j` or `J`.
- Brackets can be `()`, `[]` or `{}`.

> **Tips**: If you have expressions continuously only with difference in variable values, i.e. `num_list`, you can first call `to_postorder` and store the postorder `std::vector`. Each time, calling `eval_postorder` only can be more efficient.

## Project Background
This project is the basis of the development of [Fractal Designer](https://github.com/Teddy-van-Jerry/Fractal_Designer) 6 and is already used in Fractal Designer 5.6. This project is the core of syntax analysis in Fractal Designer project.

## Licence
[MIT Licence](LICENSE)