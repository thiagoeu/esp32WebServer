#include <FS.h>          
#include <WiFiManager.h> 
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>

#define Rele4 4
#define Led  2


/***INSTANCIANDO OBJETOS***********************************************************************************************************************************/


// Configuraçoes e Endereço de Internet WIFI
WiFiServer server                (8090);
//Variável para armazenar a solicitação HTTP
String header;
// Hora Atual
unsigned long currentTime = millis();
// Vez anterior
unsigned long previousTime = 0; 
//Defina o tempo limite em milissegundos 
const long timeoutTime = 2000;

// Reset Wifi Manager
WiFiManager wifiManager;

// Status para controle de Led
int Status_Led = LOW;
int Estado_Do_Rele4 = LOW;

/*************************************************************************************************************************************/
void setup() {
  Serial.begin(115200);
  Serial.println("Configurar Rede Wifi");
  //  SETUP -- wifi -- //
  wifiManager.autoConnect("esp32", "12345678");
  //wifiManager.resetSettings ();   // Apaga todas redes Salvas do Wifi
  delay(150);
  WiFi.begin();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("------------------------------------------------------------");
    Serial.println("Conectado IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("------------------------------------------------------------");
    delay(150);
    server.begin();
  }
  //SETUP -- Definição dos Pinos -- //
  delay(50);
  pinMode(Led,   OUTPUT);
  pinMode(Rele4, OUTPUT);
  // Iniciam em LOW -> Desligados
  digitalWrite(Led,   LOW);
  digitalWrite(Rele4,   LOW);
}


  void loop () {

  WiFiClient client = server.available();    // Ouça os clientes que chegam
   if (client) {                             // Se um novo cliente se conectar
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // imprimir uma mensagem na porta serial
    String currentLine = "";                // faça uma string para conter os dados de entrada do cliente
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // faz um loop enquanto o cliente está conectado
      currentTime = millis();
      if (client.available()) {             // se houver bytes para ler do cliente
        char c = client.read();             // leia um byte, então
        Serial.write(c);                    // imprima no monitor serial
        header += c;
        if (c == '\n') {                    // se o byte é um caractere de nova linha
          // se a linha atual estiver em branco, você terá dois caracteres de nova linha em uma linha.
          // esse é o fim da solicitação HTTP do cliente, então envie uma resposta:
          if (currentLine.length() == 0) {
            // Os cabeçalhos HTTP sempre começam com um código de resposta (por exemplo, HTTP / 1.1 200 OK)
            // e um tipo de conteúdo para que o cliente saiba o que está por vir, em seguida, uma linha em branco:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Verifica o caracter enviado pelo html (GET)********************************

            //**************************************************
            if (header.indexOf("GET /L1") >= 0) {
              Status_Led =! Status_Led;
              digitalWrite(Led, Status_Led);
            }
            if (header.indexOf("GET /T1") >= 0) {
              Estado_Do_Rele4 =! Estado_Do_Rele4;
              digitalWrite(Rele4, Estado_Do_Rele4);
            }

            // DEFINIÇÕES - CORPO - CÓDIGO PÁGINA HTML *****************************************************

            //**************************************************
            // Exibir a página da web em HTML
            client.println("<!DOCTYPE html><html lang=\"pt-BR\">");
            client.println("<head><meta charset=\"UTF-8\">");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<title> E-Green </title>");                     // Titulo do topo da Pagina


            //**************************************************
            // CSS para estilizar os botões e Pagina
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; background-color: #8AA6A3;}");
            client.println(".button { background-color: #4CAF50; border-radius: 15px; color: white; padding: 10px 20px;");
            client.println("text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {  background-color: #555555;}</style></head>");


            //**************************************************
            // Título da página da web
            client.println("<body>    <h1>E-Green</h1> <h3> Controle </h3>");

            
            // Atualização do HTML ************************************************
            
            if (Status_Led) {
              client.print("<a href=\"/L1\"><button class=\"button\">LED-ON</button></a>");
            } else {
              client.print("<a href=\"/L1\"><button class=\"button button2\">LED-OFF</button></a>");
            }

            if (Estado_Do_Rele4) {
              client.print("<a href=\"/T1\"><button class=\"button button2\">RELE-OFF</button></a>");
            } else {
              client.print("<a href=\"/T1\"><button class=\"button\">RELE-ON</button></a>");
            } 
                   
            client.println("<P>");

            client.println("E-Green - Controle e Automações");
            client.println("<P>");

            client.println("</body></html>");

            client.println();                     // A resposta HTTP termina com outra linha em branco:
            break;                                // interromper o loop while:
          } else {
            currentLine = "";                     // se você recebeu uma nova linha, limpe currentLine:
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    // Clear the header variable
    header = "";
    // fecha a conexão:
    client.stop();
    Serial.println("Client Disconnected.");
    Serial.println("------------------------------------------------------------");
  }


}
  