#include <chrono>
#include <iostream>
#include <random>
#include <unistd.h>
#include <vector>

// Interface for Observer: Display Unit observes
class IDisplayUnit {
  std::string mName;

public:
  virtual void notify(int16_t temperature){};
  virtual std::string printName() const = 0;

  virtual ~IDisplayUnit(){};
};

class ConsoleDisplay : public IDisplayUnit {
  std::string mConsoleName;

public:
  ConsoleDisplay(const std::string name) { mConsoleName = name; }

  // Notify method called from Observer.
  void notify(int16_t temperature) override {
    std::cout << "Temperature updated: " << temperature << "°C" << std::endl;
  }
  std::string printName() const override { return mConsoleName; }
};

class CarHeadUnit : public IDisplayUnit {
  std::string mConsoleName;
  std::string mCarName;

public:
  CarHeadUnit(const std::string carName) {
    mCarName = carName;
    mConsoleName = carName + " HeadUnit";
  }
  void notify(int16_t temperature) override {
    std::cout << "Temperature updated: " << temperature << "°C" << std::endl;
  }
  std::string printName() const override { return mConsoleName; }
  std::string carName() const { return mCarName; }
};

// Subject: notifies observers as soon as something changes
// TODO: use smart pointers instead of raw pointers.
class WeatherStation {
  std::vector<IDisplayUnit *> mObservers;
  std::int16_t mTemperature;

public:
  void setTemperature(std::int16_t temperature) {
    mTemperature = temperature;
    auto start = std::chrono::high_resolution_clock::now();
    notify_observers();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // To get the value of duration use the count()
    // member function on the duration object
    std::cout << duration.count() << " us." << std::endl;
  }

  void registerObserver(IDisplayUnit *dispUnit) {
    mObservers.push_back(dispUnit);
  }

  void notify_observers() {
    for (auto *obs : mObservers) {
      if (auto carUnit = dynamic_cast<CarHeadUnit *>(obs)) {
        std::cout << "Car name: " << carUnit->carName() << std ::endl;
      }
      std::cout << "Notifiying " << obs->printName() << std::endl;
      obs->notify(mTemperature);
    }
  }
};

int main() {

  ConsoleDisplay disp1("Living room"), disp2("Kitchen");
  CarHeadUnit disp3("Nissan"), disp4("Toyota");

  WeatherStation station;

  station.registerObserver(&disp1);
  station.registerObserver(&disp2);
  station.registerObserver(&disp3);
  station.registerObserver(&disp4);

  // Generate random temperatures from -55 to +55 C.s
  std::random_device rd;
  std::mt19937 e2(rd());
  std::uniform_int_distribution<int> dist(-55, 55);

  for (auto i = 0; i <= 50; i++) {
    auto curr_temperature = dist(e2);
    station.setTemperature(curr_temperature);
    usleep(200000);
  }
  return 0;
}