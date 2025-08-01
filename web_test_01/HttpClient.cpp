void makeHTTPRequest(const char* host, int port, const char* endpoint, Stream& espSerial) {
  espSerial.print("AT+CIPSTART=\"TCP\",\"");
  espSerial.print(host);
  espSerial.print("\",");
  espSerial.println(port);
  delay(2000);
  if (espSerial.find("CONNECT")) {
    String httpRequest = String("GET ") + endpoint + " HTTP/1.1\r\n" +
                         "Host: " + host + "\r\n" +
                         "Connection: close\r\n\r\n";
    espSerial.print("AT+CIPSEND=");
    espSerial.println(httpRequest.length());
    delay(1000);
    if (espSerial.find(">")) {
      espSerial.print(httpRequest);
      delay(1000);
    }
    espSerial.println("AT+CIPCLOSE");
  }
}

void sendWeightToServer(const char* host, int port, const char* endpoint, Stream& espSerial, float weight) {
  espSerial.print("AT+CIPSTART=\"TCP\",\"");
  espSerial.print(host);
  espSerial.print("\",");
  espSerial.println(port);
  delay(2000);
  if (espSerial.find("CONNECT")) {
    String body = String("{\"weight\":") + weight + "}";
    String request = String("POST ") + endpoint + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Content-Type: application/json\r\n" +
                     "Content-Length: " + body.length() + "\r\n\r\n" +
                     body;
    espSerial.print("AT+CIPSEND=");
    espSerial.println(request.length());
    delay(1000);
    if (espSerial.find(">")) {
      espSerial.print(request);
      delay(1000);
    }
    espSerial.println("AT+CIPCLOSE");
  }
}
