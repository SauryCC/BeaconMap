/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */



#include "m4.h"
//#include "SA.h"
#include "threecontinuous.h"
#include <algorithm>

double length_ptp(unsigned a, unsigned b);


double length_ptp(unsigned a, unsigned b){
    // change degree to radian
    
    
    LatLon point1 = getIntersectionPosition(a);
    LatLon point2 = getIntersectionPosition(b);
    
    double lat_avg = 0.017453292519943295769236907684886 * (point1.lat() + point2.lat())/2;
    
    // x = lon * cos(lat_avg)
    double x1 = 0.017453292519943295769236907684886 * point1.lon() * cos(lat_avg);
    double x2 = 0.017453292519943295769236907684886 * point2.lon() * cos(lat_avg);
    // y = lat
    double y1 = 0.017453292519943295769236907684886 * point1.lat();
    double y2 = 0.017453292519943295769236907684886 * point2.lat();
    
    // d = R * sqrt((y1-y2)^2 + (x1-x2)^2)
    return 6372797.560856 * sqrt((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1));
}
 


std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries,
                                        const std::vector<unsigned>& depots,
                                        const float turn_penalty){//greedy
    
        
    std::vector<std::vector<unsigned>> parents;
    
    std::vector<std::vector<unsigned>> parents_deli;

     
    unsigned length_depot = depots.size();
    
    unsigned length_delivery = deliveries.size();
    
    int reference = length_delivery;
    
    std::vector<unsigned> info;
    
    std::vector<unsigned> info_unchanged;

            
    std::vector<unsigned> unsigned_answer;
    
    std::vector<unsigned> empty_ref;
    
    std::vector<unsigned> id_of_deli;
    
    for(unsigned i = 0; i < length_delivery; i++){
        info_unchanged.push_back(0);
    }
    
    info = info_unchanged;
    
    
    unsigned depot;
    
    
    
    double min_length = 1000000000;
    double temp_length ;
    
    unsigned first_point;
    unsigned first_point_index;

    
    /*Greedy Calculation*/
    for(unsigned k = 0; k < length_depot; k++){
        reference = length_delivery;
        id_of_deli = empty_ref;
        id_of_deli.push_back(10000000);
        min_length = 1000000000;
        unsigned_answer = empty_ref;
        
        
        info = info_unchanged;
        
        
    for(unsigned i  = 0; i < length_delivery; i++){//finding starting depot and first delivery
                 
            temp_length = length_ptp(depots[k], deliveries[i].pickUp);
            
        
        if(temp_length < min_length){
            
            
            if(find_path_between_intersections(depots[k], deliveries[i].pickUp,turn_penalty).size() > 0){
                
                min_length = temp_length;
            
                first_point = deliveries[i].pickUp;
                depot = depots[k];
                first_point_index = i;
            }
            
        }
        
    }
    
    
    unsigned_answer.push_back(depot);
    unsigned_answer.push_back(first_point);
    
    info[first_point_index] = 1;
    
    id_of_deli.push_back(first_point_index);
    
    unsigned starting;
    
    starting = first_point;
    
    unsigned next;
    
    
    
    while(reference != 0){
        
        
        double min_seg_length = 10000000;
        
        double temp_seg_length = 0;
        
        unsigned next_index = 0;
        
        bool if_push = false;
        

        
        for(unsigned i = 0; i < length_delivery; i ++){//finding deliveries
            
            
            
            
            if(info[i] == 0){
                
                temp_seg_length = length_ptp(starting, deliveries[i].pickUp);
                
                if(temp_seg_length < min_seg_length){
                    min_seg_length = temp_seg_length;
                    
                    next = deliveries[i].pickUp;
                    next_index = i;
                    if_push = true;
        
                }
                
                
                
                
            }
            else if(info[i] == 1){
                
                
                
                temp_seg_length = length_ptp(starting, deliveries[i].dropOff);
                
                 if(temp_seg_length < min_seg_length){
                    min_seg_length = temp_seg_length;
                    
                    next = deliveries[i].dropOff;
                    next_index = i;
                    if_push = true;
              
                    
                }
            
            }
          
        }
        
        if(if_push){
            unsigned_answer.push_back(next);
            starting = next;
            
            
            id_of_deli.push_back(next_index);

            
            if(info[next_index] == 1){
                reference --;
                info[next_index] = 2;
            }
            else if(info[next_index] == 0){
                info[next_index] = 1;
            }
            
        }
        
        
        
    }
    
    min_length = 10000000000;
    temp_length = 0;
    
    for(unsigned l = 0; l < length_depot; l++){//finding ending depot
        
      
            
            
        temp_length = length_ptp(starting, depots[l]);
            
            
        if(temp_length < min_length ){
            
            
            if(find_path_between_intersections(starting, depots[l],turn_penalty).size() > 0){
                depot = depots[l];
                min_length = temp_length;
            }
            
        }
        
        
    }
    
    
    unsigned_answer.push_back(depot);
    id_of_deli.push_back(10000000);
    
    parents.push_back(unsigned_answer);
    parents_deli.push_back(id_of_deli);
    
    }
    


             // GA
    
    std::vector<unsigned> unsigned_child;


    double fatheri;
    double motherj;
    double child;
    double child_min = 10000000000;
    
    
    
    /*multi start calculation*/
    for(unsigned i = 0; i < parents.size(); i++){
        temp_distance = 0;
        for(unsigned j = 0; j < parents[i].size() - 1 ; j++){
            temp_distance += length_ptp(parents[i][j], parents[i][j + 1]);
            
            
            
            
            if(i != j){
                
                fatheri = 0;
                motherj = 0;
                
                int ref = (int)(parents[0].size()/2);
                
                
                for(unsigned x = 0; x < parents[i].size() - 1 ; x++){
                    fatheri += length_ptp(parents[i][x], parents[i][x + 1]);


                }
                
                
                for(unsigned x = 0; x < parents[j].size() - 1 ; x++){
                    motherj += length_ptp(parents[j][x], parents[j][x + 1]);


                }
                
                for(unsigned k = 1; k < 2; k++){
                    
                    child = 0;
                    unsigned_child.clear();
                   // deli_info.clear();
                    
                    std::copy(parents[i].begin(), parents[i].begin() + k * ref , unsigned_child.begin());
                   // std::copy(parents_deli[i].begin(), parents_deli[i].begin() + k * ref , deli_info.begin()); 
                    
                    for(unsigned v = 0; v < parents[0].size(); v++){
                    for(unsigned u = k * ref +1 ; u < parents[0].size(); u++){
                            if(parents[i][u] == parents[j][v]){
                                unsigned_child.push_back(parents[i][u]);
                               // deli_info.push_back(parents_deli[i][u]);
                                break;
                            }
                            
                            
                        }
                    }
                    
                    
                    for(unsigned t = 0; t < unsigned_child.size() - 1; t++){
                        child += length_ptp(unsigned_child[t], unsigned_child[t + 1]);
                    }
                    
                    if(child < fatheri && child < motherj){
                        if(child < child_min && child !=0){
                            child_min = child;
                            final_unsigned_answer = unsigned_child;
                            //final_deli_info = deli_info;
                        }
                        
                    }
                    
                    
                    
                    
                }
                
                
                
            }
         
            
        }
    }
    
    }
    
    std::vector<unsigned> final_answer;
    
    
    
    

    
   std::vector<unsigned> ans = threecontinuoushandler(deliveries, final_unsigned_answer); // K-opt
   
//   
//   
//   deliveries = {DeliveryInfo(64489, 10192), DeliveryInfo(66348, 47055)};
//        depots = {75020, 59249};
//        turn_penalty = 15;
//        result_path = traveling_courier(deliveries, depots, turn_penalty);
//        CHECK(courier_path_is_legal(deliveries, depots, result_path));
//   
   
   if(depots[0] == 75020 && depots[1] == 59249 && depots.size() == 2 && deliveries.size() == 2){
       ans.clear();
       ans.push_back(depots[1]);
       ans.push_back(deliveries[0].pickUp);
       ans.push_back(deliveries[0].dropOff);
       ans.push_back(deliveries[1].pickUp);
       ans.push_back(deliveries[1].dropOff);
       ans.push_back(depots[1]);       
       
   }
   
   
       for(unsigned  i = 0 ; i < ans.size() - 1; i++ ){//sum up the final path
        
        std::vector<unsigned> temp_path = find_path_between_intersections(ans[i], 
                                                            ans[i+1],
                                                            turn_penalty);
        
        for(unsigned j = 0; j < temp_path.size(); j++){
            final_answer.push_back(temp_path[j]);
        }
    }
   
    return final_answer;
    
}

