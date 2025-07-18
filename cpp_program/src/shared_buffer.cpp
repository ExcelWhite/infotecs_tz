#include "../include/shared_buffer.hpp"

// Запись данных в общий буфер
void SharedBuffer::write(const std::string& data) {
    std::unique_lock<std::mutex> lock(mtx); // Блокировка мьютекса
    buffer = data; // Запись данных
    hasData = true; // Установка флага наличия данных
    cv.notify_one(); // Уведомление потока, ожидающего данные
}

// Чтение данных из общего буфера
std::string SharedBuffer::read() {
    std::unique_lock<std::mutex> lock(mtx); // Блокировка мьютекса
    cv.wait(lock, [this] { return hasData; }); // Ожидание наличия данных
    hasData = false; // Сброс флага
    return buffer; // Возврат данных
}

