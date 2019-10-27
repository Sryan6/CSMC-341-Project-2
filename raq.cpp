//
// raq.cpp
//

#include <iostream>
#include "raq.h"
#include <cmath>

using namespace std;

RAQ::RAQ(vector<float> data){
  //This instantiates a 2d vector of the side needed
  //to hold the precomputed data.
  for(int a = 0; a < data.size(); a++){
    _sums.resize(data.size());
    for(int b = 0; b < data.size(); b++){
      _sums[a].resize(data.size());
    }
  }
  for(int i = 0; i < data.size(); i++){
    //sets the single query values
    for(int j = 0; j < data.size(); j++){
      _sums[j][j] = data[j];
    }
    //computes the values for queries of greater range
    //based off the previously computed queries
    for(int j = 0; j < data.size() - i - 1; j++){
	_sums[j][j+1+i] = _sums[j][j+i] + _sums[j+i+1][j+i+1];
    }
  }
}

float RAQ::query(int i, int j)const{
  //returns the average of the sum query or an error if the range is invalid
  if(j < i || i < 0 || j >= _sums.size()){
    throw(domain_error("Invalid Query range"));
  }
    return _sums[i][j] / (j - i + 1);
}

void RAQ::dump()const{
  cout << "RAQ.dump():\n" << endl;
  //prints out the structure of the array
  for(int i = 0; i < _sums.size(); i++){
    for(int j = 0; j < _sums[i].size(); j++){
      cout << _sums[i][j] << " ";
    }
    cout << endl;
  }
}

BlockRAQ::BlockRAQ(vector<float> givenData){
  _bValue = (int) sqrt((float) givenData.size());
  
  //copies the parameter into the private member variable.
  for(int i = 0; i < givenData.size(); i++){
    _data.push_back(givenData[i]);
  }

  //makes blocksums an empty vector the same size as the data
  int index = -1;
  _blockSums.resize(givenData.size());

  //loads the blocksums vector with sums of blocks of size b
  for(int i = 0; i < _data.size(); i++){
    if(i % _bValue == 0){
      index++;
    }
    _blockSums[index] += _data[i];
  }
  //resizes blocksums into all the averages, cutting off
  //any unneccessary null variables.
  _blockSums.resize(index + 1);
}

float BlockRAQ::query(int i, int j)const{
  float querySum = 0;
  int avgValue = 0;

  //Throws an error if the query numbers are invalid
  if(j < i || i < 0 || j >= _data.size()){
    throw(domain_error("Invalid Query range"));
  }
  
  //Checks the first block within the range and adds any
  //extraneous values outside the block range on the minimum side
  while(i < j && i % _bValue != 0 && i != 0){
    querySum += _data[i];
    i += 1;
    avgValue += 1;
  }
  //Adds the values of the overlapped blocks from the precomputed
  //block array.
  while(i + _bValue <= j){
    querySum += _blockSums[i/_bValue];
    i += _bValue;
    avgValue += _bValue;
  }
  //Adds any values outside of the blocks near the maximum side from
  //the original array.
  while(i <= j){
    querySum += _data[i];
    i+= 1;
    avgValue += 1;
  }
  //returns the average of the computed sum of the query
  return querySum / avgValue;
}

void BlockRAQ::dump()const{
  //prints required fields of the BlockRAQ
  cout << "Number of Blocks: " << _blockSums.size() << endl;
  cout << "Block size: " << _bValue << endl;
  cout << "Block averages: ";
  for(int i = 0; i < _blockSums.size(); i++){
    cout << _blockSums[i] / _bValue << " ";
  }
}
