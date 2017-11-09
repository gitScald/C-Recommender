//! Stopword Class
/*!
An object of this class is used for stopword filtering. It utilized an external file, whose filepath
is specified (OPTIONAL).
*/
#ifndef STOPWORD_H
#define STOPWORD_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Stopword {
    //! Overload Operator<<
    /*!
    Output Stopwords and Stopword Filepath using Output Stream
    */
    friend std::ostream& operator<<(std::ostream& os, const Stopword& s);

public:
    //! Constructor
    /*!
    Creates a Stopword object with no stopwords specified in vector "stopwords".
    */
    Stopword() {};
    //! Constructor
    /*!
    Utilizing the parameter and an Input Stream (File) Object, the method reads
    and extrapulates the stopwords from the specified filepath, if the stopword filepath 
    is correct. For each string token, it is pushed into the vector "stopwords".
    */
    explicit Stopword(const std::string& stopw_fp);
    //! Overloaded Operator()
    /*!
    This overloaded operator is called to verify if a specified input string token s
    exist in the vector "stopwords".
    */
    bool operator() (const std::string& s) const;
    //! Accessor Method
    /*!
    Returns the value of member variable "name_". This variable is specified when the explicit
    constructor is called.
    */
    const std::string& name() const { return name_; };

private:
    std::string name_;//!< Stopword Filepath
    std::vector<std::string> stopwords; //!< Vector of Stopword String Tokens
};

#endif // !STOPWORD_H