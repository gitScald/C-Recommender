#include "stopword.h"
#include "stringops.h"
#include <algorithm>
#include <iostream>

/*!
\param stopw_fp is the String Stopword Filepath
*/
Stopword::Stopword(const std::string& stopw_fp) {
    name_ = stopw_fp;

    std::ifstream ifs{ stopw_fp };
    if (!ifs)
        std::cerr << "WARNING: Could not establish input stream with '"
            << stopw_fp << "'" << std::endl;

    else {
        std::string stopword;
        while (ifs >> stopword)
            stopwords.push_back(stopword);
    }
}
/*!
\param s is a String token
\return Boolean value; If stopword vector contains string token
*/
bool Stopword::operator() (const std::string& s) const {
    // check if stopword object contains the token
    if (stopwords.empty())
        return false;

    for (std::vector<std::string>::const_iterator it{ stopwords.begin() };
        it != stopwords.end();
        ++it) {
        std::string token{ *it };
        lower(strip(token));
        if (token == s)
            return true;
    }

    return false;
}
/*!
\param os is a Output Stream object
\param s is a Stopword Object
\return Returns the Ouput Stream object once data has been pushed to stream.
*/
std::ostream& operator<<(std::ostream& os, const Stopword& s) {
    os << "Stopwords extracted from '" << s.name() << "':" << std::endl;
    for (std::vector<std::string>::const_iterator it{ s.stopwords.begin() };
        it != s.stopwords.end();
        ++it)
        os << *it << std::endl;

    return os;
}