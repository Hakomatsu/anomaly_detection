#include <iostream>
#include <boost/math/distributions/chi_squared.hpp>
#include "matplotlibcpp.h"
using namespace std;
using namespace boost::math;
namespace plt = matplotlibcpp;

double Uniform(){
  return ((double)rand()+1.0)/((double)RAND_MAX+2.0);
}

vector<double> GenerateData(int data_size){
  vector<double> data;
  // srand((unsigned)time(NULL));
  srand(100);
  double mu=0.0, sig=10.0;
  for(int i = 0; i < data_size; i++){
    double z = sqrt(-2.0*log(Uniform()))*sin(2.0*M_PI*Uniform());
    double val = 170.0+mu+sig*z;
    data.push_back(val);
  }

  return data;
}

double ChisqrSetCriticalValue(double dof, double los){
  chi_squared dist(dof);
  double p_val = quantile(complement(dist, los));

  return p_val;
}

double CalculateAnomaly(double mu, double sig, double val){
  return pow((val-mu), 2)/sig;
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
  plt::xlabel("Index");
  plt::ylabel("Anomaly Score");
  plt::show();
}

int main(void){
  // Generate normal data according to normal distribution
  vector<double> normal_data = GenerateData(100);

  // Calculate mu and sigma
  double mu = 0.0, sig = 0.0;
  for(int i = 0; i < normal_data.size(); i++)
    mu += normal_data[i]/normal_data.size();
  for(int i = 0; i < normal_data.size(); i++)
    sig += pow(normal_data[i]-mu,2)/normal_data.size();

  // Generate abnormal data
  vector<double> abnormal_data = GenerateData(3);
  for(int i = 0; i < abnormal_data.size(); i++){
    double data_value = abnormal_data[i];
    normal_data.push_back(data_value + 50);
  }

  // Set anomary threshold (The significance level is set 0.01% in this case)
  double a_th = ChisqrSetCriticalValue(1, 0.01);
  cout << "a_th: " << a_th << endl;

  // Calcurate anomaly value
  vector<double> anomaly_data;
  for(int i = 0; i < normal_data.size(); i++){
    double data_val = normal_data[i];
    double anomaly = CalculateAnomaly(mu, sig, data_val);
    anomaly_data.push_back(anomaly);
    if(a_th <= anomaly){
      cout << "index: " << i << " is abnormal data" << endl;
      cout << "value: " << data_val << endl;
      cout << "anomaly score: " << anomaly << endl;
    }
  }

  // Plot anomaly data
  Visualize(anomaly_data, a_th);
  return 0;
}
