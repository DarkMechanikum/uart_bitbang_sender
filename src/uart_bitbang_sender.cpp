#include "uart_bitbang_sender.hpp"
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <iostream>

BitBangUartSender::BitBangUartSender(const std::string& chipname, unsigned int tx_pin, unsigned int baudrate)
    : chipname(chipname), tx_pin(tx_pin), delay_us(1000000 / baudrate), running(false), value_to_send(0.0) {}

BitBangUartSender::~BitBangUartSender() {
    stop();
}

void BitBangUartSender::set_value(double value) {
    value_to_send.store(value);
}

bool BitBangUartSender::start() {
    chip = gpiod_chip_open_by_name(chipname.c_str());
    if (!chip) {
        perror("gpiod_chip_open_by_name");
        return false;
    }

    tx_line = gpiod_chip_get_line(chip, tx_pin);
    if (!tx_line) {
        perror("gpiod_chip_get_line");
        gpiod_chip_close(chip);
        return false;
    }

    if (gpiod_line_request_output(tx_line, "uart_bitbang", 1) < 0) {
        perror("gpiod_line_request_output");
        gpiod_chip_close(chip);
        return false;
    }

    running = true;
    thread = std::thread(&BitBangUartSender::tx_loop, this);
    return true;
}

void BitBangUartSender::stop() {
    running = false;
    if (thread.joinable()) thread.join();
    if (tx_line) gpiod_line_release(tx_line);
    if (chip) gpiod_chip_close(chip);
}

void BitBangUartSender::tx_loop() {
    while (running) {
        double val = value_to_send.load();

        std::ostringstream ss;
        ss << std::fixed << std::setprecision(3) << val;

        send_string(ss.str());
        send_char('\n');

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void BitBangUartSender::send_string(const std::string& str) {
    for (char c : str) {
        send_char(c);
    }
}

void BitBangUartSender::send_char(char c) {
    send_start_bit();
    for (int i = 0; i < 8; ++i) {
        send_byte((c >> i) & 0x01);
    }
    send_stop_bit();
}

void BitBangUartSender::send_start_bit() {
    gpiod_line_set_value(tx_line, 0);
    usleep(delay_us);
}

void BitBangUartSender::send_stop_bit() {
    gpiod_line_set_value(tx_line, 1);
    usleep(delay_us);
}

void BitBangUartSender::send_byte(uint8_t bit) {
    gpiod_line_set_value(tx_line, bit);
    usleep(delay_us);
}
