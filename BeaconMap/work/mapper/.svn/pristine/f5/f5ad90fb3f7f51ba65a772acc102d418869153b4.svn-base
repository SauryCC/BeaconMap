/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "threecontinuous.h"

struct Path {
    std::vector <unsigned> intersection ; 
    double distance; 
};

Path bestpath; 
double dis;
std::unordered_map <unsigned, std::vector<unsigned>> drop; 
std::unordered_map <unsigned, std::vector<unsigned>> pick; 




double distance (unsigned from, unsigned to){
    
    
    LatLon froml=getIntersectionPosition(from);
    LatLon tol=getIntersectionPosition(to);
    // change degree to radian
    double lat_avg = DEG_TO_RAD * (froml.lat()+ tol.lat())/2;
    
    // x = lon * cos(lat_avg)
    double x1 = DEG_TO_RAD * froml.lon() * cos(lat_avg);
    double x2 = DEG_TO_RAD * tol.lon() * cos(lat_avg);
    // y = lat
    double y1 = DEG_TO_RAD * froml.lat();
    double y2 = DEG_TO_RAD * tol.lat();
    
    // d = R * sqrt((y1-y2)^2 + (x1-x2)^2)
    return EARTH_RADIUS_IN_METERS * sqrt((y2-y1)*(y2-y1) + (x2-x1)*(x2-x1));


}


void InitialPath (const std::vector<DeliveryInfo>& deliveries){
   
    
//    for (unsigned i = 0; i< deliveries.size(); i++){
//        bestpath.intersection.push_back(deliveries[i].pickUp);
//    }
//    for (unsigned i = 0; i< deliveries.size(); i++){
//        bestpath.intersection.push_back(deliveries[i].dropOff);
//    }
//    
//    for (unsigned i = 0; i< bestpath.intersection.size(); i++){
//        if (i != (bestpath.intersection.size() -1 )){
//            bestpath.distance += distance(bestpath.intersection[i], bestpath.intersection[i+1]);
//        }
//    }
     for (unsigned i = 0; i<deliveries.size();i++){
        std::unordered_map <unsigned,std::vector<unsigned>>  ::iterator it = drop.find(deliveries[i].dropOff);
        if (it == drop.end()){
            drop.insert(std::pair<unsigned,std::vector<unsigned>>(deliveries[i].dropOff, std::vector<unsigned>())); 
            drop[deliveries[i].dropOff].push_back(deliveries[i].pickUp);
        }
        else {
            drop[deliveries[i].dropOff].push_back(deliveries[i].pickUp);
        }
    }
    
   
    
    for (unsigned i = 0; i<deliveries.size();i++){
        std::unordered_map <unsigned, std::vector <unsigned>> ::iterator it = pick.find(deliveries[i].pickUp); // one intersection can pick up multiple packages
        
        if (it == pick.end()){
            pick.insert(std::pair<unsigned, std::vector<unsigned>>(deliveries[i].pickUp, std::vector<unsigned>()));
            pick[deliveries[i].pickUp].push_back(deliveries[i].dropOff);
        }
        else {
            pick[deliveries[i].pickUp].push_back(deliveries[i].dropOff); 
        }
        
    }
   

}

void sixcontinuous (){
       Path currentpath;
               currentpath.intersection= bestpath.intersection; 
               currentpath.distance = bestpath.distance ;
//       double bestdis ; 
       //double newdis; 
       Path newpath; 
       bool valid = true;
       //std::vector<unsigned> combinations;
//       int num = 0;
       
//       while (num>2000 ){
       
       if (currentpath.intersection.size()>= 8) {
   for (unsigned time= 0; time < 2; time ++){        
       for (unsigned first = 1; first <(currentpath.intersection.size()-7 );first ++ )
       {
               std::vector<unsigned> combinations; 
               combinations.insert (combinations.end(), bestpath.intersection.begin()+first,bestpath. intersection.begin()+first+6);
               // original best distance; 
//               bestdis = bestpath.distance;
               
               
               std:: sort (combinations.begin(), combinations.end()); 
               
               do {
                   for (unsigned i = 0 ; i<combinations.size() ; i++)
                   {
                       if (drop.find(combinations[i])!= drop.end())
                       {
                           // this position is a drop off point, then 
                           // check its corresponding pick up point to see if it in the list 
                           for (unsigned j = 0; j <combinations.size(); j++)
                           {
                               if (combinations[j] != combinations[i]) // avoid check the same element 
                               {
                                   for (unsigned k = 0; k < drop[combinations[i]].size(); k++) // the pick up corresponding to this drop 
                                   {
                                       if (drop[combinations[i]][k] == combinations[j]) // if we find the corresponding pick up 
                                       {
                                           if (j>i){ // i represents the the drop off and j represents the position of its corresponding pick up
                                               valid = false ;
                                               break;
                                           }
                                       }
                                   }
                               }
                               if (!valid) break;
                           }
                       }
                       if (!valid) break; 
                   }
                   if (valid)
                   {
                       newpath.intersection = currentpath.intersection; 
                       std::copy (combinations.begin(),combinations.end(), newpath.intersection.begin()+first);
                       newpath.distance = 0;
                       for (unsigned i = 0; i < (newpath.intersection.size()-1); i++ )
                         newpath.distance += distance (newpath.intersection[i], newpath.intersection[i+1]); 
                       
                       if (newpath.distance < bestpath.distance){
                           bestpath.distance = newpath.distance; 
                           bestpath.intersection = newpath.intersection;
                           currentpath.intersection = newpath.intersection;
                           
                          
                       }
                       
                   }
                   else {
                     valid = true; // reset the flag 
                   }
               }
                while( std::next_permutation(combinations.begin(), combinations.end()));
               

                combinations.clear();
       } 
   }
       }
//       num ++;
       
}

void fivecontinuous (){
       Path currentpath;
               currentpath.intersection= bestpath.intersection; 
               currentpath.distance = bestpath.distance ;
//       double bestdis ; 
      // double newdis; 
       Path newpath; 
       bool valid = true;
      // std::vector<unsigned> combinations;
//       int num = 0;
       
//       while (num>2000 ){
       
       if (currentpath.intersection.size()>= 8) {
   for (unsigned time= 0; time < 10; time ++){        
       for (unsigned first = 1; first <(currentpath.intersection.size()-6 );first ++ )
       {
               std::vector<unsigned> combinations; 
               combinations.insert (combinations.end(), bestpath.intersection.begin()+first,bestpath. intersection.begin()+first+5);
               // original best distance; 
//               bestdis = bestpath.distance;
               
               
               std:: sort (combinations.begin(), combinations.end()); 
               
               do {
                   for (unsigned i = 0 ; i<combinations.size() ; i++)
                   {
                       if (drop.find(combinations[i])!= drop.end())
                       {
                           // this position is a drop off point, then 
                           // check its corresponding pick up point to see if it in the list 
                           for (unsigned j = 0; j <combinations.size(); j++)
                           {
                               if (combinations[j] != combinations[i]) // avoid check the same element 
                               {
                                   for (unsigned k = 0; k < drop[combinations[i]].size(); k++) // the pick up corresponding to this drop 
                                   {
                                       if (drop[combinations[i]][k] == combinations[j]) // if we find the corresponding pick up 
                                       {
                                           if (j>i){ // i represents the the drop off and j represents the position of its corresponding pick up
                                               valid = false ;
                                               break;
                                           }
                                       }
                                   }
                               }
                               if (!valid) break;
                           }
                       }
                       if (!valid) break; 
                   }
                   if (valid)
                   {
                       newpath.intersection = currentpath.intersection; 
                       std::copy (combinations.begin(),combinations.end(), newpath.intersection.begin()+first);
                       newpath.distance = 0;
                       for (unsigned i = 0; i < (newpath.intersection.size()-1); i++ )
                         newpath.distance += distance (newpath.intersection[i], newpath.intersection[i+1]); 
                       
                       if (newpath.distance < bestpath.distance){
                           bestpath.distance = newpath.distance; 
                           bestpath.intersection = newpath.intersection;
                           currentpath.intersection = newpath.intersection;
                           
                          
                       }
                       
                   }
                   else {
                     valid = true; // reset the flag 
                   }
               }
                while( std::next_permutation(combinations.begin(), combinations.end()));
               

                combinations.clear();
       } 
   }
       }
//       num ++;
       
}
void fourcontinuous (){
       Path currentpath;
               currentpath.intersection= bestpath.intersection; 
               currentpath.distance = bestpath.distance ;
//       double bestdis ; 
      // double newdis; 
       Path newpath; 
       bool valid = true;
       //std::vector<unsigned> combinations;
//       int num = 0;
       
//       while (num>2000 ){
       
       if (currentpath.intersection.size()>= 10) {
   for (unsigned time= 0; time < 10; time ++){        
       for (unsigned first = 1; first <(currentpath.intersection.size()-5 );first ++ )
       {
               std::vector<unsigned> combinations; 
               combinations.insert (combinations.end(), bestpath.intersection.begin()+first,bestpath. intersection.begin()+first+4);
               // original best distance; 
//               bestdis = bestpath.distance;
               
               
               std:: sort (combinations.begin(), combinations.end()); 
               
               do {
                   for (unsigned i = 0 ; i<combinations.size() ; i++)
                   {
                       if (drop.find(combinations[i])!= drop.end())
                       {
                           // this position is a drop off point, then 
                           // check its corresponding pick up point to see if it in the list 
                           for (unsigned j = 0; j <combinations.size(); j++)
                           {
                               if (combinations[j] != combinations[i]) // avoid check the same element 
                               {
                                   for (unsigned k = 0; k < drop[combinations[i]].size(); k++) // the pick up corresponding to this drop 
                                   {
                                       if (drop[combinations[i]][k] == combinations[j]) // if we find the corresponding pick up 
                                       {
                                           if (j>i){ // i represents the the drop off and j represents the position of its corresponding pick up
                                               valid = false ;
                                               break;
                                           }
                                       }
                                   }
                               }
                               if (!valid) break;
                           }
                       }
                       if (!valid) break; 
                   }
                   if (valid)
                   {
                       newpath.intersection = currentpath.intersection; 
                       std::copy (combinations.begin(),combinations.end(), newpath.intersection.begin()+first);
                       newpath.distance = 0;
                       for (unsigned i = 0; i < (newpath.intersection.size()-1); i++ )
                         newpath.distance += distance (newpath.intersection[i], newpath.intersection[i+1]); 
                       
                       if (newpath.distance < bestpath.distance){
                           bestpath.distance = newpath.distance; 
                           bestpath.intersection = newpath.intersection;
                           currentpath.intersection = newpath.intersection;
                           
                          
                       }
                       
                   }
                   else {
                     valid = true; // reset the flag 
                   }
               }
                while( std::next_permutation(combinations.begin(), combinations.end()));
               

                combinations.clear();
       } 
   }
       }
//       num ++;
       
}


void threecontinous (){
       Path currentpath;
               currentpath.intersection= bestpath.intersection; 
               currentpath.distance = bestpath.distance ;
//      double bestdis ; 
      // double newdis; 
       Path newpath; 
       bool valid = true;
       //std::vector<unsigned> combinations;
       //int num = 0;
       
//       while (num>2000 ){
       
       if (currentpath.intersection.size()>= 10) {
   for (unsigned time= 0; time < 15; time ++){        
       for (unsigned first = 1; first <(currentpath.intersection.size()-4 );first ++ )
       {
               std::vector<unsigned> combinations; 
               combinations.insert (combinations.end(), bestpath.intersection.begin()+first,bestpath. intersection.begin()+first+3);
               // original best distance; 
//               bestdis = bestpath.distance;
               
               
               std:: sort (combinations.begin(), combinations.end()); 
               
               do {
                   for (unsigned i = 0 ; i<combinations.size() ; i++)
                   {
                       if (drop.find(combinations[i])!= drop.end())
                       {
                           // this position is a drop off point, then 
                           // check its corresponding pick up point to see if it in the list 
                           for (unsigned j = 0; j <combinations.size(); j++)
                           {
                               if (combinations[j] != combinations[i]) // avoid check the same element 
                               {
                                   for (unsigned k = 0; k < drop[combinations[i]].size(); k++) // the pick up corresponding to this drop 
                                   {
                                       if (drop[combinations[i]][k] == combinations[j]) // if we find the corresponding pick up 
                                       {
                                           if (j>i){ // i represents the the drop off and j represents the position of its corresponding pick up
                                               valid = false ;
                                               break;
                                           }
                                       }
                                   }
                               }
                               if (!valid) break;
                           }
                       }
                       if (!valid) break; 
                   }
                   if (valid)
                   {
                       newpath.intersection = currentpath.intersection; 
                       std::copy (combinations.begin(),combinations.end(), newpath.intersection.begin()+first);
                       newpath.distance = 0;
                       for (unsigned i = 0; i < (newpath.intersection.size()-1); i++ )
                         newpath.distance += distance (newpath.intersection[i], newpath.intersection[i+1]); 
                       
                       if (newpath.distance < bestpath.distance){
                           bestpath.distance = newpath.distance; 
                           bestpath.intersection = newpath.intersection;
                           currentpath.intersection = newpath.intersection;
                           
                          
                       }
                       
                   }
                   else {
                     valid = true; // reset the flag 
                   }
               }
                while( std::next_permutation(combinations.begin(), combinations.end()));
               

                combinations.clear();
       } 
   }
       }
//       num ++;
       
}


std::vector<unsigned> threecontinuoushandler(const std::vector<DeliveryInfo>& deliveries, 
                                         std::vector<unsigned> ans){
    
    bestpath.intersection = ans; 
    for (unsigned i = 0; i< bestpath.intersection.size()-1 ; i++){
        bestpath.distance += distance(bestpath.intersection[i] , bestpath.intersection[i+1]);
    }
    InitialPath (deliveries);
    sixcontinuous ();
    fivecontinuous();
    fourcontinuous();
    threecontinous();;
    
  
    
    
    
    
    return bestpath.intersection; 
}