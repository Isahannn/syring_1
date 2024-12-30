#include "pch.h" // Если используете предкомпилированные заголовки
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "TextProcessor.h"
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

// Логирование
void logInfo(const std::string& message) {
    spdlog::get("console")->info(message);
}

void logError(const std::string& message) {
    spdlog::get("console")->error(message);
}

// Тесты для TextProcessor
TEST(TextProcessorTests, CleanTextRemovesExtraSpaces) {
    std::string input = "This   is a    test.";
    std::string expected = "This is a test.";
    EXPECT_EQ(cleanText(input), expected);
}

TEST(TextProcessorTests, CleanTextTrimsLeadingAndTrailingSpaces) {
    std::string input = "   Leading and trailing spaces.   ";
    std::string expected = "Leading and trailing spaces.";
    EXPECT_EQ(cleanText(input), expected);
}

TEST(TextProcessorTests, CleanTextHandlesEmptyString) {
    std::string input = "";
    std::string expected = "";
    EXPECT_EQ(cleanText(input), expected);
}

TEST(TextProcessorTests, ParseTextCreatesParagraph) {
    std::string input = "This is the first sentence. This is the second.";
    Paragraph paragraph = parseText(input);
    EXPECT_EQ(paragraph.getSentences().size(), 2);
    EXPECT_EQ(paragraph.getSentences()[0].getWords().size(), 5);
}

TEST(TextProcessorTests, FindUniqueWordReturnsUniqueWord) {
    std::string input = "This is a sample text. This text is for testing.";
    Paragraph paragraph = parseText(input);
    Word uniqueWord = findUniqueWord(paragraph);
    EXPECT_EQ(uniqueWord.value, "a");
}

TEST(TextProcessorTests, FindUniqueWordInMultipleSentences) {
    std::string input = "The cat sat on the mat. The dog sat on the log.";
    Paragraph paragraph = parseText(input);
    Word uniqueWord = findUniqueWord(paragraph);
    EXPECT_EQ(uniqueWord.value, "cat");
}

TEST(TextProcessorTests, FindUniqueWordInDifferentSentences) {
    std::string input = "The quick brown fox. The quick red fox.";
    Paragraph paragraph = parseText(input);
    Word uniqueWord = findUniqueWord(paragraph);
    EXPECT_EQ(uniqueWord.value, "red");
}

TEST(TextProcessorTests, FindUniqueWordHandlesEmptyParagraph) {
    Paragraph emptyParagraph;
    EXPECT_THROW(findUniqueWord(emptyParagraph), std::runtime_error);
}

TEST(TextProcessorTests, FindUniqueWordHandlesSingleWord) {
    std::string input = "Only one word.";
    Paragraph paragraph = parseText(input);
    Word uniqueWord = findUniqueWord(paragraph);
    EXPECT_EQ(uniqueWord.value, "one");
}

// Новый тест на отсутствие уникального слова
TEST(TextProcessorTests, FindUniqueWordHandlesNoUniqueWords) {
    std::string input = "This is a test. This is also a test.";
    Paragraph paragraph = parseText(input);
    EXPECT_THROW(findUniqueWord(paragraph), std::runtime_error); // Ожидаем исключение, так как уникального слова нет
}

// Новый тест на нахождение определенного слова
TEST(TextProcessorTests, FindUniqueWordHandlesSpecificWord) {
    std::string input = "Apple banana apple orange.";
    Paragraph paragraph = parseText(input);
    Word uniqueWord = findUniqueWord(paragraph);
    EXPECT_EQ(uniqueWord.value, "banana"); // Ожидаем, что "banana" будет уникальным словом
}

// Тест на существование файла
TEST(FileTests, InputFileExists) {
    std::ifstream file("input.txt");
    EXPECT_TRUE(file.good()); // Проверяем, что файл открыт успешно
}

// Тест на содержание файла
TEST(FileTests, InputFileContainsExpectedText) {
    std::ifstream file("input.txt");
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::string expectedContent = "This is a sample text. This text is for testing.\n"
        "The quick brown fox jumps over the lazy dog.\n"
        "Apple banana apple orange.\n"
        "The cat sat on the mat. The dog sat on the log.\n";
    EXPECT_EQ(content, expectedContent);
}

int main(int argc, char** argv) {
    // Инициализация логгера
    spdlog::stdout_color_mt("console");
    logInfo("Starting the program...");

    // Запуск тестов
    ::testing::InitGoogleTest(&argc, argv);
    logInfo("Running tests...");
    int test_result = RUN_ALL_TESTS();
    logInfo("Tests completed.");

    // Основная логика программы
    std::ifstream file("input.txt");
    if (!file) {
        logError("Error opening file!");
        return 1;
    }

    logInfo("Starting word count in the file: input.txt");
    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    Paragraph paragraph = parseText(text);

    try {
        Word uniqueWord = findUniqueWord(paragraph);
        logInfo("Unique word in the first sentence: " + uniqueWord.value);
    }
    catch (const std::runtime_error& e) {
        logError(e.what());
    }

    return test_result; // Возвращаем результат тестов
}