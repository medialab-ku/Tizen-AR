#ifndef LOD_H_
#define LOD_H_

#include "Tracking.h"
#include "ORBextractor.h"

namespace ORB_SLAM2
{

class Tracking;
class ORBextractor;

class LoD
{
public:
	LoD();
	LoD(Tracking* t, int DefaultORBPoints, float fScaleFactor, int DefaultIPLevels, int IniThFAST, int MinThFAST);

	void Set();

	int GetORBPointNumber() { return mORBPoints; }
	int GetORBIPLevels() { return mIPLevels; }
	void GetORBParams(int& points, float& scale, int& levels, int& iniFAST, int& minFAST);

	void SetORBPointNumber(int num) { mORBPoints = num; }
	void SetIPLevelNumber(int num) { mIPLevels = num; }
	void SetORBPointRatio(float ratio);
	void SetIPLevelRatio(float ratio);

	int GetPOIters() { return mPoseOptimizeIters; }
	int GetPOLevels() { return mPoseOptimizeLevels; }
    float GetPOSampleRate() { return mPoseOptimizeSampleRate; }

	void SetPOIters(int num) { mPoseOptimizeIters = num; }
	void SetPOLevels(int num) { mPoseOptimizeLevels = num; }
    void SetPoSampleRate(float num) { mPoseOptimizeSampleRate = num; }

private:
	Tracking* tracking;

	/// ORB Settings params
	int mORBPoints;
	int mIPLevels;

	int mDefaultORBPoints;
	float mfScaleFactor;
	int mDefaultIPLevels;
	int mIniThFAST;
	int mMinThFAST;

	// Pose Optimization params
	int mPoseOptimizeIters;
	int mPoseOptimizeLevels;
    float mPoseOptimizeSampleRate;
};
}


#endif /* LOD_H_ */
