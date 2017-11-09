//!IndexItem class
/*!
[BASE CLASS] IndexItem class corresponds to container object that contains data derived from documents, which are tokenized.
*/
#ifndef INDEX_ITEM_H
#define INDEX_ITEM_H

//#include "document_indexer.h"
//#include "sentence_indexer.h"
#include <iostream>
#include <map>
#include <string>
 
class DocumentIndexer;

class DocumentIndexer;

class IndexItem {
    //friend class DocumentIndexer;
    //friend class SentenceIndexer;

    friend std::ostream& operator<<(std::ostream& os, const DocumentIndexer& i);

public:
    //!Empty Constructor
    IndexItem() {};
    //!Explicit Constructor
    /*!
    Specify IndexItem data to be stored within container object. name_ = name of document, content_ = content of Item 
    */
    explicit IndexItem(const std::string& name, const std::string& content = std::string())
        : name_{ name }, content_{ content } {};
    //!Virtual Deconstructor. 
    virtual ~IndexItem() {};
    //!Override Operator()
    /*!
    Virtual Function. Override operator operator(). Return value from member function contains() 
    */
    virtual bool operator() (const std::string& s) const { return contains(s); };
    //!Contains Method
    /*!
    Pure Virtual Function. 
    Return true if dictionary contains string input string parameter
    */
    virtual bool contains(const std::string& s) const = 0;
    //!Contains Method
    /*!
    Pure Virtual Function. Return true if dictionary contains string input string parameter
    */
    const std::string& content() const { return content_; };
    //!Accessor Method
    /*!
    Virtual Method. 
    Returns member variable name_
    */
    virtual const std::string& name() const { return name_; };
    //!Accessor Method
    /*!
    Returns member variable content_
    */
    virtual const size_t size() const { return content_.length(); };

protected:
    //!String Member Variable
    /*!
    Corressponding to name of Document
    */
    std::string name_;
    //!String Member Variable
    /*!
    Corressponding to content of IndexItem
    */
    std::string content_;
    //!Map Member Variable
    /*!
    Corresponding to map containing String values as key, which represent the content of IndexItem. 
    */
    std::map<std::string, int> dict;
};

#endif // !INDEX_ITEM_H