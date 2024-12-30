#include "pch.h"
#include "TextProcessor.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <regex> // Подключаем библиотеку для работы с регулярными выражениями

// Функция для очистки текста от лишних пробелов
std::string cleanText(const std::string& input) {
    std::string output;
    bool lastWasSpace = false;

    for (char c : input) {
        if (std::isspace(c)) {
            if (!lastWasSpace) {
                output += ' '; // Добавляем один пробел
                lastWasSpace = true;
            }
        }
        else {
            output += c; // Добавляем символ
            lastWasSpace = false;
        }
    }

    // Удаляем пробелы в начале и в конце строки
    output.erase(0, output.find_first_not_of(" "));
    output.erase(output.find_last_not_of(" ") + 1);

    return output; // Возвращаем очищенную строку
}

// Функция для разбора текста на параграфы и предложения
Paragraph parseText(const std::string& input) {
    Paragraph paragraph;
    std::istringstream stream(input);
    std::string sentence;

    while (std::getline(stream, sentence, '.')) { // Разделяем по точкам
        if (!sentence.empty()) {
            Sentence s;
            std::istringstream wordStream(sentence);
            std::string word;
            while (wordStream >> word) { // Разбиваем на слова
                Word w;
                w.value = cleanText(word); // Очищаем слово
                s.addWord(w);
            }
            paragraph.addSentence(s);
        }
    }

    return paragraph; // Возвращаем параграф
}

// Функция для нахождения уникального слова с использованием регулярных выражений
Word findUniqueWord(const Paragraph& paragraph) {
    std::unordered_map<std::string, int> wordCount;
    std::regex wordRegex(R"([A-Za-zА-Яа-яЁё]+)"); // Регулярное выражение для поиска слов

    for (const auto& sentence : paragraph.getSentences()) {
        for (const auto& word : sentence.getWords()) {
            if (std::regex_match(word.value, wordRegex)) { // Проверяем, соответствует ли слово регулярному выражению
                wordCount[word.value]++;
            }
        }
    }

    for (const auto& pair : wordCount) {
        if (pair.second == 1) {
            Word uniqueWord;
            uniqueWord.value = pair.first;
            return uniqueWord; // Возвращаем уникальное слово
        }
    }

    throw std::runtime_error("No unique word found"); // Генерируем исключение, если уникальное слово не найдено
}