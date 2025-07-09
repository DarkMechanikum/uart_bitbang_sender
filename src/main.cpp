#include "uart_bitbang_sender.hpp"
#include <chrono>
#include <thread>

int main() {
    BitBangUartSender sender("gpiochip1", 259, 9600); // GPIO17 (BCM), 9600 baud
    if (!sender.start()) return 1;

    double x = 0.0;
    while (true) {
        sender.set_value(x);
        x += 0.25;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}
