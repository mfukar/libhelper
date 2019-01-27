#include "pch.h"

#include "../libhelper/aho-corasick.h"

int main (int argc, char *argv[]) {
    std::ifstream ifs ("tags.txt");
    std::vector<std::string> tags;
    std::string line;
    while (std::getline (ifs, line)) {
        tags.emplace_back (line);
    }
    std::ifstream mfs ("messages.txt");
    std::vector<std::string> messages;
    while (std::getline (mfs, line)) {
        messages.emplace_back (line);
    }

    auto preprocess_start = std::chrono::high_resolution_clock::now ();
    struct ac_trie trie;
    ac_trie_init (&trie);
    for (const std::string & tag : tags) {
        if (!ac_trie_insert (&trie, tag.c_str ())) {
            std::cerr << "ac_trie_insert crapped out at " << tag << std::endl;
        }
    }
    ac_build_failure_function (&trie);
    auto preprocess_end = std::chrono::high_resolution_clock::now ();

    /* Micro: */
    size_t nmatches = 0;
    auto time_start = std::chrono::high_resolution_clock::now ();
    for (const auto message : messages) {
        struct ac_result res = ac_search (&trie, message.c_str(), 4, 0);
        if (ac_search_matched (res)) ++nmatches;
    }
    auto time_end = std::chrono::high_resolution_clock::now ();
    auto elapsed_time = std::chrono::duration<double, std::milli> (time_end - time_start).count ();
    std::cout << "Preprocessing time: "
        << std::chrono::duration<double, std::milli> (preprocess_end - preprocess_start).count ()
        << " ms" << std::endl
        << "Matching time     : " << elapsed_time
        << " ms for " << messages.size() / elapsed_time /  1000 << " Mmps" << std::endl;

    std::cout << "Matches           : " << nmatches << " out of " << messages.size() << std::endl;
    return 0;
}