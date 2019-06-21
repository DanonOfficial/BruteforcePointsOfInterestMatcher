#include <iostream>
#include <filesystem>
#include <tuple>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace cv::xfeatures2d;

std::vector<Mat> loadImages(const std::string &path) {
    std::vector<Mat> result;
    using namespace std::filesystem;
    for (auto &p: directory_iterator(path)) {
        std::string name = p.path();
        result.push_back(imread(name.c_str(), 0));
    }
    return result;
}

size_t matchImages(const Mat &image1, const Mat &image2, float threshold = 0.75) {
    Mat descriptors1, descriptors2;
    cv::Ptr<cv::Feature2D> f2d = cv::xfeatures2d::SiftFeatureDetector::create();
    std::vector<cv::KeyPoint> keypointsImage1;
    f2d->detect(image1, keypointsImage1);
    f2d->compute(image1, keypointsImage1, descriptors1);
    std::vector<cv::KeyPoint> keypointsImage2;
    f2d->detect(image2, keypointsImage2);
    f2d->compute(image2, keypointsImage2, descriptors2);


    BFMatcher matcher(NORM_L2);
    std::vector<std::vector<DMatch> > matches;
    matcher.knnMatch(descriptors1, descriptors2, matches, 2);

    std::vector<DMatch> goodMatches;
    for (size_t i = 0; i < matches.size(); i++) {
        if (matches[i][0].distance < threshold * matches[i][1].distance) {
            goodMatches.push_back(matches[i][0]);
        }
    }

    //epipolar mathcing
    std::vector<int> pointIndexes1;
    std::vector<int> pointIndexes2;
    for (auto &it: goodMatches) { //choose only goodMathces
        pointIndexes1.push_back(it.queryIdx);
        pointIndexes2.push_back(it.trainIdx);
    }
    std::vector<Point2f> selPoints1, selPoints2;
    KeyPoint::convert(keypointsImage1, selPoints1, pointIndexes1);
    KeyPoint::convert(keypointsImage2, selPoints2, pointIndexes2);


    std::vector<uchar> inliners(keypointsImage1.size(), 0);
    findFundamentalMat(Mat(selPoints1), Mat(selPoints2), inliners, FM_RANSAC, 3, 0.99);
    size_t result = 0;
    for (auto &i: inliners) {
        if (i != 0) {
            result++;
        }
    }
    return result;

}

void writeEdgeList(const std::string &name, const std::vector<std::tuple<size_t, size_t, size_t>> &adjacencyList,
                   size_t threshold = 0) {
    std::ofstream out(name);
    for (auto &i: adjacencyList) {
        if (std::get<2>(i) > threshold) {
            out << std::get<0>(i) << " " << std::get<1>(i) << " " << std::get<2>(i) << std::endl;
        }
    }
}

int main(int argc, char **argv) {
    auto images = loadImages(argv[1]);
    std::cout << "Amount of images: " << images.size() << std::endl;
    std::vector<std::tuple<size_t, size_t, size_t>> edgeList; // from-to-weight
    for (size_t i = 0; i < images.size(); i++) {
        for (size_t j = (i + 1); j < images.size(); j++) {
            size_t matchAmount = matchImages(images[i], images[j]);
            if (matchAmount != 0) {
                edgeList.emplace_back(i, j, matchAmount);
            }
        }
    }
//    for (auto &i: edgeList) {
//        std::cout << std::get<0>(i) << " " << std::get<1>(i) << " " << std::get<2>(i) << std::endl;
//    }
    writeEdgeList("resultList.txt", edgeList, atoll(argv[2]));
    return 0;
}

