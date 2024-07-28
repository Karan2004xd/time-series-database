#include <gtest/gtest.h>
#include "../../../core/observer/include/observable.hpp"
#include "../../../core/observer/include/observer.hpp"

struct Person : Observable<Person> {
  int age_;
  Person(int age) : age_(age) {}

  int getAge_() { return age_; }

  void setAge_(int age) {
    age_ = age;
    notify_(*this, "age");
  }
};

struct PersonImpl : Observer<Person> {
  int newAge_;
  void dataChanged_(Person &source, const std::string &args) override {
    if (args == "age") {
      newAge_ = source.getAge_();
    }
  }
};

TEST(ObserverSuite, SubscribeNewObserver) {
  Person p {10};
  PersonImpl pi;

  p.subscribe_(&pi);
  p.setAge_(12);

  ASSERT_EQ(pi.newAge_, 12);
}

TEST(ObserverSuite, UnSubscribeObserver) {
  Person p {10};
  PersonImpl pi;

  p.subscribe_(&pi);
  p.setAge_(12);

  p.unsubscribe_(&pi);
  p.setAge_(14);

  ASSERT_EQ(pi.newAge_, 12);
  ASSERT_NE(pi.newAge_, 14);
}

TEST(ObserverSuite, SubscribeToExistingObserver) {
  Person p {10};
  PersonImpl pi;

  p.subscribe_(&pi);
  p.subscribe_(&pi);

  ASSERT_EQ(p.getTotalObservers(), 1);
}

TEST(ObserverSuite, UnSubscribeNotExistingObserver) {
  Person p {10};
  PersonImpl pi;

  EXPECT_NO_THROW({
    p.unsubscribe_(&pi);
    p.unsubscribe_(&pi);
  });
}
