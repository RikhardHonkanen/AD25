#include <array>
#include <atomic>
#include <condition_variable>
#include <ctime>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#ifndef CAPACITY
#error "CAPACITY must be defined (use -DCAPACITY)"
#endif
static_assert(CAPACITY > 7, "CAPACITY must be greater than 7");
std::atomic<size_t> COUNTER{1000};

class Vehicle
{
    size_t id;
    std::string name;
    std::string type;

  public:
    Vehicle(std::string _name, std::string _type) : id{COUNTER}, name{_name}, type{_type}
    {
        COUNTER++;
    }
    friend std::ostream &operator<<(std::ostream &out, const Vehicle &vehicle)
    {
        vehicle.print(out);
        return out; /* Using print as a virtual member function to do the actual printing */
    }
    virtual void print(std::ostream &out) const { out << "Base"; }

    size_t getID(void) const { return id; }
    std::string getName(void) const { return name; }
    std::string getType(void) const { return type; }
};

class Car : public Vehicle
{
    int max_passengers = 4;

  public:
    Car(std::string name = "") : Vehicle{name, "Car"} {};
    int getMaxPassengers(void) const { return max_passengers; }

    void print(std::ostream &out) const override
    {
        out << "ID: " << this->getID() << "\nType: Car\nModel: " << this->getName()
            << "\nMax. Passengers: " << this->getMaxPassengers() << std::endl;
    }
};

class Truck : public Vehicle
{
    float max_load_weight = 2000;

  public:
    Truck(std::string name = "") : Vehicle{name, "Truck"} {};
    float getMaxLoadWeight(void) const { return max_load_weight; }

    void print(std::ostream &out) const override
    {
        out << "ID: " << this->getID() << "\nType: Truck\nModel: " << this->getName()
            << "\nMax. load weight: " << this->getMaxLoadWeight() << std::endl;
    }
};

class Warehouse
{
    std::size_t head{0};
    std::size_t tail{0};
    std::size_t count{0};
    std::shared_ptr<Vehicle> array[CAPACITY]{};

  public:
    Warehouse() = default;

    Warehouse(const Warehouse &) = delete;            // Uncopyable
    Warehouse &operator=(const Warehouse &) = delete; // Unassignable

    std::mutex mtx;
    std::condition_variable cv;

    bool push(const std::shared_ptr<Vehicle> item) noexcept
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return count < CAPACITY; });

        array[tail] = item;
        tail = (tail + 1) % CAPACITY;
        ++count;

        lock.unlock();
        cv.notify_all();

        return true;
    }

    bool pop(std::shared_ptr<Vehicle> &item) noexcept
    {
        bool status{false};

        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return count > 0; });

        status = true;
        item = array[head];
        head = (head + 1) % CAPACITY;
        --count;

        lock.unlock();
        cv.notify_all();

        return status;
    }

    int capacity(void) noexcept { return CAPACITY; }

    int available(void) noexcept { return count; }

    bool full(void) noexcept { return CAPACITY == count; }

    void clear(void) noexcept
    {
        std::lock_guard<std::mutex> lock(mtx);

        head = 0;
        tail = 0;
        count = 0;

        cv.notify_all();
    }

    ~Warehouse() = default;
};

static void consumer(Warehouse &warehouse)
{
    std::shared_ptr<Vehicle> value;
    static std::mutex omtx;

    while (1)
    {
        warehouse.pop(value);

        omtx.lock();
        std::cout << "[" << std::this_thread::get_id() << "] Consumed: \n" << *value << std::endl;
        omtx.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(500 + (rand() % 501)));
    }
}

int main(int argc, char *argv[])
{
    Warehouse warehouse{};

    std::srand(std::time(nullptr));
    int CONSUMERS{std::stoi(argv[1])};

    for (int i = 0; i < CONSUMERS; i++)
    {
        std::thread thread(consumer, std::ref(warehouse));
        thread.detach();
    }

    while (1)
    {
        if (rand() % 2)
        {
            auto c{std::make_shared<Car>("Volvo")};
            warehouse.push(c);
        }
        else
        {
            auto t{std::make_shared<Truck>("Scania")};
            warehouse.push(t);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500 + (rand() % 501)));
    }

    return 0;
}