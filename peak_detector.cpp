#include "peak_detector.h"
#include <algorithm>
#include <QVector>
#include <set>

QVector<float> medianFilter(const QVector<float>& input, int windowSize) {
    QVector<float> result(input.size());
    std::multiset<float> window;

    int halfWindowSize = windowSize / 2;

    for (int i = 0; i < input.size(); ++i) {
        if (i >= windowSize) {
            window.erase(window.find(input[i - windowSize]));
        }

        window.insert(input[i]);

        if (i >= halfWindowSize) {
            auto midIter = std::next(window.begin(), halfWindowSize);
            result[i - halfWindowSize + 1] = *midIter;
        }
    }

    for (int i = input.size() - halfWindowSize; i < input.size(); ++i) {
        window.erase(window.find(input[i - windowSize + halfWindowSize]));
        auto midIter = std::next(window.begin(), halfWindowSize);
        result[i] = *midIter;
    }

    return result;
}


//QVector<float> medianFilter(const QVector<float>& input, int windowSize) {
//    QVector<float> result = input;
//    QVector<float> window(windowSize);

//    for (int i = 0; i < input.size(); ++i) {
//        for (int j = 0; j < windowSize; ++j) {
//            int idx = i - windowSize / 2 + j;
//            if (idx < 0) {
//                idx = 0;
//            }
//            if (idx >= input.size()) {
//                idx = input.size() - 1;
//            }
//            window[j] = input[idx];
//        }
//        std::nth_element(window.begin(), window.begin() + windowSize / 2, window.end());
//        result[i] = window[windowSize / 2];
//    }
//    return result;
//}

QVector<std::pair<int, int>> findPeaks(const QVector<float>& data, float floorLevel, int minPeakWidth, int maxPeakWidth) {
    QVector<std::pair<int, int>> peaks;
    bool inPeak = false;
    int peakStart = 0;

    for (int i = 0; i < data.size(); ++i) {
        if (!inPeak && data[i] > floorLevel) {
            inPeak = true;
            peakStart = i;
        } else if (inPeak && data[i] <= floorLevel) {
            inPeak = false;
            int peakWidth = i - peakStart;
            if (peakWidth >= minPeakWidth && peakWidth <= maxPeakWidth) {
                peaks.push_back(std::make_pair(peakStart, i - 1));
            }
        }
    }

    if (inPeak) {
        int peakWidth = data.size() - peakStart;
        if (peakWidth >= minPeakWidth && peakWidth <= maxPeakWidth) {
            peaks.push_back(std::make_pair(peakStart, data.size() - 1));
        }
    }

    return peaks;
}


std::string search(QString inputFileName, QString outputFileName, int minPeakWidth, int maxPeakWidth){
    int windowSize = 5000;
    QFile file(inputFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open file for reading";
        return "fail";
    }

    QDataStream in(&file);
    in.setByteOrder(QDataStream::LittleEndian);

    QVector<float> data;
    float value;
    while (!in.atEnd()) {
        in >> value;
        data.push_back(value);
    }

    file.close();

    QVector<float> filteredData = medianFilter(data, windowSize);

    QVector<float> sortedData = filteredData;
    std::nth_element(sortedData.begin(), sortedData.begin() + sortedData.size() / 2, sortedData.end());
    float floorLevel = sortedData[sortedData.size() / 2];

    QVector<std::pair<int, int>> peaks = findPeaks(filteredData, floorLevel, minPeakWidth, maxPeakWidth);

    QFile outputFile(outputFileName);
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for writing:" << outputFileName;
        return "fail";
    }

    QTextStream out(&outputFile);
    for (int i = 0; i < peaks.size(); ++i) {
        out << peaks[i].first << " " << peaks[i].second << "\n";
    }
    outputFile.close();

    std::string res_str = "Found " + std::to_string(peaks.size()) + " peaks.";
    return res_str;
}
