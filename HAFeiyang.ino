#include <ESP8266WiFi.h>
#include <iostream>
#include <WebSocketsClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <PubSubClient.h>
#include "SmartPointer.h"


// WiFi参数
const char* ssid = "你的WiFi网络名称";
const char* password = "moto1984";

// Home Assistant WebSocket参数
const char* haHost = "192.168.31.62";
const int haPort = 8123;
const char* haAccessToken = "你的Home Assistant访问令牌";

const char* mqttBroker = "MQTT代理服务器地址";
const int mqttPort = 1883;
const char* mqttClientId = "MQTT客户端ID";
const char* mqttTopic = "要发布的MQTT主题";

// WebSocket客户端和HTTP服务器
WebSocketsClient webSocket;
ESP8266WebServer server(80);


/////////////////////////////////HTTPClient to call home assistan api/////////////////////////////////////
class HTTPClient {
private:
  WiFiClientSecure client;
  const char* host;
  int port;
  const char* accessToken;

public:
  HTTPClient(const char* _host, int _port, const char* _accessToken) {
    host = _host;
    port = _port;
    accessToken = _accessToken;
  }

  DynamicJsonDocument get(const char* path) {
    DynamicJsonDocument response(1024);

    if (client.connect(host, port)) {
      client.print(String("GET ") + path + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Authorization: Bearer " + accessToken + "\r\n" +
                   "Connection: close\r\n\r\n");

      while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          break;
        }
      }

      while (client.available()) {
        String responseLine = client.readStringUntil('\n');
        responseLine.trim();
        if (responseLine.length() > 0) {
          deserializeJson(response, responseLine);
        }
      }

      client.stop();
    }

    return response;
  }

  DynamicJsonDocument post(const char* path, const char* data) {
    DynamicJsonDocument response(1024);

    if (client.connect(host, port)) {
      client.print(String("POST ") + path + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Authorization: Bearer " + accessToken + "\r\n" +
                   "Content-Type: application/json\r\n" +
                   "Content-Length: " + String(strlen(data)) + "\r\n" +
                   "Connection: close\r\n\r\n" +
                   data);

      while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          break;
        }
      }

      while (client.available()) {
        String responseLine = client.readStringUntil('\n');
        responseLine.trim();
        if (responseLine.length() > 0) {
          deserializeJson(response, responseLine);
        }
      }

      client.stop();
    }

    return response;
  }
};

/////////////////////////////////FYWebSocketsClient to call home assistan api/////////////////////////////////////

class FYWebSocketsClient {
private:
  WiFiClientSecure client;
  const char* host;
  int port;
  const char* accessToken;
  WebSocketsClient webSocket;
  // void (*messageCallback)(const char*);
  std::function<void(const char*)> messageCallback;
public:
  FYWebSocketsClient(){}

  void begin(const char* _host, int _port, const char* _accessToken, std::function<void(const char*)> callback) {
    messageCallback = callback;
    host = _host;
    port = _port;
    accessToken = _accessToken;
  }
  void connectWebSocket() {
    String auth = "Bearer " + String(accessToken);
    webSocket.setAuthorization(auth.c_str());
    webSocket.enableHeartbeat(15000, 5000, 2);
    webSocket.onEvent([this](WStype_t type, uint8_t* payload, size_t length) {
      if (type == WStype_TEXT) {
        // 处理收到的WebSocket消息
        handleWebSocketMessage(payload, length);
      }
    });
    webSocket.beginSocketIO(host,port, "/api/websocket");
  }

  void sendWebSocketMessage(const char* message) {
    webSocket.sendTXT(message);
  }

  void handleMQTTMessage(const char* topic, byte* payload, unsigned int length) {
    // 处理收到的MQTT消息
    // ...
  }

  void loop() {
    webSocket.loop();
  }



private:
  void handleWebSocketMessage(uint8_t* payload, size_t length) {
    // 处理收到的WebSocket消息的逻辑
    // 调用回调函数处理消息
    if (messageCallback != nullptr) {
      messageCallback(reinterpret_cast<const char*>(payload));
    }
  }
};

/////////////////////////////////MQTTClient to call home assistan api/////////////////////////////////////
class MQTTClient {
private:
  PubSubClient mqttClient;
  // void (*messageCallback)(const char*);
  std::function<void(const char* , byte* , unsigned int )> messageCallback;
public:
  MQTTClient() {}

  void begin(const char* broker, int port, const char* clientId, std::function<void(const char* , byte* , unsigned int )> callback) {
    messageCallback = callback;
    mqttClient.setServer(broker, port);
    mqttClient.setCallback([this](char* topic, byte* payload, unsigned int length) {
      handleMQTTMessage(topic, payload, length);
    });
    connect();
  }

  void loop() {
    mqttClient.loop();
  }
void connect() {
    while (!mqttClient.connected()) {
      if (mqttClient.connect(mqttClientId)) {
        Serial.println("连接到MQTT代理服务器...");
        mqttClient.subscribe(mqttTopic);
      }
      else {
        Serial.print("连接到MQTT代理服务器失败，错误码=");
        Serial.println(mqttClient.state());
        delay(5000);
      }
    }
  }

  void publish(const char* topic, const char* payload) {
    mqttClient.publish(topic, payload);
  }

  void handleMessageFromWebSocket(const char* message){
    // 处理收到的Websocket消息
      // ...
  }

private:
  void handleMQTTMessage(char* topic, byte* payload, unsigned int length) {
    // 处理收到的MQTT消息
    // ...
  }
};
/////////////////////////////////main to call home assistan api/////////////////////////////////////
class HAfeiyang{
  private:
  MQTTClient* mqttClient_;
  HTTPClient* httpclient_;
  FYWebSocketsClient* webSocketClient_;
public:
  HAfeiyang() {}
  ~HAfeiyang() {
    if(mqttClient_) {delete mqttClient_; }
    if(httpclient_) {delete httpclient_; }
    if(webSocketClient_) {delete webSocketClient_; }
  }
  void begin() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("连接到WiFi网络...");
    }

    // websocket client api
    webSocketClient_ = new FYWebSocketsClient();
    webSocketClient_->begin(haHost, haPort, haAccessToken, [this](const char* message)->void {
       mqttClient_->handleMessageFromWebSocket(message);
    });
    // 连接到Home Assistant的WebSocket接口
    webSocketClient_->connectWebSocket();
    // 发送WebSocket消息
    const char* message = "Hello, Home Assistant!";
    webSocketClient_->sendWebSocketMessage(message);

    // httpclient api
    httpclient_ = new HTTPClient(haHost, haPort, haAccessToken);

    // mqtt client api
    mqttClient_ = new MQTTClient();
    mqttClient_->begin(mqttBroker, mqttPort, mqttClientId,[this](const char* topic, byte* payload, unsigned int length)->void {
       webSocketClient_->handleMQTTMessage(topic,payload,length);
    });


     

  // // 发送GET请求获取Home Assistant的信息
  // DynamicJsonDocument respget = HTTPClient_->get("/api/states");
  // Serial.println("GET 响应:");
  // serializeJsonPretty(respget, Serial);
  

  // // 发送POST请求更新Home Assistant的状态
  // DynamicJsonDocument postData(128);
  // postData["entity_id"] = "light.kitchen";
  // postData["state"] = "on";
  //   // 序列化JsonDocument为JSON字符串
  // String jsonString;
  // serializeJson(postData, jsonString);
  // DynamicJsonDocument respoost = HTTPClient_->post("/api/services/light/turn_on", jsonString.c_str());
  // Serial.println("POST 响应:");
  // serializeJsonPretty(respoost, Serial);

  }

  void loop() {
    webSocketClient_->loop();
    mqttClient_->loop();
  }

  void sendWebSocketMessage(const char* message) {
     webSocketClient_->sendWebSocketMessage(message);
  }

  String sendHTTPRequest(const char* method, const char* url, const char* payload) {
    // if (strcmp(method, "GET") == 0) {
    //   return HTTPClient_->get(url);
    // }
    // else if (strcmp(method, "POST") == 0) {
    //   return HTTPClient_->post(url, payload);
    // }
    // else {
    //   return "";
    // }


  }

  void publishMQTTMessage(const char* topic, const char* payload) {
    mqttClient_->publish(topic, payload);
  }

  // void DataCallback(std::function<void(void)> callback)
  // {
  //   std::cout << "Start FuncCallback!" << std::endl;
  //   callback();
  //   std::cout << "End FuncCallback!" << std::endl;
  // }
  // DataCallback( [&](){
  //     std::cout << "This is callback_handler";
  // });

};




/////////////////////////////////main to call home assistan api/////////////////////////////////////
SmartPointer<HAfeiyang> HAfeiyang_(new HAfeiyang());

void setup() {
  Serial.begin(115200);
  HAfeiyang_->begin();

}

//std::function<void(char*, uint8_t*, unsigned int)> callback
void loop() {
  // put your main code here, to run repeatedly:
  HAfeiyang_->loop();
}
