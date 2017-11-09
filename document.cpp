#include "document.h"
#include "stringops.h"
#include "word_tokenizer.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

void Document::init() {
    // builds the dictionary from the given file
    // get the short file name for table display
    const size_t slash{ name_.find_last_of('/') + 1 };
    name_short = name_.substr(slash);

    std::ifstream ifs{ name_.c_str() };
    if (!ifs)
        std::cerr << "WARNING: Could not establish input stream with '"
        << name_ << "'" << std::endl;

    else {
        // save the contents of the file
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        content_ = buffer.str();

        // reset stringstream and tokenize contents
        WordTokenizer t;
        buffer.clear();
        buffer.seekg(0);
        std::vector<std::string> tokens = t.tokenize(buffer);

        // dump the contents of tokens into dict and weights
        for (std::vector<std::string>::const_iterator it{ tokens.begin() };
            it != tokens.end();
            ++it)
            *this << *it;
    }
}

/*!
\param s is a String token
\return Boolean value representing whether or not the map contains the word specified.
*/
bool Document::contains(const std::string& s) const {
    // returns true if the document contains the token
    std::map<std::string, int>::const_iterator it{ dict.find(s) };
    if (it != dict.end())
        return true;

    return false;
}

/*!
\param d is a Document object
\param s is a String token
\return Document object once addition has been completed, regardless of success.
*/
Document& operator<<(Document& d, const std::string& s) {
    // adds a token to the dictionary if not an empty string
    // increments the count of the token if already present
    if (s.empty())
        return d;

    if (!d(s)) {
        // update dictionary
        d.dict.insert(std::pair<std::string, int>{s, 1});
        if (s.length() > d.longest)
            d.longest = s.length();
    }
    else
        ++d.dict.at(s);

    return d;
}

/*!
\param os is a Output Stream Object
\param d is a Document Object whose content will be streamed.
\return Ouput Stream object once data has been pushed to stream. 
*/
std::ostream& operator<<(std::ostream& os, const Document& d) {
    os << "'" << d.name() << "' contents:" << std::endl << std::endl
        << d.content() << std::endl << std::endl
        << "'" << d.name() << "' dictionary:" << std::endl;

    for (std::map<std::string, int>::const_iterator it{ d.dict.begin() };
        it != d.dict.end();
        ++it)
        os << std::left << std::setw(d.longest) << it->first << " : "
        << std::right << it->second << std::endl;

    os << std::endl;

    return os;
}