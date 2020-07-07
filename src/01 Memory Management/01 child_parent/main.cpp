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
