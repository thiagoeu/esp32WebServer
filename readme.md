# ESP32 Web Server Automation Project
Este projeto utiliza um ESP32 para controlar um LED através de um servidor web. O código configura o ESP32 para se conectar a uma rede Wi-Fi e disponibiliza uma página HTML simples para permitir o controle do LED a partir de um navegador web.

# Requisitos
    Placa ESP32
    Biblioteca FS.h
    Biblioteca WiFiManager
    Biblioteca WiFi.h
    Biblioteca DNSServer.h
    Biblioteca WebServer.h
    Funcionamento


Conexão Wi-Fi: O ESP32 utiliza a biblioteca WiFiManager para configurar a conexão com a rede Wi-Fi. Um ponto de acesso chamado "esp32" com a senha "12345678" é criado para permitir a configuração da rede.

Servidor Web: Um servidor web é iniciado na porta 8090. Quando um cliente se conecta, o ESP32 responde com uma página HTML que permite o controle de um LED.

# Controle do LED: 

A página HTML exibida contém dois botões: um para ligar o LED (LED-ON) e outro para desligá-lo (LED-OFF). O estado atual do LED é refletido nos botões.