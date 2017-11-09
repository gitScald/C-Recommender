#include "document_indexer.h"
#include "stringops.h"
#include "word_tokenizer.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

/*!
\param i is a Integer Position within Document Vector
\return Pointer to IndexItem
*/
const IndexItem* DocumentIndexer::operator[] (size_t i) const {
    const IndexItem* pii = &docs.at(i);

    return pii;
}

/*!
\param s is a String Token
\return Boolean value; True if token exist in index.
*/		
bool DocumentIndexer::contains(const std::string& s) const {
    // returns true if the index contains the token
    std::map<std::string, DocumentIndexer::vec_pair>::const_iterator it{ index.find(s) };
    if (it != index.end())
        return true;

    return false;
}

void DocumentIndexer::normalize() {
    // computes token weights
    for (std::map<std::string, DocumentIndexer::vec_pair>::const_iterator m_it{ index.begin() };
        m_it != index.end();
        ++m_it) {
        for (size_t doc{ 0 }; doc != size(); ++doc) {
            // compute and write weights in the map entry
            index.at(m_it->first).second.at(doc) = weight(m_it->first, doc);
        }
    }

    normalized = true;
}
/*!
\param s is a User Input Query String
\param i is a Integer defining Return size
\return Vector<QueryResult> containing Top i Cosine Similarity 
*/
const std::vector<QueryResult> DocumentIndexer::query
    (const std::string& s, size_t i) const {
    // throw an exception if the index is not normalized
    if (!normalized)
        throw IndexerExcept();

    // tokenize the query string
    std::stringstream ss{ s };
    WordTokenizer t;
    const std::vector<std::string> tokens{ t.tokenize(ss) };

    // get longest token length
    size_t query_longest{ 0 };
    for (std::vector<std::string>::const_iterator it{ tokens.begin() };
        it != tokens.end();
        ++it) {
        if (it->length() > query_longest)
            query_longest = it->length();
    }

    // get query term frequencies
    // query_ is of the form (token, <freq, weight>)
    std::map<std::string, Indexer::query_pair> query_;
    query_freqs(query_, tokens);

    // get query and document weights
    std::map<std::string, std::vector<double>> doc_weights;
    query_weights(query_, doc_weights);

    // print query weights; similar to operator<< for indexer, but hackier
    // NOTE: for later implementations, use a Query class to handle this output
    const std::string lbl_wm{ "QUERY WEIGHTS" };
    const std::string v_separator{ Indexer::v_separator(3) };
    const size_t weight_prec{ 3 };
    const size_t w_token{ std::max(lbl_wm.length(), query_longest) + 4 };
    const size_t w_total{ w_token + 3 * v_separator.length() };
    const std::string h_separator{ Indexer::h_separator(w_total) };

    // print matrix headers
    std::cout << " " << std::left << std::setw(w_token) << lbl_wm << std::endl
        << " " << h_separator << std::endl;

    // print matrix body
    for (std::map<std::string, Indexer::query_pair>::const_iterator it{ query_.begin() };
        it != query_.end();
        ++it) {

        // save os flags before using std::fixed
        const std::ios_base::fmtflags unfixed{ std::cout.flags() };
        std::cout << " " << std::left << std::setw(w_token) << it->first << v_separator
            << std::fixed << std::setw(weight_prec + 1) << std::right
            << std::setprecision(weight_prec) << it->second.second << v_separator;

        // reset os flags after using std::fixed
        std::cout.flags(unfixed);
        std::cout << std::endl;
    }
    std::cout << " " << h_separator << std::endl;

    // return only the top i results
    std::vector<QueryResult> results{ cos_similarity(query_, doc_weights, tokens) };
    results.resize(std::min(results.size(), i));

    return results;
}

/*!
\param s is a String Token
\return Integer occurence of Token in Documents
*/
int DocumentIndexer::item_freq(const std::string& s) const {
    // returns the number of documents a token is found in
    int freq{ 0 };
    std::map<std::string, DocumentIndexer::vec_pair>::const_iterator m_it{ index.find(s) };
    if (m_it != index.end()) {
        for (std::vector<int>::const_iterator v_it{ m_it->second.first.begin() };
            v_it != m_it->second.first.end();
            ++v_it)
            if (*v_it > 0)
                freq += 1;
    }

    return freq;
}

/*!
\param s is a String Token
\param i is a Integer index of Document in Map
\return Integer occurence of Token in a Document at index i
*/
int DocumentIndexer::term_freq(const std::string& s, const int i) const {
    // return the count of a token in the document at the given index
    int freq{ 0 };
    if (docs.at(i).contains(s))
        freq = docs.at(i).dict.at(s);

    return freq;
}
/*!
\param s is a String Token
\param i is a Imteger index of Document in Map
\return Double value corresponding to the normalized term frequency of a token in a Document at index i
*/
double DocumentIndexer::norm_tf(const std::string& s, int i) const {
    // computes the normalized tf of a token in a document
    // normalized term frequency is zero if term frequency is zero
    if (contains(s))
        return std::max((1 + log(static_cast<double>(term_freq(s, i)))), 0.0);

    return 0;
}
/*!
\param s is a String Token
\return Double value corresponding to the normalized document frequency of a token
*/
double DocumentIndexer::norm_idf(const std::string& s) const {
    // computes the normalized idf of a token
    if (contains(s))
        return log(static_cast<double>(docs.size()) / static_cast<double>(item_freq(s)));

    return 0;
}
/*!
\param s is a String Token
\param i is a Integer index of Document in Map
\return Double value corresponding to the weight of a token in a Document at index i
*/
double DocumentIndexer::weight(const std::string& s, int i) const {
    // returns the weight of a token in the document at the given index
    if (contains(s))
        return norm_tf(s, i) * norm_idf(s);

    return 0;
}
/*!
\param q is a Map<token(String), <freq(int), weight(double)>>
\param t is a Vector<String> corresponding to User Input Query
*/
void DocumentIndexer::query_freqs(std::map<std::string, Indexer::query_pair>& q,
    const std::vector<std::string>& t) const {
    // gets query term frequencies
    std::map<std::string, Indexer::query_pair>::const_iterator m_it{ q.end() };
    for (std::vector<std::string>::const_iterator it{ t.begin() };
        it != t.end();
        ++it) {
        // increment the frequency if the token is already present
        m_it = q.find(*it);
        if (m_it != q.end())
            ++q.at(*it).first;

        // otherwise create an entry for it
        else
            q.insert(std::pair<std::string, Indexer::query_pair>
        { *it, Indexer::query_pair(std::make_pair(1, 0.0)) });
    }
}
/*!
\param q is a Map<token(String), <freq(int), weight(double)>>
\param dw is a Map<token(String), vector<document weight(double)>>
*/
void DocumentIndexer::query_weights(std::map<std::string, Indexer::query_pair>& q,
    std::map<std::string, std::vector<double>>& dw) const {
    // gets query and document weights
    double tf{ 0 };
    double idf{ 0 };
    for (std::map<std::string, Indexer::query_pair>::iterator q_it{ q.begin() };
        q_it != q.end();
        ++q_it) {
        // get query weights
        tf = std::max(1 + log(static_cast<double>(q_it->second.first)), 0.0);
        idf = norm_idf(q_it->first);
        q_it->second.second = tf * idf;

        // get document weights
        dw.insert(std::pair<std::string, std::vector<double>>
        {q_it->first, std::vector<double>(docs.size())});
        for (size_t doc{ 0 }; doc != docs.size(); ++doc)
            dw.at(q_it->first).at(doc) = weight(q_it->first, doc);
    }
}
/*!
\param q is a Map<token(String), <freq(int), weight(double)>>
\param dw is a Map<token(String), vector<document weight(double)>>
\param t is a Vector<String> corresponding to User Input Query
\return Vector<QueryResult> corresponds to Cosine Similarity between User Query and each Document (Decreasing Order)
*/
const std::vector<QueryResult> DocumentIndexer::cos_similarity(
    const std::map<std::string, Indexer::query_pair>& q,
    const std::map<std::string, std::vector<double>>& dw,
    const std::vector<std::string>& t) const {
    // computes cosine similarity between query and each document
    std::vector<QueryResult> results;
    std::string token;
    double num{ 0 };
    double len1{ 0 };
    double len2{ 0 };
    double den{ 0 };
    for (size_t doc{ 0 }; doc != docs.size(); ++doc) {
        num = 0;
        len1 = 0;
        len2 = 0;
        den = 0;
        for (std::vector<std::string>::const_iterator it{ t.begin() };
            it != t.end();
            ++it) {
            num += q.at(*it).second * dw.at(*it).at(doc);
            len1 += std::pow(q.at(*it).second, 2);
            len2 += std::pow(dw.at(*it).at(doc), 2);
        }
        den = std::sqrt(len1) * std::sqrt(len2);

        // construct and add query result (score is zero if den is zero)
        // emplace_back() makes use of move operations
        if (den == 0)
            results.emplace_back(QueryResult(&docs.at(doc), 0));
        else
            results.emplace_back(QueryResult(&docs.at(doc), num / den));
    }

    // sort results before returning (highest score first)
    std::sort(results.begin(), results.end());

    return results;
}

/*!
\param d is a Document object
\param i is a Indexer object
\return Indexer object i (parameter)
*/
DocumentIndexer& operator>>(const Document& d, DocumentIndexer& i) {
    // mirror of Indexer::operator<<(Indexer, Document)
    return i << d;
}
/*!
\param d is a Document object
\param i is a Indexer object
\return Indexer object i (parameter)
*/
DocumentIndexer& operator<<(DocumentIndexer& i, const Document& d) {
    i.normalized = false;
    i.docs.push_back(d);
    i.longest = std::max(i.longest, d.longest);

    // first expand the map entries to accomodate another document
    if (i.size() > 0) {
        for (std::map<std::string, DocumentIndexer::vec_pair>::iterator it{ i.index.begin() };
            it != i.index.end();
            ++it) {
            i.index.at(it->first).first.resize(i.size());
            i.index.at(it->first).second.resize(i.size());
        }
    }

    // add the document dictionary to the index
    for (std::map<std::string, int>::const_iterator it{ d.dict.begin() };
        it != d.dict.end();
        ++it) {
        // if the key is not already in the index, insert vectors of zeroes
        if (!i(it->first)) {
            i.index.insert(std::pair<std::string, DocumentIndexer::vec_pair>
                { it->first, DocumentIndexer::vec_pair(std::make_pair(std::vector<int>(i.size()),
                std::vector<double>(i.size()))) });
        }

        // write the document token count to the end of the map entry
        i.index.at(it->first).first.at(i.size() - 1) = it->second;
    }

    return i;
}
/*!
\param i is a Indexer object
\param ix is a Index object
\return Indexer object i (parameter)
*/
DocumentIndexer& operator<<(DocumentIndexer& i, const Index& ix) {
    // creates and adds documents from an index
    std::vector<std::string> docs_fp{ ix.docs() };
    for (std::vector<std::string>::const_iterator it{ docs_fp.begin() };
        it != docs_fp.end();
        ++it)
        i << Document{ *it };

    return i;
}
/*!
\param i is a Indexer object
\param s is a Stopword object
\return Indexer object i (parameter)
*/
DocumentIndexer& operator<<(DocumentIndexer& i, const Stopword& s) {
    // sets the indexer's stopword object
    i.stopword = s;

    return i;
}
/*!
\param os is a Output Stream object
\param i is a Indexer object
\return Output Stream object os (parameter)
*/
std::ostream& operator<<(std::ostream& os, const DocumentIndexer& i) {
    // label for document-term matrix
    const std::string lbl_dtm{ "DOCUMENT-TERM MATRIX" };

    // label for document frequency
    const std::string lbl_df{ "doc_freq" };

    // a vertical separator (spaces)
    const std::string v_separator{ Indexer::v_separator(3) };

    // width of the longest token in the index
    const size_t w_token{ std::max(lbl_dtm.length(), i.longest) + 4 };

    // widths of the document columns (based on the length of names)
    std::vector<size_t> w_cols;

    // total width of the index table
    size_t w_total{ w_token };

    // count of tokens printed out (differs if filtered or not)
    std::vector<int> tokens(i.size());

    // print matrix headers
    os << " " << std::left << std::setw(w_token) << lbl_dtm << v_separator;
    for (std::vector<Document>::const_iterator it{ i.docs.begin() };
        it != i.docs.end();
        ++it) {
        w_cols.push_back(it->name().length());
        w_total += it->name().length() + v_separator.length();
        os << std::right << it->name() << v_separator;
    }
    os << std::right << lbl_df << v_separator;
    w_cols.push_back(lbl_df.length());
    w_total += lbl_df.length() + v_separator.length();

    // a horizontal separator
    const std::string h_separator{ Indexer::h_separator(w_total) };
    os << std::endl << " " << h_separator << std::endl;

    // print matrix body
    size_t col;
    for (std::map<std::string, DocumentIndexer::vec_pair>::const_iterator m_it{ i.index.begin() };
        m_it != i.index.end();
        ++m_it) {
        col = 0;

        // check if the token is a stopword before printing
        if (!i.stopword(m_it->first)) {
            os << " " << std::left << std::setw(w_token) << m_it->first << v_separator;
            for (std::vector<int>::const_iterator v_it{ m_it->second.first.begin() };
                v_it != m_it->second.first.end();
                ++v_it) {
                os << std::setw(w_cols.at(col)) << std::right << *v_it << v_separator;

                // add the token count to the document total
                tokens.at(col++) += *v_it;
            }

            // print document frequency in the last column
            os << std::setw(w_cols.at(col)) << std::right << i.item_freq(m_it->first)
                << std::endl;
        }
    }
    os << " " << h_separator << std::endl;

    // print statistics
    os << " " << std::right << std::setw(w_token) << "TOTAL" << v_separator;
    for (col = 0; col != i.size(); ++col)
        os << std::setw(w_cols.at(col)) << std::right << tokens.at(col) << v_separator;
    os << std::endl << " " << h_separator << std::endl << std::endl;

    // label for weight matrix
    const std::string lbl_wm{ "WEIGHT MATRIX" };

    // precision with which to display weights
    const size_t weight_prec{ 3 };

    // print matrix headers
    os << " " << std::left << std::setw(w_token) << lbl_wm << v_separator;
    for (std::vector<Document>::const_iterator it{ i.docs.begin() };
        it != i.docs.end();
        ++it)
        os << std::right << it->name() << v_separator;
    os << std::endl << " " << h_separator << std::endl;

    // print matrix body
    for (std::map<std::string, DocumentIndexer::vec_pair>::const_iterator m_it{ i.index.begin() };
        m_it != i.index.end();
        ++m_it) {
        col = 0;

        // check if the token is a stopword before printing
        // save os flags before using std::fixed
        const std::ios_base::fmtflags unfixed{ os.flags() };
        if (!i.stopword(m_it->first)) {
            os << " " << std::left << std::setw(w_token) << m_it->first << v_separator
                << std::fixed;
            for (std::vector<double>::const_iterator v_it{ m_it->second.second.begin() };
                v_it != m_it->second.second.end();
                ++v_it)
                os << std::setw(w_cols.at(col)) << std::right
                    << std::setprecision(weight_prec) << *v_it << v_separator;

            // reset os flags after using std::fixed
            os.flags(unfixed);
            os << std::endl;
        }
    }
    os << " " << h_separator << std::endl;

    return os;
}