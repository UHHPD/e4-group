#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <cmath>

#include "Data.hh"

// generic function comparing two values of some type T used later for int and
// double
template <class T>
bool testEqual(const std::string& name, T expected, T real) {
  if (expected != real) {
    std::cout << "(" << name << ": <" << expected << ">"
              << ", " << real << ") ";
    return false;
  }
  return true;
}

bool testReadingSize() {
  std::cout << "testReadingSize...";
  Data datA("testA");
  return testEqual<int>("size", 1, datA.size());
}

bool testReadingMeasurement() {
  std::cout << "testReadingMeasurement...";
  Data datA("testA");
  return testEqual("measurement", 10., datA.measurement(0));
}

bool testReadingBinEdges() {
  std::cout << "testReadingBinEdges...";
  Data datA("testA");
  return testEqual("bin low", 0., datA.binLow(0)) &&
         testEqual("bin high", 1., datA.binHigh(0));
}

bool testReadingErrors() {
  std::cout << "testReadingErrors...";
  Data datA("testA");
  return testEqual("error", 2., datA.error(0));
}

bool testCopyConstructor() {
  std::cout << "testCopyConstructor...";
  Data a("testA");
  Data b(a);
  Data c = a;  // equivalent to Data c(a)
  return testEqual("measurement", 10., b.measurement(0)) &&
         testEqual("measurement", 10., c.measurement(0));
}

void runTests() {
  std::cout << "running tests...\n";
  std::vector<std::function<bool()> > tests(
      {testReadingSize, testReadingMeasurement, testReadingBinEdges,
       testReadingErrors, testCopyConstructor});
  for (auto test : tests)
    std::cout << (test() ? " ok" : " FAILED!") << std::endl;
}
std::vector<std::vector<double>> summation(Data a, Data b){
    std::vector<std::vector<double>> c;
    std::vector<double> value;
    std::vector<double> error;
    for(int i=0;i<a.size();i++){
        value.push_back((a.measurement(i)/pow(a.error(i),2)+b.measurement(i)/pow(b.error(i),2))/(1/pow(a.error(i),2)+1/pow(b.error(i),2)));
        error.push_back(sqrt(1/(1/pow(a.error(i),2)+1/pow(b.error(i),2))));
    }
    c.push_back(value);
    c.push_back(error);
    return c;

}
std::vector<std::vector<double>> summation_alt(std::vector<std::vector<double>> a_alt, Data b_alt){
    std::vector<std::vector<double>> c_alt;
    std::vector<double> value_alt;
    std::vector<double> error_alt;
    for(int i=0;i<b_alt.size();i++){
        value_alt.push_back((a_alt[0][i]/pow(a_alt[1][i],2)+b_alt.measurement(i)/pow(b_alt.error(i),2))/(1/pow(a_alt[1][i],2)+1/pow(b_alt.error(i),2)));
        error_alt.push_back(sqrt(1/(1/pow(a_alt[1][i],2)+1/pow(b_alt.error(i),2))));
    }
    c_alt.push_back(value_alt);
    c_alt.push_back(error_alt);
    return c_alt;
}
double function(double x){
    return 0.005-0.00001*x+0.08*exp(-0.015*x);
}
std::vector<double> function_vector(Data dat){
  std::vector<double> f;
  for(int i=0;i<dat.size();i++){
    f.push_back(function(dat.binCenter(i)));
  }
  return f;
}
double chi_squared_comb(std::vector<double> value,std::vector<double> error, std::vector<double> f){
  double chi_square=0;
  for(int i=0;i<value.size();i++){
    chi_square=chi_square+pow(value[i]-f[i],2)/pow(error[i],2);
  }
  chi_square=chi_square/52;
  return chi_square;
}

int main() {
  using namespace std;

  cout << "******************************************************" << endl;
  runTests();
  cout << "******************************************************" << endl;
  // create an object which holds data of experiment A
  Data datA("exp_A");
  Data datB("exp_B");
  Data datC("exp_C");
  Data datD("exp_D");

  // here is the data from experiment A
  cout << "bin 27: from " << datA.binLow(27) << " to " << datA.binHigh(27)
       << endl;
  cout << "measurement of experiment A in bin 27: " << datA.measurement(27)
       << endl;
  cout << "measurement of experiment B in bin 27: " << datB.measurement(27)
       << endl;
  cout << "measurement of experiment C in bin 27: " << datB.measurement(27)
       << endl;
  cout << "measurement of experiment D in bin 27: " << datB.measurement(27)
       << endl;
    std::cout<<"Amount of data points not being in agreement"<<std::endl;
    int AB=datA.checkCompatibility(datB,2);
    std::cout<<AB<<std::endl;
    int AC=datA.checkCompatibility(datC,2);
    std::cout<<AB<<std::endl;
    int AD=datA.checkCompatibility(datD,2);
    std::cout<<AB<<std::endl;
    int BC=datB.checkCompatibility(datC,2);
    std::cout<<AB<<std::endl;
    int BD=datB.checkCompatibility(datD,2);
    std::cout<<AB<<std::endl;
    int CD=datC.checkCompatibility(datD,2);
    std::cout<<AB<<std::endl;

    std::cout<<"Chi_squared for A-D"<<std::endl;
    std::vector<double> pred=function_vector(datA);
    double chi_A=datA.chi_squared(pred);
    double chi_B=datB.chi_squared(pred);
    double chi_C=datC.chi_squared(pred);
    double chi_D=datD.chi_squared(pred);
    std::cout<<chi_A<<std::endl;
    std::cout<<chi_B<<std::endl;
    std::cout<<chi_C<<std::endl;
    std::cout<<chi_D<<std::endl;
    std::vector<std::vector<double>> Hilf1=summation(datA,datB);
    std::vector<std::vector<double>> Hilf2=summation_alt(Hilf1,datC);
    std::vector<std::vector<double>> Hilf3=summation_alt(Hilf2,datD);
    double Chi_comb=chi_squared_comb(Hilf3[0],Hilf3[1],pred);
    std::cout<<"combined Chi^2"<<std::endl;
    std::cout<<Chi_comb<<std::endl;



  return 0;
}
