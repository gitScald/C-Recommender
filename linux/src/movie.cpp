#include "movie.h"
#include "word_tokenizer.h"
#include <sstream>
/*!
\param os is a Output Stream Object
\param m is a Movie Object
\return Output Stream Object os containing in its buffer the name, id, release date and summary of Movie Object m
*/
std::ostream& operator<<(std::ostream& os, const Movie& m) {
    // provides output for movie data members
    os << "[" << m.id_ << "] \""
        << m.name_ << "\" (released "
        << m.release_date_ << ")"
        << std::endl << std::endl
        << m.content_;

    return os;
}

void Movie::init() {
    // create movie document object
    doc = Document(name_, content_);
}