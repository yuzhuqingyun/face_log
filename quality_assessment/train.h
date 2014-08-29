#pragma once
#ifndef QUALITY_ASSESSMENT_TRAIN_H
#define QUALITY_ASSESSMENT_TRAIN_H

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"
#include <string.h>
#include <iostream>

using namespace cv;
using namespace std;

int TrainQualityAssessment(const string strDirectory);
#endif