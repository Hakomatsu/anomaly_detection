#include <iostream>
#include <boost/math/distributions/chi_squared.hpp>
using namespace std;
using namespace boost::math;

int main(void){
  chi_squared dist(1);
  double q = quantile(complement(dist, 0.01));

  cout << q << endl;

  return 0;
}
