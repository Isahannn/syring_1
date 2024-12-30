#include "pch.h"
#include "TextProcessor.h"
#include <algorithm>
#include <cctype>
#include <sstream>
#include <regex> // ���������� ���������� ��� ������ � ����������� �����������

// ������� ��� ������� ������ �� ������ ��������
std::string cleanText(const std::string& input) {
    std::string output;
    bool lastWasSpace = false;

    for (char c : input) {
        if (std::isspace(c)) {
            if (!lastWasSpace) {
                output += ' '; // ��������� ���� ������
                lastWasSpace = true;
            }
        }
        else {
            output += c; // ��������� ������
            lastWasSpace = false;
        }
    }

    // ������� ������� � ������ � � ����� ������
    output.erase(0, output.find_first_not_of(" "));
    output.erase(output.find_last_not_of(" ") + 1);

    return output; // ���������� ��������� ������
}

// ������� ��� ������� ������ �� ��������� � �����������
Paragraph parseText(const std::string& input) {
    Paragraph paragraph;
    std::istringstream stream(input);
    std::string sentence;

    while (std::getline(stream, sentence, '.')) { // ��������� �� ������
        if (!sentence.empty()) {
            Sentence s;
            std::istringstream wordStream(sentence);
            std::string word;
            while (wordStream >> word) { // ��������� �� �����
                Word w;
                w.value = cleanText(word); // ������� �����
                s.addWord(w);
            }
            paragraph.addSentence(s);
        }
    }

    return paragraph; // ���������� ��������
}

// ������� ��� ���������� ����������� ����� � �������������� ���������� ���������
Word findUniqueWord(const Paragraph& paragraph) {
    std::unordered_map<std::string, int> wordCount;
    std::regex wordRegex(R"([A-Za-z�-��-���]+)"); // ���������� ��������� ��� ������ ����

    for (const auto& sentence : paragraph.getSentences()) {
        for (const auto& word : sentence.getWords()) {
            if (std::regex_match(word.value, wordRegex)) { // ���������, ������������� �� ����� ����������� ���������
                wordCount[word.value]++;
            }
        }
    }

    for (const auto& pair : wordCount) {
        if (pair.second == 1) {
            Word uniqueWord;
            uniqueWord.value = pair.first;
            return uniqueWord; // ���������� ���������� �����
        }
    }

    throw std::runtime_error("No unique word found"); // ���������� ����������, ���� ���������� ����� �� �������
}