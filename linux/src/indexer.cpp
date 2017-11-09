#include "indexer.h"
#include <sstream>
/*!
\param w is a Size_T value corresponding to size of horizontal separator 
\return String value corresponding to generated horizontal seperator 
*/
const std::string Indexer::h_separator(size_t w) {
    // return a horizontal separator for indexer output
    std::stringstream buffer;
    for (size_t i{ 0 }; i != w; ++i)
        buffer << "-";

    return buffer.str();
}
/*!
\param w is a Size_T value corresponding to size of vertical separator
\return String value corresponding to generated vertical seperator
*/
const std::string Indexer::v_separator(size_t w) {
    // return a vertical separator for indexer output
    std::stringstream buffer;
    for (size_t i{ 0 }; i != w; ++i)
        buffer << " ";

    return buffer.str();
}