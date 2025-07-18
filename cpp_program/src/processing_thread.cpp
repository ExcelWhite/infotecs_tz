#include <iostream>
#include <thread>
#include <cctype>
#include <string>
#include <sstream>
#include "../include/shared_buffer.hpp"

extern void sendToFlutter(int sum); // Объявление функции для отправки данных во Flutter

// Извлечение и суммирование чисел из строки
int extractAndSumDigits(const std::string& data) {
    int sum = 0;
    for (char c : data) {
        if (std::isdigit(c)) {
            sum += c - '0'; // Суммируем только цифры
        }
    }
    return sum;
}

// Поток для обработки данных из общего буфера
void processingThread(SharedBuffer& buffer) {
    while (true) {
        std::string data = buffer.read(); // Чтение данных из буфера
        std::cout << "Обработанные данные: " << data << "\n";

        int sum = extractAndSumDigits(data); // Суммирование цифр
        std::cout << "Сумма цифр: " << sum << "\n";

        sendToFlutter(sum); // Отправка суммы во Flutter

        // Запрос на следующий ввод после завершения обработки
        std::cout << "Enter digits (max 64): " << std::flush;
    }
}

