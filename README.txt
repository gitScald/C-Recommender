 * Authors:     Benjamin VIAL  (29590765)
                Mandeep Tahim  (26935478)
                Daniel Carrera (26729886)
 * Course:      COMP 345
 * Instructor:  René WITTE
 * Due date:    November 21st, 2017

 C++ Movie Recommender
 =====================

This is a C++11 implementation of a movie recommendation system.

Compilation:
------------

Navigate to the root directory and call:
    make

If needed, make a clean install by calling:
    make clean
    make

Usage:
------

Upon successful compilation, navigate to the following subdirectories to test the program of interest:
    googler/exe
    indexing/exe
    summarizer/exe
    recommender/exe

NOTE: Both 'googler' and 'indexing' programs will search for the 'files/' directory, in which should be located 'index.txt' (which holds the paths of all documents to reference), 'extra_doc.txt' (an extra document to index after an initial normalization), and 'stopwords.txt' (optional). The 'summarizer' program uses data provided in the 'a3data/' directory, and the `recommender` program uses a `MovieSummaries` directory (provided by the professor).

NOTE: Failure to provide any of the indexing text files will cause a WARNING, but not a RUN-TIME ERROR. This means the index will display an empty table, and query operations will likely fail.

The application was developed with the following:

Operating system:   Microsoft Windows 7 64-bit SP1 6.1.7.7601
IDE:                Microsoft Visual Studio 2017 Community Edition 15.3.1
Compilers:          Microsoft Visual C++ 14.10.25008
                    G++ 6.3.0 (MinGW port for Windows)
                    G++ 4.9.2 (ENCS Linux environment)


Archive contents:

    - a3data/                   Documents to process (provided by the professor)
        q1docs/*.txt            Documents to index to process questions
        q2docs/*.txt
        q3docs/*.txt
        *.txt                   Questions to query the index with
    - MovieSummaries/           Documents to process (provided by the professor)
        movie.metadata.tsv      Movie metadata to process
        plot_summaries.txt      Movie summaries to process
                                (other documents not used)
    - doc/                      Project documentation (created with Doxygen)
        html/*                  HTML documentation
        latex/*                 LaTeX documentation
    - files/
        doc1.txt                Sample documents to index
        doc2.txt
        doc3.txt
        extra_doc.txt           Extra document (not included in index.txt), to test normalize() method
        index.txt               Index file for Linux directory structure
        index_windows.txt       Index file for Windows directory structure (if using a files/ folder with VS)
        stopwords.txt           Sample stopwords file
    - googler/
        exe/                    Target for executable
        obj/                    Target for object files
        makefile                Makefile for googler executable
        sample.txt              Sample interaction with googler
    - indexing/
        exe/                    Target for executable
        obj/                    Target for object files
        makefile                Makefile for indexing executable
        sample.txt              Sample interaction with indexing
    - summarizer/
        exe/                    Target for executable
        obj/                    Target for object files
        makefile                Makefile for summarizer executable
        sample.txt              Sample interaction with summarizer
    - recommender/
        exe/                    Target for executable
        obj/                    Target for object files
        makefile                Makefile for recommender executable
        sample.txt              Sample interaction with recommender
    - src/
        *.cpp                   Source files
        *.h                     Header files
    - makefile                  Root makefile for complete project
    - README.txt:               This document