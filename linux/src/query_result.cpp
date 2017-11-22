#include "document.h"
#include "query_result.h"
#include "movie.h"
#include "sentence.h"
#include <iomanip>
#include <typeinfo>

/*!
\param qr is a QueryResult object.
\return QueryResult object (*this)
*/
QueryResult& QueryResult::operator=(const QueryResult& qr) {
    if (this != &qr) {
        item_ = qr.item_;
        score_ = qr.score_;
    }

    return *this;
}

/*!
\param qr is a QueryResult object.
\return QueryResult object (*this)
*/
QueryResult& QueryResult::operator=(QueryResult&& qr) {
    if (this != &qr) {
        item_ = std::move(qr.item_);
        score_ = std::move(qr.score_);
    }

    return *this;
}

QueryResult::operator bool() const {
    if (item_)
        return true;

    return false;
};

const std::string& QueryResult::content() const {
    return item_->content();
};

/*!
\param os is a Output Stream Object
\param qr is a QueryResult Object
\return QueryResult object (*this)
*/
std::ostream& operator<<(std::ostream& os, const QueryResult& qr) {
    // save os flags before using std::fixed
    const std::ios_base::fmtflags unfixed{ os.flags() };

    // document output
    const Document* pd = dynamic_cast<const Document*>(qr.item_);
    if (pd) {
        os << "'" << qr.item_->name() << "' - cosine similarity of "
            << std::fixed << std::setprecision(QueryResult::w_prec) << qr.score_;
    }

    else {
        // sentence output
        const Sentence* ps = dynamic_cast<const Sentence*>(qr.item_);
        if (ps) {
            os << "("
                << std::fixed << std::setprecision(QueryResult::w_prec) << qr.score_
                << ") " << *ps;
        }

        else {
            // movie output
            const Movie* pm = dynamic_cast<const Movie*>(qr.item_);
            if (pm)
                os << *pm;
        }
    }

    os.flags(unfixed);

    return os;
}

/*!
\param qr1 is a QueryResult Object
\param qr2 is a QueryResult Object
\return Boolean value
*/
bool operator<(const QueryResult& qr1, const QueryResult& qr2) {
    // comparison operator for std::sort()

    // check if the items are sentences (sort is then by document, then by position)
    if (typeid(qr1.item_) == typeid(Sentence*) && typeid(qr2.item_) == typeid(Sentence*)) {
        // cast is safe now
        const Sentence* ps1 = dynamic_cast<const Sentence*>(qr1.item_);
        const Sentence* ps2 = dynamic_cast<const Sentence*>(qr2.item_);

        // the sentence that comes from the document that was indexed first wins
        if (ps1->name() < ps2->name())
            return true;

        // if both sentences come from the same document, they are compared by position
        return ps1->pos() < ps2->pos();
    }

    // if the items are documents, simply compare them by score
    return qr1.score_ > qr2.score_;
}