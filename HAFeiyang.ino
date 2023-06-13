#include <ESP8266WiFi.h>
#include <iostream>
#include <WebSocketsClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <PubSubClient.h>
#include "SmartPointer.h"
#include "HAHttpApi.h"


// WiFi参数
const char* ssid = "Dafu9039";
const char* password = "moto1984";

// Home Assistant WebSocket参数
const char* haHost = "edgetest.thundercomm.com";
const int haPort = 4089;
const char* haAccessToken = "uMEh5o4vxTMTmWaPpspn3mkGWb95Yxu9tplK1Y9b3ZY";

//mqtt 客户端和服务器
const char* mqttBroker = "mqtt1.feiyangkeji.com";
const int mqttPort = 1884;
const char* mqttClientId = "u5Yxu9tplK1Y9b3ZY";
const char* mqttTopic = "/HAFeiyang/tts";

// WebSocket客户端和HTTP服务器
WebSocketsClient webSocket;
ESP8266WebServer server(80);


/////////////////////////////////HTTPClient to call home assistan api/////////////////////////////////////
// Here's a breakdown of the class and its methods:

// The class has private member variables WiFiClientSecure client, const char* host, int port, and const char* accessToken to store the connection details.
// The private method extractHostnameAndEndpoint extracts the hostname and endpoint from a given URL.
// The public method begin initializes the connection details.
// The method check_api_status sends a GET request to the specified API endpoint and returns the message from the response.
// The method get_configuration sends a GET request to the specified API endpoint and returns the configuration as a JSON object.
// The method get_events sends a GET request to the specified API endpoint and returns an array of event objects.
// The method get_services sends a GET request to the specified API endpoint and returns an array of service objects.
// The method get_HistoryPeriod sends a GET request to the specified API endpoint with optional parameters and returns a history period object.
// The method get_LogbookEntries sends a GET request to the specified API endpoint with optional parameters and returns a logbook object.
// The method get_states sends a GET request to the specified API endpoint and returns an array of state objects.
// The method callService sends a POST request to the specified API endpoint with service data and returns the response as a JSON object.
// The method get_entity_state sends a GET request to the specified API endpoint with an entity ID and returns the state of the entity as a state object.
// The method get_error_log sends a GET request to the specified API endpoint and returns the error log entries as a vector of ErrorLogEntry objects.
// The method get_camera_image sends a GET request to the specified API endpoint to retrieve camera image data.
// The method get_Calendars sends a GET request to the specified API endpoint and returns the list of calendar entities as a JSON object.
// The method get_CalendarEvents sends a GET request to the specified API endpoint with calendar and time parameters and returns the list of calendar events as a CalendarEventsEntity object.
// The method updateOrCreateState sends a POST request to the specified API endpoint with entity ID, state, and attributes and returns the updated state as a statesEntity object.
// The method post_change_states sends a POST request to the specified API endpoint with domain, service, and entity ID and returns the changed states as a statesEntity object.
// The method render_template sends a POST request to the specified API endpoint with a template string and returns the rendered template as a string.
// The method check_config sends a POST request to the specified API endpoint to check the configuration and returns the check result as a CheckConfigResult object.
// The method handle_intent sends a POST request to the specified API endpoint with intent name and data and returns the intent result as an IntentResult object.
// The private methods get and post handle the actual HTTP requests using the WiFiClientSecure library.
class HTTPClient {
private:
  WiFiClientSecure client;
  const char* host;
  int port;
  const char* accessToken;
private:
  void extractHostnameAndEndpoint(const String& url, String& hostname, String& endpoint) {
    int protocolIndex = url.indexOf("://");
    int pathIndex = url.indexOf('/', protocolIndex + 3);

    if (protocolIndex == -1 || pathIndex == -1) {
      Serial.println("Invalid URL format");
      return;
    }

    hostname = url.substring(protocolIndex + 3, pathIndex);
    endpoint = url.substring(pathIndex);
  }
public:
  HTTPClient() {}
  void begin(const char* _host, int _port, const char* _accessToken) {
    host = _host;
    port = _port;
    accessToken = _accessToken;
  }
  
  //Returns a message if the API is up and running.
  String check_api_status() {
    const char* path = "api/";

    DynamicJsonDocument response = get(path);

    String message = response["message"].as<String>();

    return message;
}

//Returns the current configuration as JSON.
Config get_configuration() {
  const char* path = "api/config";

  DynamicJsonDocument response = get(path);
  Config config;
  config.parse(response);

  return config;
}

//Returns an array of event objects. Each event object contains event name and listener count.
 EventArray get_events() {
  const char* path = "api/events";

  DynamicJsonDocument response = get(path);
  EventArray events;
  events.parse(response);


  return events;
}

  // GET请求示例：获取服务数组
  ServiceArray get_services() {
    const char* path = "api/services";

    DynamicJsonDocument response = get(path);
    ServiceArray service;
    service.parse(response);

    return service;
  }

  // 获取历史记录的GET请求示例
   HistoryPeriod get_HistoryPeriod(const char* timestamp, const char* entityIds, const char* endTime = "", bool minimalResponse = false, bool noAttributes = false, bool significantChangesOnly = false) {
    char path[200];
    snprintf(path, sizeof(path), "/api/history/period/%s?filter_entity_id=%s&end_time=%s&minimal_response=%s&no_attributes=%s&significant_changes_only=%s",
             timestamp, entityIds, endTime, minimalResponse ? "true" : "false", noAttributes ? "true" : "false", significantChangesOnly ? "true" : "false");
    DynamicJsonDocument response =  get(path);
    HistoryPeriod historyPeriod;
    historyPeriod.parse(response);
    return historyPeriod;
  }

    // GET请求示例：获取日志记录条目
  Logbook get_LogbookEntries(const char* timestamp, const char* endTime = "", const char* entity = "") {
    char path[200];
    snprintf(path, sizeof(path), "/api/logbook/%s?end_time=%s&entity=%s", timestamp, endTime, entity);
    DynamicJsonDocument response =  get(path);

    Logbook logbook;
    logbook.parse(response);
    return logbook;
  }

  // GET请求示例：获取所有实体状态
  //Returns an array of state objects. Each state has the following attributes: entity_id, state, last_changed and attributes.
State* get_states() {
  const char* path = "api/states";

  DynamicJsonDocument response = get(path);

  if (response.isNull()) {
    return nullptr;  // Error handling: return nullptr or throw an exception
  }

  JsonArray stateArray = response.as<JsonArray>();
  State* states = new State[stateArray.size()];

  int index = 0;
  for (const JsonVariant& stateVariant : stateArray) {
    if (stateVariant.is<JsonObject>()) {
      const JsonObject& stateObject = stateVariant.as<JsonObject>();
      State& state = states[index];
      state.parse(stateObject);
      index++;
    }
  }

  return states;
}


  //Returns a state object for specified entity_id. Returns 404 if not found.
  State get_entity_state(const String& entity_id) {
    String path = "api/states/" + entity_id;
    DynamicJsonDocument response = get((const char*)path.c_str());
    if (response.isNull()) {
      return State();  // Error handling: return an empty EntityState object or throw an exception
    }

    State entityState;
    entityState.parse(response.as<JsonObject>());

    return entityState;
}


//Retrieve all errors logged during the current session of Home Assistant as a plaintext response.
// Vector<ErrorLogEntry> errorLog = get_error_log();

// // Iterate over the error log entries
// for (const ErrorLogEntry& logEntry : errorLog) {
//   String timestamp = logEntry.timestamp;
//   String message = logEntry.message;
//   // ...
// }

std::vector<ErrorLogEntry> get_error_log() {
  String path = "api/error_log";

  // DynamicJsonDocument response = get((const char*)path.c_str());

  // // Split the response into individual log entries
  // std::vector<String> logEntries;
  // response.split('\n', logEntries);

  std::vector<ErrorLogEntry> errorLog;
  // for (const String& logEntry : logEntries) {
  //   ErrorLogEntry errorLogEntry;
  //   errorLogEntry.parse(logEntry);
  //   errorLog.push_back(errorLogEntry);
  // }

  return errorLog;
}

// GET请求示例：获取相机图像数据
void get_camera_image(const char* cameraEntityId, const char* time, const char* token) {
  // const int bufferSize = 200;
  // char path[bufferSize];
  // snprintf(path, bufferSize, "/api/camera_proxy/%s?time=%s", cameraEntityId, time);

  // String authorizationHeader = "Bearer ";
  // authorizationHeader += token;

  // String contentTypeHeader = "Content-Type: application/json";

  // String filename = "image.jpg";

  // if (WiFi.status() == WL_CONNECTED) {
  //   WiFiClient client;

  //   if (client.connect("localhost", 8123)) {
  //     client.print(String("GET ") + path + " HTTP/1.1\r\n" +
  //                  "Host: localhost\r\n" +
  //                  "Authorization: " + authorizationHeader + "\r\n" +
  //                  contentTypeHeader + "\r\n" +
  //                  "Connection: close\r\n\r\n");

  //     bool isHeader = true;
  //     bool isContent = false;

  //     while (client.connected()) {
  //       String line = client.readStringUntil('\n');

  //       if (line == "\r") {
  //         isHeader = false;
  //         isContent = true;
  //       }

  //       if (!isHeader && isContent) {
  //         File imageFile = SPIFFS.open(filename, FILE_WRITE);
  //         if (imageFile) {
  //           imageFile.print(line);
  //           imageFile.close();
  //         }
  //       }
  //     }
  //   }

  //   client.stop();
  // }
}

  //Returns the list of calendar entities.
  CalendarEventsEntity get_Calendars() {
    DynamicJsonDocument response = get("/api/calendars");
    if (response.isNull()) {
      return CalendarEventsEntity();  // Error handling: return an empty EntityState object or throw an exception
    }
    CalendarEventsEntity calendarEvent;
    calendarEvent.parse(response);
    return calendarEvent;

  }

  //Returns the list of calendar events for the specified calendar entity_id between the start and end times (exclusive).
  //The events in the response have a start and end that contain either dateTime or date for an all day event.

  CalendarEventsEntity get_CalendarEvents( const char* calendarEntityId, const char* startTime, const char* endTime) {
    char path[200];
    snprintf(path, sizeof(path), "/api/calendars/%s?start=%s&end=%s", calendarEntityId, startTime, endTime);
    DynamicJsonDocument response = get(path);
    if (response.isNull()) {
      return CalendarEventsEntity();  // Error handling: return an empty EntityState object or throw an exception
    }
    CalendarEventsEntity calendarEvent;
    calendarEvent.parse(response);
    return calendarEvent;

  }

  statesEntity updateOrCreateState(const String& entity_id, const String& state, const JsonObject& attributes) {
    String endpoint = "/api/states/" + entity_id;
    // Build the JSON payload
    StaticJsonDocument<256> jsonDoc;
    jsonDoc["state"] = state;
    jsonDoc["attributes"] = attributes;

    // Serialize the JSON payload
    String data;
    serializeJson(jsonDoc, data);

    DynamicJsonDocument response =  post( (const char*)endpoint.c_str(), (const char*)data.c_str());

    statesEntity entity;
    entity.parse(response);
    return entity;

  }
  
  //Calls a service within a specific domain. Will return when the service has been executed or after 10 seconds, whichever comes first.
  //You can pass an optional JSON object to be used as service_data.
  //The result will include any states that changed while the service was being executed, even if their change was the result of something else happening in the system.
  statesEntity post_change_states(const String& domain, const String& service, const String& entityId) {
    char path[200];
    snprintf(path, sizeof(path), "api/services/%s/%s", domain.c_str(), service.c_str());

    // Build the JSON payload
    StaticJsonDocument<256> jsonDoc;
    jsonDoc["entity_id"] = entityId;

    // Serialize the JSON payload
    String data;
    serializeJson(jsonDoc, data);

    DynamicJsonDocument doc = post(path, data.c_str());
    statesEntity states;

    // Parse the response into the StateChange object
     states.parse(doc);

    return states;
  }

//Render a Home Assistant template. See template docs for more information.https://www.home-assistant.io/docs/configuration/templating
  String render_template(const String& templateString) {
  const char* path = "api/template";

  StaticJsonDocument<256> payload;
  payload["template"] = templateString;

  String payloadStr;
  serializeJson(payload, payloadStr);

  DynamicJsonDocument response = post(path, (const char*)payloadStr.c_str());

  String renderedTemplate = response.as<String>();

  return renderedTemplate;
}

//Trigger a check of configuration.yaml. No additional data needs to be passed in with this request. Needs config integration enabled.
//If the check is successful, the following will be returned
// {
//     "errors": null,
//     "result": "valid"
// }
// If the check fails, the errors attribute in the object will list what caused the check to fail. For example:

// {
//     "errors": "Integration not found: frontend:",
//     "result": "invalid"
// }
CheckConfigResult check_config() {
  const char* path = "api/config/core/check_config";

  DynamicJsonDocument response = post(path, "");

  CheckConfigResult result;
  result.errors = response["errors"].as<String>();
  result.result = response["result"].as<String>();

  return result;
}

// Handle an intent.
// You must add intent: to your configuration.yaml to enable this endpoint.
IntentResult handle_intent(const String& intentName, const JsonObject& intentData) {
  const char* path = "api/intent/handle";

  JsonObject payload;
  payload["name"] = intentName;
  payload["data"] = intentData;

  String payloadStr;
  serializeJson(payload, payloadStr);

  DynamicJsonDocument response = post(path, payloadStr.c_str());

  IntentResult result;
  result.name = response["name"].as<String>();
  result.slots = response["slots"];

  return result;
}


private:
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
  // Global variable to track the next ID
  int nextId = 0;
  // Global variable to track the current ID for subscribe_events command
  int currentSubscribeEventsId = 0;
public:
  FYWebSocketsClient(){}
  // Function to get the next unique ID
  int getNextId() {
    return ++nextId;
  }
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

public:

  void subscribeEvents(const std::string& eventType = "") {
    // Increment the ID for each subscribe_events command
    currentSubscribeEventsId++;

    // Construct the JSON message
    DynamicJsonDocument message(256);
    message["id"] = currentSubscribeEventsId;
    message["type"] = "subscribe_events";
    if (!eventType.empty()) {
      message["event_type"] = eventType;
    }

    String payloadStr;
    serializeJson(message, payloadStr);

    // Send the message
    sendWebSocketMessage((const char*)payloadStr.c_str());
  }

 //You can also subscribe to one or more triggers with subscribe_trigger. These are the same triggers syntax as used for automation triggers. You can define one or a list of triggers.
 //subscribeTrigger("state", "binary_sensor.motion_occupancy", "off", "on");
 void subscribeTrigger(const std::string& platform, const std::string& entityId, const std::string& fromState, const std::string& toState) {
  // Construct the JSON message
  DynamicJsonDocument message(256);
  message["id"] = getNextId();
  message["type"] = "subscribe_trigger";

  // Create the trigger object
  JsonObject trigger = message.createNestedObject("trigger");
  trigger["platform"] = platform;
  trigger["entity_id"] = entityId;
  trigger["from"] = fromState;
  trigger["to"] = toState;

  // Send the message
  sendWebSocketMessage(serializeJson(message));
}
private:
  void handleWebSocketMessage(uint8_t* payload, size_t length) {

    // 处理收到的WebSocket消息的逻辑
    // 调用回调函数处理消息
    if (messageCallback != nullptr) {
      messageCallback(reinterpret_cast<const char*>(payload));
    }

    // Parse the incoming JSON message
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, payload, length);

    // Check if parsing was successful
    if (error) {
      // Error handling
      return;
    }

    // Check the message type
    std::string messageType = message["type"].as<std::string>();
    if (messageType == "result") {
      // Check if the result is for the subscribe_events command
      int messageId = doc["id"];
      if (messageId == currentSubscribeEventsId) {
        bool success = doc["success"];
        if (success) {
          // Subscription is active
          // You can handle the successful subscription here
        } else {
          // Subscription failed
          // You can handle the failed subscription here
        }
      }
    } else if (messageType == "event") {
      // Handle other types of events
      // ...
    }else if (messageType == "trigger") {
      // Handle trigger message
      int messageId = message["id"].as<int>();
      JsonObject trigger = message["event"]["variables"]["trigger"].as<JsonObject>();

      // Extract trigger information
      std::string triggerId = trigger["id"].as<std::string>();
      std::string platform = trigger["platform"].as<std::string>();
      std::string entityId = trigger["entity_id"].as<std::string>();
      // Extract more trigger information as needed

      // Do something with the trigger information
      // Example: Print trigger details
      Serial.println("Received trigger message:");
      Serial.println("Trigger ID: " + triggerId);
      Serial.println("Platform: " + platform);
      Serial.println("Entity ID: " + entityId);
      // Print more trigger details as needed
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
  HAfeiyang() {
      webSocketClient_ = new FYWebSocketsClient();
      mqttClient_ = new MQTTClient();
      httpclient_ = new HTTPClient();
  }
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

 
    // mqtt client api
    mqttClient_->begin(mqttBroker, mqttPort, mqttClientId,[this](const char* topic, byte* payload, unsigned int length)->void {
       webSocketClient_->handleMQTTMessage(topic,payload,length);
    });

   // websocket client api
    webSocketClient_->begin(haHost, haPort, haAccessToken, [this](const char* message)->void {
       mqttClient_->handleMessageFromWebSocket(message);
    });
    // 连接到Home Assistant的WebSocket接口
    webSocketClient_->connectWebSocket();
    // // 发送WebSocket消息
    // const char* message = "Hello, Home Assistant!";
    // webSocketClient_->sendWebSocketMessage(message);

    // // httpclient api
     httpclient_->begin(haHost, haPort, haAccessToken);

    // 




     

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


//   HistoryPeriod history;
// DynamicJsonDocument response = getHistoryPeriod(timestamp, entityIds, endTime, minimalResponse, noAttributes, significantChangesOnly);
// history.parse(response);

// // 现在可以访问history对象的属性
// for (size_t i = 0; i < history.entityIds.size(); i++) {
//   Serial.println("Entity ID: " + history.entityIds[i]);
//   Serial.println("Attributes: " + history.attributes[i]);
//   Serial.println("Last Changed: " + history.lastChanged[i]);
//   Serial.println("Last Updated: " + history.lastUpdated[i]);
//   Serial.println("State: " + history.states[i]);
// }

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


// HTTPClient httpClient;
// httpClient.begin("localhost", 8123, "YOUR_ACCESS_TOKEN");

// DynamicJsonDocument response = httpClient.getApiStatus();
// Serial.println(response["message"].as<String>());

}

//std::function<void(char*, uint8_t*, unsigned int)> callback
void loop() {
  // put your main code here, to run repeatedly:
  HAfeiyang_->loop();
}
