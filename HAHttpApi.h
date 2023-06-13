#ifndef __HAHTTPAPI__H__   
#define __HAHTTPAPI__H__


#include "StringArray.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <vector>

//https://developers.home-assistant.io/docs/api/rest/
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

  Event() {}
};

class EventArray {
public:
  std::vector<Event> events;

  void parse( DynamicJsonDocument& json) {
    if (json.is<JsonArray>()) {
      JsonArray eventArray = json.as<JsonArray>();
      for (const JsonVariant& eventVariant : eventArray) {
        if (eventVariant.is<JsonObject>()) {
          const JsonObject& eventObject = eventVariant.as<JsonObject>();
          Event event;
          event.eventName = eventObject["event"].as<String>();
          event.listenerCount = eventObject["listener_count"].as<int>();
          events.push_back(event);
        }
      }
    }
  }
};
///////////////////////////////////Service//////////////////////////////////////////////
class Service {
public:
  String domain;
  StringArray services;

  void parse(const  JsonObject& json) {
    domain = json["domain"].as<String>();
    
    const JsonArray& servicesArray = json["services"].as<JsonArray>();
    for (const JsonVariant& eventVariant : servicesArray) {
        services.add(eventVariant.as<String>());
    }
  }
};

class ServiceArray {
public:
  std::vector<Service> services;

  void parse( DynamicJsonDocument& json) {
    if (json.is<JsonArray>()) {
      JsonArray eventArray = json.as<JsonArray>();
      for (const JsonVariant& eventVariant : eventArray) {
        if (eventVariant.is<JsonObject>()) {
          const JsonObject& eventObject = eventVariant.as<JsonObject>();
          Service service;
          service.parse(eventObject);
          services.push_back(service);
        }
      }
    }
  }
};

///////////////////////////////////HistoryPeriod//////////////////////////////////////////////
class HistoryPeriod {
public:
  StringArray entityIds;
  StringArray attributes;
  StringArray lastChanged;
  StringArray lastUpdated;
  StringArray states;

  void parse( DynamicJsonDocument& json) {
    // Clear previous data
    entityIds.clear();
    attributes.clear();
    lastChanged.clear();
    lastUpdated.clear();
    states.clear();

    // Parse history periods
    for (const JsonVariant& period : json.as<JsonArray>()) {
      if (period.is<JsonArray>()) {
        const JsonArray& periodArray = period.as<JsonArray>();
        if (periodArray.size() > 0) {
          const JsonObject& entity = periodArray[0].as<JsonObject>();
          entityIds.add(entity["entity_id"].as<String>());
          attributes.add(entity["attributes"].as<String>());
          lastChanged.add(entity["last_changed"].as<String>());
          lastUpdated.add(entity["last_updated"].as<String>());
          states.add(entity["state"].as<String>());
        }
      }
    }
  }
};
///////////////////////////////////LogbookEntry//////////////////////////////////////////////
class LogbookEntry {
public:
  String contextUserId;
  String domain;
  String entityId;
  String message;
  String name;
  String when;

  void parse(const JsonObject& json) {
    contextUserId = json["context_user_id"].as<String>();
    domain = json["domain"].as<String>();
    entityId = json["entity_id"].as<String>();
    message = json["message"].as<String>();
    name = json["name"].as<String>();
    when = json["when"].as<String>();
  }
};

class Logbook {
public:
  StringArray entityIds;
  StringArray messages;

  void parse(DynamicJsonDocument& json) {
    // Clear previous data
    entityIds.clear();
    messages.clear();

    // Parse logbook entries
    for (const JsonVariant& entry : json.as<JsonArray>()) {
      if (entry.is<JsonObject>()) {
        const JsonObject& entryObject = entry.as<JsonObject>();
        LogbookEntry logbookEntry;
        logbookEntry.parse(entryObject);
        entityIds.add(logbookEntry.entityId);
        messages.add(logbookEntry.message);
      }
    }
  }

};
// // Create an instance of the HTTPClient class
// HTTPClient httpClient;

// // Set the host, port, and access token
// const char* host = "localhost";
// int port = 8123;
// const char* accessToken = "YOUR_TOKEN";

// // Initialize the HTTP client
// httpClient.begin(host, port, accessToken);

// // Define the timestamp, end time, and entity ID
// const char* timestamp = "2016-12-29T00:00:00+02:00";
// const char* endTime = "2099-12-31T00:00:00+02:00";
// const char* entity = "sensor.temperature";

// // Get logbook entries
// DynamicJsonDocument entries = httpClient.getLogbookEntries(timestamp, endTime, entity);

// // Process the logbook entries
// for (const auto& entry : entries.as<JsonArray>()) {
//   // Access the logbook entry data
//   const char* contextUserId = entry["context_user_id"].as<const char*>();
//   const char* domain = entry["domain"].as<const char*>();
//   const char* entityId = entry["entity_id"].as<const char*>();
//   const char* message = entry["message"].as<const char*>();
//   const char* name = entry["name"].as<const char*>();
//   const char* when = entry["when"].as<const char*>();

//   // Do something with the logbook entry data
//   Serial.println("Context User ID: " + String(contextUserId));
//   Serial.println("Domain: " + String(domain));
//   Serial.println("Entity ID: " + String(entityId));
//   Serial.println("Message: " + String(message));
//   Serial.println("Name: " + String(name));
//   Serial.println("When: " + String(when));

//   // ...
// }

class State {
public:
public:
  String entity_id;
  String state;
  String last_changed;
  String last_updated;
  JsonObject attributes;

  void parse(const JsonObject& json) {
    entity_id = json["entity_id"].as<String>();
    state = json["state"].as<String>();
    last_changed = json["last_changed"].as<String>();
    last_updated = json["last_updated"].as<String>();
    attributes = json["attributes"].as<JsonObject>();
  }
};

class statesEntity {
public:
  std::vector<State> states;

  void parse(DynamicJsonDocument& json) {
    JsonArray stateArray = json.as<JsonArray>();
    states.clear();

    for (const JsonVariant& stateVariant : stateArray) {
      if (stateVariant.is<JsonObject>()) {
        const JsonObject& stateObject = stateVariant.as<JsonObject>();
        State state;
        state.parse(stateObject);
        states.push_back(state);
      }
    }
  }
};
///////////////////////////////////CalendarManager//////////////////////////////////////////////
class Calendar {
public:
  String entity_id;
  String name;

  void parse(const JsonObject& json) {
    entity_id = json["entity_id"].as<String>();
    name = json["name"].as<String>();
  }
};

class CalendarsEntity {
public:
  std::vector<Calendar> calendars;

  void parse( DynamicJsonDocument& json) {
    JsonArray calendarArray = json.as<JsonArray>();
    calendars.clear();

    for (const JsonVariant& calendarVariant : calendarArray) {
      if (calendarVariant.is<JsonObject>()) {
        const JsonObject& calendarObject = calendarVariant.as<JsonObject>();
        Calendar calendar;
        calendar.parse(calendarObject);
        calendars.push_back(calendar);
      }
    }
  }

};

//  // Initialize the CalendarManager and HTTPClient
//   CalendarManager calendarManager;
//   HTTPClient httpClient;
//   httpClient.begin("localhost", 8123, "YOUR_TOKEN");

//   // Get the list of calendars and print the information
//   calendarManager.getCalendars(httpClient, "YOUR_TOKEN");

//   for (const Calendar& calendar : calendarManager.calendars) {
//     Serial.println("Entity ID: " + calendar.entity_id);
//     Serial.println("Name: " + calendar.name);
//     Serial.println();
//   }
class CalendarEvent {
public:
  String summary;
  String startDateTime;
  String startDate;
  String endDateTime;
  String endDate;
  String description;
  String location;

  void parse(const JsonObject& json) {
    summary = json["summary"].as<String>();

    if (json["start"].containsKey("dateTime")) {
      startDateTime = json["start"]["dateTime"].as<String>();
    } else if (json["start"].containsKey("date")) {
      startDate = json["start"]["date"].as<String>();
    }

    if (json["end"].containsKey("dateTime")) {
      endDateTime = json["end"]["dateTime"].as<String>();
    } else if (json["end"].containsKey("date")) {
      endDate = json["end"]["date"].as<String>();
    }

    if (json.containsKey("description")) {
      description = json["description"].as<String>();
    }

    if (json.containsKey("location")) {
      location = json["location"].as<String>();
    }
  }
};

class CalendarEventsEntity {
public:
  std::vector<CalendarEvent> events;

  void parse( DynamicJsonDocument& json) {
    JsonArray eventArray = json.as<JsonArray>();
    events.clear();

    for (const JsonVariant& eventVariant : eventArray) {
      if (eventVariant.is<JsonObject>()) {
        const JsonObject& eventObject = eventVariant.as<JsonObject>();
        CalendarEvent event;
        event.parse(eventObject);
        events.push_back(event);
      }
    }
  }

};
  // // Initialize the CalendarEventManager and HTTPClient
  // CalendarEventManager calendarEventManager;
  // HTTPClient httpClient;
  // httpClient.begin("localhost", 8123, "YOUR_TOKEN");

  // // Get the calendar events for the specified calendar entity_id and time range
  // calendarEventManager.getCalendarEvents(httpClient, "YOUR_TOKEN", "calendar.holidays", "2022-05-01T07:00:00.000Z", "2022-06-12T07:00:00");

  // // Print the information of each calendar event
  // for (const CalendarEvent& event : calendarEventManager.events) {
  //   Serial.println("Summary: " + event.summary);

  //   if (!event.startDateTime.isEmpty()) {
  //     Serial.println("Start Date/Time: " + event.startDateTime);
  //   } else if (!event.startDate.isEmpty()) {
  //     Serial.println("Start Date: " + event.startDate);
  //   }

  //   if (!event.endDateTime.isEmpty()) {
  //     Serial.println("End Date/Time: " + event.endDateTime);
  //   } else if (!event.endDate.isEmpty()) {
  //     Serial.println("End Date: " + event.endDate);
  //   }

  //   if (!event.description.isEmpty()) {
  //     Serial.println("Description: " + event.description);
  //   }

  //   if (!event.location.isEmpty()) {
  //     Serial.println("Location: " + event.location);
  //   }

  //   Serial.println();
  // }



struct CheckConfigResult {
  String errors;
  String result;
};

struct IntentResult {
  String name;
  JsonObject slots;
};

class ErrorLogEntry {
public:
  String timestamp;
  String message;

  void parse(const String& logEntry) {
    char logEntryCopy[logEntry.length() + 1];
    logEntry.toCharArray(logEntryCopy, sizeof(logEntryCopy));

    char* timestampPtr = strtok(logEntryCopy, " ");
    if (timestampPtr != NULL) {
      timestamp = String(timestampPtr);
      char* messagePtr = strtok(NULL, "");
      if (messagePtr != NULL) {
        message = String(messagePtr);
      } else {
        message = "";
      }
    } else {
      timestamp = "";
      message = logEntry;
    }
  }
};

#endif
