#include "movie_indexer.h"
#include "stringops.h"
#include "word_tokenizer.h"
#include <sstream>

std::ostream& operator<<(std::ostream& os, const MovieIndexer& mi) {
    // simple index output
    for (std::vector<Movie>::const_iterator it{ mi.movies.begin() };
        it != mi.movies.end();
        ++it)
        os << *it << std::endl << std::endl;
    
    return os;
}

std::string MovieIndexer::summary(const std::string& s) const {
    std::string title;
    std::string content;
    for (std::vector<Movie>::const_iterator it{ movies.begin() };
        it != movies.end();
        ++it) {
        title = it->name_;
        //lower(strip(title));
        if (title == s)
            content = it->content_;
    }

    return content;
}

bool MovieIndexer::contains(const std::string& s) const {
    // returns true if the movie is in the index
    // precondition: movie title has already been lowered and stripped
    std::string title;
    for (std::vector<Movie>::const_iterator it{ movies.begin() };
        it != movies.end();
        ++it) {
        title = it->name_;
        //lower(strip(title));
        if (title == s)
            return true;
    }

    return false;
}

const IndexItem* MovieIndexer::operator[] (size_t i) const {
    // returns a pointer to the movie at the given index
    const IndexItem* pii = &movies.at(i);

    return pii;
}

void MovieIndexer::normalize() {
    // calls member document indexer normalize() method
    index.normalize();
    normalized = true;
}

const std::vector<QueryResult> MovieIndexer::query(
    const std::string& s, size_t i) const {
    // throw an exception if the index is not normalized
    if (!normalized)
        throw IndexException("INDEX_NOT_NORMALIZED");

    // lower and strip movie title
    std::string query_title{ s };
    //lower(strip(query_title));

    // throw an exception if the movie is not in the index
    if (!contains(s))
        throw IndexException("MOVIE_NOT_IN_INDEX");

    std::string query_Summary(summary(s));

    if (query_Summary == "(no summary available)")
        throw IndexException("MOVIE_DOES_NOT_CONTAIN_SUMMARY");

    std::cout << query_Summary << std::endl;
    std::stringstream ss{ query_Summary };
    WordTokenizer t;
    const std::vector<std::string> tokens{ t.tokenize(ss) };

    std::map<std::string, Indexer::query_pair> query_;
    query_freqs(query_, tokens);

    std::map<std::string, std::vector<double>> doc_weights;
    query_weights(query_, doc_weights);

    /*
    Mandeep Note:
    -Methods from DocumentIndexer work.
    -Correct Weight/Freq? 
    -lower(strip(string)) throws C++ Assert Debugging Error. Cannot figure out issue.
    */


    // ********************************************************
    // TODO: figure out how to get term frequencies and weights
    // for all movie summaries
    // ********************************************************

    //// get query term frequencies
    //// query_ is of the form (token, <freq, weight>)
    //std::map<std::string, Indexer::query_pair> query_;
    //query_freqs(query_, tokens);

    //// get query and document weights
    //std::map<std::string, std::vector<double>> doc_weights;
    //query_weights(query_, doc_weights);

    //// return only the top i results
    //std::vector<QueryResult> results{ cos_similarity(query_, doc_weights, tokens) };
    //results.resize(std::min(results.size(), i));

    // ********************************************************

    //return results;

    return std::vector<QueryResult>();
}

int MovieIndexer::item_freq(const std::string& s) const {
    // calls member document indexer item_freq() method
    return index.item_freq(s);
}

int MovieIndexer::term_freq(const std::string& s, int i) const {
    // calls member document indexer term_freq() method
    return index.term_freq(s, i);
}

double MovieIndexer::norm_tf(const std::string& s, int i) const {
    // calls member document indexer norm_tf() method
    return index.norm_tf(s, i);
}

double MovieIndexer::norm_idf(const std::string& s) const {
    // calls member document indexer norm_idf() method
    return index.norm_idf(s);
}

double MovieIndexer::weight(const std::string& s, int i) const {
    // calls member document indexer weight() method
    return index.weight(s, i);
}

void MovieIndexer::query_freqs(std::map<std::string, Indexer::query_pair>& q,
    const std::vector<std::string>& t) const {
    index.query_freqs(q, t);
}

void MovieIndexer::query_weights(std::map<std::string, Indexer::query_pair>& q,
    std::map<std::string, std::vector<double>>& dw) const {
    index.query_weights(q, dw);
}

const std::vector<QueryResult> MovieIndexer::cos_similarity(
    const std::map<std::string, Indexer::query_pair>& q,
    const std::map<std::string, std::vector<double>>& dw,
    const std::vector<std::string>& t) const {
    // TODO
    return std::vector<QueryResult>();
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
            std::cout << "Processing metadata file '" << data_fp_ << "'...";
            tokenize_data(ss);
            std::cout << " done." << std::endl;
        }
    }

    // add movie document objects to the index
    for (std::vector<Movie>::const_iterator it{ movies.begin() };
        it != movies.end();
        ++it)
        index << it->doc;
}

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
        }

        // read next entry
        std::getline(ss, metadata);
    }
}

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