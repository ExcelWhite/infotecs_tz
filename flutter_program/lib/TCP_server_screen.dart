import 'dart:convert';
import 'dart:io';

import 'package:flutter/material.dart';
import 'package:lottie/lottie.dart';

class TCPServerScreen extends StatefulWidget {
  const TCPServerScreen({super.key});

  @override
  State<TCPServerScreen> createState() => _TCPServerScreenState();
}

class _TCPServerScreenState extends State<TCPServerScreen> {
  String _message = 'Waiting for connection...';
  int? _number;
  ServerSocket? serverSocket;
  bool? _isError;

  @override
  void initState() {
    super.initState();
    startTCPServer();
  }

  void startTCPServer() async {
    try {
      serverSocket = await ServerSocket.bind(InternetAddress.loopbackIPv4, 4040);
      serverSocket!.listen(handleClient);
      setState(() {
        _message = 'Server is running. Awaiting data...';
      });
    } catch (e) {
      setState(() {
        _message = 'Error starting server: $e';
      });
    }
  }

  // Метод для обработки клиента
  void handleClient(Socket client) {
    client.listen((List<int> data) {
      String received = utf8.decode(data).trim();
      debugPrint('Received: $received');

      final intValue = int.tryParse(received);
      String newMessage;
      bool? isError;

      

      if (intValue == null) {
        newMessage = 'Error: Received non-integer data';
      } else if (received.length > 2 && intValue % 32 == 0) {
        newMessage = 'Valid data received';
        isError = false;
      } else {
        newMessage = 'Error: is not valid (length <= 2 or not divisible by 32)';
        isError = true;
      }

      // Обновление состояния виджета
      setState(() {
        _number = intValue;
        _message = newMessage;
        _isError = isError;
      });
    }, onError: (err) {
      client.close();
      setState(() {
        _number = null;
        _message = 'Error with client: $err';
      });
    }, onDone: () {
      client.close();
      setState(() {
        _number = null;
        _message = 'Client disconnected. Waiting for new connection...';
      });
    });
  }

  @override
  void dispose() {
    serverSocket?.close(); // Закрытие сокета при уничтожении виджета
    super.dispose();
  }


  @override
  Widget build(BuildContext context) {
    final size = MediaQuery.of(context).size;
    return Scaffold(
      backgroundColor: Color(0xFFF5F5F5), // Цвет фона
      extendBodyBehindAppBar: true,
      appBar: AppBar(
        backgroundColor: Colors.transparent, // Прозрачный AppBar
        title: SizedBox(
          width: size.width*0.9,
          child: Center(
            child: _reusableText(
              text: 'Message Screen',
              fontSize: 20,
              fontWeight: FontWeight.w700
            )
          ),
        )
      ),
      body: SafeArea(
        child: Container(
          width: size.width,
          height: size.height,
          padding: EdgeInsets.symmetric(
            horizontal: size.width*0.05, 
            vertical: size.height*0.05
          ),
          
          child: SizedBox(
            width: size.width*0.8,
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              crossAxisAlignment: CrossAxisAlignment.center,
              mainAxisSize: MainAxisSize.min,
              children: [
                // Анимация в зависимости от состояния
                Lottie.asset(
                  _isError == null 
                    ? 'assets/dash_loading.json' // Загрузка
                    : _isError!
                      ? 'assets/error.json' // Ошибка
                      : 'assets/success.json', // Успех
                  
                  repeat: true,
                  width: 150,
                  fit: BoxFit.fill
                ),

                const SizedBox(height: 20,),

                // Отображение полученного числа, если оно не null
                _number != null 
                  ? _reusableText(
                    text: _number!.toString(),
                    fontSize: 32,
                    fontWeight: FontWeight.w900
                ) : SizedBox.shrink(),

                const SizedBox(height: 20,),

                // Отображение сообщения
                Flexible(
                  child: _reusableText(
                    text: _message,
                    color: Color(0xFF333333),
                  ),
                )

              ],
            ),
          ),
        ),
      ),
    );
  }




  Widget _reusableText({
    required String text,
    double? fontSize,
    FontWeight? fontWeight,
    Color? color,
  }) {
    return Text(
      text,
      style: TextStyle(
        fontSize: fontSize ?? 16,
        fontWeight: fontWeight ?? FontWeight.w500,
        color: color ?? Color(0xFF1A1A1A),
      ),
      textAlign: TextAlign.center,
      softWrap: true,
      overflow: TextOverflow.visible,
    );
  }
}