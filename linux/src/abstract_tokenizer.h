//!AbstractTokenizer Class
/*!
[BASE CLASS] 
This class is responsible for breaking an input stream of characters into individual IndexItem objects.
*/

#ifndef ABSTRACT_TOKENIZER_H
#define ABSTRACT_TOKENIZER_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class AbstractTokenizer {
    //! Overload Operator<<
    /*!
    Output Delimiters using Output Stream
    */
    friend std::ostream& operator<<(std::ostream& os, const AbstractTokenizer& t);

public:
    //! Empty Constructor
    AbstractTokenizer() {};
    //! Constructor
    /*!
    This Constructor assigns member variable "bad_chars" a String value, corresponding to characters
    that will be used to split all input streams into individual tokens.
    */
    explicit AbstractTokenizer(const std::string& bad)
        : bad_chars{ bad } {};

    //! Tokenize Input Stream
    /*!
    Tokenize input stream with specified delimiters. 
    Return vector of tokens.
    */
    virtual const std::vector<std::string>& tokenize(std::stringstream& ss) = 0;

protected:
    std::string bad_chars; //!< String contains delimiters
    std::vector<std::string> tokens; //!< String Vector containing split tokens
};

#endif // !ABSTRACT_TOKENIZER_H
