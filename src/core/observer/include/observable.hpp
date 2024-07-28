#pragma once

#include <algorithm>
#include <vector>
#include <string>
#include "observer.hpp"

template <typename ValueType> class Observable {
private:
  std::vector<Observer<ValueType> *> observers__;

public:
  void notify_(ValueType &source, const std::string &args) {
    for (auto observer : observers__) {
      observer->dataChanged_(source, args);
    }
  }

  void subscribe_(Observer<ValueType> *observer) {
    if (std::find(observers__.begin(), observers__.end(), observer) != observers__.end()) return ;
    observers__.push_back(observer);
  }

  void unsubscribe_(Observer<ValueType> *observer) {
    observers__.erase(std::find(observers__.begin(), observers__.end(), observer));
  }

  const size_t getTotalObservers() const { return observers__.size(); }
};
