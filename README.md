# C++ Movie Recommender

## Important: MS Windows

If you want to run the project in Windows (be it in Visual Studio or Eclipse), you will have to do one of the following:

* Comment out function `main()` in `gindexer.cpp`, in `googler.cpp`, and in `recommender.cpp` (to run `summarizer`), or
* Comment out function `main()` in `googler.cpp`, in `summarizer.cpp`, and in `recommender.cpp` (to run `indexing`), or
* Comment out function `main()` in `gindexer.cpp`, in `summarizer.cpp`, and in `recommender.cpp` (to run `googler`)
* Comment out function `main()` in `gindexer.cpp`, in `googler.cpp`, and in `summarizer.cpp` (to run `recommender`)

Because Windows IDEs don't allow the creation of multiple executables within the same project, and because it's a pain to link another project with one, let alone two different IDEs, I've opted to allow to swap between four `main()` programs by commenting the other one out.

## Linux

When testing the program under Linux, use the file structure located in the `linux/` folder, navigate to the root folder, and call `make`. Then, to run either executable, call the following:

* `googler/exe/googler`, or
* `indexing/exe/indexing`, or
* `summarizer/exe/summarizer`, or
* `recommender/exe/recommender`

If you need to rebuild the project, call `make clean` in the root directory, followed by another `make`.

## Description

This repository holds the source files and resources for the fourth assignment in COMP 345.

## Branches

To contribute to the project, please use the `dev` branch. Do NOT commit to the `master` branch without checking that files will properly merge between the two branches.