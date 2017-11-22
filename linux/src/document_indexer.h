//!Class Document Indexer
/*!
[Derived Class]
This class acts as kind of the gateway for the program. In a simplified manner, it: 
- Prompts user for indexing and query
- Creates an Index object from a specified file path
- Create Document objects for files specified in Index object, using classes Stopword and Tokenizer
- Populate map with tokens from Documents
- Calculate weights for words of specified documents
- Determine query score per documents using class Query_Result
*/

#ifndef DOCUMENT_INDEXER_H
#define DOCUMENT_INDEXER_H

#include "document.h"
#include "index.h"
#include "indexer.h"
#include <exception>
#include <map>

//! Forward Declarations
class Document;
class Index;

class DocumentIndexer :public Indexer {
    friend class MovieIndexer;

    //! Shorthand for Index Table Display (Term Frequency, tf-idf Weight)
    typedef std::pair<std::vector<int>, std::vector<double>> vec_pair;
    //! Overload operator>>
    /*!
    Mirrors "Indexer::operator<<(Indexer, Document)
    */
    friend DocumentIndexer& operator>>(const Document& d, DocumentIndexer& i);
    //! Overload operator<<
    /*!
    If needed, expands the Indexer object to accommodate another document.
    Iterate and push tokens from Document object onto Indexer i, creating tokens (keys)
    if they don't already exist with a value of zero vectors.
    */
    friend DocumentIndexer& operator<<(DocumentIndexer& i, const Document& d);
    //! Overload operator<<
    /*!
    Creates and adds Document Objects from Index.
    */
    friend DocumentIndexer& operator<<(DocumentIndexer& i, const Index& ix);
    //! Overload operator<<
    /*!
    Sets the Indexer's Stopword object.
    */
    friend DocumentIndexer& operator<<(DocumentIndexer& i, const Stopword& s);
    //! Overload operator<<
    /*!
    Print to an Output Stream Object all relevant information from the Indexer object.
    */
    friend std::ostream& operator<<(std::ostream& os, const DocumentIndexer& i);

public:
    //! Empty Constructor
    DocumentIndexer() {};
    virtual ~DocumentIndexer() {};

    const IndexItem* operator[] (size_t i) const override;

    //! Token Verification Method
    /*!
    Iterate through indexer and verifies if String Token parameter exists within private map "index", where
    the token corresponds to the the maps key.*/
    bool contains(const std::string& s) const override;
    //! Compute Weights
    /*!
    This method utilizes the method weight() to calculate and store the weight of each token from the
    container "index".
    */
    void normalize() override;
    //! Search and Process Query
    /*!
    If the data has been normalized, a search query specified by the user (String s) is tokenized using
    Tokenizer object and stored in a string vector. Using this vector, the method calculates the term 
    frequencies, followed by the its weight in regards to the document weight, and stores the results in
    a QueryResult vector. (See QueryResult class for more details). 

    Depending on the input parameter i (default 10), this method returns the Top i Documents that demonstrate 
    from most to least relevance, which document is more like to contain the search query.
    */
    const std::vector<QueryResult> query(
        const std::string& s, size_t i = 10) const override;
    size_t size() const override { return docs.size(); };

private:
    //! Token(s) Map Container
    /*!
    This map contains all the tokens derived from the documents. 

    It's Key represents tokens as String objects.
    It's Value corresponds to a vector pair <int, double> where the integer value designates the document index
    from where the token exist, while the double value represents its weight in respects to the designated document
    index.
    */
    std::map<std::string, vec_pair> index;
    //!Document Vector Member Variable
    /*!
    Contains list of documents to be indexer by Document_Indexer 
    */
    std::vector<Document> docs;

    //! Member Integer Function
    /*!
    Document Frequency; Used in calculation of Weight.
    */
    int item_freq(const std::string& s) const override;
    //! Member Integer Function
    /*!
    Term Frequency; Used in calculation of Weight.
    */
    int term_freq(const std::string& s, int i) const override;
    //! Member Double Function
    /*!
    Normalized Term Frequency; Used in calculation of Weight.
    */
    double norm_tf(const std::string& s, int i) const override;
    //! Member Double Function
    /*!
    Normalized Document Frequency; Used in calculation of Weight.
    */
    double norm_idf(const std::string& s) const override;
    //! Member Double Function
    /*!
    Weight = (1 + Log (term frequency)) * Log ( # of Documents / Document Frequency) 
    */
    double weight(const std::string& s, int i) const override;
    //! Query Term Frequency 
    /*!
    Get Query Term Frequencies and update the frequency if the token is already pressent in the map q.
    Otherwise, create an new key entry. 
    */
    void query_freqs(std::map<std::string, Indexer::query_pair>& q,
        const std::vector<std::string>& t) const override;
    //! Query and Document Weights
    /*!
    Update the Query and Document weights by iterating through both map parameters. 
    */
    void query_weights(std::map<std::string, Indexer::query_pair>& q,
        std::map<std::string, std::vector<double>>& dw) const override;
    //! Calculate the Cosine Similarity
    /*!
    Create a vector of QueryResult through which the cosine similarity is calculated. Returns 
    the vector of QueryResult containing the match score of each document to the search query.
    */
    const std::vector<QueryResult> cos_similarity(
        const std::map<std::string, Indexer::query_pair>& q,
        const std::map<std::string, std::vector<double>>& dw,
        const std::vector<std::string>& t) const override;
};

#endif // !DOCUMENT_INDEXER_H
