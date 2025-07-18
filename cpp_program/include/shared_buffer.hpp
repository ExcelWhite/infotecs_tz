#pragma once
#include <string>
#include <mutex>
#include <condition_variable>

// Класс для общего буфера, который используется для синхронизации потоков
class SharedBuffer
{
private:
    std::string buffer; // Хранит данные
    std:: mutex mtx; // Мьютекс для синхронизации доступа к буферу
    std:: condition_variable cv; // Условная переменная для ожидания данных
    bool hasData = false; // Флаг наличия данных в буфере
    
public:
    void write(const std::string& data); // Метод для записи данных в буфер
    std::string read(); // Метод для чтения данных из буфера
};

