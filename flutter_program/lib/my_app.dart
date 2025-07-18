import 'package:flutter/material.dart';
import 'package:flutter_program/TCP_server_screen.dart';

// Основной класс приложения
class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: TCPServerScreen(),
      debugShowCheckedModeBanner: false,
    );
  }
}