#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

int main() {
    std::ifstream file("words.txt");
    std::string line;
    std::map<std::string, int> wordCount;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string word;
        while (ss >> word) {
            wordCount[word]++;
        }
    }

    for (const auto& pair : wordCount) {
        std::cout << pair.first << " : " << pair.second << std::endl;
    }

    return 0;
}