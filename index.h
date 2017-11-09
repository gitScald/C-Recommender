//! Class Index
/*!
Index object is initialized to keep track of the list of files that are imported into the search engine.
The file paths of these files are kept stored in a string vector so system may iterate through them.
*/
#ifndef INDEX_H
#define INDEX_H

#include "document_indexer.h"
#include <string>
#include <vector>

//! Forward Declarations
class DocumentIndexer;

class Index {
    friend DocumentIndexer& operator<<(DocumentIndexer& i, const Index& ix);

public:
    //! Constructor
    /*
    Construct an Index object using an index filepath which directs to a list of files that will be 
    used to populate the search engine, storing the path to each file specified from the index file in
    a vector of string object. 
    */
    explicit Index(const std::string& index_fp);
    //!Accessor Method
    /*!
    Returns the vector of string, specifying the list of documents that will be used to populate the
    search engine.
    */
    const std::vector<std::string>& docs() const{ return docs_fp; };
    //! Accessor Method
    /*!
    Returns the name and filepath of the index file, storing the documents. 
    */
    const std::string& name() const { return name_; };

private:
    //! String Member Variable. 
    /*!
    Name of file at index filepath (index_fp)
    */
    std::string name_;
    //! Vector String Member Variable
    /*!
    Vector containing filepaths of documents as string objects. 
    */
    std::vector<std::string> docs_fp;
};

#endif // !INDEX_H