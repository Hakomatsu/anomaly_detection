#include <iostream>
#include <boost/math/distributions/chi_squared.hpp>
#include <opencv2/core/core.hpp>

class AnomalyDetection{
public:
  AnomalyDetection(){
    std::cout << "AnomalyDetection class was called" << std::endl;
  };
  ~AnomalyDetection(){
    std::cout << "AnomalyDetection class was deleted" << std::endl;
  };
  void Settings();
  void SetDistributionParams(double mu, double sig){
    mu_ = mu;
    sig_ = sig;
  };
  void SetDistributionParams(cv::Mat mu, cv::Mat sig){
    mu_m_ = mu;
    sig_m_ = sig;
  };
  void SetCriticalValue(double dof, double los){
    chi_squared dist(dof);
    double p_val = quantile(complement(dist, los));
    p_val_ = p_val;
  };
  double GetCriticalValue(void){
    return p_val_;
  };
  double CalculateAnomaly(double val){
    return pow((val-mu_), 2)/sig_;
  };
  double CalculateAnomaly(cv::Mat val){
    cv::Mat sig_inv = sig_m_.inv(cv::DECOMP_SVD);
    cv::Mat anomaly = (val - mu_m_).t()*sig_inv*(val - mu_m_);
    return anomaly.at<double>(0, 0);
  };
  void SetAnomalyVector(std::vector<double> data){
    std::vector<double> anomaly_data;
    for(int i = 0; i < data.size(); i++){
      double value = data[i];
      double anomaly = CalculateAnomaly(value);
      anomaly_data.push_back(anomaly);
    }
    anomaly_uni_ = anomaly_data;
  };
  void SetAnomalyVector(std::vector<cv::Mat> data){
    std::vector<double> anomaly_data;
    for(int i = 0; i < data.size(); i++){
      double value = data[i];
      double anomaly = CalculateAnomaly(value);
      anomaly_data.push_back(anomaly);
    }
    anomaly_uni_ = anomaly_data;
  };
  std::vector<double> GetAnomalyVector(void){
    return anomaly_uni_;
  };

private:
  double p_val_;
  double mu_, sig_;
  std::vector<double> anomaly_uni_;
  cv::Mat mu_m_, sig_m_;
};
