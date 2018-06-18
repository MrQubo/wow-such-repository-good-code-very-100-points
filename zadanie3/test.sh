#!/bin/bash
g++ -std=c++17 -isystem -pthread treap_test.cpp -lgtest -o treap_test
./treap_test
