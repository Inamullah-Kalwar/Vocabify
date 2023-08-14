#include <iostream>
#include <vector>
#include <string>
#include "functions.h"

int main() {
    std::vector<WordEntry> wordList;
    std::string filename = "word_database.csv";
    loadWordsFromFile(filename, wordList);

    int choice;
    do {
        printInstructions();
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                addNewWord(wordList);
                break;
            case 2:
                searchWord(wordList);
                break;
            case 3:
                modifyExistingWord(wordList);
                break;
            case 4:
                deleteWord(wordList);
                break;
            case 5:
                markWordAsLearned(wordList);
                break;
            case 6:
                exportWords(wordList);
                break;
            case 7:
                importWords(wordList);
                break;
            case 8:
                displayFlashcards(wordList);
                break;
            case 9:
                vocabularyQuiz(wordList);
                break;
            case 10:
                displayAllWords(wordList);
                break;
            case 11:
                displayLearnedWords(wordList);
                break;
            case 12:
                displayWordsToLearn(wordList);
                break;
            case 13:
                saveWordsToFile(filename, wordList);
                std::cout << "Exiting the program. Thank you!\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 13);

    return 0;
}