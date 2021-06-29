#include <iostream>
#include <boost/math/distributions/chi_squared.hpp>
#include <opencv2/core/core.hpp>

class AnomalyDetection{
public:
  AnomalyDetection();
  ~AnomalyDetection();
  void Settings();
  void SetDistributionParams(double mu, double sig);
  void SetDistributionParams(cv::Mat mu, cv::Mat sig);
  void SetCriticalValue(double dof, double los);
  double GetCriticalValue(void);
  double CalculateAnomaly(double val);
  double CalculateAnomaly(cv::Mat val);
  void SetAnomalyVector(std::vector<double> data);
  void SetAnomalyVector(std::vector<cv::Mat> data);
  std::vector<double> GetAnomalyVector(void);

private:
  double p_val_;
  double mu_, sig_;
  std::vector<double> anomaly_uni_;
  cv::Mat mu_m_, sig_m_;
};
