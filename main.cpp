#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include "base64.h"


using namespace std;

bool saveAsPNG(const std::string &filename, const unsigned char *image, int width, int height, int channels) {
    cv::Mat img(height, width, CV_8UC(channels), const_cast<unsigned char *>(image));
    if (!cv::imwrite(filename, img)) {
        std::cerr << "Failed to save image as PNG: " << filename << std::endl;
        return false;
    }
    return true;
}

void compressImage(const std::string &inputPath, const std::string &outputPath, int quality = 50) {
    cv::Mat image = cv::imread(inputPath);
    if (image.empty()) {
        std::cerr << "Failed to open image: " << inputPath << std::endl;
        return;
    }
    std::vector<int> compression_params;
    compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
    compression_params.push_back(quality);
    cv::imwrite(outputPath, image, compression_params);
}

int main() {
    for (int i = 1; i < 11; ++i) {
        //Read the BASE64 data
        std::ifstream inputFile(R"(C:\Users\saad.zubairi\CLionProjects\opencv_test\)" + to_string(i) + ".txt");
        std::string base64Data((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        inputFile.close();

        //Decode base64 data
        std::string decodedData = base64_decode(base64Data, false);

        //Encode the binary into an image format
        std::vector<uchar> buffer(decodedData.begin(), decodedData.end());
        cv::Mat image = cv::imdecode(buffer, cv::IMREAD_UNCHANGED);

        // Save the image as a PNG file
        cv::imwrite("output" + to_string(i) + ".png", image);

        //Compression Code:
        string fileName = "output" + std::to_string(i);
        int compressionFactor = 0;
        std::ifstream file(R"(C:\Users\saad.zubairi\CLionProjects\opencv_test\cmake-build-debug\)" + fileName + ".png",
                           std::ios::binary);
        if (file) {
            file.seekg(0, std::ios::end);
            std::streampos fileSize = file.tellg();
            file.close();
            if (fileSize != -1) {
                compressionFactor = 5000000.0 / fileSize * 100;
                cout << "Compression Factor for " << fileName << ": " << compressionFactor;
            } else
                cout << "[-] FILE SIZE: N/A";
        } else {
            cout << "Failed to open file.\n";
            continue;
        }
        string inputPath = R"(C:\Users\saad.zubairi\CLionProjects\opencv_test\cmake-build-debug\)" + fileName + ".png";
        string outputPath = fileName + "_compressed.jpg";
        if (compressionFactor < 100) {
            compressImage(inputPath, outputPath, compressionFactor);
        } else {
            compressImage(inputPath, outputPath, 95);
        }
        cout << " [+] Compressed " + fileName << ".png" << "\n";
    }
    return 0;
}