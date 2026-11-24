#include <gtest/gtest.h>
#include "AnomalyDetector.hpp"

TEST(DetectorTests, ZScoreFindsSpike) {
    std::vector<LogEntry> sample = {
        {"10:01:00", "10.0.1.10", 95.0,  480},
        {"10:01:05", "10.0.1.11", 102.0, 510},
        {"10:01:10", "10.0.1.12", 99.0,  495},
        {"10:01:15", "10.0.1.13", 104.0, 520},
        {"10:01:20", "192.168.1.99", 8500.0, 290000}
    };

    ZScoreDetector detector(1.8);
    auto found = detector.detect(sample);

    ASSERT_EQ(found.size(), 1);
    EXPECT_EQ(found[0], 4);
}

TEST(DetectorTests, IQRFindsSpike) {
    std::vector<LogEntry> sample = {
        {"10:01:00", "10.0.1.10", 95.0,  480},
        {"10:01:05", "10.0.1.11", 102.0, 510},
        {"10:01:10", "10.0.1.12", 99.0,  495},
        {"10:01:15", "10.0.1.13", 104.0, 520},
        {"10:01:20", "192.168.1.99", 8500.0, 290000}
    };

    IQRDetector detector(1.5);
    auto found = detector.detect(sample);

    ASSERT_EQ(found.size(), 1);
    EXPECT_EQ(found[0], 4);
}

TEST(DetectorTests, NoAnomaliesInNormalTraffic) {
    std::vector<LogEntry> normal_logs = {
        {"11:00:01", "10.0.1.10", 98.0,  500},
        {"11:00:05", "10.0.1.11", 101.0, 510},
        {"11:00:10", "10.0.1.12", 99.5,  490},
        {"11:00:15", "10.0.1.13", 102.0, 520}
    };

    ZScoreDetector zscore(1.8);
    IQRDetector iqr(1.5);

    EXPECT_TRUE(zscore.detect(normal_logs).empty());
    EXPECT_TRUE(iqr.detect(normal_logs).empty());
}