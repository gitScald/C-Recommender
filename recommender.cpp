#include "movie_indexer.h"

int main(int argc, char* argv[]) {
    std::cout << "***  START RECOMMENDER PROGRAM  ***" << std::endl;

    // Windows path: files/*.txt
    const std::string data_fp{ "MovieSummaries/movie.metadata.tsv" };
    const std::string summary_fp{ "MovieSummaries/plot_summaries.txt" };

    // Linux path: ../files/*.txt
    //const std::string index_fp{ "../MovieSummaries/movie.metadata.tsv" };
    //const std::string stopw_fp{ "../MovieSummaries/plot_summaries.txt" };

    MovieIndexer{ data_fp, summary_fp };

    //DocumentIndexer fil;
    //DocumentIndexer unfil;

    //std::cout << "Generating unfiltered index...";
    //unfil << ix;
    //std::cout << " done." << std::endl;

    //std::cout << "Normalizing unfiltered index...";
    //unfil.normalize();
    //std::cout << " done." << std::endl;

    //std::cout << "Generating filtered index...";
    //fil << ix;
    //fil << s;
    //std::cout << " done." << std::endl;

    //std::cout << "Normalizing filtered index...";
    //fil.normalize();
    //std::cout << " done." << std::endl;

    //const std::string h_separator{ Indexer::h_separator(80) };
    //std::cout << "Type in a query to search through (normalized) unfiltered index: ";
    //std::string query;
    //std::vector<QueryResult> results;
    //std::getline(std::cin, query);
    //while (query != std::string()) {
    //    std::cout << std::endl;
    //    results = unfil.query(query);
    //    std::cout << std::endl;

    //    std::cout << "QUERY: " << query << std::endl;
    //    for (size_t i{ 0 }; i != results.size(); ++i)
    //        if (results.at(i))
    //            std::cout << " #" << (i + 1) << ": " << results.at(i) << std::endl;

    //    std::cout << "Type in another query to search through unfiltered index: ";
    //    std::getline(std::cin, query);
    //}

    //std::cout << "Adding another document to the unfiltered index...";
    //unfil << Document(extra_fp);
    //std::cout << " done." << std::endl;

    //std::cout << "Type in a query to search through (not normalized) unfiltered index: ";

    //// clear std::cin to ignore the last [Enter] key press
    //std::cin.clear();
    //std::getline(std::cin, query);
    //try {
    //    std::cout << "QUERY: " << query << std::endl << h_separator << std::endl;
    //    results = unfil.query(query);
    //}
    //catch (IndexException& e) {
    //    std::cout << "Exception thrown: " << e.what() << std::endl;
    //}
    //// finally blocks not supported by C++ :(

    std::cout << "***  END RECOMMENDER PROGRAM  ***" << std::endl;
}