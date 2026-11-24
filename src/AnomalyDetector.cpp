#include "AnomalyDetector.hpp"
#include <cmath>
#include <algorithm>

std::vector<size_t> ZScoreDetector::detect(const std::vector<LogEntry>& logs) const {
    std::vector<size_t> anomalies;
    if (logs.size() < 2) return anomalies;

    double sum = 0.0;
    for (const auto& entry : logs) {
        sum += entry.response_time_ms;
    }
    double mean = sum / static_cast<double>(logs.size());

    double sq_diff_sum = 0.0;
    for (const auto& entry : logs) {
        double diff = entry.response_time_ms - mean;
        sq_diff_sum += diff * diff;
    }
    double std_dev = std::sqrt(sq_diff_sum / static_cast<double>(logs.size()));
    if (std_dev == 0.0) return anomalies;

    for (size_t i = 0; i < logs.size(); ++i) {
        double z_score = std::abs(logs[i].response_time_ms - mean) / std_dev;
        if (z_score > threshold_) {
            anomalies.push_back(i);
        }
    }
    return anomalies;
}

std::vector<size_t> IQRDetector::detect(const std::vector<LogEntry>& logs) const {
    std::vector<size_t> anomalies;
    if (logs.size() < 4) return anomalies;

    std::vector<double> values;
    values.reserve(logs.size());
    for (const auto& entry : logs) {
        values.push_back(entry.response_time_ms);
    }
    std::sort(values.begin(), values.end());

    size_t n = values.size();
    double q1 = values[n / 4];
    double q3 = values[(3 * n) / 4];
    double iqr = q3 - q1;
    double upper_bound = q3 + multiplier_ * iqr;

    for (size_t i = 0; i < logs.size(); ++i) {
        if (logs[i].response_time_ms > upper_bound) {
            anomalies.push_back(i);
        }
    }
    return anomalies;
}