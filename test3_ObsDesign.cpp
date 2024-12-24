#include <iostream>
#include <vector>

class DisplayUnit {
  std::string mName;

public:
  virtual void notify(int16_t temperature){};
  virtual std::string printName() const = 0;

  virtual ~DisplayUnit(){};
};

class ConsoleDisplay : public DisplayUnit {
  std::string mConsoleName;

public:
  ConsoleDisplay(const std::string name) { mConsoleName = name; }
  void notify(int16_t temperature) override {
    std::cout << "Temperature updated: " << temperature << "°C" << std::endl;
  }
  std::string printName() const override { return mConsoleName; }
};

class CarHeadUnit : public DisplayUnit {
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

class WeatherStation {
  std::vector<DisplayUnit *> mObservers;
  std::int16_t mTemperature;

public:
  void setTemperature(std::int16_t temperature) {
    mTemperature = temperature;
    notify_observers();
  }

  void registerObserver(DisplayUnit *dispUnit) {
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
  CarHeadUnit disp3("Nissan");

  WeatherStation station;

  station.registerObserver(&disp1);
  station.registerObserver(&disp2);
  station.registerObserver(&disp3);

  station.setTemperature(55);

  return 0;
}