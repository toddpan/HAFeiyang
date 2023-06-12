#ifndef STRING_ARRAY_H
#define STRING_ARRAY_H

#include <Arduino.h>

class StringArray {
private:
  static const int MAX_SIZE = 10;
  String data[MAX_SIZE];
  int size;

public:
  class Iterator {
  private:
    String* ptr;

  public:
    Iterator(String* p) : ptr(p) {}

    String& operator*() const {
      return *ptr;
    }

    Iterator& operator++() {
      ++ptr;
      return *this;
    }

    bool operator!=(const Iterator& other) const {
      return ptr != other.ptr;
    }
  };

  StringArray() : size(0) {}

  void add(const String& value) {
    if (size < MAX_SIZE) {
      data[size++] = value;
    }
  }

  String operator[](int index) const {
    if (index >= 0 && index < size) {
      return data[index];
    }
    return "";
  }

  int getSize() const {
    return size;
  }

  Iterator begin() {
    return Iterator(data);
  }

  Iterator end() {
    return Iterator(data + size);
  }
};

#endif  // STRING_ARRAY_H