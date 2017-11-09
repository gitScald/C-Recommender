//!Sentence class
/*!
[DERIVED CLASS] 
Sentence objects represents one sentence from a Document in the Index. 
It is a container object that contains a string pertaining to the actual sentence within said document, along with its respective position, weight, size and document name.
*/
#ifndef SENTENCE_H
#define SENTENCE_H

#include "index_item.h"

class Sentence :public IndexItem {
    friend class SentenceIndexer;
    //!Overload operator==. 
    /*!
    This function compares two sentences objects, comparing their position and their content. True if they're the same.
    */
    friend bool operator==(const Sentence& s1, const Sentence& s2);
    //!Overload operator!=. 
    /*!
    This function compares two sentence objects, comparing their position and their content. True if they're not the same.
    */    
    friend bool operator!=(const Sentence& s1, const Sentence& s2);
    //!Overload operator<. 
    /*!
    This function compares the two sentence objects, comparing their position and their name. If they're from the same document (name_), then compare the two position, else compare their document name (name_)
    */    
    friend bool operator<(const Sentence& s1, const Sentence& s2);
    //!Overload operator<<. 
    /*!
    This function adds a token to the dictionary.
    */    
    friend Sentence& operator<<(Sentence& s, const std::string& str);
    //!Overload operator<<. 
    /*!
    Output Sentence object content to Output Stream
    */    
    friend std::ostream& operator<<(std::ostream& os, const Sentence& s);

public:
    //!Empty Constructor.
    /*!
    Empty derived constructor referencing base class IndexItem 
    */
    Sentence()
        : IndexItem() {};
    //!Constructor. 
    /*!
    Constructor for object Sentence. Storing the name of the document, content and position. The Name and Content makes reference to base class object IndexItem.
    */
    explicit Sentence(const std::string& name, const std::string& content, int pos)
        : IndexItem(name, content), pos_{ static_cast<size_t>(pos) } { init(); };
    //!Copy Constructor
    Sentence(const Sentence& s)
        : IndexItem(s.name_, s.content_), pos_{ s.pos_ } { init(); };
    //!Virtual Deconstructor
    virtual ~Sentence() {};
    //!Override operator() 
    /*!
    Override operator from base class operator(). Return value from member function contains
    */
    bool operator()(const std::string& s) const override { return contains(s); };
    //!Override function contains. 
    /*!
    Override function contains from base class. Return true if dictionary contains string input string parameter
    */
    bool contains(const std::string& s) const override;
    //!Accessor method. 
    /*!
    Returm position of Sentence within a Document
    */
    size_t pos() const { return pos_; };
    const size_t size() const override;

private:
    //!Size_t Member Variable. 
    /*!
    Position of sentence in Document
    */
    size_t pos_{ 0 };
    //!Map Member Variable. 
    /*!
    Map String sentences derived from a Document using Tokenizer
    */
    std::map<std::string, std::pair<int, double>> dict;
    //!Member Integer Function. 
    /*!
    Document Frequency; Used in calculation of Weight.
    */
    int item_freq(const std::string& s) const;
    //!Member Integer Function. 
    /*!
    Term Frequency; Used in calculation of Weight.
    */
    int term_freq(const std::string& s) const;
    //!Member Integer Function. 
    /*!
    Normalized Term Frequency; Used in calculation of Weight.
    */
    double norm_tf(const std::string& s) const;
    //!Member Integer Function. 
    /*!
    Normalized Document Frequency; Used in calculation of Weight.
    */
    double norm_idf(const std::string& s) const;
    //!Member Integer Function. 
    /*!
    Weight = (1 + Log (term frequency)) * Log ( # of Documents / Document Frequency)
    */
    double weight(const std::string& s) const;

    void normalize();
    void init();

};

#endif // !SENTENCE_H