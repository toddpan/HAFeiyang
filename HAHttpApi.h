#ifndef __HAHTTPAPI__H__   
#define __HAHTTPAPI__H__

#include <ArduinoJson.h>
#include "StringArray.h"

#include <ArduinoJson.h>
class Config {
public:
  StringArray components;
  String configDir;
  int elevation;
  double latitude;
  String locationName;
  double longitude;
  String timeZone;
  struct {
    String length;
    String mass;
    String temperature;
    String volume;
  } unitSystem;
  String version;
  StringArray whitelistExternalDirs;

  void parse(const DynamicJsonDocument& json) {
    // Parse components
    const JsonObjectConst& componentsJson = json["components"].as<JsonObjectConst>();
    for (JsonObjectConst::iterator it = componentsJson.begin(); it != componentsJson.end(); ++it) {
      // 获取键和值
      const char* key = it->key().c_str();
      const JsonVariantConst& value = it->value();
      components.add(value.as<String>());
    }

    // Parse other properties
    configDir = json["config_dir"].as<String>();
    elevation = json["elevation"].as<int>();
    latitude = json["latitude"].as<double>();
    locationName = json["location_name"].as<String>();
    longitude = json["longitude"].as<double>();
    timeZone = json["time_zone"].as<String>();

    const JsonObjectConst& unitSystemJson = json["unit_system"].as<JsonObjectConst>();
    unitSystem.length = unitSystemJson["length"].as<String>();
    unitSystem.mass = unitSystemJson["mass"].as<String>();
    unitSystem.temperature = unitSystemJson["temperature"].as<String>();
    unitSystem.volume = unitSystemJson["volume"].as<String>();

    version = json["version"].as<String>();

    const JsonObjectConst& whitelistExternalDirsJson = json["whitelist_external_dirs"].as<JsonObjectConst>();
    for (JsonObjectConst::iterator it = whitelistExternalDirsJson.begin(); it != whitelistExternalDirsJson.end(); ++it) {
      // 获取键和值
      const char* key = it->key().c_str();
      const JsonVariantConst& value = it->value();
      whitelistExternalDirs.add(value.as<String>());
    }

  }
};
///////////////////////////////////Event//////////////////////////////////////////////
class Event {
public:
  String eventName;
  int listenerCount;

  Event(const String& name, int count) : eventName(name), listenerCount(count) {}
};

class EventArray {
public:
  std::vector<Event> events;

  void parse(const DynamicJsonDocument& json) {
    if (json.is<JsonArray>()) {
      const JsonObjectConst& jsonArray = json.as<JsonObjectConst>();
    //   for (const auto& eventJson : jsonArray) {
    //     if (eventJson.is<JsonObject>()) {
    //       const JsonObject& eventObj = eventJson.as<JsonObject>();
    //       String eventName = eventObj["event"].as<String>();
    //       int listenerCount = eventObj["listener_count"].as<int>();
    //       events.emplace_back(eventName, listenerCount);
    //     }
    //   }
     }
  }
};
#endif
