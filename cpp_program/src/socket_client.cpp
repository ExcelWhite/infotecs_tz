#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <netinet/in.h>
#include <arpa/inet.h>

int sock = -1; // Дескриптор сокета

// Функция для повторного подключения к серверу
void reconnect() {
    if (sock != -1) {
        close(sock); // Закрытие старого сокета
    }

    sock = socket(AF_INET, SOCK_STREAM, 0); // Создание нового сокета
    if (sock < 0) {
        std::cerr << "❌ Ошибка создания сокета.\n";
        sock = -1;
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4040); // Порт сервера
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // IP-адрес сервера

    int result = connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)); // Подключение к серверу
    if (result == 0) {
        std::cerr << "✅ Подключено к серверу Flutter.\n";
    } else {
        std::cerr << "❌ Не удалось подключиться к Flutter. Сервер недоступен.\n";
        close(sock);
        sock = -1;
    }
}

// Функция для отправки данных во Flutter
void sendToFlutter(int sum) {
    std::string message = std::to_string(sum) + "\n"; // Добавление новой строки для ясности
    bool sentSuccessfully = false;

    for (int attempt = 0; attempt < 3; ++attempt) {
        if (sock == -1) {
            reconnect(); // Повторное подключение, если сокет закрыт
        }

        if (sock == -1) {
            std::cerr << "⚠️ Сервер Flutter недоступен. Повторная попытка...\n";
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Ожидание перед повторной попыткой
            continue;
        }

        // Проверка, не был ли сокет закрыт
        char buffer;
        ssize_t peekResult = recv(sock, &buffer, 1, MSG_PEEK | MSG_DONTWAIT);
        if (peekResult == 0) {
            std::cerr << "⚠️ Сервер Flutter отключен.\n";
            close(sock);
            sock = -1;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        // Попытка отправки данных
        ssize_t sent = send(sock, message.c_str(), message.length(), 0);
        if (sent <= 0) {
            std::cerr << "⚠️ Не удалось отправить данные. Сброс соединения.\n";
            close(sock);
            sock = -1;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        } else {
            sentSuccessfully = true; // Успешная отправка
            break;
        }
    }

    if (!sentSuccessfully) {
        std::cerr << "❌ Не удалось отправить данные во Flutter после 3 попыток.\n";
    }
}

