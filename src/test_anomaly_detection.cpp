#include <iostream>
#include <map>
#include <boost/math/distributions/chi_squared.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "matplotlibcpp.h"
using namespace std;
using namespace boost::math;
namespace plt = matplotlibcpp;

double Uniform(){
  return ((double)rand()+1.0)/((double)RAND_MAX+2.0);
}

vector<cv::Mat> GenerateData(int data_size, int dim, cv::Mat offset){
  vector<cv::Mat> data;
  // srand((unsigned)time(NULL));
  srand(M_PI);
  double mu=0.0, sig=10.0;
  for(int i = 0; i < data_size; i++){
    cv::Mat point = cv::Mat::zeros(dim, 1, CV_64FC1);
    for(int j = 0; j < dim; j++){
      double offset_val = offset.at<double>(j, 0);
      double z = sqrt(-2.0*log(Uniform()))*sin(2.0*M_PI*Uniform());
      double val = offset_val+mu+sig*z;
      point.at<double>(j, 0) = val;
    }
    data.push_back(point);
  }

  return data;
}

double ChisqrSetCriticalValue(double dof, double los){
  chi_squared dist(dof);
  double p_val = quantile(complement(dist, los));

  return p_val;
}

double CalculateAnomaly(cv::Mat mu, cv::Mat sig, cv::Mat val){
  cv::Mat sig_inv = sig.inv(cv::DECOMP_SVD);
  cv::Mat anomaly = (val - mu).t()*sig_inv*(val - mu);
  return anomaly.at<double>(0, 0);
}

pair<cv::Mat, cv::Mat> CalcMuAndSig(vector<cv::Mat> data){
  cv::Mat mu, sig;
  for(int i = 0; i < data.size(); i++){
    if(i == 0)
      mu = data[i]/data.size();
    else
      mu += data[i]/data.size();
  }

  for(int i = 0; i < data.size(); i++){
    cv::Mat diff = data[i]-mu, diff_t = (data[i]-mu).t();
    if(i == 0)
      sig = (diff*diff_t)/data.size();
    else
      sig += (diff*diff_t)/data.size();
  }

  pair<cv::Mat, cv::Mat> mu_and_sig;
  mu_and_sig = make_pair(mu, sig);
  
  return mu_and_sig;
}

void Visualize(vector<double> anomary_data, double a_th){
  int size = anomary_data.size();
  vector<double> x(size), y(size), y2(size);
  for(int i = 0; i < size; i++){
    x.at(i) = i;
    y.at(i) = anomary_data[i];
    y2.at(i) = a_th;
  }

  plt::plot(x, y, "o");
  plt::plot(x, y2, "--");
  plt::show();
}

int main(void){
  // Generate normal data according to normal distribution
  cv::Mat offset_mat = (cv::Mat_<double>(2, 1) << 170.0, 65.0);
  vector<cv::Mat> normal_data = GenerateData(100, 2, offset_mat);

  // Calculate mu and sigma
  pair<cv::Mat, cv::Mat> mu_sig = CalcMuAndSig(normal_data);
  cv::Mat mu = mu_sig.first, sig = mu_sig.second;
  
  // Generate abnormal data
  vector<cv::Mat> abnormal_data = GenerateData(3, 2, offset_mat);
  cv::Mat abnormal_offset = (cv::Mat_<double>(2, 1) << 30, 20);
  for(int i = 0; i < abnormal_data.size(); i++){
    cv::Mat data_value = abnormal_data[i];
    for(int j = 0; j < data_value.rows; j++)
      data_value.at<double>(j, 0) += abnormal_offset.at<double>(j, 0);
    normal_data.push_back(data_value);
  }

  // Set anomary threshold (The significance level is set 0.01% in this case)
  double a_th = ChisqrSetCriticalValue(1, 0.01);
  cout << "a_th: " << a_th << endl;

  // Calcurate anomaly value
  vector<double> anomaly_data;
  for(int i = 0; i < normal_data.size(); i++){
    cv::Mat data_val = normal_data[i];
    double anomaly = CalculateAnomaly(mu, sig, data_val);
    anomaly_data.push_back(anomaly);
    if(a_th <= anomaly){
      cout << "index: " << i << " is anomary data" << endl;
      cout << "value: " << data_val << endl;
      cout << "anomaly value: " << anomaly << endl;
    }
  }

  // Plot 2-dim data
  int size = normal_data.size();
  vector<double> x(size), y(size);
  for(int i = 0; i < size; i++){
    cv::Mat data_val = normal_data[i];
    x.at(i) = data_val.at<double>(0, 0);
    y.at(i) = data_val.at<double>(1, 0);
  }
  plt::plot(x, y, "o");
  plt::show();

  // Plot anomaly data
  Visualize(anomaly_data, a_th);
  return 0;
}
