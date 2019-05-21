#ifndef PLANEDETECTOR_H
#define PLANEDETECTOR_H

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/SVD>
#include <vector>
#include <iostream>
#include <ctime>

using namespace std;

namespace ORB_SLAM2
{

/// Plane detector class
/// A plane is defined as Eigen::Vector4f (a, b, c, d) which means ax + by + cz + d = 0 

class PlaneDetector
{
public:
    PlaneDetector();
    bool IsActive() { return active; }
    void SetActive(bool set) { active = set; }
    void GetPlane(Eigen::Vector4f& plane, Eigen::Vector3f& position, int& inliers);
    Eigen::Vector4f Detect(vector<Eigen::Vector3f> &points);

public:

private:
    float DistToPlane(Eigen::Vector3f &point, Eigen::Vector4f &planeEq);
    Eigen::Vector3f CalcCentroid(std::vector<Eigen::Vector3f> &points);
    float CalcStdDiv(vector<Eigen::Vector3f> &points);
    Eigen::Vector4f CalcPlane(Eigen::Vector3f &point1, Eigen::Vector3f &point2, Eigen::Vector3f &point3);
    Eigen::Vector4f CalcPlane(vector<Eigen::Vector3f> &points);
    int NumIters(float inlierProb, float acceptedFailProb);
    bool IsPlaneNull(Eigen::Vector4f plane);
    Eigen::Vector4f LeastSquares(std::vector<Eigen::Vector3f> points);

private:
    bool active;
    Eigen::Vector4f plane;
    Eigen::Vector3f position;
    int inlierCount;
};

}


#endif
