#!/bin/bash

 nvcc isgood.cu tensor.cu cudatensor.cu ../permutation/permutation.cpp -o isgood -lgtest -lpthread -g --relocatable-device-code=true 
