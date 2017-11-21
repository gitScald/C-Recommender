#ifndef MOVIE_INDEXER_H
#define MOVIE_INDEXER_H

#include "document_indexer.h"
#include "movie.h"

class MovieIndexer :public Indexer {
    friend std::ostream& operator<<(std::ostream& os, const MovieIndexer& mi);

public:
    MovieIndexer() {}
    explicit MovieIndexer(const std::string& data_fp,
        const std::string& summary_fp)
        : data_fp_{ data_fp }, summary_fp_{ summary_fp } { init(); }

    const IndexItem* operator[] (size_t i) const override;
    std::string summary(const std::string & s) const;
    bool contains(const std::string& s) const override;
    void normalize() override;
    const std::vector<QueryResult> query(
        const std::string& s, size_t i = 5) const override;
    size_t size() const override { return movies.size(); };

private:
    std::string data_fp_;
    std::string summary_fp_;
    std::vector<Movie> movies;
    DocumentIndexer index;

    void init();
    void tokenize_data(std::stringstream& ss);
    void tokenize_summary(std::stringstream& ss);
    int item_freq(const std::string& s) const override;
    int term_freq(const std::string& s, int i) const override;
    double norm_tf(const std::string& s, int i) const override;
    double norm_idf(const std::string& s) const override;
    double weight(const std::string& s, int i) const override;
    void query_freqs(std::map<std::string, Indexer::query_pair>& q,
        const std::vector<std::string>& t) const override;
    void query_weights(std::map<std::string, Indexer::query_pair>& q,
        std::map<std::string, std::vector<double>>& dw) const override;
    const std::vector<QueryResult> cos_similarity(
        const std::map<std::string, Indexer::query_pair>& q,
        const std::map<std::string, std::vector<double>>& dw,
        const std::vector<std::string>& t) const override;
};

#endif // !MOVIE_INDEXER_H