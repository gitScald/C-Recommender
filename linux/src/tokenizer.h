//! Tokenizer Class
/*!
This class is responsible for breaking an input stream of characters into individual tokens,
by splitting it at white space and punctuation characters
*/

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Tokenizer {
    //! Overload Operator<<
    /*!
    Output Delimiters using Output Stream
    */
    friend std::ostream& operator<<(std::ostream& os, const Tokenizer& t);

public:
    //! Empty Constructor
    Tokenizer() {};
    //! Constructor
    /*!
    This Constructor assigns member variable "bad_chars" a String value, corresponding to characters
    that will be used to split all input streams into individual tokens.
    */
    explicit Tokenizer(const std::string& bad)
        : bad_chars{ bad } {};

    //! Tokenize Input Stream
    /*!
    Tokenize input stream with specified delimiters. 
    Return vector of tokens.
    */
    const std::vector<std::string>& tokenize(std::stringstream& ss);

private:
    std::string bad_chars; //!< String contains delimiters
    std::vector<std::string> tokens; //!< String Vector containing split tokens
};

#endif // !TOKENIZER_H
