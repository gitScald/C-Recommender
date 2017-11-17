#include "movie.h"
#include "word_tokenizer.h"
#include <sstream>

std::ostream& operator<<(std::ostream& os, const Movie& m) {
    // provides output for movie data members
    os << "[" << m.id_ << "] \""
        << m.name_ << "\" ("
        << m.release_date_ << ")"
        << std::endl << std::endl
        << m.content_;

    return os;
}

void Movie::build_index() {
    // builds the index of the movie summary
    if (!doc.content().empty()) {
        std::stringstream buffer;
        buffer << content_;
        doc.content_ = buffer.str();

        // reset stringstream and tokenize summary
        WordTokenizer t;
        buffer.clear();
        buffer.seekg(0);
        std::vector<std::string> word_tokens = t.tokenize(buffer);

        // dump the contents of tokens into dict and weights
        for (std::vector<std::string>::const_iterator it{ word_tokens.begin() };
            it != word_tokens.end();
            ++it)
            doc << *it;
    }
}