# DPLL SAT Solver - Group F - Ngoc Lam DO

## Overview
This project is developed using C++14 with IDE Clion 2022.2.4 on Windows 10. Due to time limitations, building and running the project on different platforms with different methods have not been tested. 
Besides reading the input directory for the DIMACS file, the program doesn't have other platform-sensitive implementations—Further explanation down below.

## Install C++ and compiler/IDE
C++14 is the minimal version required to run the project. To install a compiler for C++.

On macOS: Open the terminal and run this command using Xcode Command Line Tools:

```
xcode-select --install

```

On Linux(Debian/Ubuntu): Open the terminal and run this command using GCC(GNU Compiler Collection)

```
sudo dnf install gcc-c++

```

Using IDE: It is recommended to use this method. Some popular IDEs: Visual Studio Code, Clion, Eclipse ...

Verify version
Run this code to see the version of C++ compiler:

```
g++ --version
```

## Run and build project
After pulling or directly downloading the project from git. Following these steps on the terminal run the solver.
Noting that 
1. Navigate to project directory:

```
cd /path/to/Project2

```

2. Compile the code: Compile 'main.cpp'.

```
g++ -std=c++14 -o Project2 main.cpp

```

3. Run the executable:
```
./Project2

```

4. Input directory to DIMACS file.
After running the project, this message will be outputted to the terminal.
```
Please enter the full directory to the file: 

```

Enter the full directory (not just from the working directory) to the DIMACS file that wanted to be solved. There aren't implementations for adapting the directory, the suitable pattern of the directory needs to be input 
Windows uses `\` while Linux or Mac uses `/`. Example for Windows.
```
D:\...\path\to\Project 2\foldername\file_name.cnf

```

5. Read output results.
The output will have the form of a chain of assignments from the oldest to the latest: `[Variable|value|b/f]-`
Value = `1` if `true`,
Value = `0` if `false`,
`b` for `branching`,
`f` for `forcing`,
Multiple chains will be printed after one another if the solver needs to backtrack.
The backtracked chains will be the same as the previous chain (the chain above) from the start to the point of the backtracking stop.
Satisfiable problems have the last chain as the correct result. Variables that don't appear in any knot could be assigned with both values.
Unsatisfiable problems have the last chain with all forcing assignments (`[.|.|f]`).


