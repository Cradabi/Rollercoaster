#include <QCoreApplication>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

// Функция для определения порядка байтов системы
bool isLittleEndian() {
    int num = 1;
    return (*(char *)&num == 1);
}

// Функция для чтения float в формате little endian
float readFloatLittleEndian(std::ifstream& file) {
    float value;
    char buffer[sizeof(float)];
    file.read(buffer, sizeof(float));

    // Преобразование из little endian в нативный формат
    if (!isLittleEndian()) {
        std::cout << 1 << std::endl;
        std::reverse(std::begin(buffer), std::end(buffer));
    }

    std::memcpy(&value, buffer, sizeof(float));
    return value;
}

// Функция для медианной фильтрации
std::vector<float> medianFilter(const std::vector<float>& signal, int windowSize) {
    std::vector<float> filtered(signal.size());
    std::vector<float> window(windowSize);

    for (int i = 0; i < signal.size(); ++i) {
        for (int j = 0; j < windowSize; ++j) {
            int idx = i - windowSize / 2 + j;
            if (idx < 0) idx = 0;
            if (idx >= signal.size()) idx = signal.size() - 1;
            window[j] = signal[idx];
        }
        std::nth_element(window.begin(), window.begin() + windowSize / 2, window.end());
        filtered[i] = window[windowSize / 2];
    }

    return filtered;
}

// Функция для расчета стандартного отклонения
float calculateStdDev(const std::vector<float>& signal) {
    float sum = 0.0f, sum_sq = 0.0f;
    for (float value : signal) {
        sum += value;
        sum_sq += value * value;
    }
    float mean = sum / signal.size();
    return std::sqrt(sum_sq / signal.size() - mean * mean);
}



int main() {
    // Чтение данных из бинарного файла
    std::vector<float> signal;
    std::ifstream file("rollercoaster/detector_src_32f.bin", std::ios::binary);

    if (!file) {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    float minValue = std::numeric_limits<float>::max();
    float maxValue = std::numeric_limits<float>::lowest();
    double sum = 0.0;
    int count = 0;

//    while (file) {
//        float value = readFloatLittleEndian(file);
//        if (file) {
//            signal.push_back(value);
//            minValue = std::min(minValue, value);
//            maxValue = std::max(maxValue, value);
//            sum += value;
//            count++;
//        }
//    }
//    file.close();

//    // Вывод статистики
//    std::cout << "Read " << signal.size() << " values from the file." << std::endl;
//    std::cout << "Min value: " << minValue << std::endl;
//    std::cout << "Max value: " << maxValue << std::endl;
//    std::cout << "Average value: " << (sum / count) << std::endl;

//    // Проверка на отрицательные значения
//    bool allNegative = std::all_of(signal.begin(), signal.end(), [](float v) { return v < 0; });
//    std::cout << "All values are negative: " << (allNegative ? "Yes" : "No") << std::endl;

//    // Параметры
//    int windowSize = 5; // Размер окна медианного фильтра

//    // Применение медианного фильтра
//    std::vector<float> filtered = medianFilter(signal, windowSize);

//    // Вычитание отфильтрованного сигнала из исходного
//    std::vector<float> noise(signal.size());
//    for (int i = 0; i < signal.size(); ++i) {
//        noise[i] = signal[i] - filtered[i];
//    }

//    // Расчет уровня шума
//    float noiseLevel = calculateStdDev(noise);

//    // Вывод результатов
//    std::cout << "Estimated noise level: " << noiseLevel << std::endl;

//    return 0;
//}
