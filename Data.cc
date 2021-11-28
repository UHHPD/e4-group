#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <cmath>
#include <vector>

using namespace std;

Data::Data(const std::string& filename) {
  ifstream file(filename);

  // check if file opened correctly
  if (!file.is_open()) {
    throw std::runtime_error("Error could not open file with name: " +
                             filename);
  }

  int size = -1;
  file >> size;

  // read in data from file: m_siz+1 bin edges
  for (int i = 0; i < size + 1; ++i) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }
  // read in data from file: m_siz bin contents
  for (int i = 0; i < size; ++i) {
    double entries;
    file >> entries;
    m_data.push_back(entries);
  }
  // read in data from file: m_siz bin error
  for(int i = 0;i<size;++i){
    double error;
    file >> error;
    m_error.push_back(error);
  }

  // done! close the file
  file.close();

  assertSizes();
};

void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }
int Data::checkCompatibility(const Data& in, int n){
    int k=0;
    for(int i=0;i<m_data.size();i++){
        double diff =sqrt(pow(m_data[i]-in.measurement(i),2));
        double error=sqrt(pow(m_error[i],2)+pow(in.error(i),2));
        if (diff > n*error){
            k++;
        }
        
    }
    return k;
}
double Data::chi_squared(std::vector<double> f){
  double chi_square=0;
  for(int i=0;i<m_data.size();i++){
    chi_square=chi_square+pow(m_data[i]-f[i],2)/pow(m_error[i],2);
  }
  chi_square=chi_square/52;
  return chi_square;
}


