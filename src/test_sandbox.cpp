#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;

double Uniform(){
  double uni = ((double)rand()+1.0)/((double)RAND_MAX+2.0);
  return uni;
}

int main(void){
  srand((unsigned)time(NULL));
  double mu = 0.0, sig = 10.0;
  for(int i = 0; i < 10; i++){
    double z = sqrt(-2.0*log(Uniform()))*sin(2.0*M_PI*Uniform());
    double val = 170.0+mu+sig*z;
    cout << val << endl;
  }

  return 0;
}
