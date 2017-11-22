#include "sentence_indexer.h"
#include "sentence_tokenizer.h"
#include "word_tokenizer.h"
#include <algorithm>
#include <iomanip>
/*!
\param i is a integer value corresponding to a position
\return Pointer to IndexItem in a Vector of Sentences 
*/
const IndexItem* SentenceIndexer::operator[] (size_t i) const {
    for (std::vector<Sentence>::const_iterator it{ sentences.begin() };
        it != sentences.end();
        ++it) {
        if (it->pos() == i) {
            const IndexItem* pii{ &sentences.at(i) };

            return pii;
        }
    }
    
    // if index is not found, return nullptr
    return nullptr;
}

/*!
\param s is a String Token
\return Boolean value; True if token exist in index.
*/
bool SentenceIndexer::contains(const std::string& s) const {
    // returns true if the index contains the token
    for (std::vector<Sentence>::const_iterator it{ sentences.begin() };
        it != sentences.end();
        ++it) {
        if (it->contains(s))
            return true;
    }

    return false;
}
/*!
\param s is a Sentence object
\return Boolean value. True if Sentence object exist within Vector of Sentence sentences 
*/
bool SentenceIndexer::contains(const Sentence& s) const {
    for (std::vector<Sentence>::const_iterator it{ sentences.begin() };
        it != sentences.end();
        ++it) {
        if (*it == s)
            return true;
    }

    return false;
}

void SentenceIndexer::normalize() {
    // normalize indexed sentences
    for (std::vector<Sentence>::iterator it{ sentences.begin() };
        it != sentences.end();
        ++it)
        it->normalize();

    normalized = true;
}
/*!
\param s is a User Inputer Query String
\param i is a size_t defining return size
\return Vector<QueryResult> containing Top i Cosine Similarity 
*/
const std::vector<QueryResult> SentenceIndexer::query(
    const std::string& s, size_t i) const {
    // throw an exception if the index is not normalized
    if (!normalized)
        throw IndexException("INDEX_NOT_NORMALIZED");

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
    std::map<std::string, std::vector<double>> sen_weights;
    query_weights(query_, sen_weights);

    std::vector<QueryResult> results{ cos_similarity(query_, sen_weights, tokens) };

    return results;
}
/*!
\param i is a SentenceIndexer object
\param d is a Document object
\return SentenceIndexer Object from input containing Sentence objects derived from Document d 
*/
SentenceIndexer& operator<<(SentenceIndexer& i, const Document& d) {
    i.normalized = false;
    i.docs.push_back(d);
    i.longest = std::max(i.longest, d.longest);

    // create a tokenizer accounting for abbreviations or not
    // by default, tokens will be split on punctuation characters ".?!"
    SentenceTokenizer t;
    if (!i.tokenizer_fp.empty())
        t = SentenceTokenizer(".?!", i.tokenizer_fp);
    else
        t = SentenceTokenizer(".?!");

    // tokenize document contents
    std::stringstream ss{ d.content() };
    std::vector<std::string> tokens = t.tokenize(ss);

    // create sentences from tokens and add them to the index
    Sentence s;
    size_t pos{ 0 };
    std::string doc{ d.name() };
    for (std::vector<std::string>::const_iterator it{ tokens.begin() };
        it != tokens.end();
        ++it)
        i.sentences.emplace_back(Sentence(doc, *it, pos++));

    return i;
}
/*!
\param i is a SentenceIndexer object
\param ix is a Index object containing list of Documents
\return SentenceIndexer Object from input containing Documents specified from Index ix 
*/
SentenceIndexer& operator<<(SentenceIndexer& i, const Index& ix) {
    // creates and adds documents from an index
    std::vector<std::string> docs_fp{ ix.docs() };
    for (std::vector<std::string>::const_iterator it{ docs_fp.begin() };
        it != docs_fp.end();
        ++it)
        i << Document{ *it };

    return i;
}
/*!
\param os is a Output Stream object
\param i is a SentenceIndexer object
\return Output Stream object os whose buffer contains data retrieved from SentenceIndexer i 
*/
std::ostream& operator<<(std::ostream& os, const SentenceIndexer& i) {
    // check for an empty index
    if (i.size() > 0) {
        std::string name{ i.sentences.at(0).name() };
        for (std::vector<Sentence>::const_iterator it{ i.sentences.begin() };
            it != i.sentences.end();
            ++it) {
            // add an extra newline after each document
            if (it->name() != name) {
                name = it->name();
                os << std::endl;
            }

            os << *it << std::endl;
        }
    }

    else
        os << "(empty)";

    return os;
}
/*!
\param s is a String token
\return Integer occurence of Token in Documents 
*/
int SentenceIndexer::item_freq(const std::string& s) const {
    // returns the number of sentences a token is found in
    int freq{ 0 };
    for (std::vector<Sentence>::const_iterator it{ sentences.begin() };
        it != sentences.end();
        ++it) {
        if (it->contains(s))
            freq += 1;
    }

    return freq;
}
/*!
\param s is a String token
\param i is a Integer index of Document in Map
\return Integer occurence of Token in a Document at index i 
*/
int SentenceIndexer::term_freq(const std::string& s, int i) const {
    // return the count of a token in the sentence at the given index
    int freq{ 0 };
    if (sentences.at(i).contains(s))
        freq = sentences.at(i).term_freq(s);

    return freq;
}
/*!
\param s is a String token
\param i is a Integer index of Document in Map
\return Double value corresponding to the normalized term frequency of a token in a Document at index i 
*/
double SentenceIndexer::norm_tf(const std::string& s, int i) const {
    // computes the normalized tf of a token in a sentence
    // normalized term frequency is zero if term frequency is zero
    if (contains(s))
        return std::max((1 + log(static_cast<double>(term_freq(s, i)))), 0.0);

    return 0;
}
/*!
\param s is a String token
\return Double value corresponding to the normalized document frequency of a token 
*/
double SentenceIndexer::norm_idf(const std::string& s) const {
    // computes the normalized idf of a token
    if (contains(s))
        return log(static_cast<double>(sentences.size()) / static_cast<double>(item_freq(s)));

    return 0;
}
/*!
\param s is a String token
\param i is a Integer index of Document in Map
\return Double value corresponding to the weight of a token in a Document at index i 
*/
double SentenceIndexer::weight(const std::string& s, int i) const {
    // returns the weight of a token in the sentence at the given index
    if (contains(s))
        return norm_tf(s, i) * norm_idf(s);

    return 0;
}
/*!
\param q is a Map<token(String, <freq(int), weight(double>>
\param t is a Vector<String> corresponding to User Input Query 
*/
void SentenceIndexer::query_freqs(std::map<std::string, Indexer::query_pair>& q,
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
\param sw is a Map<token(String), vector<document weight(double)>> 
*/
void SentenceIndexer::query_weights(std::map<std::string, Indexer::query_pair>& q,
    std::map<std::string, std::vector<double>>& sw) const {
    // gets query and sentence weights
    double tf{ 0 };
    double idf{ 0 };
    for (std::map<std::string, Indexer::query_pair>::iterator q_it{ q.begin() };
        q_it != q.end();
        ++q_it) {
        // get query weights
        tf = std::max(1 + log(static_cast<double>(q_it->second.first)), 0.0);
        idf = norm_idf(q_it->first);
        q_it->second.second = tf * idf;

        // get sentence weights
        sw.insert(std::pair<std::string, std::vector<double>>{q_it->first,
            std::vector<double>(sentences.size())});
        for (size_t sen{ 0 }; sen != sentences.size(); ++sen)
            sw.at(q_it->first).at(sen) = weight(q_it->first, sen);
    }
}
/*!
\param q is a Map<token(String), <freq(int), weight(double)>> 
\param sw is a Map<token(String), vector<document weight(double)>> 
\param t is a Vector<String> corresponding to User Input Query 
\return Vector<QueryResult> corresponds to Cosine Similarity between User Query and each Document (Decreasing Order) 
*/
const std::vector<QueryResult> SentenceIndexer::cos_similarity(
    const std::map<std::string, Indexer::query_pair>& q,
    const std::map<std::string, std::vector<double>>& sw,
    const std::vector<std::string>& t) const {
    // computes cosine similarity between query and each sentence
    std::vector<QueryResult> results;
    std::string token;
    double num{ 0 };
    double len1{ 0 };
    double len2{ 0 };
    double den{ 0 };
    for (size_t sen{ 0 }; sen != sentences.size(); ++sen) {
        num = 0;
        len1 = 0;
        len2 = 0;
        den = 0;
        for (std::vector<std::string>::const_iterator it{ t.begin() };
            it != t.end();
            ++it) {
            num += q.at(*it).second * sw.at(*it).at(sen);
            len1 += std::pow(q.at(*it).second, 2);
            len2 += std::pow(sw.at(*it).at(sen), 2);
        }
        den = std::sqrt(len1) * std::sqrt(len2);

        // construct and add query result (score is zero if den is zero)
        // emplace_back() makes use of move operations
        if (den == 0)
            results.emplace_back(QueryResult(&sentences.at(sen), 0));
        else
            results.emplace_back(QueryResult(&sentences.at(sen), num / den));
    }

    // sort results before returning (highest score first)
    std::sort(results.begin(), results.end());

    return results;
}