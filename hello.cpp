
#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Shared buffer
#define buff_size 5
int buff[buff_size];

// Semaphores
// Producer can start when buffer is empty
counting_semaphore<buff_size> signal_to_producer(buff_size);

// Consumer can start when buffer contains items
counting_semaphore<buff_size> signal_to_consumer(0);

void producer() {
    while (1) {
        signal_to_producer.acquire();

        cout << "Produced = ";

        for (int i = 0; i < buff_size; ++i) {
            buff[i] = i * i;  // Task of producer
            cout << buff[i] << " ";
            cout.flush();

            this_thread::sleep_for(milliseconds(200));
        }

        cout << endl;

        signal_to_consumer.release();
    }
}

void consumer() {
    while (1) {
        signal_to_consumer.acquire();

        cout << "Consumed = ";

        for (int i = buff_size - 1; i >= 0; --i) {
            cout << buff[i] << " ";
            cout.flush();

            buff[i] = 0;  // Task of consumer
            this_thread::sleep_for(milliseconds(200));
        }

        cout << endl << endl;

        signal_to_producer.release();
    }
}

int main() {
    thread t1(producer);
    thread t2(consumer);

    t1.join();
    t2.join();

    return 0;
}
