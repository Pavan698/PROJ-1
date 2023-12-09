#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>

class TrieNode {
public:
    std::unordered_map<char, TrieNode*> children;
    bool isEndOfWord;

    TrieNode() : isEndOfWord(false) {}
};

class Trie {
private:
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const std::string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            if (current->children.find(ch) == current->children.end()) {
                current->children[ch] = new TrieNode();
            }
            current = current->children[ch];
        }
        current->isEndOfWord = true;
    }

    bool search(const std::string& word) {
        TrieNode* node = searchNode(word);
        return (node != nullptr && node->isEndOfWord);
    }

    TrieNode* searchNode(const std::string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            if (current->children.find(ch) == current->children.end()) {
                return nullptr; // Prefix not found
            }
            current = current->children[ch];
        }
        return current;
    }

    // Helper function for suggestions
    void suggestUtil(TrieNode* node, const std::string& currentWord, std::vector<std::string>& suggestions) {
        std::string modifiedCurrentWord = currentWord;  // Create a non-const copy

        if (node->isEndOfWord) {
            suggestions.push_back(modifiedCurrentWord);
        }

        for (const auto& child : node->children) {
            modifiedCurrentWord.push_back(child.first);
            suggestUtil(child.second, modifiedCurrentWord, suggestions);
            modifiedCurrentWord.pop_back();
        }
    }

    // Function to suggest words similar to the given word
    std::vector<std::string> suggest(const std::string& word) {
        TrieNode* node = searchNode(word);
        std::vector<std::string> suggestions;

        if (node != nullptr) {
            suggestUtil(node, word, suggestions);
        }

        return suggestions;
    }
};

// Function to load words from a file into the Trie
void loadDictionaryFromFile(Trie& trie, const std::string& filename) {
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string word;
        while (file >> word) {
            trie.insert(word);
        }
        file.close();
    } else {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
}

int main() {
    Trie trie;

    // Load words from a file into the Trie
    loadDictionaryFromFile(trie, "dictionary.txt");  // Replace "dictionary.txt" with your file path

    while (true) {
        // Check spelling and suggest corrections
        std::string inputWord;
        std::cout << "Enter a word (or type 'exit' to quit): ";
        std::cin >> inputWord;

        if (inputWord == "exit") {
            break; // Exit the loop if the user types 'exit'
        }

        if (trie.search(inputWord)) {
            std::cout << "The word is spelled correctly.\n";
        } else {
            std::cout << "The word is misspelled. Suggestions:\n";
            std::vector<std::string> suggestions = trie.suggest(inputWord);

            if (suggestions.empty()) {
                std::cout << "No suggestions found.\n";
            } else {
                for (const std::string& suggestion : suggestions) {
                    std::cout << suggestion << "\n";
                }
            }
        }
    }

    return 0;
}
