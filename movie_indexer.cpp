#include "movie_indexer.h"
#include "word_tokenizer.h"
#include <algorithm>
#include <future>
#include <sstream>
/*!
\param os is a Output Stream object
\param mi is a MovieIndexer object
\return Output Stream object os whose buffer contains data retrieved from MovieIndexer mi
*/
std::ostream& operator<<(std::ostream& os, const MovieIndexer& mi) {
    // simple index output
    for (std::vector<Movie>::const_iterator it{ mi.movies.begin() };
        it != mi.movies.end();
        ++it)
        os << *it << std::endl << std::endl;
    
    return os;
}
void MovieIndexer::index_docs() {
    normalized = false;

    // allocate map space
    size_t doc{ 0 };
    for (std::vector<Document>::const_iterator v_it{ docs.begin() };
        v_it != docs.end();
        ++v_it) {
        for (std::map<std::string, int>::const_iterator m_it{ v_it->dict.begin() };
            m_it != v_it->dict.end();
            ++m_it) {
            // check if token is not already in the index
            std::unordered_map<std::string, MovieIndexer::vec_pair>::const_iterator um_it{ index.find(m_it->first) };

            if (um_it == index.end())
                // create map entry if not
                index.insert(std::pair<std::string, MovieIndexer::vec_pair>{ m_it->first,
                    MovieIndexer::vec_pair(std::vector<int>(movies.size()),
                        std::vector<double>(movies.size())) });

            // add the token count to the index
            index.at(m_it->first).first.at(doc) = m_it->second;
        }

        // continue to next document
        ++doc;
    }
}
/*!
\param s is a String variable corresponding to movie title
\return String corresponding to movie summary stored within movies vector
*/
std::string MovieIndexer::summary(const std::string& s) const {
    std::string content;
    for (std::vector<Movie>::const_iterator it{ movies.begin() };
        it != movies.end();
        ++it) {
        if (it->name_ == s)
            content = it->content_;
    }

    return content;
}
/*!
\param s is a String variable corresponding to movie title
\return True if movie exists within movies vector
*/
bool MovieIndexer::contains(const std::string& s) const {
    // returns true if the movie is in the index
    for (std::vector<Movie>::const_iterator it{ movies.begin() };
        it != movies.end();
        ++it) {
        if (it->name_ == s)
            return true;
    }

    return false;
}
bool MovieIndexer::in_docs(const std::string& s) const {
    // returns true if any of the documents contain the token
    for (std::vector<Document>::const_iterator it{ docs.begin() };
        it != docs.end();
        ++it) {
        if (it->contains(s))
            return true;
    }
    return false;
}
/*!
\param i is a integer value corresponding to a position
\return Pointer to IndexItem in a Vector of Movie
*/
const IndexItem* MovieIndexer::operator[] (size_t i) const {
    // returns a pointer to the movie at the given index
    const IndexItem* pii = &movies.at(i);

    return pii;
}

void MovieIndexer::normalize() {
    // computes token weights
    for (std::unordered_map<std::string, MovieIndexer::vec_pair>::const_iterator m_it{ index.begin() };
        m_it != index.end();
        ++m_it) {
        for (size_t doc{ 0 }; doc != size(); ++doc)
            // compute and write weights in the map entry
            index.at(m_it->first).second.at(doc) = weight(m_it->first, doc);
    }

    normalized = true;
}
/*!
\param s is a User Inputer Query String
\param i is a size_t defining return size
\return Vector<QueryResult> containing Top i Cosine Similarity
*/
const std::vector<QueryResult> MovieIndexer::query(
    const std::string& s, size_t i) const {
    // throw an exception if the index is not normalized
    if (!normalized)
        throw IndexException("INDEX_NOT_NORMALIZED");

    // throw an exception if the movie is not in the index
    if (!contains(s))
        throw IndexException("MOVIE_NOT_IN_INDEX");

    std::string summary_(summary(s));

    std::stringstream ss{ summary_ };
    WordTokenizer t;
    const std::vector<std::string> tokens{ t.tokenize(ss) };

    std::map<std::string, Indexer::query_pair> query_;
    std::cout << "Computing query term frequencies...";
    query_freqs(query_, tokens);
    std::cout << " done." << std::endl;

    std::cout << "Computing query term weights...";
    std::map<std::string, std::vector<double>> doc_weights;
    query_weights(query_, doc_weights);
    std::cout << " done." << std::endl;

    // compute cosine similarities
    std::cout << "Computing cosine similarities...";
    std::vector<QueryResult> results{ cos_similarity(query_, doc_weights, tokens) };

    // first result will be the queried movie itself, so take one more result
    results.resize(std::min(results.size(), i + 1));
    std::cout << " done." << std::endl;

    return results;
}
/*!
\param s is a String token
\return Integer occurence of Token
*/
int MovieIndexer::item_freq(const std::string& s) const {
    // returns the number of summaries a token is found in
    int freq{ 0 };
    std::unordered_map<std::string, MovieIndexer::vec_pair>::const_iterator m_it{ index.find(s) };
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
\param s is a String token
\param i is a Integer index of Document in Map in DocumentIndexer
\return Integer occurence of Token
*/
int MovieIndexer::term_freq(const std::string& s, int i) const {
    // return the count of a token in the summary at the given index
    int freq{ 0 };
    if (docs.at(i).contains(s))
        freq = docs.at(i).dict.at(s);

    return freq;
}
/*!
\param s is a String token
\param i is a Integer index of Document in Map in DocumentIndexer
\return Double value corresponding to the normalized term frequency of a token in a Document at index i in DocumentIndexer
*/
double MovieIndexer::norm_tf(const std::string& s, int i) const {
    // computes the normalized tf of a token in a summary
    // normalized term frequency is zero if term frequency is zero
    if (docs.at(i).contains(s))
        return std::max((1 + log(static_cast<double>(term_freq(s, i)))), 0.0);

    return 0;
}
/*!
\param s is a String token
\return Double value corresponding to the normalized document frequency of a token
*/
double MovieIndexer::norm_idf(const std::string& s) const {
    // computes the normalized idf of a token
    if (in_docs(s))
        return log(static_cast<double>(docs.size()) / static_cast<double>(item_freq(s)));

    return 0;
}
/*!
\param s is a String token
\param i is a Integer index of Document in Map in DocumentIndexer
\return Double value corresponding to the weight of a token in a Document at index in DocumentIndexer
*/
double MovieIndexer::weight(const std::string& s, int i) const {
    // returns the weight of a token in the summary at the given index
    // std::async() causes Linux to have PMS, take this out for the demo
    std::future<double> future_tf{ std::async(&MovieIndexer::norm_tf, this, s, i) };
    std::future<double> future_idf{ std::async(&MovieIndexer::norm_idf, this, s) };

    double idf = future_idf.get();
    if (idf == 0)
        return 0;

    return future_tf.get() * idf;

    //double tf{ norm_tf(s, i) };
    //double idf{ norm_idf(s) };

    //if (idf == 0)
    //    return 0;

    //return tf * idf;
}
/*!
\param q is a Map<token(String, <freq(int), weight(double>>
\param t is a Vector<String> corresponding to User Input Query
*/
void MovieIndexer::query_freqs(std::map<std::string, Indexer::query_pair>& q,
    const std::vector<std::string>& t) const {
    // gets query term frequencies
    std::map<std::string, MovieIndexer::query_pair>::const_iterator m_it{ q.end() };
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
void MovieIndexer::query_weights(std::map<std::string, Indexer::query_pair>& q,
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
const std::vector<QueryResult> MovieIndexer::cos_similarity(
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
    for (size_t mov{ 0 }; mov != movies.size(); ++mov) {
        num = 0;
        len1 = 0;
        len2 = 0;
        den = 0;
        for (std::vector<std::string>::const_iterator it{ t.begin() };
            it != t.end();
            ++it) {
            num += q.at(*it).second * dw.at(*it).at(mov);
            len1 += q.at(*it).second * q.at(*it).second;
            len2 += dw.at(*it).at(mov) * dw.at(*it).at(mov);
        }
        den = std::sqrt(len1) * std::sqrt(len2);

        // construct and add query result (score is zero if den is zero)
        // emplace_back() makes use of move operations
        if (den == 0)
            results.emplace_back(QueryResult(&movies.at(mov), 0));
        else
            results.emplace_back(QueryResult(&movies.at(mov), num / den));
    }

    // sort results before returning (highest score first)
    std::sort(results.begin(), results.end());

    return results;
}

void MovieIndexer::init() {
    // grab and tokenize movie summaries
    if (!summary_fp_.empty()) {
        std::ifstream ifs{ summary_fp_ };

        if (!ifs)
            std::cerr << "WARNING: Could not establish input stream with '"
            << summary_fp_ << "'" << std::endl;

        else {
            std::stringstream ss;
            ss << ifs.rdbuf();
            std::cout << "Processing summary file '" << summary_fp_
                << "'..." << std::endl;
            tokenize_summary(ss);
        }
    }

    // grab and tokenize movie metadata
    if (!data_fp_.empty()) {
        std::ifstream ifs{ data_fp_ };

        if (!ifs)
            std::cerr << "WARNING: Could not establish input stream with '"
            << data_fp_ << "'" << std::endl;

        else {
            std::stringstream ss;
            ss << ifs.rdbuf();
            std::cout << "Processing metadata file '" << data_fp_
                << "'..." << std::endl;
            tokenize_data(ss);
        }
    }

    // add movie document objects to the index and build it
    std::cout << "Indexing document objects...";
    for (std::vector<Movie>::const_iterator it{ movies.begin() };
        it != movies.end();
        ++it)
        docs.push_back(it->doc);
    index_docs();
    std::cout << " done." << std::endl;
}
/*!
\param ss is a StringStream Object containing metadata
*/
void MovieIndexer::tokenize_data(std::stringstream& ss) {
    // fill up movies vector with metadata
    std::map<std::string, Movie&> movies_map;
    std::map<std::string, Movie&>::iterator m_it;
    std::vector<Movie>::iterator v_it;

    std::string metadata;
    std::string data;
    std::getline(ss, metadata);

    std::string id, name, release_date;

    // loop through movie objects and make a map out of their ids
    std::cout << "Building map entries for movie objects...";
    for (v_it = movies.begin();
        v_it != movies.end();
        ++v_it) 
        movies_map.insert(std::pair<std::string, Movie&>(v_it->id_, *v_it));
    std::cout << " done." << std::endl;

    std::cout << "Processing movie metadata and summaries...";
    while (metadata != std::string()) {
        // movie entries enter a stringstream
        std::stringstream metadata_ss;
        metadata_ss << metadata;

        // then each movie entry is tokenized using tab delimiters
        std::getline(metadata_ss, data, '\t');
        id = data;
        
        // skip extra id
        std::getline(metadata_ss, data, '\t');

        std::getline(metadata_ss, data, '\t');
        name = data;

        std::getline(metadata_ss, data, '\t');
        release_date = data;

        // fetch movie metadata, mapping to its id
        m_it = movies_map.find(id);
        if (m_it != movies_map.end()) {
            m_it->second.name_ = name;
            m_it->second.release_date_ = release_date;
            m_it->second.init();
        }

        // read next entry
        std::getline(ss, metadata);
    }
    std::cout << " done." << std::endl;
}
/*!
\param ss is a StringStream Object containing movie summary data
*/
void MovieIndexer::tokenize_summary(std::stringstream& ss) {
    // fill up movies vector with plot summaries
    std::string summary;
    std::string data;
    std::getline(ss, summary);

    std::string id, content;


    std::cout << "Fetching plot summaries...";
    while (summary != std::string()) {
        // movie entries enter a stringstream
        std::stringstream summary_ss;
        summary_ss << summary;

        // then each movie entry is tokenized using tab delimiters
        std::getline(summary_ss, data, '\t');
        id = data;

        std::getline(summary_ss, data);
        content = data;

        // create a movie object from the summary
        movies.emplace_back(Movie(std::string(), content, id, std::string()));

        // read next entry
        std::getline(ss, summary);
    }
    std::cout << " done." << std::endl;
}