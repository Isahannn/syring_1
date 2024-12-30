#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <string>
#include <vector>
#include <stdexcept>
#include <unordered_map>
#include <sstream>

// Структура для слова
struct Word {
    std::string value;
};

// Класс для предложения
class Sentence {
public:
    void addWord(const Word& word) {
        words.push_back(word);
    }
    const std::vector<Word>& getWords() const {
        return words;
    }

private:
    std::vector<Word> words;
};

// Класс для параграфа
class Paragraph {
public:
    void addSentence(const Sentence& sentence) {
        sentences.push_back(sentence);
    }
    const std::vector<Sentence>& getSentences() const {
        return sentences;
    }

private:
    std::vector<Sentence> sentences;
};

// Функции для обработки текста
std::string cleanText(const std::string& input);
Paragraph parseText(const std::string& input);
Word findUniqueWord(const Paragraph& paragraph);

#endif // TEXTPROCESSOR_H