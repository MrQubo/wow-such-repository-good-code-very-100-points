#!/bin/bash
g++ -isystem -pthread treap_test.cpp -lgtest -o treap_test
./treap_test
