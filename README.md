**Please keep in mind that this project is from Spring 2018.**

## Phone forwarding

This repository contains the solution to three connected programming assignments for the Spring 2018 Individual Programming Project course at the University of Warsaw. 

The specification and comments is written in Polish. Here you can find a brief description of the project. You can find full project requirements in the `docs/` folder.

#### Goal

The goal is to build a tool for processing phone numbers and an associated interpreter/. This project is largely theoretical and includes several complicated data structures.

#### Compiling and running the solution

To compile the solution in Release mode, one must call the following instructions:
```bash
mkdir release
cd release
cmake ..
make
make doc
```

For the Debug mode, commands are as follows:
```bash
mkdir debug
cd debug
cmake -D CMAKE_BUILD_TYPE=Debug ..
make
make doc
```

#### Part 1: Implementing the `phone_forward.h` functionality.

The goal was to implement the `src/phone_forward.h` module and add the Doxygen documentation. 

You can find the solution to part 1 in the commit `0235dd8d0ce7d068573b336d2babf2fcd6f8ba0f`


#### Part 2: Building an interpreter.

Build an interpreter of a following language. Three lexems:
- `number`: nonempty sequence of digits 0-9.
- `id`: nonempty sequence of digits and ASCII letters that begins with a letter. `NEW` and `DEL` words are restricted
- `operator`: `NEW`, `DEL`, `>`, `?`

Operators `NEW` and `DEL` serve to create, switch or remove the base of redirections. At least 100 bases are supported.
- `NEW <id>`: switch to the base with a name `<id>`, if it doesn't exist, set it as a current base
- `DEL <id>`: remove a base with a name `<id>`
 
There are multiple operations:
- `<number1> > <number2>`: adds a redirection from `<number1>` to `<number2>`
- `number ?`: outputs a redirection from a given number
- `? number`: outputs redirections to a given number
- `DEL number`: removes all redirections whose prefix is `number`

Symbols might be separated with whitespace characters or not (if it doesn't lead to ambiguity). Language might contain comments, which start with a sequence `$$`.

In case of syntax errors, a message `ERROR n` is sent to output, where `n` is a number of the first non-interpretable byte. If a program ends unexpectadly, `ERROR EOF` goes to output.

In case of execution errors, `ERROR operator n` is sent output, where `operator` is an operator name and `n` is a position of that operator. If no base is set, any operation on numbers is considered wrong.

My solution to that part is located in the commit `d4bae3c222d6731d8b5cd02b713431ad17b716ea`

#### Part 3: Refinement

Added a function to the interface and an operator `@` to the text interface.
