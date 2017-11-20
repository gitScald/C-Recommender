#include "movie.h"
#include "word_tokenizer.h"
#include <sstream>

std::ostream& operator<<(std::ostream& os, const Movie& m) {
    // provides output for movie data members
    os << "[" << m.id_ << "] \""
        << m.name_ << "\" (released "
        << m.release_date_ << ")"
        << std::endl << std::endl
        << m.content_;

    return os;
}

void Movie::build_index() {
    // create movie document object
    doc = Document(name_, content_);
}