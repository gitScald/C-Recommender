//#include "document_indexer.h"
//
//int main(int argc, char* argv[]) {
//    std::cout << "***  START GOOGLER PROGRAM  ***" << std::endl;
//
//    // Windows path: files/*.txt
//    //const std::string index_fp{ "files/index_windows.txt" };
//    //const std::string stopw_fp{ "files/stopwords.txt" };
//    //const std::string extra_fp{ "files/extra_doc.txt" };
//
//    // Linux path: ../files/*.txt
//    const std::string index_fp{ "../files/index.txt" };
//    const std::string stopw_fp{ "../files/stopwords.txt" };
//    const std::string extra_fp{ "../files/extra_doc.txt" };
//
//    std::cout << "Processing index file '" << index_fp << "'...";
//    Index ix{ index_fp };
//    std::cout << " done." << std::endl;
//
//    std::cout << "Processing stopwords file '" << stopw_fp << "'...";
//    Stopword s{ stopw_fp };
//    std::cout << " done." << std::endl;
//
//    DocumentIndexer fil;
//    DocumentIndexer unfil;
//
//    std::cout << "Generating unfiltered index...";
//    unfil << ix;
//    std::cout << " done." << std::endl;
//
//    std::cout << "Normalizing unfiltered index...";
//    unfil.normalize();
//    std::cout << " done." << std::endl;
//
//    std::cout << "Generating filtered index...";
//    fil << ix;
//    fil << s;
//    std::cout << " done." << std::endl;
//
//    std::cout << "Normalizing filtered index...";
//    fil.normalize();
//    std::cout << " done." << std::endl;
//
//    const std::string h_separator{ Indexer::h_separator(80) };
//    std::cout << "Type in a query to search through (normalized) unfiltered index: ";
//    std::string query;
//    std::vector<QueryResult> results;
//    std::getline(std::cin, query);
//    while (query != std::string()) {
//        std::cout << std::endl;
//        results = unfil.query(query);
//        std::cout << std::endl;
//
//        std::cout << "QUERY: " << query << std::endl;
//        for (size_t i{ 0 }; i != results.size(); ++i)
//            if (results.at(i))
//                std::cout << " #" << (i + 1) << ": " << results.at(i) << std::endl;
//
//        std::cout << "Type in another query to search through unfiltered index: ";
//        std::getline(std::cin, query);
//    }
//
//    std::cout << "Adding another document to the unfiltered index...";
//    unfil << Document(extra_fp);
//    std::cout << " done." << std::endl;
//
//    std::cout << "Type in a query to search through (not normalized) unfiltered index: ";
//
//    // clear std::cin to ignore the last [Enter] key press
//    std::cin.clear();
//    std::getline(std::cin, query);
//    try {
//        std::cout << "QUERY: " << query << std::endl << h_separator << std::endl;
//        results = unfil.query(query);
//    }
//    catch (IndexerExcept& e) {
//        std::cout << "Exception thrown: " << e.what() << std::endl
//            << "***  END GOOGLER PROGRAM  ***" << std::endl;
//    }
//    // finally blocks not supported by C++ :(
//}