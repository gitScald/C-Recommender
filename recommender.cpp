#include "movie_indexer.h"

int main(int argc, char* argv[]) {
    std::cout << "***  START RECOMMENDER PROGRAM  ***" << std::endl;

    // Windows path: path/*
    const std::string data_fp{ "MovieSummaries/movie.metadata.tsv" };
    const std::string summary_fp{ "MovieSummaries/plot_summaries.txt" };

    // Linux path: ../path/*
    //const std::string index_fp{ "../MovieSummaries/movie.metadata.tsv" };
    //const std::string stopw_fp{ "../MovieSummaries/plot_summaries.txt" };

    std::cout << "Generating movie index..." << std::endl;
    MovieIndexer mov{ data_fp, summary_fp };

    std::cout << "Normalizing movie index...";
    mov.normalize();
    std::cout << " done.";

    const std::string h_separator{ Indexer::h_separator(80) };
    std::cout << "Type in a movie title to get recommendations: ";
    std::string query;
    std::vector<QueryResult> results;
    std::getline(std::cin, query);
    while (query != std::string()) {
        std::cout << std::endl;
        try {
            results = mov.query(query);
            std::cout << std::endl;

            // print out top 5 (by default) recommendations
            std::cout << "BECAUSE YOU WATCHED: " << query << std::endl;
            for (size_t i{ 0 }; i != results.size(); ++i)
                if (results.at(i)) {
                    std::cout << " #" << (i + 1) << ": " << results.at(i) << std::endl
                        << h_separator << std::endl << std::endl
                        << "Press [Enter] to display the next recommendation: ";
                    std::cin.get();
                }
        }

        // catch potential index exceptions
        catch (IndexException& e) {
            std::cout << "An error occured: " << e.what() << std::endl;
        }

        // clear std::cin to ignore the last [Enter] key press
        std::cin.clear();
        std::cout << "Type in another movie title to get more recommendations: ";
        std::getline(std::cin, query);
    }

    std::cout << "***  END RECOMMENDER PROGRAM  ***" << std::endl;
}