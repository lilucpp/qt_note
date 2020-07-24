# Qt node
## 1. 资源管理
### 1.1 child/parent

```c++
#include <QCoreApplication>
#include <QDebug>
#include <QString>
#include <QTimer>
#include "test.h"

void foo() {
  mytest* t = new mytest();
  t->setObjectName("parent");
  mytest* child = new mytest(t);
  child->setObjectName("child");
  delete t;
}

void test() {
  QCoreApplication::instance()->exit();
}

int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);

  foo();

  mytest* t = new mytest(&a);
  t->setObjectName("ob...");

  QTimer::singleShot(1000, test);
  return a.exec();
}

```
扩展

1. [QCoreApplication::exec()](https://doc.qt.io/qt-5/qcoreapplication.html#exec)

   Enters the main event loop and waits until [exit](https://doc.qt.io/qt-5/qcoreapplication.html#exit)() is called. 

   It is necessary to call this function to start event handling. The main event loop receives events from the window system and dispatches these to the application widgets.

   More advanced idle processing schemes can be achieved using [processEvents](https://doc.qt.io/qt-5/qcoreapplication.html#processEvents)().

   We recommend that you connect clean-up code to the [aboutToQuit()](https://doc.qt.io/qt-5/qcoreapplication.html#aboutToQuit) signal, instead of putting it in your application's `main()` function because on some platforms the exec() call may not return. For example, on Windows when the user logs off, the system terminates the process after Qt closes all top-level windows. 

```c++
void test() {
  QCoreApplication::instance()->exit();
}

int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);

  QTimer::singleShot(1000, test);
  return a.exec();
}
```

2. [processEvents](https://doc.qt.io/qt-5/qcoreapplication.html#processEvents)

   You can call this function occasionally when your program is busy performing a long operation (e.g. copying a file).

   在主线程中进行大量计算时，一个可能的方法时调用processEvents。

   应当尽可能的使主线程（gui）轻量，大量的I/O、计算应使用**QtConcurrent::run**异步完成，或在单独的**QThread**中运行。

   参考：[How to make Qt work when main thread is busy?](https://stackoverflow.com/questions/1386043/how-to-make-qt-work-when-main-thread-is-busy)

3. 析构函数中使用this问题

   this在析构函数中仍然有效。但是在析构函数中虚函数无法正常工作。

   Effective c++ item9：绝不在构造和析构函数中调用virtual函数。

4. [aboutToQuit()](https://doc.qt.io/qt-5/qcoreapplication.html#aboutToQuit)

   **signal** void QCoreApplication::aboutToQuit()

   This signal is emitted when the application is about to quit the main event loop, e.g. when the event loop level drops to zero. This may happen either after a call to [quit](qcoreapplication.html#quit)() from inside the application or when the user shuts down the entire desktop session.

   The signal is particularly useful if your application has to do some last-second cleanup. Note that no user interaction is possible in this state.

   Note: This is a private signal. It can be used in signal connections but cannot be emitted by the user.

### 1.2 QPoint

The [QPointer](qpointer.html) class is a template class that provides guarded pointers to [QObject](qobject.html).

A guarded pointer, [QPointer](qpointer.html)<T>, behaves like a normal C++ pointer T *, except that it is automatically set to 0 when the referenced object is destroyed (unlike normal C++ pointers, which become "dangling pointers" in such cases). T must be a subclass of [QObject](qobject.html).

```c++
void foo() {
  mytest* t = new mytest();
  QPointer<mytest> pt(t);
  delete t;
  if (pt) {
    pt->setObjectName("parent");
    mytest* child = new mytest(pt);
    child->setObjectName("child");
  }
}
```

### 1.3 QScopePoint

```c++
void foo() {
  QScopedPointer<mytest> pt(new mytest());
  pt->setObjectName("parent");
}
```

### 1.4 QSharePointer

The QSharedPointer class holds a strong reference to a shared pointer. Like [shared_ptr](https://zh.cppreference.com/w/cpp/memory/shared_ptr) in the c++11.
The QWeakPointer is an automatic weak reference to a pointer in C++. It cannot be used to dereference the pointer directly, but it can be used to verify if the pointer has been deleted or not in another context.
Therefore, to access the pointer that QWeakPointer is tracking, you must first promote it to QSharedPointer and verify if the resulting object is null or not. QSharedPointer guarantees that the object isn't deleted, so if you obtain a non-null object, you may use the pointer. See QWeakPointer::toStrongRef() for an example.

扩展：
关于智能指针，[cppreferrence.com](https://zh.cppreference.com/w/cpp/memory/)中介绍的更为易懂。
1. shared_ptr:
std::shared_ptr 是通过指针保持对象共享所有权的智能指针。多个 shared_ptr 对象可占有同一对象。下列情况之一出现时销毁对象并解分配其内存：
最后剩下的占有对象的 shared_ptr 被销毁；

最后剩下的占有对象的 shared_ptr 被通过 operator= 或 reset() 赋值为另一指针。
用 delete 表达式或在构造期间提供给 shared_ptr 的定制删除器销毁对象。
shared_ptr 能在存储指向一个对象的指针时共享另一对象的所有权。此特性能用于在占有其所属对象时，指向成员对象。存储的指针为 get() 、解引用及比较运算符所访问。被管理指针是在 use_count 抵达零时传递给删除器者。
2. weak_ptr:
std::weak_ptr 是一种智能指针，它对被 std::shared_ptr 管理的对象存在非拥有性（“弱”）引用。在访问所引用的对象前必须先转换为 std::shared_ptr。

std::weak_ptr 用来表达临时所有权的概念：当某个对象只有存在时才需要被访问，而且随时可能被他人删除时，可以使用 std::weak_ptr 来跟踪该对象。需要获得临时所有权时，则将其转换为 std::shared_ptr，此时如果原来的 std::shared_ptr 被销毁，则该对象的生命期将被延长至这个临时的 std::shared_ptr 同样被销毁为止。

std::weak_ptr 的另一用法是打断 std::shared_ptr 所管理的对象组成的环状引用。若这种环被孤立（例如无指向环中的外部共享指针），则 shared_ptr 引用计数无法抵达零，而内存被泄露。能令环中的指针之一为弱指针以避免此情况。

### 1.5 QObject::Deletelater

// 没有完全理解，真实场景再考虑吧

## 2 Container
QList QVector QLinkedList QMap QSet QHash等
容器类的使用类似STL中的容器，提供的函数多余STL（indexOf，removeAt，takeAt，uniqueKeys，startsWith...）。
细微差别：
[https://web.archive.org/web/20160902015144/http://blog.codeimproved.net/posts/qtl-stl.html](https://web.archive.org/web/20160902015144/http://blog.codeimproved.net/posts/qtl-stl.html)

## 3 Working with settings
