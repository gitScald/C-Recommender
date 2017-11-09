//! StringOps Class
/*!
Class handles various string operations performed throughout the system.
*/

#ifndef STRINGOPS_H
#define STRINGOPS_H

#include <algorithm>
#include <string>

//! String to Lowercase
/*!
Converts String to Lowercase using String method.

\param s is a String parameter
\return String s with lowercase format.
*/
inline std::string& lower(std::string& s) {
    // converts a string to lowercase
    std::transform(s.begin(), s.end(), s.begin(), tolower);

    return s;
}

//! Remove unwanted characters
/*!
Removes all unwanted characters based in parameters

\param s is a String s token, before the cleanse.
\param bad_chars is a String containing characters as a template
\return String s with characters from bad_chars removed. 
*/
inline std::string& strip(std::string& s, const std::string& bad_chars=std::string()) {
    // strips a string of all forbidden characters
    // by default, removes all punctuation
    if (bad_chars.empty())
        s.erase(std::remove_if(s.begin(), s.end(), ispunct), s.end());

    else {
        for (std::string::const_iterator it{ bad_chars.begin() };
            it != bad_chars.end();
            ++it)
            s.erase(std::remove(s.begin(), s.end(), *it), s.end());
    }

    return s;
}

#endif // !STRINGOPS_H