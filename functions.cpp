#include "functions.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

// Define ANSI escape sequences for colors
#define RESET_COLOR "\033[0m"
#define BLUE_COLOR "\033[34m"
#define GREEN_COLOR "\033[32m"
#define YELLOW_COLOR "\033[33m"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

void loadWordsFromFile(const std::string& filename, std::vector<WordEntry>& wordList) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening the file: " << filename << std::endl;
        return;
    }

    wordList.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word, definition, usage, pronunciation, learned;
        if (std::getline(iss, word, ',') &&
            std::getline(iss, definition, ',') &&
            std::getline(iss, usage, ',') &&
            std::getline(iss, pronunciation, ',') &&
            std::getline(iss, learned)) {
            wordList.push_back({word, definition, usage, pronunciation, (learned == "1")});
        }
    }
    file.close();
}

void saveWordsToFile(const std::string& filename, const std::vector<WordEntry>& wordList) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening the file: " << filename << std::endl;
        return;
    }

    for (const auto& wordEntry : wordList) {
        file << wordEntry.word << "," << wordEntry.definition << ","
             << wordEntry.usage << "," << wordEntry.pronunciation << ","
             << (wordEntry.learned ? "1" : "0") << "\n";
    }
    file.close();
}

void addNewWord(std::vector<WordEntry>& wordList) {
    WordEntry newWord;
    std::cout << "Enter the new word: ";
    std::cin.ignore();
    std::getline(std::cin, newWord.word);

    std::cout << "Enter its definition: ";
    std::getline(std::cin, newWord.definition);

    std::cout << "Enter its usage: ";
    std::getline(std::cin, newWord.usage);

    std::cout << "Enter its pronunciation: ";
    std::getline(std::cin, newWord.pronunciation);

    newWord.learned = false;
    wordList.push_back(newWord);

    std::cout << "Word added successfully!\n";
}

void searchWord(const std::vector<WordEntry>& wordList) {
    if (wordList.empty()) {
        std::cout << "No words in the database.\n";
        return;
    }

    std::string wordToSearch;
    std::cout << "Enter the word to search: ";
    std::cin.ignore();
    std::getline(std::cin, wordToSearch);

    bool found = false;
    for (const auto& wordEntry : wordList) {
        if (wordEntry.word == wordToSearch) {
            std::cout << "Word: " << BLUE_COLOR << wordEntry.word << RESET_COLOR << std::endl;
            std::cout << "Definition: " << wordEntry.definition << std::endl;
            std::cout << "Usage: " << wordEntry.usage << std::endl;
            std::cout << "Pronunciation: " << wordEntry.pronunciation << std::endl;
            std::cout << "Learned: " << (wordEntry.learned ? GREEN_COLOR "Yes" RESET_COLOR : RESET_COLOR "No") << std::endl;
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Word not found in the database.\n";
    }
}

void modifyExistingWord(std::vector<WordEntry>& wordList) {
    std::string wordToUpdate;
    std::cout << "Enter the word to modify: ";
    std::cin.ignore();
    std::getline(std::cin, wordToUpdate);

    auto it = std::find_if(wordList.begin(), wordList.end(),
                           [&wordToUpdate](const WordEntry& entry) {
                               return entry.word == wordToUpdate;
                           });

    if (it != wordList.end()) {
        WordEntry& wordEntry = *it;
        std::cout << "Word found. Please update the information:\n";
        std::cout << "Definition: ";
        std::getline(std::cin, wordEntry.definition);

        std::cout << "Usage: ";
        std::getline(std::cin, wordEntry.usage);

        std::cout << "Pronunciation: ";
        std::getline(std::cin, wordEntry.pronunciation);

        std::cout << "Word updated successfully!\n";
    } else {
        std::cout << "Word not found in the database.\n";
    }
}

void deleteWord(std::vector<WordEntry>& wordList) {
    if (wordList.empty()) {
        std::cout << "No words in the database.\n";
        return;
    }

    std::string wordToDelete;
    std::cout << "Enter the word to delete: ";
    std

::cin.ignore();
    std::getline(std::cin, wordToDelete);

    auto it = std::remove_if(wordList.begin(), wordList.end(),
                             [&wordToDelete](const WordEntry& entry) {
                                 return entry.word == wordToDelete;
                             });

    if (it != wordList.end()) {
        wordList.erase(it, wordList.end());
        std::cout << "Word deleted successfully!\n";
    } else {
        std::cout << "Word not found in the database.\n";
    }
}

void markWordAsLearned(std::vector<WordEntry>& wordList) {
    std::string wordToMark;
    std::cout << "Enter the word to mark as learned: ";
    std::cin.ignore();
    std::getline(std::cin, wordToMark);

    auto it = std::find_if(wordList.begin(), wordList.end(),
                           [&wordToMark](const WordEntry& entry) {
                               return entry.word == wordToMark;
                           });

    if (it != wordList.end()) {
        it->learned = true;
        std::cout << "Word marked as learned!\n";
    } else {
        std::cout << "Word not found in the database.\n";
    }
}

void exportWords(const std::vector<WordEntry>& wordList) {
    if (wordList.empty()) {
        std::cout << "No words in the database.\n";
        return;
    }

    std::string filename;
    std::cout << "Enter the filename to export to (e.g., word_export.csv): ";
    std::cin.ignore();
    std::getline(std::cin, filename);

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening the file: " << filename << std::endl;
        return;
    }

    for (const auto& wordEntry : wordList) {
        file << wordEntry.word << "," << wordEntry.definition << ","
             << wordEntry.usage << "," << wordEntry.pronunciation << ","
             << (wordEntry.learned ? "1" : "0") << "\n";
    }
    file.close();

    std::cout << "Words exported successfully to " << filename << "\n";
}

void importWords(std::vector<WordEntry>& wordList) {
    std::string filename;
    std::cout << "Enter the filename to import from (e.g., word_import.csv): ";
    std::cin.ignore();
    std::getline(std::cin, filename);

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening the file: " << filename << std::endl;
        return;
    }

    std::vector<WordEntry> importedWords;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word, definition, usage, pronunciation, learned;
        if (std::getline(iss, word, ',') &&
            std::getline(iss, definition, ',') &&
            std::getline(iss, usage, ',') &&
            std::getline(iss, pronunciation, ',') &&
            std::getline(iss, learned)) {
            importedWords.push_back({word, definition, usage, pronunciation, (learned == "1")});
        }
    }
    file.close();

    if (importedWords.empty()) {
        std::cout << "No words found in the import file.\n";
        return;
    }

    wordList.insert(wordList.end(), importedWords.begin(), importedWords.end());
    std::cout << "Words imported successfully from " << filename << "\n";
}

void displayFlashcards(const std::vector<WordEntry>& wordList) {
    if (wordList.empty()) {
        std::cout << "No words in the database.\n";
        return;
    }

    // Shuffle the word list for random order
    std::vector<WordEntry> shuffledList = wordList;
    std::random_shuffle(shuffledList.begin(), shuffledList.end());

    std::cout << "==== Flashcards ====\n";
    for (const auto& wordEntry : shuffledList) {
        std::cout << "Word: " << BLUE_COLOR << wordEntry.word << RESET_COLOR << std::endl;
        std::cout << "Definition: " << wordEntry.definition << std::endl;
        std::cout << "Usage: " << wordEntry.usage << std::endl;
        std::cout << "Pronunciation: " << wordEntry.pronunciation << std::endl;
        std::cout << "Learned: " << (wordEntry.learned ? GREEN_COLOR "Yes" RESET_COLOR : RESET_COLOR "No") << std::endl;
        std::cout << "Press Enter to continue to the next word...";
        getch();
        std::cout << std::endl;
    }
    std::cout << "Flashcards review completed!\n";
}

void vocabularyQuiz(const std::vector<WordEntry>& wordList) {
    if (wordList.empty()) {
        std::cout << "No words in the database.\n";
        return;
    }

    int numQuestions;
    std::cout << "Enter the number of questions for the quiz: ";
    std::cin >> numQuestions;

    if (numQuestions <= 0 || numQuestions > wordList.size()) {
        std::cout << "Invalid number of questions.\n";
        return;
    }

    // Shuffle the word list for random order
    std::vector<WordEntry> shuffledList = wordList;
    std::random_shuffle(shuffledList.begin(), shuffledList.end());

    int score = 0;
    std::cout << "==== Vocabulary Quiz ====\n";
    for (int i = 0; i < numQuestions; ++i) {
        const WordEntry& wordEntry = shuffledList[i];
        std::cout << "Question " << (i + 1) << ": What is the definition of " << BLUE_COLOR << wordEntry.word << RESET_COLOR << "?\n";
        std::string userDefinition, userUsage;
        std::cout << "Your answer (definition): ";
        std::cin.ignore();
        std::getline(std::cin, userDefinition);
        std::cout << "Your answer (usage): ";
        std::getline(std::cin, userUsage);

        if (userDefinition == wordEntry.definition && userUsage == wordEntry.usage) {
            std::cout << "Correct!\n";
            score++;
        } else {
            std::cout << "Incorrect. The correct answers are:\n";
            std::cout << "Definition: " << GREEN_COLOR << wordEntry.definition << RESET_COLOR << std::endl;
            std::cout << "Usage: " << GREEN_COLOR << wordEntry.usage << RESET_COLOR << std::endl;
        }
    }

    std::cout << "Quiz completed. Your score: " << score << " out of " << numQuestions << std::endl;
}

void displayAllWords(const std::vector<WordEntry>& wordList) {
    if (wordList.empty()) {
        std::cout << "No words in the database.\n";
        return;
    }

    const int pageSize = 5; // Number of words to display per page
    int currentPage = 0;
    int totalPages = (wordList.size() - 1) / pageSize + 1;

    while (currentPage < totalPages) {
        std::cout << "==== All Words (Page " << (currentPage + 1) << "/" << totalPages << ") ====\n";
        int startIdx = currentPage * pageSize;
        int endIdx = std::min(startIdx + pageSize, static_cast<int>(wordList.size()));

        for (int i = startIdx; i < endIdx; ++i) {
            const auto& wordEntry = wordList[i];
            std::cout << "Word: " << BLUE_COLOR << wordEntry.word << RESET_COLOR << std::endl;
            std::cout << "Definition: " << wordEntry.definition << std::endl;
            std::cout << "Usage: " << wordEntry.usage << std::endl;
            std::cout << "Pronunciation: " << wordEntry.pronunciation << std::endl;
            std::cout << "Learned: " << (wordEntry.learned ? GREEN_COLOR "Yes" : RESET_COLOR "No") << std::endl;
            std::cout << std::endl;
        }

        std::cout << "Press Enter to continue to the next page (or enter 'q' to quit): ";
        std::string userInput;
        std::getline(std::cin, userInput);

        if (userInput == "q" || userInput == "Q") {
            break;
        }

        currentPage++;
    }
}

void displayLearnedWords(const std::vector<WordEntry>& wordList) {
    if (wordList.empty()) {
        std::cout << "No words in the database.\n";
        return;
    }

    const int pageSize = 5; // Number of words to display per page
    int currentPage = 0;
    int totalPages = (wordList.size() - 1) / pageSize + 1;

    while (currentPage < totalPages) {
        std::cout << "==== Learned Words (Page " << (currentPage + 1) << "/" << totalPages << ") ====\n";
        int startIdx = currentPage * pageSize;
        int endIdx = std::min(startIdx + pageSize, static_cast<int>(wordList.size()));

        bool foundLearnedWord = false;
        for (int i = startIdx; i < endIdx; ++i) {
            const auto& wordEntry = wordList[i];
            if (wordEntry.learned) {
                std::cout << "Word: " << BLUE_COLOR << wordEntry.word << RESET_COLOR << std::endl;
                std::cout << "Definition: " << wordEntry.definition << std::endl;
                std::cout << "Usage: " << wordEntry.usage << std::endl;
                std::cout << "Pronunciation: " << wordEntry.pronunciation << std::endl;
                std::cout << std::endl;
                foundLearnedWord = true;
            }
        }

        if (!foundLearnedWord) {
            std::cout << "No learned words found on this page.\n";
        }

        std::cout << "Press Enter to continue to the next page (or enter 'q' to quit): ";
        std::string userInput;
        std::getline(std::cin, userInput);

        if (userInput == "q" || userInput == "Q") {
            break;
        }

        currentPage++;
    }
}

void displayWordsToLearn(const std::vector<WordEntry>& wordList) {
    if (wordList.empty()) {
        std::cout << "No words in the database.\n";
        return;
    }

    const int pageSize = 5; // Number of words to display per page
    int currentPage = 0;
    int totalPages = (wordList.size() - 1) / pageSize + 1;

    while (currentPage < totalPages) {
        std::cout << "==== Words to Learn (Page " << (currentPage + 1) << "/" << totalPages << ") ====\n";
        int startIdx = currentPage * pageSize;
        int endIdx = std::min(startIdx + pageSize, static_cast<int>(wordList.size()));

        bool foundWordsToLearn = false;
        for (int i = startIdx; i < endIdx; ++i) {
            const auto& wordEntry = wordList[i];
            if (!wordEntry.learned) {
                std::cout << "Word: " << BLUE_COLOR << wordEntry.word << RESET_COLOR << std::endl;
                std::cout << "Definition: " << wordEntry.definition << std::endl;
                std::cout << "Usage: " << wordEntry.usage << std::endl;
                std::cout << "Pronunciation: " << wordEntry.pronunciation << std::endl;
                std::cout << std::endl;
                foundWordsToLearn = true;
            }
        }

        if (!foundWordsToLearn) {
            std::cout << "No words to learn found on this page.\n";
        }

        std::cout << "Press Enter to continue to the next page (or enter 'q' to quit): ";
        std::string userInput;
        std::getline(std::cin, userInput);

        if (userInput == "q" || userInput == "Q") {
            break;
        }

        currentPage++;
    }
}

void printInstructions() {
    std::cout <<
        "================================\n"
        "     Welcome to Vocabify\n"
        "================================\n"
        BLUE_COLOR 
        "1. Add a new word\n"
        "2. Search for a word\n"
        "3. Modify an existing word\n"
        "4. Delete a word\n"
        "5. Mark a word as learned\n"
        "6. Export words to a file\n"
        "7. Import words from a file\n"
        "8. Display flashcards\n"
        "9. Take a vocabulary quiz\n"
        "10. List all words\n"
        "11. List learned words\n"
        "12. List words to learn\n"
        "13. Exit\n" 
        RESET_COLOR;
}