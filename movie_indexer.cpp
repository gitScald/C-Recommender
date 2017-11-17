#include "movie_indexer.h"
#include <sstream>

bool MovieIndexer::contains(const std::string& s) const {
    for (std::vector<Movie>::const_iterator it{ movies.begin() };
        it != movies.end();
        ++it) {
        if (it->doc.contains(s))
            return true;
    }

    return false;
}

const IndexItem* MovieIndexer::operator[] (size_t i) const {
    const IndexItem* pii = &movies.at(i);

    return pii;
}

void MovieIndexer::normalize() {

}

const std::vector<QueryResult> MovieIndexer::query(
    const std::string& s, size_t i) const {
    return std::vector<QueryResult>();
}

int MovieIndexer::item_freq(const std::string& s) const {
    return 0;
}

int MovieIndexer::term_freq(const std::string& s, int i) const {
    return 0;
}

double MovieIndexer::norm_tf(const std::string& s, int i) const {
    return 0;
}

double MovieIndexer::norm_idf(const std::string& s) const {
    return 0;
}

double MovieIndexer::weight(const std::string& s, int i) const {
    return 0;
}

void MovieIndexer::query_freqs(std::map<std::string, Indexer::query_pair>& q,
    const std::vector<std::string>& t) const {

}

void MovieIndexer::query_weights(std::map<std::string, Indexer::query_pair>& q,
    std::map<std::string, std::vector<double>>& dw) const {

}

const std::vector<QueryResult> MovieIndexer::cos_similarity(
    const std::map<std::string, Indexer::query_pair>& q,
    const std::map<std::string, std::vector<double>>& dw,
    const std::vector<std::string>& t) const {
    return std::vector<QueryResult>();
}

void MovieIndexer::init() {
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

    // grab and tokenize movie summaries
    if (!summary_fp_.empty()) {
        std::ifstream ifs{ summary_fp_ };

        if (!ifs)
            std::cerr << "WARNING: Could not establish input stream with '"
            << summary_fp_ << "'" << std::endl;

        else {
            std::stringstream ss;
            ss << ifs.rdbuf();
            std::cout << "Processing summary file '" << summary_fp_ << "'...";
            tokenize_summary(ss);
            std::cout << " done." << std::endl;
        }
    }
}

void MovieIndexer::tokenize_data(std::stringstream& ss) {
    // fill up movies vector with metadata
    std::stringstream metadata_ss;
    std::string metadata;
    std::getline(ss, metadata);

    std::string dummy;
    size_t id;
    std::string name;
    std::string release_date;

    while (metadata != std::string()) {
        metadata_ss << metadata;
        dummy = name = release_date = std::string();
        id = 0;

        // fetch movie metadata, skipping extra id
        metadata_ss >> id >> dummy >> name >> release_date;
        movies.emplace_back(Movie(name, std::string(), id, release_date));

        // clear metadata stringstream
        metadata_ss.str(std::string());
    }
}

void MovieIndexer::tokenize_summary(std::stringstream& ss) {
    // fill up movies vector with plot summaries
    std::map<size_t, std::string> summaries;
    std::map<size_t, std::string>::const_iterator m_it;

    std::stringstream summary_ss;
    std::string summary;
    std::getline(ss, summary);

    size_t id;
    std::string content;

    while (summary != std::string()) {
        summary_ss << summary;
        id = 0;
        content = std::string();

        // fetch movie summary, mapping to its id
        summary_ss >> id >> content;
        m_it = summaries.find(id);
        if (m_it == summaries.end()) {
            summaries.insert(std::pair<size_t, std::string>
                (std::make_pair(id, content)));
        }

        // clear summary stringstream
        summary_ss.str(std::string());
    }

    // loop through summary entries and add them to the movie objects
    for (std::vector<Movie>::iterator v_it{ movies.begin() };
        v_it != movies.end();
        ++v_it) {
        m_it = summaries.find(v_it->id_);
        if (m_it != summaries.end())
            v_it->doc.content_ = m_it->second;
    }
}