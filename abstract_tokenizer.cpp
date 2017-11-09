#include "abstract_tokenizer.h"
#include "stringops.h"

/*!
\param os is a Output Stream Object
\param t is a Tokenizer Object
\return Output Stream Object
*/
std::ostream& operator<<(std::ostream& os, const AbstractTokenizer& t) {
    os << "Tokenizer object with the following delimiters: "
        << ((t.bad_chars != std::string()) ? "(all punctuation and whitespaces)"
            : t.bad_chars)
        << std::endl;

    return os;
}