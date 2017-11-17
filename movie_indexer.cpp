#include "movie_indexer.h"
#include <sstream>

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
            tokenize_data(ss);
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
            tokenize_summary(ss);
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