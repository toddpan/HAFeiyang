#ifndef __SmartPointer__H__   
#define __SmartPointer__H__

template<typename T>
class SmartPointer {
private:
  T* ptr; // 指向数据的指针
  int* refCount; // 引用计数

public:
  // 构造函数
  SmartPointer(T* p) {
    ptr = p;
    refCount = new int(1); // 初始化引用计数为1
  }

  // 拷贝构造函数
  SmartPointer(const SmartPointer<T>& sp) {
    ptr = sp.ptr;
    refCount = sp.refCount;
    (*refCount)++; // 增加引用计数
  }

  // 析构函数
  ~SmartPointer() {
    (*refCount)--; // 减少引用计数

    // 如果引用计数为0，则释放内存
    if (*refCount == 0) {
      delete ptr;
      delete refCount;
    }
  }

  // 重载->运算符
  T* operator->() {
    return ptr;
  }

  // 重载*运算符
  T& operator*() {
    return *ptr;
  }
};

#endif
