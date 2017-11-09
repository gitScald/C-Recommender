//! QueryResult Class
/*!
Container class, where each QueryResult object contains the document name and their document score, which is based
off a scoring function that computes the cosine similarity between their document vector and a user specified
query vector. 
*/

#ifndef QUERY_RESULT_H
#define QUERY_RESULT_H

#include "index_item.h"
#include <string>
#include <iostream>

// forward declaration 
class IndexItem; 

class QueryResult {
    // output the sentence contents to main()
    friend int main(int argc, char* argv[]);
    //! Overload operator<<
    /*!
    This function prints out to an output stream object the QueryResult's objects Name and Score. 
    Saves the OStream flags.
    */
    friend std::ostream& operator<<(std::ostream& os, const QueryResult& qr);
    //! Overload operator<
    /*!
    Compares score of two QueryResult objects. 
    True if qr1 is greater than qr2
    */
    friend bool operator<(const QueryResult& qr1, const QueryResult& qr2);

public:
    //! Constructor
    /*!
    Specifies a empty document with a score of 0.
    */
    QueryResult()
        : item_{ nullptr }, score_{ 0.0 } {};
    //! Constructor
    /*!
    Constructor assigning the document name to member variable "doc" and score to member
    variable "score".
    */
    explicit QueryResult(const IndexItem* item, double score)
        : item_{ item }, score_ { score } {};
    //! Copy Constructor
    /*!
    (Used by std::sort())
    */
    QueryResult(const QueryResult& qr)
        : item_{ qr.item_ }, score_{ qr.score_ } {};
    //! Move Constructor
    QueryResult(QueryResult&& qr)
        : item_{ std::move(qr.item_) }, score_{ std::move(qr.score_) } {};
    ~QueryResult() {};

    //! Overload Operator=
    /*!
    (Used by std::sort())
    Copy Assignment
    */
    QueryResult& operator=(const QueryResult& qr);
    //! Overload Operator=
    /*!
    Move Assignment
    */
    QueryResult& operator=(QueryResult&& qr);
    //! Overload Operator()
    /*!
    Verifies that that QueryResult object isn't an empty QR object.
    */
    explicit operator bool() const;

    const std::string& content() const;

private:
    const static size_t w_prec{ 3 };
    const IndexItem* item_;
    double score_; //!< Document (ranking) Score
};

#endif // !QUERY_RESULT_H