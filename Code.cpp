/*Name: Sumaiya Mohamud, Jayath Mallireddy, Pavin Karri
  Class: CS303
  Project: Spellchecker #1
  S-Sumaiya J-Jayath P-Pavin
  */

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>

// TrieNode class represents a node in the Trie
//S
class TrieNode {
public:
    std::unordered_map<char, TrieNode*> children; // Map to store child nodes
    bool isEndOfWord; // Flag indicating if the current node represents the end of a word

    TrieNode() : isEndOfWord(false) {} // Constructor to initialize TrieNode
};

// Trie class represents a Trie data structure
//P
class Trie {
private:
    TrieNode* root; // Root of the Trie

public:
    Trie() {
        root = new TrieNode(); // Constructor to initialize Trie with an empty root
    }

    // Function to insert a word into the Trie
    //J and P
    void insert(const std::string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            // If the current character is not a child, create a new node
            if (current->children.find(ch) == current->children.end()) {
                current->children[ch] = new TrieNode();
            }
            current = current->children[ch]; // Move to the next node
        }
        current->isEndOfWord = true; // Mark the last node as the end of the word
    }

    // Function to search for a word in the Trie
    //J
    bool search(const std::string& word) {
        TrieNode* node = searchNode(word);
        return (node != nullptr && node->isEndOfWord);
    }

    // Function to search for a node corresponding to a word in the Trie
    //S
    TrieNode* searchNode(const std::string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            // If the current character is not a child, the prefix is not found
            if (current->children.find(ch) == current->children.end()) {
                return nullptr;
            }
            current = current->children[ch]; // Move to the next node
        }
        return current; // Return the last node corresponding to the word
    }

    // Helper function for suggestions
    //S and J
    void suggestUtil(TrieNode* node, const std::string& currentWord, std::vector<std::string>& suggestions) {
        std::string modifiedCurrentWord = currentWord; // Create a non-const copy

        // If the current node represents the end of a word, add it to suggestions
        if (node->isEndOfWord) {
            suggestions.push_back(modifiedCurrentWord);
        }

        // Recursively explore child nodes
        //S and P
        for (const auto& child : node->children) {
            modifiedCurrentWord.push_back(child.first); // Append the character to the current word
            suggestUtil(child.second, modifiedCurrentWord, suggestions); // Recursively call for the child node
            modifiedCurrentWord.pop_back(); // Backtrack: remove the last character for the next iteration
        }
    }

    // Function to suggest words similar to the given word
    // P
    std::vector<std::string> suggest(const std::string& word) {
        TrieNode* node = searchNode(word);
        std::vector<std::string> suggestions;

        // If the word is found, suggest words based on the Trie
        //J
        if (node != nullptr) {
            suggestUtil(node, word, suggestions);
        }

        return suggestions; // Return the list of suggestions
    }
};

// Function to load words from a file into the Trie
//S
void loadDictionaryFromFile(Trie& trie, const std::string& filename) {
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string word;
        while (file >> word) {
            trie.insert(word); // Insert each word from the file into the Trie
        }
        file.close();
    } else {
        std::cerr << "Error opening file: " << filename << std::endl; // Print an error message if the file cannot be opened
    }
}

// Main function //P and J
int main() {
    Trie trie;

    // Load words from a file into the Trie
    loadDictionaryFromFile(trie, "dictionary.txt"); // Replace "dictionary.txt" with your file path

    while (true) {
        // Check spelling and suggest corrections
        //J
        std::string inputWord;
        std::cout << "Enter a word (or type 'exit' to quit): ";
        std::cin >> inputWord;

        if (inputWord == "exit") {
            break; // Exit the loop if the user types 'exit'
        }

        // Check if the entered word is spelled correctly
        //P
        if (trie.search(inputWord)) {
            std::cout << "The word is spelled correctly.\n";
        } else {
            std::cout << "The word is misspelled. Suggestions:\n";
            std::vector<std::string> suggestions = trie.suggest(inputWord);

            // Print suggestions or a message if no suggestions are found
          //S
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

/*Sources used:
https://www.geeksforgeeks.org/spell-checker-using-trie/#
*/

