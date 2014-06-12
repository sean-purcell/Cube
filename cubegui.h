#ifndef CUBEGUI_H
#define CUBEGUI_H

#include <opencv2/core/core.hpp>

#include <vector>
#include <string>

/* returns a vector of size 8 with the 4 corners */
std::vector<cv::Point> getcorners(std::string winname, cv::Mat cubeimg);

void showclosest(std::string winname, cv::Mat frame);

#endif

