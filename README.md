# Log Anomaly Detector

A C++17 command-line tool developed for the Production Software Engineering course to detect response-time outliers in server logs using statistical data mining techniques (**Z-Score** and **IQR**).

## Structure
- `include/LogEntry.hpp`: Data structure for a parsed CSV log row.
- `include/AnomalyDetector.hpp` & `src/AnomalyDetector.cpp`: Strategy interface and implementations for Z-Score and IQR outlier detection.
- `src/main.cpp`: CSV reader and CLI runner supporting individual algorithms or consensus mode (`both`) with severity classification (`[CRITICAL]` vs `[WARNING]`).
- `tests/test_detectors.cpp`: Unit tests using GoogleTest.

## Build and Run
```bash
mkdir out && cd out
cmake ..
cmake --build .
ctest --output-on-failure
./log_detector ../data/sample_logs.csv
./log_detector ../data/sample_logs.csv zscore
./log_detector ../data/sample_logs.csv iqr