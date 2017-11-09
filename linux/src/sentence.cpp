#include "sentence.h"
#include "word_tokenizer.h"
#include <algorithm>
/*!
\param s1 is a Sentence Object
\param s2 is a Sentence Object
\return Boolean value. True if both s1 and s2 are equal in position and in document name 
*/
bool operator==(const Sentence& s1, const Sentence& s2) {
    return (s1.name_ == s2.name_ && s1.pos_ == s2.pos_);
}
/*!
\param s1 is a Sentence Object
\param s2 is a Sentence Object
\return Boolean value. True if both s1 and s2 not are equal in position and in document name 
*/
bool operator!=(const Sentence& s1, const Sentence& s2) {
    return !(s1 == s2);
}
/*!
\param s1 is a Sentence Object
\param s2 is a Sentence Object
\return Boolean value. True if position of S1 is less than S2 if they're from the same Document else True if name of S1 is less than S2 
*/
bool operator<(const Sentence& s1, const Sentence& s2) {
    if (s1.name_ == s2.name_)
        return s1.pos_ < s2.pos_;

        return s1.name_ < s2.name_;
}
/*!
\param s is a Sentence object
\param str is a String data reference
\return Sentence object s containing str token in its dictionary.
*/
Sentence& operator<<(Sentence& s, const std::string& str) {
    // adds a token to the dictionary if not an empty string
    // increments the count of the token if already present
    if (str.empty())
        return s;

    // update dictionary
    if (!s(str))
        s.dict.insert(std::pair<std::string, std::pair<int, double>>{str, std::make_pair(1, 0.0)});
    else
        ++s.dict.at(str).first;

    return s;
}
/*!
\param os is a Output Stream Object
\param s is a Sentence Object
\return Output Stream Object os containing in its buffer the name and position of Sentence Object s 
*/
std::ostream& operator<<(std::ostream& os, const Sentence& s) {
    os << "[" << s.name() << "] "
        << "[" << s.pos() << "] "
        << s.content_;

    return os;
}
/*!
\param s is a String token
\return Boolean value representing whether or not the map contains the sentence specified. 
*/
bool Sentence::contains(const std::string& s) const {
    for (std::map<std::string, std::pair<int, double>>::const_iterator it{ dict.begin() };
        it != dict.end();
        ++it) {
        if (it->first == s)
            return true;
    }

    return false;
}
const size_t Sentence::size() const {
    // return the number of words in a sentence
    size_t size{ 0 };
    for (std::map<std::string, std::pair<int, double>>::const_iterator it{ dict.begin() };
        it != dict.end();
        ++it)
        size += it->second.first;

    return size;
}
/*!
\param s is a String token
\return Integer occurrence of Token in Sentence
*/
int Sentence::item_freq(const std::string& s) const {
    if (contains(s))
        return 1;

    return 0;
}
/*!
\param s is a String token
\return Integer occurence of Token s in a Sentence dictionary 
*/
int Sentence::term_freq(const std::string& s) const {
    int freq{ 0 };
    if (contains(s))
        freq = dict.at(s).first;

    return freq;
}
/*!
\param s is a String token
\return Double value corresponding to the normalized term frequency of a token s in Sentence 
*/
double Sentence::norm_tf(const std::string& s) const {
    if (contains(s))
        return std::max((1 + log(static_cast<double>(term_freq(s)))), 0.0);

    return 0;
}
/*!
\param s is a String token
\return Double value corresponding to the normalized document frequency of a token 
*/
double Sentence::norm_idf(const std::string& s) const {
    if (contains(s))
        return log(1 / static_cast<double>(item_freq(s)));

    return 0;
}
/*!
\param s is a String token
\return Double value corresponding to the weight of a token in Sentence 
*/
double Sentence::weight(const std::string& s) const {
    if (contains(s))
        return norm_tf(s) * norm_idf(s);

    return 0;
}

void Sentence::normalize() {
    // computes token weights
    for (std::map<std::string, std::pair<int, double>>::iterator m_it{ dict.begin() };
        m_it != dict.end();
        ++m_it)
        m_it->second.second = weight(m_it->first);
}

void Sentence::init() {
    // tokenize the sentence words
    std::stringstream ss{ content_ };
    WordTokenizer t;
    const std::vector<std::string> tokens = t.tokenize(ss);

    // add the tokens to the dict
    for (std::vector<std::string>::const_iterator it{ tokens.begin() };
        it != tokens.end();
        ++it)
        *this << *it;
}