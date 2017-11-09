#include "index.h"
#include <fstream>
#include <iostream>


/*!
\param index_fp is a string corresponding to the filepath of Index of files
*/
Index::Index(const std::string& index_fp) {
    name_ = index_fp;
    std::ifstream ifs{ index_fp.c_str() };

    if (!ifs)
        std::cerr << "WARNING: Could not establish input stream with '"
        << index_fp << "'" << std::endl;

    else {
        std::string fp;
        while (ifs >> fp)
            docs_fp.push_back(fp);
    }
}