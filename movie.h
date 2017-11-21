//!Movie class
/*!
[DERIVED CLASS]
Movie objects represents one movie from the Index.
It is a container object that contains a string pertaining to the title(name_), summary(content_), movie id (id_), release date (release_date_)
*/
#ifndef MOVIE_H
#define MOVIE_H

#include "document.h"

class Movie :public IndexItem {
    friend class MovieIndexer;
    //!Overload operator<<. 
    /*!
    Output Movie object content to Output Stream
    */
    friend std::ostream& operator<<(std::ostream& os, const Movie& m);

public:
    //!Empty Constructor.
    /*!
    Empty derived constructor referencing base class IndexItem
    */
    Movie()
        : IndexItem() {}
    //!Constructor. 
    /*!
    Constructor for object Movie. Storing the name (title), content (summary), id (movie id) and release (release date). The Name and Content makes reference to base class object IndexItem.
    */
    explicit Movie(const std::string& name, const std::string& content,
        const std::string& id, const std::string& release)
        : IndexItem(name, content), id_{ id }, release_date_{ release } {}
    //!Copy Constructor
    Movie(const Movie& m)
        : IndexItem(m.name_, m.content_), id_{ m.id_ },
        release_date_{ m.release_date_ } {}
    //!Virtual Deconstructor
    virtual ~Movie() {};
    //!Override operator() 
    /*!
    Override operator from base class operator(). Return value from member function contains
    */
    bool operator()(const std::string& s) const override { return contains(s); }
    //!Override function contains. 
    /*!
    Override function contains from base class. Return true if Document contains string input string parameter
    */
    bool contains(const std::string& s) const override { return doc.contains(s); }
    const size_t size() const override { return doc.size(); };
    //!Accessor Method
    const std::string& id() const { return id_; };
    //!Accessor Method
    const std::string& release_date() const { return release_date_; }
    //!Void Method
    /*!
    Create/Initialize Document object 
    */
    void build_index();

private:
    //!String Member Variable
    /*!
    Stores designated Movie ID
    */
    std::string id_;
    //!String Member Variable
    /*!
    Stores designated Movie Release Date
    */
    std::string release_date_;
    //!Document Member Object
    /*!
    Document Object stores summary of object. Incorporated in DocumentIndexer.
    */
    Document doc;
};

#endif // !MOVIE_H