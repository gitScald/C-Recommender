//! SentenceTokenizer class
/*!
[DERIVED CLASS] 

This class is responsible for breaking an input stream of characters into individual sentences, by splitting it at the end of a typical sentence punctuation, while altogether watching out for name abbreviations and prefixes.
*/
#ifndef SENTENCE_TOKENIZER_H
#define SENTENCE_TOKENIZER_H

#include "abstract_tokenizer.h"

class SentenceTokenizer :public AbstractTokenizer {
public:
    //!Empty Constructor
    /*!
    Empty derived constructor referencing base class AbstractTokenizer 
    */
    SentenceTokenizer()
        : AbstractTokenizer() {};
    //!Constructor. 
    /*!
    This Constructor assigns member variable "bad" a String value, corresponding to characters that will be used to split all input streams into individual sentence tokens. Member variable "abbrevs_fp" is also specified, referring to a file path that contains a text file with predetermined abbreviates and prefixes that the routine should not split at.
    */
    explicit SentenceTokenizer(const std::string& bad, const std::string& abbrevs_fp = std::string())
        : AbstractTokenizer(bad), name_{ abbrevs_fp } { init(); }
    //!Accessor Method. 
    /*!
    Accessor Method to return name, corresponding to document name.
    */
    const std::string& name() const { return name_; };
    //!Tokenizer Method. 
    /*!
    Splits StringStream reference into vector of string sentences
    */
    const std::vector<std::string>& tokenize(std::stringstream& ss) override;

private:
    //!String Member Variable. 
    /*!
    Corresponds to name of Document
    */
    std::string name_;
    //!Vector of String Data Members. 
    /*!
    Corresponds to vector of Abbreviations using during tokenization
    */
    std::vector<std::string> abbrevs;

    void init();
};

#endif // !SENTENCE_TOKENIZER_H