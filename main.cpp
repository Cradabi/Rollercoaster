#include <QFile>
#include <QDataStream>
#include <QVector>
#include <QDebug>
#include <algorithm>

// Функция для медианной фильтрации
QVector<float> medianFilter(const QVector<float>& input, int windowSize) {
    QVector<float> result = input;
    QVector<float> window(windowSize);

    for (int i = 0; i < input.size(); ++i) {
        for (int j = 0; j < windowSize; ++j) {
            int idx = i - windowSize / 2 + j;
            if (idx < 0) {
                idx = 0;
            }
            if (idx >= input.size()) {
                idx = input.size() - 1;
            }
            window[j] = input[idx];
        }
        std::nth_element(window.begin(), window.begin() + windowSize / 2, window.end());
        result[i] = window[windowSize / 2];
    }
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        qDebug() << "Usage: " << argv[0] << " <string1> <string2> <int1> <int2>";
        return 1;
    }

    QString inputFileName = argv[1];
    QString outputFileName = argv[2];
    int minPeakWidth = std::stoi(argv[3]);
    int maxPeakWidth = std::stoi(argv[4]);
    int windowSize = 50; // Размер окна фильтра


    QFile file(inputFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open file for reading";
        return 1;
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

    // Применяем медианный фильтр
    QVector<float> filteredData = medianFilter(data, windowSize);

    // Находим медиану отфильтрованных данных
    QVector<float> sortedData = filteredData;
    std::nth_element(sortedData.begin(), sortedData.begin() + sortedData.size() / 2, sortedData.end());
    float floorLevel = sortedData[sortedData.size() / 2];

    // Находим горки
    QVector<int> peakStarts;
    QVector<int> peakEnds;
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
                peakStarts.push_back(peakStart);
                peakEnds.push_back(i - 1);
            }
        }
    }

    // Если последняя горка не закончилась
    if (inPeak) {
        int peakWidth = data.size() - peakStart;
        if (peakWidth >= minPeakWidth && peakWidth <= maxPeakWidth) {
            peakStarts.push_back(peakStart);
            peakEnds.push_back(data.size() - 1);
        }
    }

    // Записываем результаты в файл
    QFile outputFile(outputFileName);
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for writing:" << outputFileName;
        return 1;
    }

    QTextStream out(&outputFile);
    for (int i = 0; i < peakStarts.size(); ++i) {
        out << peakStarts[i] << " " << peakEnds[i] << "\n";
    }
    outputFile.close();

    qDebug() << "Found" << peakStarts.size() << "peaks. Results saved to" << outputFileName;

    return 0;
}
