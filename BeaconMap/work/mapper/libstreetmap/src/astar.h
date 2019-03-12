/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   astar.h
 * Author: liuyixu6
 *
 * Created on March 17, 2018, 3:50 PM
 */

#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <queue>
#include <unordered_map>
#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "aNode.h"
#include <utility> // define std::pair

typedef std::pair<double, aNode*> aPair; // the first vertex stores F value, second is aNode* itself	

class astar{
public:

    std::vector<unsigned> get_path(aNode &start, aNode &end, double turn_penalty);
    
    std::vector<unsigned> get_poi_path(aNode &start, const std::vector<unsigned> &end, double turn_penalty);
    
    ~astar(){

        for(std::vector<aNode*>::iterator it = aNode_map.begin(); it != aNode_map.end(); ++it){
            delete *it; // delete allocated aNodes in aNode_map
        }
        aNode_map.clear();
    }

private:
    
    aNode* find_path(aNode &start, aNode &end, double turn_penalty);
    aNode* find_poi_path(aNode &start, const std::vector<unsigned> &end_poi_inter, double turn_penalty);
    
    std::vector<unsigned> get_surround_segments(const aNode* curr) const;
    
    double calcG(aNode* curr, aNode* temp, double turn_penalty);
    

private:
    std::vector<aNode*> aNode_map; // keep track of all aNodes, use to delete them in destruction
    
    std::priority_queue<aPair, std::vector<aPair>, std::greater<aPair> > openset; // min heap
    
    std::unordered_map <unsigned, double> smallest_value; // key=intersection_id, value=G_value of that node
};

#endif /* ASTAR_H */

