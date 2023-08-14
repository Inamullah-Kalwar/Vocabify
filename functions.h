#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include <string>

struct WordEntry {
    std::string word;
    std::string definition;
    std::string usage;
    std::string pronunciation;
    bool learned;
};

// Function prototypes
void loadWordsFromFile(const std::string& filename, std::vector<WordEntry>& wordList);
void saveWordsToFile(const std::string& filename, const std::vector<WordEntry>& wordList);
void addNewWord(std::vector<WordEntry>& wordList);
void searchWord(const std::vector<WordEntry>& wordList);
void modifyExistingWord(std::vector<WordEntry>& wordList);
void deleteWord(std::vector<WordEntry>& wordList);
void markWordAsLearned(std::vector<WordEntry>& wordList);
void exportWords(const std::vector<WordEntry>& wordList);
void importWords(std::vector<WordEntry>& wordList);
void displayFlashcards(const std::vector<WordEntry>& wordList);
void vocabularyQuiz(const std::vector<WordEntry>& wordList);
void displayAllWords(const std::vector<WordEntry>& wordList);
void displayLearnedWords(const std::vector<WordEntry>& wordList);
void displayWordsToLearn(const std::vector<WordEntry>& wordList);
void printInstructions();

#endif