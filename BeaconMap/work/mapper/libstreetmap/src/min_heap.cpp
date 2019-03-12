/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "min_heap.h"
#include <iostream>
//
//min_heap::min_heap(int* array, int length) : _vector(length)
//{
//    for(int i = 0; i < length; ++i)
//    {
//        _vector[i] = array[i];
//    }
//
//    Heapify();
//}

min_heap::min_heap(const vector<aNode*>& vector) : _vector(vector)
{
    Heapify();
}

min_heap::min_heap() 
{
}


void min_heap::delete_heap(){
    
    for(std::vector<aNode*>::const_iterator it = _vector.begin(); it != _vector.end(); ++it){
        delete *it;
    }
    _vector.clear();
}

void min_heap::Heapify()
{
    unsigned length = _vector.size();
    for(int i=length-1; i>=0; --i)
    {
        BubbleDown(i);
    }
}

void min_heap::BubbleDown(unsigned index)
{
    unsigned length = _vector.size();
    unsigned leftChildIndex = 2*index + 1;
    unsigned rightChildIndex = 2*index + 2;

    if(leftChildIndex >= length)
        return; //index is a leaf

    unsigned minIndex = index;

    if(_vector[index]->F > _vector[leftChildIndex]->F)
    {
        minIndex = leftChildIndex;
    }
    
    if((rightChildIndex < length) && (_vector[minIndex]->F > _vector[rightChildIndex]->F))
    {
        minIndex = rightChildIndex;
    }

    if(minIndex != index)
    {
        //need to swap
        aNode* temp = _vector[index];
        _vector[index] = _vector[minIndex];
        _vector[minIndex] = temp;
        BubbleDown(minIndex);
    }
}

void min_heap::BubbleUp(unsigned index)
{
    if(index == 0)
        return;

    unsigned parentIndex = (index-1)/2;

    if(_vector[parentIndex]->F > _vector[index]->F)
    {
        aNode* temp = _vector[parentIndex];
        _vector[parentIndex] = _vector[index];
        _vector[index] = temp;
        BubbleUp(parentIndex);
    }
}

void min_heap::Insert(aNode* newNode)
{
    unsigned length = _vector.size();
    if(length == 0){
        _vector.push_back(newNode);
        return;
    }
    
    _vector.push_back(newNode);

    BubbleUp(length);
}

aNode* min_heap::GetMin()
{
//    cout << _vector[0]->index << endl;
//    cout << (_vector[0] == NULL) << endl;
    return _vector[0];
}
    
void min_heap::DeleteMin()
{
    unsigned length = _vector.size();

    if(length == 0)
    {
        return;
    }
    
    _vector[0] = _vector[length-1];
    _vector.pop_back();

    BubbleDown(0);
}

aNode* min_heap::in_heap(aNode* node){
    for(std::vector<aNode*>::const_iterator it = _vector.begin(); it != _vector.end(); ++it){
        if((*it)->index == node->index){
            return *it;
        }
    }
    return NULL;
}