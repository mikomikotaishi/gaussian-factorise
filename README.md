# Gaussian Integer Factoriser (C++)

## Usage:
```gaussian-factorise```<br>
```gaussian-factorise <-h/-help/h/help>```<br>
```gaussian-factorise <-r/-random/r/random>```<br>
```gaussian-factorise [integer]```<br>

If no argument is given, the program will prompt the user for an Gaussian integer.
If the provided Gaussian integer is "`random`" or "`r`", the program will generate a random integer.
Otherwise, the argument will be interpreted as an integer.

The program will then factorise the integer into Gaussian primes.

Also accepts `j` as the imaginary unit. `a + bi` and `bi + a` are both accepted forms.

## Examples:
```./gaussian-factorise 2```<br>
    Output: `(-i)(1 + i)(1 + i)`<br>
```./gaussian-factorise 7```<br>
    Output: `(7)`<br>
```./gaussian-factorise 425 - 470i```<br>
    Output: `(-1)(1 + 2i)(2 + i)(94 + 85i)`<br>
