#include "stringops.h"
#include "tokenizer.h"

/*!
\param ss is a String Stream Object
\return String (Token) Vector
*/
const std::vector<std::string>& Tokenizer::tokenize(std::stringstream& ss) {
    // returns a vector of tokens from the given stringstream
    std::string token;
    while (ss >> token) {
        lower(strip(token));
        tokens.push_back(token);
    }

    return tokens;
}

/*!
\param os is a Output Stream Object
\param t is a Tokenizer Object
\return Output Stream Object
*/
std::ostream& operator<<(std::ostream& os, const Tokenizer& t) {
    os << "Tokenizer object with the following delimiters: "
        << ((t.bad_chars != std::string()) ? "(all punctuation and whitespaces"
            : t.bad_chars)
        << std::endl;

    return os;
}