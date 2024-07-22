#include "peak_detector.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    if (argc < 5) {
        qDebug() << "Usage: " << argv[0] << " <input_file> <output_file> <min_peak_width> <max_peak_width>";
        return 1;
    }

    QString inputFileName = argv[1];
    QString outputFileName = argv[2];
    int minPeakWidth = QString(argv[3]).toInt();
    int maxPeakWidth = QString(argv[4]).toInt();
    std::string res = search(inputFileName, outputFileName, minPeakWidth, maxPeakWidth);
    std::cout << res << "\n";

    return 0;
}



