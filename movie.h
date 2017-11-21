#ifndef MOVIE_H
#define MOVIE_H

#include "document.h"

class Movie :public IndexItem {
    friend class MovieIndexer;

    friend std::ostream& operator<<(std::ostream& os, const Movie& m);

public:
    Movie()
        : IndexItem() {}
    explicit Movie(const std::string& name, const std::string& content,
        const std::string& id, const std::string& release)
        : IndexItem(name, content), id_{ id }, release_date_{ release } {}
    Movie(const Movie& m)
        : IndexItem(m.name_, m.content_), id_{ m.id_ },
        release_date_{ m.release_date_ } {}
    virtual ~Movie() {};

    bool operator()(const std::string& s) const override { return contains(s); }
    bool contains(const std::string& s) const override { return doc.contains(s); }
    const size_t size() const override { return doc.size(); };
    const std::string& id() const { return id_; };
    const std::string& release_date() const { return release_date_; }
    void init();

private:
    std::string id_;
    std::string release_date_;
    Document doc;
};

#endif // !MOVIE_H