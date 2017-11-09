//#include "document_indexer.h"
//
//int main(int argc, char* argv[]) {
//    std::cout << "***  START INDEXER PROGRAM  ***" << std::endl;
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
//    std::cout << "Press [Enter] to display unfiltered index: ";
//    std::cin.get();
//    std::cout << unfil << std::endl << "***  END UNFILTERED INDEX  ***" << std::endl;
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
//    std::cout << "Press [Enter] to display filtered index: ";
//    std::cin.get();
//    std::cout << fil << std::endl << "***  END FILTERED INDEX  ***" << std::endl;
//
//    std::cout << "***  END INDEXER PROGRAM  ***" << std::endl;
//
//    return 0;
//}