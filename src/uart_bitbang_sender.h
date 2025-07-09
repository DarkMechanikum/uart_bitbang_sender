#pragma once
#include <atomic>
#include <chrono>
#include <future>
#include <gpiod.h>
#include <string>
#include <thread>

class BitBangUartSender {
public:
    BitBangUartSender(const std::string& chipname, unsigned int tx_pin, unsigned int baudrate = 600);
    ~BitBangUartSender();

    void set_value(double value);
    bool start();
    void stop();

private:
    std::string chipname;
    unsigned int tx_pin;
    int delay_us;

    struct gpiod_chip* chip = nullptr;
    struct gpiod_line* tx_line = nullptr;

    std::atomic<double> value_to_send;
    std::atomic<bool> running;
    std::thread thread;

    void tx_loop();
    void send_byte(uint8_t byte);
    void send_start_bit();
    void send_stop_bit();
    void send_char(char c);
    void send_string(const std::string& str);
};
