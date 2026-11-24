#pragma once
#include "LogEntry.hpp"
#include <vector>
#include <string>

class AnomalyDetector {
public:
    virtual ~AnomalyDetector() = default;
    virtual std::vector<size_t> detect(const std::vector<LogEntry>& logs) const = 0;
    virtual std::string getName() const = 0;
};

class ZScoreDetector : public AnomalyDetector {
private:
    double threshold_;
public:
    explicit ZScoreDetector(double threshold = 1.8) : threshold_(threshold) {}
    std::vector<size_t> detect(const std::vector<LogEntry>& logs) const override;
    std::string getName() const override { return "Z-Score"; }
};

class IQRDetector : public AnomalyDetector {
private:
    double multiplier_;
public:
    explicit IQRDetector(double multiplier = 1.5) : multiplier_(multiplier) {}
    std::vector<size_t> detect(const std::vector<LogEntry>& logs) const override;
    std::string getName() const override { return "IQR"; }
};