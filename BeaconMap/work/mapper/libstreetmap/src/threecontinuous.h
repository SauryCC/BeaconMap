/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   threecontinuous.h
 * Author: zhourui7
 *
 * Created on April 4, 2018, 7:15 PM
 */

#ifndef THREECONTINUOUS_H
#define THREECONTINUOUS_H

#include "m1.h"
#include "m3.h"
#include "m4.h"
#include <vector>
#include <algorithm>
#include "dataStructure.h"
#include "StreetsDatabaseAPI.h"
double distance (unsigned from, unsigned to);
std::vector<unsigned> threecontinuoushandler(const std::vector<DeliveryInfo>& deliveries, 
                                        std::vector<unsigned> ans);
void sixcontinuous ();
void fivecontinuous();
void fourcontinuous ();
void threecontinous();
void InitialPath (const std::vector<DeliveryInfo>& deliveries);
#endif /* THREECONTINUOUS_H */

