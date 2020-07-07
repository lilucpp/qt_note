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



### 1.3 QScopePoint
### 1.4 QSharePointer
### 1.5 QObject::Deletelater.