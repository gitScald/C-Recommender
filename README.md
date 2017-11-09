# C++ Essay Generator

## Important: MS Windows

If you want to run the project in Windows (be it in Visual Studio or Eclipse), you will have to do one of the following:

* Comment out function `main()` in `gindexer.cpp` and in `googler.cpp` (to run `summarizer`), or
* Comment out function `main()` in `googler.cpp` and in `summarizer.cpp` (to run `indexing`), or
* Comment out function `main()` in `gindexer.cpp` and in `summarizer.cpp` (to run `googler`)

Because Windows IDEs don't allow the creation of multiple executables within the same project, and because it's a pain to link another project with one, let alone two different IDEs, I've opted to allow to swap between three `main()` programs by commenting the other one out.

## Linux

When testing the program under Linux, use the file structure located in the `linux/` folder, navigate to the root folder, and call `make`. Then, to run either executable, call the following:

* `googler/exe/googler`, or
* `indexing/exe/indexing`, or
* `summarizer/exe/summarizer`

If you need to rebuild the project, call `make clean` in the root directory, followed by another `make`.

## Description

This repository holds the source files and resources for the third assignment in COMP 345.

## Branches

To contribute to the project, please use the `dev` branch. Do NOT commit to the `master` branch without checking that files will properly merge between the two branches.