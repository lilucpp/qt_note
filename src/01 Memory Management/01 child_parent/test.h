#ifndef TEST_H
#define TEST_H

#include <QObject>

class mytest : public QObject {
  Q_OBJECT
 public:
  explicit mytest(QObject* parent = nullptr);
  ~mytest();

 signals:
};

#endif  // TEST_H
