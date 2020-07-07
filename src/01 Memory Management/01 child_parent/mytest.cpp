#include "QDebug"
#include "test.h"

mytest::mytest(QObject* parent) : QObject(parent) {
  qInfo() << "constructor...";
}
mytest::~mytest() {
  qInfo() << "destructor..." << this;
}