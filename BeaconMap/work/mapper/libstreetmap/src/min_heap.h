/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   min_heap.h
 * Author: liuyixu6
 *
 * Created on March 19, 2018, 9:56 PM
 */

#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <vector>
#include "aNode.h"
using namespace std;

class min_heap
{
private:
    vector<aNode*> _vector;
    void BubbleDown(unsigned index);
    void BubbleUp(unsigned index);
    void Heapify();

public:
//    min_heap(int* array, unsigned length);
    min_heap(const vector<aNode*>& vector);
    min_heap();
    
    void delete_heap();

    void Insert(aNode* newNode);
    aNode* GetMin();
    void DeleteMin();
    aNode* in_heap(aNode* node);
};

#endif /* MIN_HEAP_H */

