#include "stringops.h"
#include "word_tokenizer.h"

/*!
\param ss is a String Stream Object
\return String (Token) Vector
*/
const std::vector<std::string>& WordTokenizer::tokenize(std::stringstream& ss) {
    // returns a vector of tokens from the given stringstream
    std::string token;
    while (ss >> token) {
        lower(strip(token));
        tokens.push_back(token);
    }

    return tokens;
}