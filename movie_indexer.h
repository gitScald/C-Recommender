#ifndef MOVIE_INDEXER_H
#define MOVIE_INDEXER_H

#include "indexer.h"
#include "movie.h"

class MovieIndexer :public Indexer {
public:
    MovieIndexer() {}
    explicit MovieIndexer(const std::string& data_fp,
        const std::string& summary_fp)
        : data_fp_{ data_fp }, summary_fp_{ summary_fp }
    { init(); }

private:
    std::string data_fp_;
    std::string summary_fp_;
    std::vector<Movie> movies;

    void init();
    void tokenize_data(std::stringstream& ss);
    void tokenize_summary(std::stringstream& ss);
};

#endif // !MOVIE_INDEXER_H