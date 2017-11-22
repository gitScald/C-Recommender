//!SentenceIndexer class
/*!
[DERIVED CLASS] This class acts as kind of the gateway for the program. In a simplified manner, it:
- Prompts user for indexing and query
- Creates an Index object from a specified file path
- Create Movie objects for movies specified in Index object.
- Calculate weights for sentence of specified Movies based on their summary
- Determine query score per movie using class Query_Result

\author Benjamin Vial	29590765
\author Mandeep Tahim   26935478
\author Daniel Carrera  26729886

\note COMP 345 Assignment 4 - Fall 2017

*/
#ifndef MOVIE_INDEXER_H
#define MOVIE_INDEXER_H

//#include "document_indexer.h"
#include "movie.h"
#include <unordered_map>

class MovieIndexer :public Indexer {
    typedef std::pair<std::vector<int>, std::vector<double>> vec_pair;

    //!Overload operator<<. 
    /*!
    Print to an Output Stream Object all relevant information from the MovieIndexer object.
    */
    friend std::ostream& operator<<(std::ostream& os, const MovieIndexer& mi);

public:
    //!Empty Constructor
    /*!
    Empty derived constructor referencing base class Indexer
    */
    MovieIndexer() {}
    //!Constructor
    /*!
    Constructor for object SentenceIndexer. Storing the filepath to the list of movies (data_fp) and their summary (summary fp). The constructor makes rerence to the base class Indexer.
    */
    explicit MovieIndexer(const std::string& data_fp,
        const std::string& summary_fp)
        : data_fp_{ data_fp }, summary_fp_{ summary_fp } { init(); }
    //!Destructor
    /*!
    Virtual destructor
    */
    virtual ~MovieIndexer() {};

    const IndexItem* operator[] (size_t i) const override;
    //!Retrieve Method
    /*!
    Retrieves summary (content_) from Vector<Movie> movies based on title (input s)
    */
    std::string summary(const std::string & s) const;
    //!Contains Method. 
    /*!
    Member Function verifies String token (title) exist in stl container movies
    */
    bool contains(const std::string& s) const override;
    //!Compute Weights. 
    /*!
    This method utilizes the method weight() to calculate and store the weight of each token.
    */
    void normalize() override;
    //!Search and Process Query. 
    /*!
    If the data has been normalized, a search query specified by the user (String s) is tokenized using Tokenizer object and stored in a string vector. Using this vector, the method calculates the term frequencies, followed by the its weight in regards to the document weight, and stores the results in a QueryResult vector. (See QueryResult class for more details).
    Depending on the input parameter i (default 5), this method returns movies from most to least relevance.
    */
    const std::vector<QueryResult> query(
        const std::string& s, size_t i = 5) const override;
    //!Accessor Method. 
    /*!
    Returns size of vector Movie movies
    */
    size_t size() const override { return movies.size(); };

private:
    //!Member String Variable. 
    /*!
    Contains filepath of the metadata
    */
    std::string data_fp_;
    //!Member String Variable. 
    /*!
    Contains filepath of the movie summary
    */
    std::string summary_fp_;
    //!Member Vector Movie Variable. 
    /*!
    Contains Vector of Movie that are parsed through the Indexer
    */
    std::vector<Movie> movies;
    //!Member DocumentIndexer variable. 
    /*!
    To avoid redundancy of code, methods from DocumentIndexer are used for reference.
    */
    //DocumentIndexer index;

    //!Member Boolean Function
    /*!
    Returns true if token s is contained within any Ddocument docs
    */
    bool in_docs(const std::string& s) const;
    //!Member Vector Document Variable. 
    /*!
    Contains Vector of Document that are parsed through the DocumentIndexer
    */
    std::vector<Document> docs;


    //! Token UnorderedMap Container
    /*!
    This map contains all the tokens derived from the documents.

    It's Key represents tokens as String objects.
    It's Value corresponds to a vector pair <int, double> where the integer value designates the document index
    from where the token exist, while the double value represents its weight in respects to the designated document
    index.
    */
    std::unordered_map<std::string, vec_pair> index;

    //!Void Member Function
    /*!
    Stores document tokens in Unordered Map index. If the token is not already in the within the map, a new entry is created
    */
    void index_docs();
    //!Void Member Function
    /*!
    Initilize Movie_Indexer by:
    - Fetching and Tokenizing Movie Summary using summary_fp_
    - Fetching and Tokenizing Movie Metadata using data_fp_
    - Store Movie Document objects into index
    */
    void init();
    //!Void Member Function
    /*!
    Process and Tokenize metadata using the data_fp_ variable.
    */
    void tokenize_data(std::stringstream& ss);
    //!Void Member Function
    /*!
    Process and Tokenize summary data using the summary_fp_ variable.
    */
    void tokenize_summary(std::stringstream& ss);
    //!Member Integer Function.
    /*!
    Movie Frequency; Used in calculation of Weight. Refers to DocumentIndexer method of same name.
    */
    int item_freq(const std::string& s) const override;
    //!Member Integer Function. 
    /*!
    Term Frequency; Used in calculation of Weight. Refers to DocumentIndexer method of same name.
    */
    int term_freq(const std::string& s, int i) const override;
    //!Member Double Function. 
    /*!
    Normalized Term Frequency; Used in calculation of Weight. Refers to DocumentIndexer method of same name.
    */
    double norm_tf(const std::string& s, int i) const override;
    //!Member Double Function. 
    /*!
    Normalized Document Frequency; Used in calculation of Weight. Refers to DocumentIndexer method of same name.
    */
    double norm_idf(const std::string& s) const override;
    //!Member Double Function.
    /*!
    Weight = (1 + Log (term frequency)) * Log ( # of Documents / Document Frequency). Refers to DocumentIndexer method of same name.
    */
    double weight(const std::string& s, int i) const override;
    //!Query Term Frequency. 
    /*!
    Get Query Term Frequencies and update the frequency if the token is already pressent in the map q. Otherwise, create an new key entry. Refers to DocumentIndexer method of same name.
    */
    void query_freqs(std::map<std::string, Indexer::query_pair>& q,
        const std::vector<std::string>& t) const override;
    //!Query and Document Weights. 
    /*!
    Update the Query and Document weights by iterating through both map parameters. Refers to DocumentIndexer method of same name.
    */
    void query_weights(std::map<std::string, Indexer::query_pair>& q,
        std::map<std::string, std::vector<double>>& dw) const override;
    //!Calculate the Cosine Similarity. 
    /*!
    Create a vector of QueryResult through which the cosine similarity is calculated. Returns the vector of QueryResult.
    */
    const std::vector<QueryResult> cos_similarity(
        const std::map<std::string, Indexer::query_pair>& q,
        const std::map<std::string, std::vector<double>>& dw,
        const std::vector<std::string>& t) const override;
};

#endif // !MOVIE_INDEXER_H