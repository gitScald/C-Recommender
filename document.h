//! Document Class
/*!
Document objects represents one document in the Index, acting like a dictionary of sorts.
Each object contains the list of words within said document, along with their respective
number of occurence.
*/
#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "document_indexer.h"
#include "index_item.h"
#include <iostream>
#include <string>
#include <vector>

//Forward Declaration
class DocumentIndexer;
class SentenceIndexer;

class Document :public IndexItem {
    friend class DocumentIndexer;
    friend class Movie;
    friend class MovieIndexer;
    //! Overload operator<<
    /*!
    This function adds a string token (word corresponding to a document) to a map of a specified
    Document object. 
    */
    friend Document& operator<<(Document& d, const std::string& s);
    friend DocumentIndexer& operator<<(DocumentIndexer& i, const Document& d);
    friend SentenceIndexer& operator<<(SentenceIndexer& i, const Document& d);
    //! Overload operator<<
    /*!
    This function prints out to an output stream object, the contents of the Document object.
    Prints the information first of the Document (name_ and content_) followed by the dictionary 
    (map container)
    */
    friend std::ostream& operator<<(std::ostream& os, const Document& d);

public:
    //! Empty Constructor
    Document()
        : IndexItem() {};
    //! Constructor.
    /*!
    Constructor takes as input the file name, more precisely its file path, and 
    reads the content of the file into the document object. Using StringStream and
    Tokenizer objects, the constructor dumps the values of the input file into a private 
    map container "dict" (Key = Word, Value = Word Count). 

    */
    explicit Document(const std::string& doc_fp)
        : IndexItem(doc_fp) { init(); }
    virtual ~Document() {};
    //! Token Verification Method.
    /*!
    Method verifying if the input parameter, representing a key, exist in 
    private map container "dict".
    */
    bool contains(const std::string& s) const override;
    //! Overloading Operator ()
    /*!
    Returns boolean value of member function "contains(x)"
    where arguement x corresponds to input String parameter. 
    */
    bool operator() (const std::string& s) const override { return contains(s); };
    //!Accessor Method
    /*!
    Accessor Method. 
    Virtual Method. Returns member variable name_ 
    */
    const std::string& name() const override { return name_short; };

private:
    std::string name_short; //!< String containing name of document
    size_t longest{ 0 }; //!< Value representation of longest string key.

    void init();
};

#endif // !DOCUMENT_H