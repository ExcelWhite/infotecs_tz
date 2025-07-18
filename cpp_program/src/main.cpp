#include <iostream>
#include <thread>
#include "../include/shared_buffer.hpp"

extern void inputThread(SharedBuffer&);
extern void processingThread(SharedBuffer&);

int main() {
    SharedBuffer buffer; // Создание общего буфера

    std::cout << "Введите цифры (макс 64): " << std::flush;

    // Запуск потоков для ввода и обработки данных
    std::thread t1(inputThread, std::ref(buffer)); // Ожидание завершения потока ввода
    std::thread t2(processingThread, std::ref(buffer)); // Ожидание завершения потока обработки

    t1.join();
    t2.join();

    return 0;
}
