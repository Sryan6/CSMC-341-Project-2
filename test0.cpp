// CMSC 341 - Fall 2019 - Project 2
// test0.cpp is a simple test program

#include <iostream>
#include <vector>
#include <cassert>
#include "raq.h"

using namespace std;

// Function to compute a single query directly from the data.
// Implementation follows main().

float avg(vector<float> data, int i, int j);

int main() {

  // This is the data from the project description
  vector<float> data = {34, 16, 58, -24, 53,
			7, 97, 92, -12, 45,
			9};

  // Print the test data
  
  cout << "Test data:\n";
  for (int i = 0; i < data.size(); i++) {
    cout << data[i] << "  ";
  }
  cout << "\n\n";

  // Create the RAQ object (does precomputation) and dump the data
  // structure

  cout << "Building RAQ\n";
  RAQ raq(data);
  raq.dump();

  // Create the BlockRAQ object (does precomputation and dump the data
  // structure

  cout << "\nBuilding BlockRAQ\n";
  BlockRAQ braq(data);
  braq.dump();

  // Simple correctness test.  Loop over all (i, j) pairs; compute
  // query using avg() function, RAQ query, and BlockRAQ query;
  // compare results; print if there is a discrepancy
  
  cout << "\nCorrectness test:\n";

  bool err = false;

  for (int j = 1; j < data.size(); j++) {
    for (int i = 0; i < j; i++) {

      float x = avg(data, i, j) ;
      float y = raq.query(i, j) ;
      float z = braq.query(i, j) ;

      // Print message if direct computation disagrees with either RAQ
      // or BlockRAQ.  Note: their can be rounding errors, which are
      // not really errors.
      
      if ( (x != y ) || (x != z) ) {
	cout << "(" << i << ", " << j << "): ";
	cout << x << "  " << y << "  " << z << endl;
	err = true;
      }
    }
  }

  if (!err) {
    cout << "No errors.\n";
  }

  return 0;
}

float avg(vector<float> data, int i, int j) {
  assert( 0 <= i && i <= j && j < data.size() );
  float tmp = 0.0;
  for (int k = i; k <= j; k++) {
    tmp += data[k];
  }
  return tmp / (j - i + 1);
}
