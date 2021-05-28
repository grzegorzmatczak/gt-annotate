#ifndef ListVector_H
#define ListVector_H

#include "utils/includespdlog.h"


template<typename T>
class ListVector
{
 public:
  ListVector();
  ListVector(int size);

  bool isActive(int id);
  int addItem(T data);
  bool addItem(int id, T data);
  int leaseItem();
  bool deleteItem(int id);
  bool deleteItems();
  T& operator[] (int index);
  int size();

 private:
  int m_size;
  std::vector<T> m_vect;
  std::vector<bool> m_active;
};

#endif // ListVector_H