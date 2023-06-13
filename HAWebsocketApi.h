#ifndef __HAWebSocketAPI__H__   
#define __HAWebSocketAPI__H__


// #include <ArduinoJson.h>
#include "StringArray.h"
#include <Arduino.h>
#include <vector>
#include <iostream>
#include <string>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

https://developers.home-assistant.io/docs/api/websocket


using namespace rapidjson;
/////////////////////////////////////////////////Authentication phase/////////////////////////////////////////////////////////
// Authentication message structure
struct AuthMessage {
    std::string type;
    std::string access_token;
};

// Auth required message structure
struct AuthRequiredMessage {
    std::string type;
    std::string ha_version;
};

// Auth OK message structure
struct AuthOkMessage {
    std::string type;
    std::string ha_version;
};

// Auth invalid message structure
struct AuthInvalidMessage {
    std::string type;
    std::string message;
};

// Serialize AuthMessage object to JSON string
std::string serializeAuthMessage(const AuthMessage& authMessage) {
    Document document;
    document.SetObject();
    Document::AllocatorType& allocator = document.GetAllocator();

    document.AddMember("type", Value().SetString(authMessage.type.c_str(), allocator), allocator);
    document.AddMember("access_token", Value().SetString(authMessage.access_token.c_str(), allocator), allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    return buffer.GetString();
}

// Deserialize JSON string to AuthMessage object
AuthMessage deserializeAuthMessage(const std::string& jsonString) {
    AuthMessage authMessage;

    Document document;
    document.Parse(jsonString.c_str());

    authMessage.type = document["type"].GetString();
    authMessage.access_token = document["access_token"].GetString();

    return authMessage;
}

// Serialize AuthRequiredMessage object to JSON string
std::string serializeAuthRequiredMessage(const AuthRequiredMessage& authRequiredMessage) {
    Document document;
    document.SetObject();
    Document::AllocatorType& allocator = document.GetAllocator();

    document.AddMember("type", Value().SetString(authRequiredMessage.type.c_str(), allocator), allocator);
    document.AddMember("ha_version", Value().SetString(authRequiredMessage.ha_version.c_str(), allocator), allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    return buffer.GetString();
}

// Deserialize JSON string to AuthRequiredMessage object
AuthRequiredMessage deserializeAuthRequiredMessage(const std::string& jsonString) {
    AuthRequiredMessage authRequiredMessage;

    Document document;
    document.Parse(jsonString.c_str());

    authRequiredMessage.type = document["type"].GetString();
    authRequiredMessage.ha_version = document["ha_version"].GetString();

    return authRequiredMessage;
}

// Serialize AuthOkMessage object to JSON string
std::string serializeAuthOkMessage(const AuthOkMessage& authOkMessage) {
    Document document;
    document.SetObject();
    Document::AllocatorType& allocator = document.GetAllocator();

    document.AddMember("type", Value().SetString(authOkMessage.type.c_str(), allocator), allocator);
    document.AddMember("ha_version", Value().SetString(authOkMessage.ha_version.c_str(), allocator), allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    return buffer.GetString();
}

// Deserialize JSON string to AuthOkMessage object
AuthOkMessage deserializeAuthOkMessage(const std::string& jsonString) {
    AuthOkMessage authOkMessage;

    Document document;
    document.Parse(jsonString.c_str());

    authOkMessage.type = document["type"].GetString();
    authOkMessage.ha_version = document["ha_version"].GetString();

    return authOkMessage;
}

// Serialize AuthInvalidMessage object to JSON string
std::string serializeAuthInvalidMessage(const AuthInvalidMessage& authInvalidMessage) {
    Document document;
    document.SetObject();
    Document::AllocatorType& allocator = document.GetAllocator();

    document.AddMember("type", Value().SetString(authInvalidMessage.type.c_str(), allocator), allocator);
    document.AddMember("message", Value().SetString(authInvalidMessage.message.c_str(), allocator), allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    return buffer.GetString();
}

// Deserialize JSON string to AuthInvalidMessage object
AuthInvalidMessage deserializeAuthInvalidMessage(const std::string& jsonString) {
  AuthInvalidMessage authInvalidMessage;

  Document document;
  document.Parse(jsonString.c_str());

  authInvalidMessage.type = document["type"].GetString();
  authInvalidMessage.message = document["message"].GetString();

  return authInvalidMessage;
}

/**
int main() {
// Example usage

// Serialize AuthMessage object
AuthMessage authMessage;
authMessage.type = "auth";
authMessage.access_token = "ABCDEFGH";
std::string serializedAuthMessage = serializeAuthMessage(authMessage);
std::cout << "Serialized AuthMessage: " << serializedAuthMessage << std::endl;

// Deserialize JSON string to AuthMessage object
AuthMessage deserializedAuthMessage = deserializeAuthMessage(serializedAuthMessage);
std::cout << "Deserialized AuthMessage: type = " << deserializedAuthMessage.type
    << ", access_token = " << deserializedAuthMessage.access_token << std::endl;

// Serialize AuthRequiredMessage object
AuthRequiredMessage authRequiredMessage;
authRequiredMessage.type = "auth_required";
authRequiredMessage.ha_version = "2021.5.3";
std::string serializedAuthRequiredMessage = serializeAuthRequiredMessage(authRequiredMessage);
std::cout << "Serialized AuthRequiredMessage: " << serializedAuthRequiredMessage << std::endl;

// Deserialize JSON string to AuthRequiredMessage object
AuthRequiredMessage deserializedAuthRequiredMessage = deserializeAuthRequiredMessage(serializedAuthRequiredMessage);
std::cout << "Deserialized AuthRequiredMessage: type = " << deserializedAuthRequiredMessage.type
    << ", ha_version = " << deserializedAuthRequiredMessage.ha_version << std::endl;

// Serialize AuthOkMessage object
AuthOkMessage authOkMessage;
authOkMessage.type = "auth_ok";
authOkMessage.ha_version = "2021.5.3";
std::string serializedAuthOkMessage = serializeAuthOkMessage(authOkMessage);
std::cout << "Serialized AuthOkMessage: " << serializedAuthOkMessage << std::endl;

// Deserialize JSON string to AuthOkMessage object
AuthOkMessage deserializedAuthOkMessage = deserializeAuthOkMessage(serializedAuthOkMessage);
std::cout << "Deserialized AuthOkMessage: type = " << deserializedAuthOkMessage.type
    << ", ha_version = " << deserializedAuthOkMessage.ha_version << std::endl;

// Serialize AuthInvalidMessage object
AuthInvalidMessage authInvalidMessage;
authInvalidMessage.type = "auth_invalid";
authInvalidMessage.message = "Invalid password";
std::string serializedAuthInvalidMessage = serializeAuthInvalidMessage(authInvalidMessage);
std::cout << "Serialized AuthInvalidMessage: " << serializedAuthInvalidMessage << std::endl;

// Deserialize JSON string to AuthInvalidMessage object
AuthInvalidMessage deserializedAuthInvalidMessage = deserializeAuthInvalidMessage(serializedAuthInvalidMessage);
std::cout << "Deserialized AuthInvalidMessage: type = " << deserializedAuthInvalidMessage.type
    << ", message = " << deserializedAuthInvalidMessage.message << std::endl;

return 0;
}
*/
/////////////////////////////////////////////////Command phase/////////////////////////////////////////////////////////
// Result message structure
struct ResultMessage {
    int id;
    std::string type;
    bool success;
    struct ResultContext {
        std::string id;
        std::string parent_id;
        std::string user_id;
    } result;
};

// Serialize ResultMessage object to JSON string
std::string serializeResultMessage(const ResultMessage& resultMessage) {
    Document document;
    document.SetObject();
    Document::AllocatorType& allocator = document.GetAllocator();

    document.AddMember("id", resultMessage.id, allocator);
    document.AddMember("type", Value().SetString(resultMessage.type.c_str(), allocator), allocator);
    document.AddMember("success", resultMessage.success, allocator);

    Value resultObject(kObjectType);
    resultObject.AddMember("id", Value().SetString(resultMessage.result.id.c_str(), allocator), allocator);
    resultObject.AddMember("parent_id", Value().SetString(resultMessage.result.parent_id.c_str(), allocator), allocator);
    resultObject.AddMember("user_id", Value().SetString(resultMessage.result.user_id.c_str(), allocator), allocator);

    document.AddMember("result", resultObject, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    return buffer.GetString();
}

// Deserialize JSON string to ResultMessage object
ResultMessage deserializeResultMessage(const std::string& jsonString) {
    ResultMessage resultMessage;

    Document document;
    document.Parse(jsonString.c_str());

    resultMessage.id = document["id"].GetInt();
    resultMessage.type = document["type"].GetString();
    resultMessage.success = document["success"].GetBool();

    const Value& resultObject = document["result"];
    resultMessage.result.id = resultObject["id"].GetString();
    resultMessage.result.parent_id = resultObject["parent_id"].GetString();
    resultMessage.result.user_id = resultObject["user_id"].GetString();

    return resultMessage;
}
/*
int main() {
    // Example usage

    // Serialize ResultMessage object
    ResultMessage resultMessage;
    resultMessage.id = 6;
    resultMessage.type = "result";
    resultMessage.success = true;
    resultMessage.result.id = "326ef27d19415c60c492fe330945f954";
    resultMessage.result.parent_id = nullptr;  // Set to nullptr as an example, can be modified accordingly
    resultMessage.result.user_id = "31ddb597e03147118cf8d2f8fbea5553";
    std::string serializedResultMessage = serializeResultMessage(resultMessage);
    std::cout << "Serialized ResultMessage: " << serializedResultMessage << std::endl;

    // Deserialize JSON string to ResultMessage object
    ResultMessage deserializedResultMessage = deserializeResultMessage(serializedResultMessage);
    std::cout << "Deserialized ResultMessage: id = " << deserializedResultMessage.id
        << ", type = " << deserializedResultMessage.type
        << ", success = " << deserializedResultMessage.success
        << ", result id = " << deserializedResultMessage.result.id
        << ", result parent_id = " << deserializedResultMessage.result.parent_id
        << ", result user_id = " << deserializedResultMessage.result.user_id << std::endl;

    return 0;
}
*/
/////////////////////////////////////////////////Subscribe to events/////////////////////////////////////////////////////////

// Event message structure
struct EventMessage {
    int id;
    std::string type;
    struct EventData {
        struct EntityState {
            std::string entity_id;
            std::string last_changed;
            std::string state;
            struct Attributes {
                std::vector<int> rgb_color;
                int color_temp;
                int supported_features;
                std::vector<double> xy_color;
                int brightness;
                int white_value;
                std::string friendly_name;
            } attributes;
            std::string last_updated;
            struct EventContext {
                std::string id;
                std::string parent_id;
                std::string user_id;
            } context;
        } new_state;
        struct EntityState old_state;
    } event;
    std::string event_type;
    std::string time_fired;
    std::string origin;
    struct EventContext {
        std::string id;
        std::string parent_id;
        std::string user_id;
    } context;
};

// Serialize EventMessage object to JSON string
std::string serializeEventMessage(const EventMessage& eventMessage) {
    Document document;
    document.SetObject();
    Document::AllocatorType& allocator = document.GetAllocator();

    document.AddMember("id", eventMessage.id, allocator);
    document.AddMember("type", Value().SetString(eventMessage.type.c_str(), allocator), allocator);

    Value eventDataObject(kObjectType);

    Value new_stateObject(kObjectType);
    new_stateObject.AddMember("entity_id", Value().SetString(eventMessage.event.new_state.entity_id.c_str(), allocator), allocator);
    new_stateObject.AddMember("last_changed", Value().SetString(eventMessage.event.new_state.last_changed.c_str(), allocator), allocator);
    new_stateObject.AddMember("state", Value().SetString(eventMessage.event.new_state.state.c_str(), allocator), allocator);

    Value attributesObject(kObjectType);
    Value rgb_colorArray(kArrayType);
    for (const auto& value : eventMessage.event.new_state.attributes.rgb_color) {
        rgb_colorArray.PushBack(value, allocator);
    }
    attributesObject.AddMember("rgb_color", rgb_colorArray, allocator);
    attributesObject.AddMember("color_temp", eventMessage.event.new_state.attributes.color_temp, allocator);
    attributesObject.AddMember("supported_features", eventMessage.event.new_state.attributes.supported_features, allocator);

    Value xy_colorArray(kArrayType);
    for (const auto& value : eventMessage.event.new_state.attributes.xy_color) {
        xy_colorArray.PushBack(value, allocator);
    }
    attributesObject.AddMember("xy_color", xy_colorArray, allocator);

    attributesObject.AddMember("brightness", eventMessage.event.new_state.attributes.brightness, allocator);
    attributesObject.AddMember("white_value", eventMessage.event.new_state.attributes.white_value, allocator);
    attributesObject.AddMember("friendly_name", Value().SetString(eventMessage.event.new_state.attributes.friendly_name.c_str(), allocator), allocator);

    new_stateObject.AddMember("attributes", attributesObject, allocator);
    new_stateObject.AddMember("last_updated", Value().SetString(eventMessage.event.new_state.last_updated.c_str(), allocator), allocator);

    Value new_stateContextObject(kObjectType);
    new_stateContextObject.AddMember("id", Value().SetString(eventMessage.event.new_state.context.id.c_str(), allocator), allocator);
    new_stateContextObject.AddMember("parent_id", Value().SetString(eventMessage.event.new_state.context.parent_id.c_str(), allocator     ), allocator);
    new_stateObject.AddMember("context", new_stateContextObject, allocator);

    eventDataObject.AddMember("new_state", new_stateObject, allocator);

    Value old_stateObject(kObjectType);
    old_stateObject.AddMember("entity_id", Value().SetString(eventMessage.event.old_state.entity_id.c_str(), allocator), allocator);
    old_stateObject.AddMember("last_changed", Value().SetString(eventMessage.event.old_state.last_changed.c_str(), allocator), allocator);
    old_stateObject.AddMember("state", Value().SetString(eventMessage.event.old_state.state.c_str(), allocator), allocator);

    Value old_stateAttributesObject(kObjectType);
    old_stateAttributesObject.AddMember("supported_features", eventMessage.event.old_state.attributes.supported_features, allocator);
    old_stateAttributesObject.AddMember("friendly_name", Value().SetString(eventMessage.event.old_state.attributes.friendly_name.c_str(), allocator), allocator);

    old_stateObject.AddMember("attributes", old_stateAttributesObject, allocator);
    old_stateObject.AddMember("last_updated", Value().SetString(eventMessage.event.old_state.last_updated.c_str(), allocator), allocator);

    Value old_stateContextObject(kObjectType);
    old_stateContextObject.AddMember("id", Value().SetString(eventMessage.event.old_state.context.id.c_str(), allocator), allocator);
    old_stateContextObject.AddMember("parent_id", Value().SetString(eventMessage.event.old_state.context.parent_id.c_str(), allocator), allocator);
    old_stateContextObject.AddMember("user_id", Value().SetString(eventMessage.event.old_state.context.user_id.c_str(), allocator), allocator);

    old_stateObject.AddMember("context", old_stateContextObject, allocator);

    eventDataObject.AddMember("old_state", old_stateObject, allocator);

    document.AddMember("event", eventDataObject, allocator);
    document.AddMember("event_type", Value().SetString(eventMessage.event_type.c_str(), allocator), allocator);
    document.AddMember("time_fired", Value().SetString(eventMessage.time_fired.c_str(), allocator), allocator);
    document.AddMember("origin", Value().SetString(eventMessage.origin.c_str(), allocator), allocator);

    Value contextObject(kObjectType);
    contextObject.AddMember("id", Value().SetString(eventMessage.context.id.c_str(), allocator), allocator);
    contextObject.AddMember("parent_id", Value().SetString(eventMessage.context.parent_id.c_str(), allocator), allocator);
    contextObject.AddMember("user_id", Value().SetString(eventMessage.context.user_id.c_str(), allocator), allocator);

    document.AddMember("context", contextObject, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    return buffer.GetString();
}


// Deserialize JSON string to EventMessage object
EventMessage deserializeEventMessage(const std::string& jsonString) {
    EventMessage eventMessage;

    Document document;
    document.Parse(jsonString.c_str());

    eventMessage.id = document["id"].GetInt();
    eventMessage.type = document["type"].GetString();

    const Value& eventDataObject = document["event"];
    const Value& new_stateObject = eventDataObject["new_state"];

    eventMessage.event.new_state.entity_id = new_stateObject["entity_id"].GetString();
    eventMessage.event.new_state.last_changed = new_stateObject["last_changed"].GetString();
    eventMessage.event.new_state.state = new_stateObject["state"].GetString();

    const Value& attributesObject = new_stateObject["attributes"];
    const Value& rgb_colorArray = attributesObject["rgb_color"];
    for (const auto& value : rgb_colorArray.GetArray()) {
        eventMessage.event.new_state.attributes.rgb_color.push_back(value.GetInt());
    }
    eventMessage.event.new_state.attributes.color_temp = attributesObject["color_temp"].GetInt();
    eventMessage.event.new_state.attributes.supported_features = attributesObject["supported_features"].GetInt();

    const Value& xy_colorArray = attributesObject["xy_color"];
    for (const auto& value : xy_colorArray.GetArray()) {
        eventMessage.event.new_state.attributes.xy_color.push_back(value.GetDouble());
    }

    eventMessage.event.new_state.attributes.brightness = attributesObject["brightness"].GetInt();
    eventMessage.event.new_state.attributes.white_value = attributesObject["white_value"].GetInt();
    eventMessage.event.new_state.attributes.friendly_name = attributesObject["friendly_name"].GetString();

    eventMessage.event.new_state.last_updated = new_stateObject["last_updated"].GetString();

    const Value& new_stateContextObject = new_stateObject["context"];
    eventMessage.event.new_state.context.id = new_stateContextObject["id"].GetString();
    eventMessage.event.new_state.context.parent_id = new_stateContextObject["parent_id"].GetString();
    eventMessage.event.new_state.context.user_id = new_stateContextObject["user_id"].GetString();

    const Value& old_stateObject = eventDataObject["old_state"];

    eventMessage.event.old_state.entity_id = old_stateObject["entity_id"].GetString();
    eventMessage.event.old_state.last_changed = old_stateObject["last_changed"].GetString();
    eventMessage.event.old_state.state = old_stateObject["state"].GetString();

    const Value& old_stateAttributesObject = old_stateObject["attributes"];
    eventMessage.event.old_state.attributes.supported_features = old_stateAttributesObject["supported_features"].GetInt();
    eventMessage.event.old_state.attributes.friendly_name = old_stateAttributesObject["friendly_name"].GetString();

    eventMessage.event.old_state.last_updated = old_stateObject["last_updated"].GetString();

    const Value& old_stateContextObject = old_stateObject["context"];
    eventMessage.event.old_state.context.id = old_stateContextObject["id"].GetString();
    eventMessage.event.old_state.context.parent_id = old_stateContextObject["parent_id"].GetString();
    eventMessage.event.old_state.context.user_id = old_stateContextObject["user_id"].GetString();

    eventMessage.event_type = document["event_type"].GetString();
    eventMessage.time_fired = document["time_fired"].GetString();
    eventMessage.origin = document["origin"].GetString();

    const Value& contextObject = document["context"];
    eventMessage.context.id = contextObject["id"].GetString();
    eventMessage.context.parent_id = contextObject["parent_id"].GetString();
    eventMessage.context.user_id = contextObject["user_id"].GetString();

    return eventMessage;
}

/*
int main() {
    // Example serialization
    EventMessage eventMessage;
    eventMessage.id = 18;
    eventMessage.type = "event";
    eventMessage.event.new_state.entity_id = "light.bed_light";
    // Set other fields of the eventMessage object...

    std::string serializedJson = serializeEventMessage(eventMessage);
    std::cout << "Serialized JSON: " << serializedJson << std::endl;

    // Example deserialization
    std::string json = R"({
        "id": 18,
        "type": "event",
        "event": {
            "new_state": {
                "entity_id": "light.bed_light",
                // Set other fields of the new_state object...
            },
            "old_state": {
                "entity_id": "light.bed_light",
                // Set other fields of the old_state object...
            }
        },
        "event_type": "state_changed",
        // Set other fields of the eventMessage object...
    })";

    EventMessage deserializedEventMessage = deserializeEventMessage(json);
    std::cout << "Deserialized Event Message:" << std::endl;
    std::cout << "ID: " << deserializedEventMessage.id << std::endl;
    std::cout << "Type: " << deserializedEventMessage.type << std::endl;
    std::cout << "Event Type: " << deserializedEventMessage.event_type << std::endl;
    std::cout << "Time Fired: " << deserializedEventMessage.time_fired << std::endl;
    std::cout << "Origin: " << deserializedEventMessage.origin << std::endl;
    std::cout << "New State Entity ID: " << deserializedEventMessage.event.new_state.entity_id << std::endl;
    // Print other fields of the deserializedEventMessage object...

return 0;
}
*/


#endif
