#include "sentence_indexer.h"

int main(int argc, char* argv[]) {
   std::cout << "***  START SUMMARIZER PROGRAM  ***" << std::endl;

   // Windows path: files/*.txt
   // const std::string q1_fp{ "a3data/q1.txt" };
   // const std::string q2_fp{ "a3data/q2.txt" };
   // const std::string q3_fp{ "a3data/q3.txt" };
   // const std::string index_q1{ "a3data/q1docs/index_windows.txt" };
   // const std::string index_q2{ "a3data/q2docs/index_windows.txt" };
   // const std::string index_q3{ "a3data/q3docs/index_windows.txt" };
   // const std::string abbrevs_fp{ "files/abbreviations.txt" };

   // Linux path: ../files/*.txt
   const std::string q1_fp{ "../a3data/q1.txt" };
   const std::string q2_fp{ "../a3data/q2.txt" };
   const std::string q3_fp{ "../a3data/q3.txt" };
   const std::string index_q1{ "../a3data/q1docs/index.txt" };
   const std::string index_q2{ "../a3data/q2docs/index.txt" };
   const std::string index_q3{ "../a3data/q3docs/index.txt" };
   const std::string abbrevs_fp{ "../files/abbreviations.txt" };

   std::cout << "Processing index file(s)...";
   Index ix1{ index_q1 };
   Index ix2{ index_q2 };
   Index ix3{ index_q3 };
   std::cout << " done." << std::endl;

   SentenceIndexer sen1{ abbrevs_fp };
   SentenceIndexer sen2{ abbrevs_fp };
   SentenceIndexer sen3{ abbrevs_fp };

   std::cout << "Generating sentence index(es)...";
   sen1 << ix1;
   sen2 << ix2;
   sen3 << ix3;
   std::cout << " done." << std::endl;

   std::cout << "Normalizing sentence index(es)...";
   sen1.normalize();
   sen2.normalize();
   sen3.normalize();
   std::cout << " done." << std::endl;

   std::string q1, q2, q3, s;

   std::cout << "Processing question(s)...";
   // question 1
   std::ifstream ifs1{ q1_fp };
   if (!ifs1)
       std::cerr << "WARNING: Could not establish input stream with '"
       << q1_fp << "'" << std::endl;
   else {
       std::getline(ifs1, s);
       while (s != std::string()) {
           q1 += s;
           std::getline(ifs1, s);
       }
   }
   ifs1.close();

   // question 2
   std::ifstream ifs2{ q2_fp };
   if (!ifs2)
       std::cerr << "WARNING: Could not establish input stream with '"
       << q2_fp << "'" << std::endl;
   else {
       std::getline(ifs2, s);
       while (s != std::string()) {
           q2 += s;
           std::getline(ifs2, s);
       }
   }
   ifs2.close();

   // question 3
   std::ifstream ifs3{ q3_fp };
   if (!ifs3)
       std::cerr << "WARNING: Could not establish input stream with '"
       << q3_fp << "'" << std::endl;
   else {
       std::getline(ifs3, s);
       while (s != std::string()) {
           q3 += s;
           std::getline(ifs3, s);
       }
   }
   ifs3.close();
   std::cout << " done." << std::endl;

   std::cout << "Generating query results...";
   size_t length{ 0 };
   const size_t length_max{ 500 };
   std::vector<QueryResult> results1, results2, results3;
   results1 = sen1.query(q1, length_max);
   results2 = sen2.query(q2, length_max);
   results3 = sen3.query(q3, length_max);
   std::cout << " done." << std::endl;

   std::cout << std::endl;
   const std::string h_separator{ Indexer::h_separator(80) };

   // question 1
   std::cout << "Press [Enter] to display results to question 1: ";
   std::cin.get();
   std::cout << std::endl << h_separator << "QUESTION 1: " << q1 << std::endl << h_separator << std::endl;
   for (std::vector<QueryResult>::const_iterator it{ results1.begin() };
       it != results1.end();
       ++it) {
       // cast the IndexItem* into a Sentence*
       const Sentence* s = dynamic_cast<const Sentence*>(it->item_);
       if (s) {
           // stop when we reach the word limit
           if ((length + s->size()) < length_max) {
               std::cout << s->content() << " ";
               length += s->size();
           }
           else
               break;
       }
   }
   std::cout << std::endl << std::endl
       << "***  END QUESTION 1 (" << length << " WORDS)  ***"
       << std::endl << h_separator << std::endl << std::endl;

   // question 2
   length = 0;
   std::cout << "Press [Enter] to display results to question 2: ";
   std::cin.get();
   std::cout << std::endl << h_separator << "QUESTION 2: " << q2 << std::endl << h_separator << std::endl;
   for (std::vector<QueryResult>::const_iterator it{ results2.begin() };
       it != results2.end();
       ++it) {
       // cast the IndexItem* into a Sentence*
       const Sentence* s = dynamic_cast<const Sentence*>(it->item_);
       if (s) {
           // stop when we reach the word limit
           if ((length + s->size()) < length_max) {
               std::cout << s->content() << " ";
               length += s->size();
           }
           else
               break;
       }
   }
   std::cout << std::endl << std::endl
       << "***  END QUESTION 2 (" << length << " WORDS)  ***"
       << std::endl << h_separator << std::endl << std::endl;

   // question 3
   length = 0;
   std::cout << "Press [Enter] to display results to question 3: ";
   std::cin.get();
   std::cout << std::endl << h_separator << "QUESTION 3: " << q3 << std::endl << h_separator << std::endl;
   for (std::vector<QueryResult>::const_iterator it{ results3.begin() };
       it != results3.end();
       ++it) {
       // cast the IndexItem* into a Sentence*
       const Sentence* s = dynamic_cast<const Sentence*>(it->item_);
       if (s) {
           // stop when we reach the word limit
           if ((length + s->size()) < length_max) {
               std::cout << s->content() << " ";
               length += s->size();
           }
           else
               break;
       }
   }
   std::cout << std::endl << std::endl
       << "***  END QUESTION 3 (" << length << " WORDS)  ***"
       << std::endl << h_separator << std::endl << std::endl;

   std::cout << "***  END SUMMARIZER PROGRAM  ***" << std::endl;

   return 0;
}