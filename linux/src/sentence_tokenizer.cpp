#include "sentence_tokenizer.h"
#include "stringops.h"
#include <iostream>

void SentenceTokenizer::init() {
    if (!name_.empty()) {
        std::ifstream ifs{ name_ };

        if (!ifs)
            std::cerr << "WARNING: Could not establish input stream with '"
            << name_ << "'" << std::endl;

        else {
            std::string abbrev;
            while (ifs >> abbrev)
                abbrevs.push_back(abbrev);
        }
    }
}

/*!
\param ss is a String Stream Object
\return String (Token) Vector
*/
const std::vector<std::string>& SentenceTokenizer::tokenize(std::stringstream& ss) {
    // returns a vector of tokens from the given stringstream
    std::stringstream token;
    std::string word;
    std::string word_lc;
    size_t punct;
    std::vector<std::string>::const_iterator it;

    while (ss >> word) {
        punct = word.find_first_of(bad_chars);
        // if punctuation is detected
        if (punct != std::string::npos) {
            // if the punctuation is a period
            if (word[punct] == '.') {
                word_lc = word.substr(0, punct);
                lower(word_lc);

                it = find(abbrevs.begin(), abbrevs.end(), word_lc);
                // if an abbreviation is found, append the word and keep going
                if (it != abbrevs.end()) {
                    token << word << " ";
                }

                // if no abbreviation is found behind the period
                else {
                    // the current sentence is complete, push it back
                    token << word;
                    tokens.push_back(token.str());

                    // clear the sentence stringstream
                    token.str(std::string());
                }
            }

            // other punctuation marking the end of a sentence
            else if (word[punct] == '?' || word[punct] == '!') {
                // the current sentence is complete, push it back
                token << word;
                tokens.push_back(token.str());

                // clear the sentence stringstream
                token.str(std::string());
            }
        }

        // in all other cases, append the word to the current sentence
        else {
            // add a space since operator>> trims them out
            token << word << " ";
        }
    }

    return tokens;

}