#include "AnomalyDetector.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <algorithm>

std::vector<LogEntry> loadLogs(const std::string& filepath) {
    std::vector<LogEntry> logs;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filepath);
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        LogEntry entry;
        std::string rt, bytes;

        std::getline(ss, entry.timestamp, ',');
        std::getline(ss, entry.ip_address, ',');
        std::getline(ss, rt, ',');
        std::getline(ss, bytes, ',');

        entry.response_time_ms = std::stod(rt);
        entry.bytes_sent = std::stoi(bytes);
        logs.push_back(entry);
    }
    return logs;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: log_detector <csv_file> [zscore|iqr|both] [threshold]\n";
        return 1;
    }

    std::string file = argv[1];
    std::string mode = (argc >= 3) ? argv[2] : "both";

    try {
        double custom_param = (argc >= 4) ? std::stod(argv[3]) : -1.0;

        std::vector<LogEntry> logs = loadLogs(file);
        std::cout << "Loaded " << logs.size() << " records.\n";

        if (mode == "both") {
            ZScoreDetector z_det(custom_param > 0.0 ? custom_param : 1.8);
            IQRDetector iqr_det(1.5);

            auto z_hits = z_det.detect(logs);
            auto iqr_hits = iqr_det.detect(logs);

            std::cout << "Mode: Consensus (Z-Score + IQR)\n";
            size_t total = 0;
            for (size_t i = 0; i < logs.size(); ++i) {
                bool in_z = std::find(z_hits.begin(), z_hits.end(), i) != z_hits.end();
                bool in_iqr = std::find(iqr_hits.begin(), iqr_hits.end(), i) != iqr_hits.end();
                if (!in_z && !in_iqr) continue;

                std::string level = (in_z && in_iqr) ? "[CRITICAL]" : "[WARNING] ";
                std::cout << level << " Row " << (i + 1)
                          << " | Time: " << logs[i].timestamp
                          << " | IP: " << logs[i].ip_address
                          << " | Latency: " << logs[i].response_time_ms << " ms\n";
                ++total;
            }
            std::cout << "Total anomalies found: " << total << "\n";
            return 0;
        }

        std::unique_ptr<AnomalyDetector> detector;
        if (mode == "iqr") {
            double mult = (custom_param > 0.0) ? custom_param : 1.5;
            detector = std::make_unique<IQRDetector>(mult);
        } else {
            double thresh = (custom_param > 0.0) ? custom_param : 1.8;
            detector = std::make_unique<ZScoreDetector>(thresh);
        }

        std::cout << "Algorithm: " << detector->getName() << "\n";
        std::vector<size_t> anomalies = detector->detect(logs);

        for (size_t idx : anomalies) {
            std::cout << "[ANOMALY] Row " << (idx + 1)
                      << " | Time: " << logs[idx].timestamp
                      << " | IP: " << logs[idx].ip_address
                      << " | Latency: " << logs[idx].response_time_ms << " ms"
                      << " | Size: " << logs[idx].bytes_sent << " B\n";
        }
        std::cout << "Total anomalies found: " << anomalies.size() << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}