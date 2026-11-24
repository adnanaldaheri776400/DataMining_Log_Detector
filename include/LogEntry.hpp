#pragma once
#include <string>

struct LogEntry {
    std::string timestamp;
    std::string ip_address;
    double response_time_ms;
    int bytes_sent;
};