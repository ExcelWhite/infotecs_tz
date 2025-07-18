#include <iostream>
#include <algorithm>
#include <thread>
#include <regex>
#include "../include/shared_buffer.hpp"

// Проверка, что ввод состоит только из цифр и не превышает 64 символа
bool isValidInput(const std::string& input) {
    return std::regex_match(input, std::regex("\\d{1,64}"));
}

// Преобразование входной строки: сортировка и замена четных цифр на "KB"
std::string transformInput(std::string input) {
    std::sort(input.begin(), input.end(), std::greater<>());
    std::string result;

    for (char c : input) {
        int digit = c - '0';
        if (digit % 2 == 0) {
            result += "KB"; // Замена четных цифр
        } else {
            result += c; // Оставляем нечетные цифры
        }
    }

    return result;
}

// Поток для ввода данных от пользователя
void inputThread(SharedBuffer& buffer) {
    while (true) {
        std::string input;
        std::getline(std::cin, input); // Чтение строки из ввода

        if (!isValidInput(input)) {
            std::cout << "Invalid input. Only digits (max 64).\n";
            std::cout << "Введите цифры (макс 64): " << std::flush;
            continue;
        }

        std::string transformed = transformInput(input); // Преобразование строки
        buffer.write(transformed); // Запись в общий буфер
    }
}
