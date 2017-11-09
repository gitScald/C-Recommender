//!WordTokenizer class
/*!
[DERIVED CLASS] 

This class is responsible for breaking an input stream of characters into individual word tokens, by splitting it at white space and punctuation characters
*/
#ifndef WORD_TOKENIZER_H
#define WORD_TOKENIZER_H

#include "abstract_tokenizer.h"

class WordTokenizer :public AbstractTokenizer {
public:
    //!Empty Constructor
    WordTokenizer()
        : AbstractTokenizer() {};
    //!Constructor
    /*!
    This Constructor assigns member variable "bad_chars" a String value, corresponding to characters that will be used to split all input streams into individual tokens. 
    */
    explicit WordTokenizer(const std::string& bad)
        : AbstractTokenizer(bad) {};
    //! Tokenizer Method
    /*!
    Tokenize input stream with specified delimiters. Return vector of tokens.
    */
    const std::vector<std::string>& tokenize(std::stringstream& ss) override;
};

#endif // !WORD_TOKENIZER_H