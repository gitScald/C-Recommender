//!Indexer class
/*!
[BASE CLASS]
This class handles parsing through a list of Documents and creating specified IndexItems for each said Documents. 
The weight and score for each IndexItem is calculated and stored within in a map which will be used to compare with a Input Query 
specified by end user in order to generate results, based on the cosine similarity of the Query and IndexItem Map.
*/

#ifndef INDEXER_H
#define INDEXER_H

#include "index_item.h"
#include "query_result.h"
#include "stopword.h"
#include <exception>
#include <map>

class Indexer {
    // just to be able to print out a horizontal separator
    friend int main(int argc, char* argv[]);
	
	friend std::ostream& operator<<(std::ostream& os, const DocumentIndexer& i); 
 

public:
    typedef std::pair<int, double> query_pair;
    //!Empty Constructor
    Indexer() {};
    //! Virtual Deconstructor
    virtual ~Indexer() {};
    virtual const IndexItem* operator[] (size_t i) const = 0;
    //!Override operator() 
    /*!
    Virtual Function. Override operator(). Return value from member function contains
    */
    virtual bool operator() (const std::string& s) const { return contains(s); };
    //!Boolean Function Contains
    /*!
    Pure Virtual Function.
    Return true if dictionary contains string input parameter
    */
    virtual bool contains(const std::string& s) const = 0;
    //!Search and Process Query. 
    /*!
    Virtual Function. If the data has been normalized, a search query specified by the user (String s) is tokenized using Tokenizer object and stored in a string vector. Using this vector, the method calculates the term frequencies, followed by the its weight in regards to the document weight, and stores the results in a QueryResult vector. (See QueryResult class for more details).
    Depending on the input parameter i (default 10), this method returns the top i results.
    */
    virtual const std::vector<QueryResult> query(
        const std::string& s, size_t i = 10) const = 0;
    //!Compute Weights. 
    /*!
    Pure Virtual Function. This method utilizes the method weight() to calculate and store the weight of each token.
    */
    virtual void normalize() = 0;
    //!Accessor Method. 
    /*!
    Pure Virtual Function. Returns size of a data member.
    */
    virtual size_t size() const = 0;

protected:
    //!Stopword Object. 
    /*!
    Contains list of stopwords that shouldn't be included in Index
    */
    Stopword stopword;
    //!Size_T Member Variable. 
    /*!
    Equal to longest IndexItem
    */
    size_t longest{ 0 };
    //!Boolean Member Variable
    /*!
    Boolean Data Member to keep track whether Indexer has been normalized with the addition of new Documents 
    */
    bool normalized{ false };

    static const std::string h_separator(size_t w);
    static const std::string v_separator(size_t w);
    //!Member Integer Function
    /*!
    Pure Virtual. Item Frequency; Used in calculation of Weight.
    */
    virtual int item_freq(const std::string& s) const = 0;
    //!Member Integer Function. 
    /*!
    Pure Virtual. Term Frequency; Used in calculation of Weight.
    */
    virtual int term_freq(const std::string& s, int i) const = 0;
    //!Member Double Function. 
    /*!
    Pure Virtual. Normalized Term Frequency; Used in calculation of Weight.
    */
    virtual double norm_tf(const std::string& s, int i) const = 0;
    //!Member Double Function.
    /*!
    Pure Virtual. Normalized Document Frequency; Used in calculation of Weight.
    */
    virtual double norm_idf(const std::string& s) const = 0;
    //!Member Double Function. 
    /*!
    Pure Virtual. Weight = (1 + Log (term frequency)) * Log ( # of Documents / Document Frequency)
    */
    virtual double weight(const std::string& s, int i) const = 0;
    //!Query Term Frequency. 
    /*!
    Pure Virtual. Get Query Term Frequencies and update the frequency if the token is already pressent in the map q. Otherwise, create an new key entry.
    */
    virtual void query_freqs(std::map<std::string, Indexer::query_pair>& q,
        const std::vector<std::string>& t) const = 0;
    //!Query and Document Weights. 
    /*!
    Pure Virtual. Update the Query and Document weights by iterating through both map parameters.
    */
    virtual void query_weights(std::map<std::string, Indexer::query_pair>& q,
        std::map<std::string, std::vector<double>>& dw) const = 0;
    //!Calculate the Cosine Similarity. 
    /*!
    Pure Virtual. 
    Create a vector of QueryResult through which the cosine similarity is calculated. 
    Returns the vector of QueryResult containing the match score of each document to the search query.
    */
    virtual const std::vector<QueryResult> cos_similarity(
        const std::map<std::string, Indexer::query_pair>& q,
        const std::map<std::string, std::vector<double>>& dw,
        const std::vector<std::string>& t) const = 0;
};

class IndexException :public std::exception {
public:
    IndexException(const std::string& what)
        : what_{ what } {};
    const char* what() const noexcept override { return what_.c_str(); }

private:
    std::string what_;
};

#endif // !INDEXER_H