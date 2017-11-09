//!SentenceIndexer class
/*!
[DERIVED CLASS] This class acts as kind of the gateway for the program. In a simplified manner, it:
- Prompts user for indexing and query
- Creates an Index object from a specified file path
- Create Document objects for files specified in Index object, using classes SentneceTokenizer
- Populate map with sentence tokens from Documents
- Calculate weights for sentence of specified documents
- Determine query score per sentence using class Query_Result

\author Benjamin Vial	29590765
\author Mandeep Tahim   26935478
\author Daniel Carrera  26729886

\note COMP 345 Assignment 3 - Fall 2017

*/
#ifndef SENTENCE_INDEXER_H
#define SENTENCE_INDEXER_H

#include "document.h"
#include "indexer.h"
#include "sentence.h"

class SentenceIndexer :public Indexer {
    //!Overload operator<<. 
    /*!
    If needed, expands the Indexer object to accommodate another document. Iterate and push sentence tokens from Document object onto SentenceIndexer i, creating tokens (keys) if they don't already exist with a value of zero vectors.
    */
    friend SentenceIndexer& operator<<(SentenceIndexer& i, const Document& d);
    //!Overload operator<<. 
    /*!
    Creates and adds Sentence Objects from Index.
    */
    friend SentenceIndexer& operator<<(SentenceIndexer& i, const Index& ix);
    //!Overload operator<<. 
    /*!
    Print to an Output Stream Object all relevant information from the SentenceIndexer object.
    */
    friend std::ostream& operator<<(std::ostream& os, const SentenceIndexer& i);

public:
    //!Empty Constructor
    /*!
    Empty derived constructor referencing base class Indexer
    */
    SentenceIndexer()
        : Indexer() {};
    //!Constructor
    /*!
    Constructor for object SentenceIndexer. Storing the filepath to the abbreviation file.. The constructor makes rerence to the base class Indexer.
    */
    SentenceIndexer(const std::string& abbrevs_fp)
        : Indexer(), tokenizer_fp{ abbrevs_fp } {};
    //!Virtual Deconstructor
    virtual ~SentenceIndexer() {};

    const IndexItem* operator[] (size_t i) const override;
    //!Contains Method. 
    /*!
    Member Function verifies String token exist in Sentence Index
    */
    bool contains(const std::string& s) const override;
    //!Contains Method. 
    /*!
    Member Function verifies Sentence Object exist in Sentence Index
    */
    bool contains(const Sentence& s) const;

    void normalize() override;
    //!Search and Process Query. 
    /*!
    If the data has been normalized, a search query specified by the user (String s) is tokenized using Tokenizer object and stored in a string vector. Using this vector, the method calculates the term frequencies, followed by the its weight in regards to the document weight, and stores the results in a QueryResult vector. (See QueryResult class for more details).
    Depending on the input parameter i (default 500), this method returns sentences from most to least relevance, creating an essay of word count less than i.
    */
    const std::vector<QueryResult> query(
        const std::string& s, size_t i = 500) const override;
    //!Accessor Method. 
    /*!
    Returns size of vector documents docs
    */
    size_t size() const override { return docs.size(); };

private:
    //!Member String Variable. 
    /*!
    Contains filepath of the tokenizer
    */
    std::string tokenizer_fp;
    //!Member Vector Document Variable. 
    /*!
    Contains Vector of Documents that are parsed through the Indexer
    */
    std::vector<Document> docs;
    //!Member Vector Sentence Variable. 
    /*!
    Contains Vector of Sentencces derived from Documents
    */
    std::vector<Sentence> sentences;
    //!Token(s) Map Container. 
    /*!
    This map contains all the Sentence objects derived from the documents.
    It's Key represents tokens as Sentence objects. It's Value corresponds to a double where the value designates the weight
    */
    std::map<Sentence, double> index;
    //!Member Integer Function.
    /*!
    Document Frequency; Used in calculation of Weight.
    */
    int item_freq(const std::string& s) const override;
    //!Member Integer Function. 
    /*!
    Term Frequency; Used in calculation of Weight.
    */
    int term_freq(const std::string& s, int i) const override;
    //!Member Double Function. 
    /*!
    Normalized Term Frequency; Used in calculation of Weight.
    */
    double norm_tf(const std::string& s, int i) const override;
    //!Member Double Function. 
    /*!
    Normalized Document Frequency; Used in calculation of Weight.
    */
    double norm_idf(const std::string& s) const override;
    //!Member Double Function.
    /*!
    Weight = (1 + Log (term frequency)) * Log ( # of Documents / Document Frequency)
    */
    double weight(const std::string& s, int i) const override;
    //!Query Term Frequency. 
    /*!
    Get Query Term Frequencies and update the frequency if the token is already pressent in the map q. Otherwise, create an new key entry.
    */
    void query_freqs(std::map<std::string, Indexer::query_pair>& q,
        const std::vector<std::string>& t) const override;
    //!Query and Document Weights. 
    /*!
    Update the Query and Document weights by iterating through both map parameters.
    */
    void query_weights(std::map<std::string, Indexer::query_pair>& q,
        std::map<std::string, std::vector<double>>& sw) const override;
    //!Calculate the Cosine Similarity. 
    /*!
    Create a vector of QueryResult through which the cosine similarity is calculated. Returns the vector of QueryResult containing the match score of each document to the search query.
    */
    const std::vector<QueryResult> cos_similarity(
        const std::map<std::string, Indexer::query_pair>& q,
        const std::map<std::string, std::vector<double>>& sw,
        const std::vector<std::string>& t) const override;

};

#endif // !SENTENCE_INDEXER_H
