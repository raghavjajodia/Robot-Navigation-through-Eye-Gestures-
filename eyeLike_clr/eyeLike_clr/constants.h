#ifndef CONSTANTS_H
#define CONSTANTS_H

// Debugging
const bool kPlotVectorField = false;

// Size constants
const int kEyePercentTop = 25;//31
const int kEyePercentLeftSide = 19; //13
const int kEyePercentRightSide = 19; //13
const int kEyePercentHeight = 30;//18
const int kEyePercentWidth = 25; //35

// Preprocessing
const bool kSmoothFaceImage = false;
const float kSmoothFaceFactor = 0.005;

// Algorithm Parameters
const int kFastEyeWidth = 50;
const int kWeightBlurSize = 5;
const bool kEnableWeight = true;
const float kWeightDivisor = 1.0;
const double kGradientThreshold = 50.0;

// Postprocessing
const bool kEnablePostProcess = true;
const float kPostProcessThreshold = 0.97;

// Eye Corner
const bool kEnableEyeCorner = false;

// left right window
const double leftWinPercent=0.37;
const double rightWinPercent=0.47;

const double linePercent = 0.20;

//frames
const int NO_OF_FRAMES=3;

#endif