/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "m2_header.h"

void draw_map(){
    
    // the following code calculates average latitude
    // also helps drawing set_visible_world()
    double lat_max = getIntersectionPosition(0).lat();
    double lat_min = lat_max;
    double lon_max = getIntersectionPosition(0).lon();
    double lon_min = lon_max;
  
    for(unsigned a = 0; a < getNumberOfIntersections(); a ++){
        double temp_lat = getIntersectionPosition(a).lat();
        double temp_lon = getIntersectionPosition(a).lon();
         if(a==0){
             lat_max = temp_lat;
             lat_min = temp_lat;
             lon_max = temp_lon;
             lon_min = temp_lon;
         }
         if(lat_max < temp_lat) lat_max = temp_lat;
         if(lon_max < temp_lon) lon_max = temp_lon;
         if(lat_min > temp_lat) lat_min = temp_lat;
         if(lon_min > temp_lon) lon_min = temp_lon; 
    }
    avg_lat = 0.5*(lat_max + lat_min);
//    std::cout << lon_max << " , " << lon_min <<std::endl;
//    std::cout << "?????????????????????" <<std::endl;
    /**************************************************************************/
    
    //thumbnail load data structure
    unsigned num_of_features = getNumberOfFeatures();
    data_structure.dims_features.assign(num_of_features,std::vector<t_point> ());


    //t_bound_box word_box = get_visible_world();

    for(unsigned i = 0; i < num_of_features; i++){

        unsigned num_of_points = getFeaturePointCount(i);
        t_point list_of_points;

        for(unsigned a = 0; a < num_of_points; a++){
            list_of_points.x = getFeaturePoint(i, a).lon() * cos(avg_lat) * DEG_TO_RAD;
            list_of_points.y = getFeaturePoint(i, a).lat() * DEG_TO_RAD;
            data_structure.dims_features[i].push_back(list_of_points);
        }
        data_structure.dims_features[i].push_back(list_of_points);

    }
 
    /**************************************************************************/
    
    
    init_graphics("Some Example Graphics", t_color(235,235,235,255));
    
  //  create_button("Window", "Search", search_button);
    //create_button("Window", "OSM", osm_button);
    create_button("Window", "SearchBar", searchbar_button);
    create_button("Window", "Attractions", attraction_button);
    create_button("Window", "Subways", subway_button);
    create_button("Window", "Set_from", set_from);
    create_button("Window", "Set_to", set_to);
    create_button("Window", "Navigate", navigate);
    create_button("Window", "Click_Search", click_searching);

    
    set_visible_world (lon_min * cos(avg_lat) * DEG_TO_RAD, lat_min * DEG_TO_RAD,
                       lon_max * cos(avg_lat) * DEG_TO_RAD, lat_max * DEG_TO_RAD);
    
    update_message("Map");
    set_drawing_buffer(OFF_SCREEN);
    set_mouse_move_input(true);
    set_keypress_input(if_search);
    
    
    clearscreen();
    event_loop(act_on_mousebutton,NULL,act_on_key_press, drawscreen);
    
    close_graphics();
}

void act_on_mousebutton(float x, float y, t_event_buttonPressed button_info){
      if(button_info.button == 1 || button_info.button == 3){
            
            click = true;  
            x_click = x;
            y_click = y ; 
//            std::cout <<"x coordinate:" <<x_click << std::endl; 
//            std:: cout<< "y coordinate:" << y_click <<std::endl; 
    
           t_bound_box test_box =  t_bound_box(current_box.left(),current_box.top(),current_box.left() + 
                current_box.get_width()/15 ,current_box.top() - current_box.get_height()/15);
              //  t_bound_box test_navi_pre =  t_bound_box(current_box.left(),current_box.bottom()+current_box.get_height()/15+,current_box.left() + 
              //  current_box.get_width()/15 ,current_box.bottom);
               // t_bound_box test_navi_next =  t_bound_box(current_box.left()+current_box.get_width()* 2.5/6-current_box.get_width()/15,current_box.bottom()+current_box.get_height()/15+,current_box.left() + 
                //current_box.get_width()* 2.5/6 ,current_box.bottom);
                
           // on off mode for the click menu
           if (!click_menu){
                if ((x_click>= test_box.bottom_left().x)&&(x_click<=test_box.top_right().x) && (y_click >= test_box.top_right().y)&&(y_click <= test_box.bottom_left().y)){
                    click_menu = true; 
                    if_on_search_icon = true;
                }
                else{
                    if_on_search_icon = false;
                }
           }
            else {
                if ((x_click>= test_box.bottom_left().x)&&(x_click<=test_box.top_right().x) && (y_click >= test_box.top_right().y)&&(y_click <= test_box.bottom_left().y)){
                   click_menu = false; 
                   if_on_search_icon = true;
                }
                else{
                   if_on_search_icon = false;
                }
            }
            check_which_button_pressed();
      }    
      
    drawscreen();
}


unsigned close_inter (double lon, double lat){
    
    double ref = 10000000;
    double dist = 0;
    unsigned cl = 0;
    
    double mypoi_y = lat * DEG_TO_RAD;
    double averagelat = cos(mypoi_y);
    double mypoi_x =lon* DEG_TO_RAD * averagelat;
    
    double closest_x, closest_y;
    
    unsigned size = getNumberOfIntersections();
    
    /*things above preset values that are needed in the following comparison*/
    
    for(unsigned i = 0 ; i < size; i ++){
        
        closest_x = (data_structure.intersection[i]).lon() * averagelat * DEG_TO_RAD;
        closest_y = (data_structure.intersection[i]).lat() * DEG_TO_RAD;
        
        dist = pow(mypoi_y - closest_y, 2) + pow(closest_x - mypoi_x,2);
        
        if(ref > dist){
    
     mypoi_y = lat * DEG_TO_RAD;
     averagelat = cos(mypoi_y);
     mypoi_x =lon* DEG_TO_RAD * averagelat;
              cl = i;
              ref = dist;
        }
    }
    return cl;
}


void drawscreen(void) {

    /* The redrawing routine for still pictures.  The graphics package calls  
     * this routine to do redrawing after the user changes the window 
     * in any way.                                                    
     */
    if (!LOD_area_test(0.0001)&&(LOD_area_test(0.0002))){
        first_world= get_visible_world();
    }
   
    // Should set this if your program does any XOR drawing in callbacks.
    set_draw_mode(DRAW_NORMAL); 
    
    clearscreen(); /* Should precede drawing for all drawscreens */
    
    
    current_box = get_visible_world();
    
    //std::cout<< get_visible_world().area()<<"\n";

    mouse_move = false; // clear the flag 


    draw_feature();
    
    draw_way();

//    draw_street();
    draw_poi_and_name();
    
    draw_found_roads();
    
    if (LOD_area_test(4.0e-08)){
        draw_street_name();
    }

    draw_click();

    draw_osm();
 
    draw_click_poi_and_intersection();
    
    draw_search();
    

    
    if (LOD_area_test(0.00012)) {//draw thumbnail
        thumbnail();
    }
    
    copy_off_screen_buffer_to_screen();
  
}

void draw_click_poi_and_intersection(){
    if(if_draw_intersection){//draw found intersections when searching
        for(unsigned i = 0; i < data_structure.ids_of_intersections.size() ; i++){
            LatLon temp = getIntersectionPosition(data_structure.ids_of_intersections[i]);
            Surface icon = load_png_from_file("libstreetmap/resources/marker.png");
            draw_surface(icon, temp.lon() * cos(avg_lat) * DEG_TO_RAD , temp.lat() * DEG_TO_RAD );
        }
    }
    else{
        update_message("Intersection Not Found!");
    }
    
    
    if(if_draw_pois){//draw found poi when searching
        for(unsigned i = 0; i < data_structure.id_of_poi.size() ; i ++){
            LatLon temp = getPointOfInterestPosition(data_structure.id_of_poi[i]);
            Surface icon = load_png_from_file("libstreetmap/resources/marker.png");
            draw_surface(icon, temp.lon() * cos(avg_lat) * DEG_TO_RAD , temp.lat() * DEG_TO_RAD );
        }
    }
}

void draw_search(){
    if(if_search){//draw search bar, inout and output on screen
        
        search_bar = t_bound_box(current_box.left(),current_box.top(),current_box.left() + 
                current_box.get_width() * 2/5 ,current_box.top() - current_box.get_height()/15);
   
        result_bar = t_bound_box(current_box.right(),current_box.bottom(),current_box.right() - 
                current_box.get_width() * 2/5 ,current_box.bottom() + current_box.get_height()/15);
 
        t_bound_box auto_complete_bar_1;
        t_bound_box auto_complete_bar_2;
        
        setcolor(255,255,255,255);
        fillrect(search_bar);
        fillrect(result_bar);
    //    fillrect(menu);
  
        Surface icon = load_png_from_file("libstreetmap/resources/search.png");
        draw_surface(icon, current_box.left() , current_box.top() );
        
         input = "";
         for(unsigned i = 0; i< data_structure.input_string.size(); i++){
              input += data_structure.input_string[i];
        
          }   
        
       auto_poi_search(input);
 
       int ref = 0;
       limit_of_auto_search = 0;
       
       if(0 < auto_str_poi.size() && auto_str_poi.size() < 3){
            std::sort( auto_str_poi.begin(), auto_str_poi.end() );
            auto_str_poi.erase(std::unique(auto_str_poi.begin(), auto_str_poi.end() ), auto_str_poi.end());
               
          //  std::cout << "Are you looking for:" << std::endl;
            
           for(unsigned i = 0 ; i < auto_str_poi.size(); i++){
             //  std::cout << auto_str_poi[i] << std::endl;

               auto_complete_bar_1 = t_bound_box(current_box.left(),current_box.top() - current_box.get_height() * (i + 1)/15 ,current_box.left() + 
               current_box.get_width() * 2/5 ,current_box.top() - current_box.get_height() * (i + 2) /15);
               
               ref++;
               limit_of_auto_search++;
               
               setcolor(255,255,255,255);
               fillrect(auto_complete_bar_1);
               
               setcolor(0,0,0,255);
       
               settextrotation(0);
               drawtext(auto_complete_bar_1.get_xcenter(), auto_complete_bar_1.get_ycenter(), auto_str_poi[i] ,1); 
           }
            
       }
       else if(auto_str_poi.size() >= 3){
           
           //std::cout << "Are you looking for:" << std::endl;
           
           for(unsigned i = 0 ; i < 2; i++){
               
               //std::cout << auto_str_poi[i] << std::endl;
               
               auto_complete_bar_1 = t_bound_box(current_box.left(),current_box.top() - current_box.get_height() * (i + 1)/15 ,current_box.left() + 
               current_box.get_width() * 2/5 ,current_box.top() - current_box.get_height() * (i + 2) /15);
               
               ref++;
               limit_of_auto_search++;
               
               setcolor(255,255,255,255);
               fillrect(auto_complete_bar_1);
               
               setcolor(0,0,0,255);
       
                settextrotation(0);
                drawtext(auto_complete_bar_1.get_xcenter(), auto_complete_bar_1.get_ycenter(), auto_str_poi[i] ,1); 
           }
       }
   
       
       if(from.length() != 0){
           t_bound_box from_bar = t_bound_box(current_box.left() + current_box.get_width() * 3/5 ,
                   current_box.top(),current_box.right() ,current_box.top() - current_box.get_height()/15);
           
           setcolor(255,255,255,255);
           fillrect(from_bar);
               
           setcolor(0,0,0,255);
       
           settextrotation(0);
           drawtext(from_bar.get_xcenter(), from_bar.get_ycenter(), "From: " + from ,1); 
       }
       
       
       if(to.length() != 0){
            t_bound_box from_bar = t_bound_box(current_box.left() + current_box.get_width() * 3/5 ,
                   current_box.top()- current_box.get_height()/15,current_box.right() ,current_box.top() - current_box.get_height() * 2/15);
           
            
           setcolor(255,255,255,255);
           fillrect(from_bar);
           setcolor(0,0,0,255);
       
           settextrotation(0);
           drawtext(from_bar.get_xcenter(), from_bar.get_ycenter(), "To: " + to ,1); 
       }
       
       
       if(0 < auto_str_intersection.size() && auto_str_intersection.size() < 4){
            std::sort( auto_str_intersection.begin(), auto_str_intersection.end() );
            auto_str_intersection.erase(std::unique(auto_str_intersection.begin(), auto_str_intersection.end() ), auto_str_intersection.end());
               
            
           for(unsigned i = 0 ; i < auto_str_intersection.size(); i++){
               
               auto_complete_bar_2 = t_bound_box(current_box.left(),current_box.top() - current_box.get_height() * (i + ref + 1 )/15 ,current_box.left() + 
               current_box.get_width() * 2/5 ,current_box.top() - current_box.get_height() * (i + ref + 2) /15);
               
               limit_of_auto_search++;

               setcolor(255,255,255,255);
               fillrect(auto_complete_bar_2);
               
               setcolor(0,0,0,255);
       
               settextrotation(0);
               drawtext(auto_complete_bar_2.get_xcenter(), auto_complete_bar_2.get_ycenter(), auto_str_intersection[i] ,1); 
           }
            
            
       }
       else if(auto_str_intersection.size() >= 4){
        
           for(unsigned i = 0 ; i < 3; i++){
              
               if(auto_str_intersection[i] != ""){
               
               auto_complete_bar_2 = t_bound_box(current_box.left(),current_box.top() - current_box.get_height() * (i + ref + 1)/15 ,current_box.left() + 
               current_box.get_width() * 2/5 ,current_box.top() - current_box.get_height() * (i + ref + 2) /15);
               
               limit_of_auto_search++;

               setcolor(255,255,255,255);
               fillrect(auto_complete_bar_2);
               
               setcolor(0,0,0,255);
       
               settextrotation(0);
               drawtext(auto_complete_bar_2.get_xcenter(), auto_complete_bar_2.get_ycenter(), auto_str_intersection[i] ,1); 
               
               }
           }
       }  

       setcolor(0,0,0,255);
       
       settextrotation(0);
       drawtext(result_bar.get_xcenter(), result_bar.get_ycenter(), output ,1); 
       

       if(select_auto_search == -1 && input != ""){
           drawtext(search_bar.get_xcenter(), search_bar.get_ycenter(), input ,1); 
       }
       else if(input != ""){
           if(select_auto_search < 2){
               
                input = auto_str_poi[select_auto_search];
                drawtext(search_bar.get_xcenter(), search_bar.get_ycenter(),input ,1); 
           }
           else{
               input = auto_str_intersection[select_auto_search - ref];
               drawtext(search_bar.get_xcenter(), search_bar.get_ycenter(),input ,1); 
           }
       }
       
     //  drawtext(result_bar.get_xcenter(), result_bar.get_ycenter(), "menu" ,1); 
       
    }
}


void draw_click(){
    double x_con, y_con;
    
        if (click){
        // if  user click on the menu 
        if (click_menu){
        
            mission_prompt = t_bound_box(current_box.left(),current_box.top(),current_box.left() + 
                current_box.get_width() /6 ,current_box.bottom());
            gray_layer = t_bound_box(current_box.left() +current_box.get_width() /6,current_box.top(),current_box.right(),current_box.bottom());
            
            setcolor(32,74,135,255);
            fillrect(mission_prompt);
            //setcolor(136,138,133,100);
           // fillrect(gray_layer);
            
            journey_planner = t_bound_box(current_box.left(),current_box.top() - current_box.get_height()*3/20,current_box.left() + 
                current_box.get_width() /6 ,current_box.top() - current_box.get_height()*4/20);
            information = t_bound_box(current_box.left(),current_box.top() - current_box.get_height()*2/20,current_box.left() + 
                current_box.get_width() /6 ,current_box.top() - current_box.get_height()*3/20);
            submission= t_bound_box(current_box.left() + current_box.get_width() /6,current_box.top(),current_box.left() + 
                current_box.get_width()* 2.5/6 ,current_box.bottom());
            planning_area = t_bound_box(current_box.left() + current_box.get_width() /6,current_box.top(),current_box.left() + 
                current_box.get_width()* 2.5/6,current_box.top()-current_box.get_height()/4);
            
           // journey_planner_str = t_bound_box( journey_planner.left()+ journey_planner.get_width()/5*3, journey_planner.top(), journey_planner.right(),
                    //journey_planner.bottom());
            
//            setcolor(85,87,83,100);
//            fillrect(journey_planner);
            
            // draw information button 
            draw_surface(information_icon, current_box.left()+journey_planner.get_width()/12*1.5,current_box.top()-current_box.get_height()*2/20-current_box.get_height()/20/12);
            setcolor(0,0,0,255);
            settextrotation(0);
            drawtext(current_box.left()+ current_box.get_width()/6/12*7 ,current_box.top()-current_box.get_height()/20*(2.5), "Information",1);
            
            //draw Journey Planner button
            draw_surface(journey_icon, current_box.left()+journey_planner.get_width()/12 , current_box.top()-current_box.get_height()*3/20-current_box.get_height()/20/12);
            setcolor(0,0,0,255);
            settextrotation(0);
            std::string journey_string= "Journey Planner";
            drawtext(current_box.left()+ current_box.get_width()/6/12*7 ,current_box.top()-current_box.get_height()/20*(3.5), journey_string,1);
            
            // special feature
            if (planner_layer){
                planner_layer = false;
                setcolor(85,87,83,100);
                fillrect(journey_planner);
                
            }
            if (information_layer){
                information_layer = false;
                setcolor(85,87,83,100);
                fillrect(information);
            }
            
            // click next prompt             
            if (planner_click){
                
                 setcolor(255,255,255,255);
                 fillrect(submission);
                 setcolor (3,169,245,255);
                 fillrect(planning_area);
                 // print current time
                 setcolor(0,0,0,255);
                 settextrotation(0);
//                // now =  get_current_time();
//                 std:: string message = "Start: "+ now;
//                 drawtext(current_box.left() + current_box.get_width()*1.75/6 ,current_box.top()-current_box.get_height()*7/24,message,1);
//                 drawtext(current_box.left() + current_box.get_width()*1.75/6 ,current_box.top()-current_box.get_height()*8/24,message,1);
                 if (if_navi){
                     //double num_pages = navi.size()/6;
//                     if (num_pages < = 1){   
//                     }
                 }    
            }
            if (information_click){
                 setcolor(255,255,255,255);
                 fillrect(submission);
                 draw_surface(introduction_icon, current_box.left() + current_box.get_width() /6,current_box.top());
            }
        }        
        // if user not click on the menu 
       if(!click_menu && !if_on_search_icon){
        
       //draw intersection when left clicking 
       x_con = x_click/cos(avg_lat)/DEG_TO_RAD;
       y_con = y_click/DEG_TO_RAD;
       unsigned position = close_inter (x_con, y_con);
       double close_inter_x = data_structure.intersection[position].lon()* cos(avg_lat) * DEG_TO_RAD;
       double close_inter_y = data_structure.intersection[position].lat() * DEG_TO_RAD;
       
       double distance_x = abs(close_inter_x - x_click);
       double distance_y = abs(close_inter_y - y_click); 
       
       if ((distance_x <= 0.000005)&(distance_y <= 0.000005)) click_intersection = true;
//       // if the user click on the intersections 
       if (click_intersection){
           
           if(if_click_searching){
           
            if(click_searching_1 == false && click_searching_2 == false ){
                click_searching_1 = true;
                click_1 = position;
                draw_surface(pin ,close_inter_x, close_inter_y);
                
            }
            else if(click_searching_2 == false){
                click_searching_2 = true;
                click_2 = position;
              
                double close_inter_x_2 = data_structure.intersection[click_1].lon()* cos(avg_lat) * DEG_TO_RAD;
                double close_inter_y_2 = data_structure.intersection[click_1].lat() * DEG_TO_RAD;
                
                double close_inter_x_3 = data_structure.intersection[click_2].lon()* cos(avg_lat) * DEG_TO_RAD;
                double close_inter_y_3 = data_structure.intersection[click_2].lat() * DEG_TO_RAD;
                
                draw_surface(pin ,close_inter_x_2, close_inter_y_2);
                draw_surface(pin ,close_inter_x_3, close_inter_y_3);
                
                
                click_search_calc();
            }
            else{
                click_searching_1 = false;
                click_searching_2 = false;
                
                double close_inter_x_2 = data_structure.intersection[click_1].lon()* cos(avg_lat) * DEG_TO_RAD;
                double close_inter_y_2 = data_structure.intersection[click_1].lat() * DEG_TO_RAD;
                
                double close_inter_x_3 = data_structure.intersection[click_2].lon()* cos(avg_lat) * DEG_TO_RAD;
                double close_inter_y_3 = data_structure.intersection[click_2].lat() * DEG_TO_RAD;
                
                draw_surface(pin ,close_inter_x_2, close_inter_y_2);
                draw_surface(pin ,close_inter_x_3, close_inter_y_3);
//                double close_inter_x_2 = data_structure.intersection[click_1].lon()* cos(avg_lat) * DEG_TO_RAD;
//                double close_inter_y_2 = data_structure.intersection[click_1].lat() * DEG_TO_RAD;
//                
//                draw_surface(pin ,close_inter_x_2, close_inter_y_2);
            //    draw_surface(pin ,close_inter_x, close_inter_y);
                
            }
            
            }
           else{
            draw_surface(pin ,close_inter_x, close_inter_y);
            click_intersection = false;
            output = "Intersection name: " + getIntersectionName(position);
           }
       
       }
       else{
           // if user click on searching bar 
           if(if_click_searching){
                double close_inter_x_2 = data_structure.intersection[click_1].lon()* cos(avg_lat) * DEG_TO_RAD;
                double close_inter_y_2 = data_structure.intersection[click_1].lat() * DEG_TO_RAD;
                
                double close_inter_x_3 = data_structure.intersection[click_2].lon()* cos(avg_lat) * DEG_TO_RAD;
                double close_inter_y_3 = data_structure.intersection[click_2].lat() * DEG_TO_RAD;
                
                draw_surface(pin ,close_inter_x_2, close_inter_y_2);
                draw_surface(pin ,close_inter_x_3, close_inter_y_3);
           }
           else{
           draw_surface(arrow,x_click, y_click);
           }
       }

       }
    }
}

void draw_found_roads(){
    for(unsigned i = 0; i < found_roads.size() ; i++){
        LatLon point1, point2; 
        point1 = getIntersectionPosition(getStreetSegmentInfo(found_roads[i]).from);
        point2 = getIntersectionPosition(getStreetSegmentInfo(found_roads[i]).to);

        double x1 = point1.lon() * cos(avg_lat) * DEG_TO_RAD;
        double x2 = point2.lon() * cos(avg_lat) * DEG_TO_RAD;
        double y1 = point1.lat() * DEG_TO_RAD;
        double y2 = point2.lat() * DEG_TO_RAD;

        double temp_x1, temp_x2, temp_y1, temp_y2;

        setcolor(255,0,0,255);

        if(getStreetSegmentInfo(found_roads[i]).curvePointCount == 0){
            drawline(x1, y1, x2, y2);
        }
        else{
            LatLon temp_point1 = getStreetSegmentCurvePoint(found_roads[i], 0); // first curve point
            LatLon temp_point2 = getStreetSegmentCurvePoint(found_roads[i], getStreetSegmentInfo(found_roads[i]).curvePointCount-1); // last curve point
            // first draw first and last curve 
            //double avg_lat2 = (temp_point1.lat()+temp_point2.lat())/2; 
            temp_x1 = temp_point1.lon() * cos(avg_lat) * DEG_TO_RAD;
            temp_x2 = temp_point2.lon() * cos(avg_lat) * DEG_TO_RAD;

            temp_y1 = temp_point1.lat() * DEG_TO_RAD;
            temp_y2 = temp_point2.lat() * DEG_TO_RAD;

            drawline(x1, y1, temp_x1, temp_y1);
            drawline(temp_x2, temp_y2, x2, y2);

            // then loop street segment, find curve length between two curve points
            for(unsigned a=0; a<getStreetSegmentInfo(found_roads[i]).curvePointCount-1; a++){
            temp_point1 = getStreetSegmentCurvePoint(found_roads[i], a); // first curve point
            temp_point2 = getStreetSegmentCurvePoint(found_roads[i], a+1); // second curve point



            temp_x1 = temp_point1.lon() * cos(avg_lat) * DEG_TO_RAD; 
            temp_x2 = temp_point2.lon() * cos(avg_lat) * DEG_TO_RAD;

            temp_y1 = temp_point1.lat() * DEG_TO_RAD;
            temp_y2 = temp_point2.lat() * DEG_TO_RAD;

            drawline(temp_x1, temp_y1, temp_x2, temp_y2);
            }

        }

    }
}

void draw_street_name(){
    unsigned numofStreetSeg = getNumberOfStreetSegments();
    double curve_x2, curve_y2;
    LatLon point1, point2;
    double x1, y1, x2, y2, xc, yc;
    

     for (unsigned k =0; k< numofStreetSeg; k++){
        point1 = getIntersectionPosition(getStreetSegmentInfo(k).from);
        point2 = getIntersectionPosition(getStreetSegmentInfo(k).to);
        // x = lon * cos(lat_avg)
        x1 = point1.lon() * cos(avg_lat) * DEG_TO_RAD;
        x2 = point2.lon() * cos(avg_lat) * DEG_TO_RAD;
        // y = lat
        y1 = point1.lat() * DEG_TO_RAD;
        y2 = point2.lat() * DEG_TO_RAD;
        xc = 0.5*(x2+x1);
        yc = 0.5*(y2+y1);
        //unsigned streetID =getStreetSegmentInfo(k).streetID;
        unsigned numCurve = getStreetSegmentInfo(k).curvePointCount; 
        

        if (!(getStreetName(getStreetSegmentInfo(k).streetID) == "<unknown>")){
        if (!(numCurve == 0)){
            for ( unsigned curvept = 0; curvept< numCurve; curvept++){
                curve_x2 = getStreetSegmentCurvePoint(k,0).lon() * cos(avg_lat) * DEG_TO_RAD;
                curve_y2 = getStreetSegmentCurvePoint(k,0).lat() *  DEG_TO_RAD;
                setcolor(BLACK); 
                settextrotation(atan((curve_y2-y1)/(curve_x2-x1))*180/PI);
                drawtext(xc, yc, getStreetName(getStreetSegmentInfo(k).streetID), abs(x1-curve_x2), abs(y1-curve_y2)); 
            }
         
        }
        else {
            setcolor(BLACK); 
            settextrotation(atan((y2-y1)/(x2-x1))*180/PI);
                drawtext(xc, yc, getStreetName(getStreetSegmentInfo(k).streetID), abs(x1-x2), abs(y1-y2)); 
                settextrotation(atan((y2-y1)/(x2-x1))*180/PI);
                drawtext(xc, yc, getStreetName(getStreetSegmentInfo(k).streetID), abs(x1-x2), abs(y1-y2));
            }

        }
        if (LOD_area_test(1.6e-08)){
            if (getStreetSegmentInfo(k).oneWay){
                if ((x2-x1)>=0){
                 setcolor(BLACK);
                 settextrotation(atan((y2-y1)/(x2-x1))*180/PI);
                 drawtext(xc, yc+2.0e-7, "->", 1); 
                }
                else {
                    setcolor(BLACK);
                    settextrotation(atan((y2-y1)/(x2-x1))*180/PI);
                    drawtext(xc, yc, "<-", 1); 
                }

            }
        }
    }
         
     
}

// draw features such as buildings and lakes
void draw_feature(){
    unsigned num_of_features = getNumberOfFeatures();

    
    if (thumbnail_draw){
        t_bound_box word_box = get_visible_world();
        Surface marker = load_png_from_file("libstreetmap/resources/map-perspective-with-a-placeholder-on-it.png");
        
        
        for(unsigned i = 0; i < num_of_features; i++){

        FeatureType temp_feature = getFeatureType(i);
        unsigned num_of_points = getFeaturePointCount(i);
        
        t_point list_of_points[num_of_points];

        for(unsigned a = 0; a < num_of_points; a++){
            // the ratio between feature point to the first_world t_bound 
           list_of_points[a].x = abs(data_structure.dims_features[i][a].x-first_world.left())
                    / first_world.get_width() 
                    * word_box.get_width()/4+(word_box.left()+word_box.get_width()*3/4);
            list_of_points[a].y = word_box.bottom() + 
                    abs(data_structure.dims_features[i][a].y-first_world.bottom())
                    / first_world.get_height()*word_box.get_height()/4;
            
//             list_of_points[a].x = list_of_points[a].x +width_fix;
//             list_of_points[a].y = list_of_points[a].y +height_fix;
        }

        if(temp_feature == 1){//Park

            setcolor( 199,  238,  185, 255);
            fillpoly(list_of_points, num_of_points);
        }
        else if(temp_feature == 2){//Beach
            setcolor( 240,  240,  150, 255);
            fillpoly(list_of_points, num_of_points);
        }
        else if(temp_feature == 3){//Lake
            setcolor( 170,  218,  255, 255);
            fillpoly(list_of_points, num_of_points);
        }
        else if(temp_feature == 4){//River

            setcolor( 170,  218,  255, 255);
            fillpoly(list_of_points, num_of_points);
        }
        else if(temp_feature == 5){//Island
            setcolor(232,232,232,255);
            fillpoly(list_of_points, num_of_points);
        }
//        else if(temp_feature == 6){//Shoreline, deprecated, no longer used & should be removed
//
//        }
        else if(temp_feature == 0){//Unknown
            setcolor(LIGHTGREY);
            fillpoly(list_of_points, num_of_points);
        }
        else if(temp_feature == 8){//Green space
                setcolor( 212,  242,  202, 255);
                fillpoly(list_of_points, num_of_points);
            }
        }
        float x_marker = abs(word_box.get_center().x - first_world.left())
        / first_world.get_width()*word_box.get_width()/4+(word_box.left()+word_box.get_width()*3/4);
        
        
        float y_marker = abs(word_box.get_center().y-first_world.bottom())
        / first_world.get_height()*word_box.get_height()/4+word_box.bottom();
        
        
        draw_surface(marker, x_marker, y_marker);
    }
    /**************************************************************************/
    else {
    for(unsigned i = 0; i < num_of_features; i++){

        FeatureType temp_feature = getFeatureType(i);
        unsigned num_of_points = getFeaturePointCount(i);
        t_point list_of_points[num_of_points];

        for(unsigned a = 0; a < num_of_points; a++){

            list_of_points[a].x = getFeaturePoint(i, a).lon() * cos(avg_lat) * DEG_TO_RAD;
            list_of_points[a].y = getFeaturePoint(i, a).lat() * DEG_TO_RAD;
        }

        if(temp_feature == 1){//Park

            setcolor( 199,  238,  185, 255);
            fillpoly(list_of_points, num_of_points);
        }
        else if(temp_feature == 2){//Beach
            setcolor( 240,  240,  150, 255);
            fillpoly(list_of_points, num_of_points);
        }
        else if(temp_feature == 3){//Lake
            setcolor( 170,  218,  255, 255);
            fillpoly(list_of_points, num_of_points);
        }
        else if(temp_feature == 4){//River

            setcolor( 170,  218,  255, 255);
            fillpoly(list_of_points, num_of_points);
        }
        else if(temp_feature == 5){//Island
            setcolor(232,232,232,255);
            fillpoly(list_of_points, num_of_points);
        }
        else if(temp_feature == 6){//Shoreline, deprecated, no longer used & should be removed

        }
//        
        else if(temp_feature == 0){//Unknown
            setcolor(LIGHTGREY);
            fillpoly(list_of_points, num_of_points);
        }

    
    if (LOD_area_test(7.0e-07)){ // if the level of detail accurate to 7th zoom in 
       if(temp_feature == 7){//Building
                setcolor(251,  245,  226, 255);
                fillpoly(list_of_points, num_of_points);
            }

    }
    
    if (LOD_area_test(6.0e-06)){ // use for the level of detail 
         if(temp_feature == 10){//Stream
                setlinewidth(0.3);
                setcolor( 170,  218,  255, 255);
                for(unsigned ref = 0; ref < num_of_points - 1; ref++){
                    drawline(list_of_points[ref], list_of_points[ref+1]);
                }           
            }
         if(temp_feature == 9){//Golfcourse
                setcolor( 212,  242,  202, 255);
                fillpoly(list_of_points, num_of_points);
                
            }
         if(temp_feature == 8){//Greenspace
                setcolor( 212,  242,  202, 255);
                fillpoly(list_of_points, num_of_points);
            }

    }

        }
    }
}

void draw_poi_and_name(){
    if (!thumbnail_draw){
    
    // draw the point of interests 
    if (LOD_area_test(4.0e-08)){
        float x, y;
        Surface icon = load_png_from_file("libstreetmap/resources/small_image.png");
        unsigned numPOI = getNumberOfPointsOfInterest();
        for (unsigned i = 0; i< numPOI; i ++){ // loop all of the point of interests

           // get the position of the point of interests  
           x =  getPointOfInterestPosition(i).lon()*cos(avg_lat) * DEG_TO_RAD; 
           y =  getPointOfInterestPosition(i).lat()* DEG_TO_RAD;

           if (data_structure.POItype[i] == "pharmacy" ){ // for the different type of icon we draw the different icons
               // for performance, we only use one icons 
               draw_surface(load_png_from_file("libstreetmap/resources/pharmacy.png"),x,y);
           }

           else  draw_surface(icon, x, y);

        }
    }
    
    //draw the name for the point of interests 
     if (LOD_area_test(1.6e-09)){
        float x, y;
        for (unsigned i = 0; i< getNumberOfPointsOfInterest(); i ++){
           x =  getPointOfInterestPosition(i).lon()*cos(avg_lat) * DEG_TO_RAD; 
           y =  getPointOfInterestPosition(i).lat()* DEG_TO_RAD;
           setcolor( 0,  0,  0, 255); 
           settextrotation(0);
           drawtext(x, y+5.0e-8, getPointOfInterestName(i), 0.5); 

       }
     }
    }
}

// draw osm features with osm database
// it includes draw_subway, draw_highway, draw_tourism_point
void draw_osm(){
  
    
    if(if_draw_subway){
        draw_relation();
    }
    
//    if(if_draw_highway){
//        draw_way();
//    }
    
    if(if_draw_attraction){
        if (LOD_area_test(6.0e-06)){
        draw_node();
        }
    }

    // at the 5th zoom in level, it shows tourism points
    
}


// traverse OSMRelation, draw subway
void draw_relation(){
    std::string key,value;
    
    color_types color_indicies[] = {
        SADDLEBROWN, // The browest brown in X11
        FIREBRICK, // dark red
        LIMEGREEN, // a pleasing slightly dark green
        LIGHTMEDIUMBLUE, // A light blue, with nice contrast to white, but distinct from "BLUE" (NON-X11)
        LIGHTSKYBLUE, // A nice light blue
        //BLACK, DARKGREY, LIGHTGREY,
        RED, GREEN, CYAN, BLUE, PURPLE, // standard ranbow colours.
        PINK, LIGHTPINK, DARKGREEN, MAGENTA, // some other colours
        BISQUE, // A weird colour, not unlike the "peach" colour of pencil crayons, but closer to "Blanched Almond" and "Moccasin"
        
        THISTLE, // A sort of desaturated purple, the colour of thistle flowers
        PLUM, // much closer to pink than the colour of actual plums, and closer to its flower's colour
        KHAKI, // Wikipedia says "a light shade of yellow-green" , but this one is more yellow, and very light
        CORAL, // A burnt pinkish-orange perhaps?
        TURQUOISE, // turquoise
        MEDIUMPURPLE, // A nice medium purple
        DARKSLATEBLUE, // A deep blue, almost purple
        DARKKHAKI, // darker khaki        
        NUM_COLOR
    };
    
    int color_index = 0;
    
    // traverse relation, find relation of subway
    // there are many subway routes
    for(unsigned index=0; index<getNumberOfRelations(); ++index){
        
        const OSMRelation* r = getRelationByIndex(index);
        // traverse all tags, make sure this is a subway
        for(unsigned i=0;i<getTagCount(r); ++i){
            
            std::tie(key,value) = getTagPair(r,i);

            
            if(key == "route" && value == "subway"){

                const std::vector<OSMRelation::Member> mem = r->members();
                // traverse Member vector, get way information
                for(std::vector<OSMRelation::Member>::const_iterator it = mem.begin(); 
                        it != mem.end(); ++it){
                    
                    // make sure type is 'way', temp = Way
                    TypedOSMID::EntityType temp = TypedOSMID::EntityType::Way;
                    if(it->tid.type() == temp){
                        // find the corresponding way with OSMID
                        const OSMWay* w = data_structure.ways[it->tid];
                        
                        // draw subway lines and stations
                        draw_osm_subway_and_station(w, color_indicies[color_index]);
                        
                    }
                }
                if(color_index == 22) 
                    color_index = 0; // start using the first color
                else 
                    color_index++; // change color for another subway route
                break; // stop, because tag 'subway' is found
            }
        }
    }
}


// traverse OSMWay, draw highway
void draw_way(){
    std::string key,value;
    int R, G, B;
    int width;
    
    // traverse all OSMWay
    for(unsigned index=0; index<getNumberOfWays(); ++index){
        
        const OSMWay* w = getWayByIndex(index);
        
        // find tags that (key=highway, value=motorway/trunk)
        for(unsigned i=0;i<getTagCount(w); ++i){
            
            std::tie(key,value) = getTagPair(w,i);
            R = 255;
            G = 241;
            B = 152;
            width = 9;
            if(key == "highway" && value == "motorway"){
                // draw highway with OSMWay
                draw_osm_highway(w, R, G, B, width);
                break; // stop, because tag 'highway' is found
            }
            
            if(key == "highway" && value == "trunk"){
                // draw highway with OSMWay
                draw_osm_highway(w, R, G, B, width);
                break; // stop, because tag 'highway' is found
            }
            
            if(key == "highway" && value == "primary"){
                // draw highway with OSMWay
                draw_osm_highway(w, R, G, B, width);
                break; // stop, because tag 'highway' is found
            }
            
            if(key == "highway" && value == "motorway_link"){
                // draw highway with OSMWay
                draw_osm_highway(w, R, G, B, width);
                break; // stop, because tag 'highway' is found
            }
            
            if(key == "highway" && value == "trunk_link"){
                // draw highway with OSMWay
                draw_osm_highway(w, R, G, B, width);
                break; // stop, because tag 'highway' is found
            }
            
            if(key == "highway" && value == "primary_link"){
                // draw highway with OSMWay
                draw_osm_highway(w, R, G, B, width);
                break; // stop, because tag 'highway' is found
            }
            R = 255;
            G = 255;
            B = 243;
            width = 7;
            if (LOD_area_test(6e-05)){
                if(key == "highway" && value == "secondary"){
                    // draw highway with OSMWay
                    draw_osm_highway(w, R, G, B, width);
                    break; // stop, because tag 'highway' is found
                }
                if(key == "highway" && value == "secondary_link"){
                    // draw highway with OSMWay
                    draw_osm_highway(w, R, G, B, width);
                    break; // stop, because tag 'highway' is found
                }
            }
            
            R = 255;
            G = 250;
            B = 250;
            
            width = 6;
            if (LOD_area_test(1.6e-05)){
                if(key == "highway" && value == "tertiary"){
                    // draw highway with OSMWay
                    draw_osm_highway(w, R, G, B, width);
                    break; // stop, because tag 'highway' is found
                }
                if(key == "highway" && value == "tertiary_link"){
                    // draw highway with OSMWay
                    draw_osm_highway(w, R, G, B, width);
                    break; // stop, because tag 'highway' is found
                }
            }
            width = 5;
            if (LOD_area_test(1.6e-06)){
                if(key == "highway" && value == "residential"){
                    // draw highway with OSMWay
                    draw_osm_highway(w, R, G, B, width);
                    break; // stop, because tag 'highway' is found
                }
            }
            
            if (LOD_area_test(4e-07)){
                if(key == "highway" && value == "unclassified"){
                    // draw highway with OSMWay
                    draw_osm_highway(w, R, G, B, width);
                    break; // stop, because tag 'highway' is found
                }
                if(key == "highway" && value == "service"){
                    // draw highway with OSMWay
                    draw_osm_highway(w, R, G, B, width);
                    break; // stop, because tag 'highway' is found
                }
            }
            
            if (LOD_area_test(6e-08)){
                if(key == "highway" && value == "living_street"){
                    // draw highway with OSMWay
                    draw_osm_highway(w, R, G, B, width);
                    break; // stop, because tag 'highway' is found
                }
                if(key == "highway" && value == "pedestrian"){
                    // draw highway with OSMWay
                    draw_osm_highway(w, R, G, B, width);
                    break; // stop, because tag 'highway' is found
                }
                if(key == "highway" && value == "road"){
                    // draw highway with OSMWay
                    draw_osm_highway(w, R, G, B, width);
                    break; // stop, because tag 'highway' is found
                }
                if(key == "highway" && value == "track"){
                    // draw highway with OSMWay
                    draw_osm_highway(w, R, G, B, width);
                    break; // stop, because tag 'highway' is found
                }
                if(key == "highway" && value == "bus_guideway"){
                    // draw highway with OSMWay
                    draw_osm_highway(w, R, G, B, width);
                    break; // stop, because tag 'highway' is found
                }
                if(key == "highway" && value == "escape"){
                    // draw highway with OSMWay
                    draw_osm_highway(w, R, G, B, width);
                    break; // stop, because tag 'highway' is found
                }
                if(key == "highway" && value == "raceway"){
                    // draw highway with OSMWay
                    draw_osm_highway(w, R, G, B, width);
                    break; // stop, because tag 'highway' is found
                }
            
                if(key == "highway" && value == "bridleway"){
                    // draw highway with OSMWay
                    draw_osm_highway(w, R, G, B, width);
                    break; // stop, because tag 'highway' is found
                }
                if(key == "highway" && value == "steps"){
                    // draw highway with OSMWay
                    draw_osm_highway(w, R, G, B, width);
                    break; // stop, because tag 'highway' is found
                }
                if(key == "highway" && value == "path"){
                    // draw highway with OSMWay
                    draw_osm_highway(w, R, G, B, width);
                    break; // stop, because tag 'highway' is found
                }
            
            }
            width = 4;
            if (LOD_area_test(1.6e-09)){
                if(key == "highway" && value == "footway"){
                    // draw highway with OSMWay
                    draw_osm_highway(w, R, G, B, width);
                    break; // stop, because tag 'highway' is found
                }
            
            }
            
        }
    }
}


// traverse OSMNode, draw tourism points
void draw_node(){
    std::string key,value;
    std::string name;
    
    // traverse all OSMNode
    for(unsigned index=0; index<getNumberOfNodes(); ++index){

        const OSMNode* n = getNodeByIndex(index);
        LatLon point = n->coords();
        // find tags that (key=tourism, value=attraction)
        for(unsigned i=0;i<getTagCount(n); ++i){

            std::tie(key,value) = getTagPair(n,i);
            if(key == "tourism" && value == "attraction"){
                
                for(unsigned j=0;j<getTagCount(n); ++j){

                    std::tie(key,value) = getTagPair(n,j);
                    
                    if(key == "name"){
                        name = value;
//                        std::cout << i << ", (" << key << " , " << name << ")" << std::endl;
                    }
                }
                // draw tourism point with OSMNode
                draw_tourism_point(point, name);
                break; // stop, because tag 'tourism' is found
            }

        }
    }
}

// draw tourism point, icon is within function
void draw_tourism_point(LatLon& point, std::string name){
    Surface icon = load_png_from_file("libstreetmap/resources/camera.png");

    double x = point.lon() * cos(avg_lat) * DEG_TO_RAD;
    double y = point.lat() * DEG_TO_RAD;

    draw_surface(icon, x, y);
    if (LOD_area_test(4.0e-08)){
        setcolor(BLACK);
        settextrotation(0);
        drawtext(x, y+5.0e-8, name, 0.5); 
    }
}


// draw highway with particular color
// road is drawn as orange color outside and yellow color inside
void draw_osm_highway(const OSMWay* w, int R, int G, int B, int width){

    const std::vector<OSMID> node_ref = w->ndrefs();
    LatLon point1, point2;
    
    // traverse node reference
    for(std::vector<OSMID>::const_iterator it = node_ref.begin(); 
            it != node_ref.end(); ++it){
        
        point1 = data_structure.nodes[*it]->coords();
        if((it+1) != node_ref.end()){
            point2 = data_structure.nodes[*(it+1)]->coords();
            
            draw_highway_line(point1, point2, R, G, B, width);
        }
    }
    if(w->isClosed()){ // closed polygon
        // need to draw the line between the first and the last node
        point1 = data_structure.nodes[*(node_ref.begin())]->coords();
        point2 = data_structure.nodes[*(node_ref.end()-1)]->coords();

        draw_highway_line(point1, point2, R, G, B, width);
    }
}


// draw highway lines with two points specified
void draw_highway_line(LatLon& point1, LatLon& point2, int R, int G, int B, int width){
    // x = lon * cos(lat_avg)
    double x1 = point1.lon() * cos(avg_lat) * DEG_TO_RAD;
    double x2 = point2.lon() * cos(avg_lat) * DEG_TO_RAD;
    // y = lat
    double y1 = point1.lat() * DEG_TO_RAD;
    double y2 = point2.lat() * DEG_TO_RAD;
    
    setlinestyle(SOLID);
//    setlinewidth(9); // highway edge
//    setcolor(255, 159, 0, 255); // highway edge color
//    drawline(x1, y1, x2, y2);
    
    setlinewidth(width); // highway road
//    setcolor(WHITE);
    setcolor(R, G, B, 255);
//    setcolor(255, 235, 118, 255); // highway road color
    drawline(x1, y1, x2, y2);
}


// draw subway routes with different colors, and mark all stations
void draw_osm_subway_and_station(const OSMWay* w, color_types color){
    std::string key,value;
    std::string name;
    const std::vector<OSMID> node_ref = w->ndrefs();
    LatLon point1, point2;
    

    // traverse node reference to get every node
    for(std::vector<OSMID>::const_iterator it = node_ref.begin(); 
            it != node_ref.end(); ++it){
        
        const OSMNode* n = data_structure.nodes[*it];
        
        point1 = data_structure.nodes[*it]->coords();
        if((it+1) != node_ref.end()){
            point2 = data_structure.nodes[*(it+1)]->coords();

            draw_subway_line(point1, point2, color);
        }
        
        // at the 7th zoom in level, it shows station names
        if (LOD_area_test(7.0e-07)){
            for(unsigned i=0;i<getTagCount(n); ++i)
            {

                std::tie(key,value) = getTagPair(n,i);

                if(key == "name"){
                    name = value;
                }
                else if(key == "name:en"){ // if this country is not English-speaking country
                    name = value;
                }

                if(key == "railway" && value == "station"){
                    
                    draw_subway_station(point1, name);

                }
            }
        }

    }
    if(w->isClosed()){ // closed polygon
        // need to draw the line between the first and the last node
        point1 = data_structure.nodes[*(node_ref.begin())]->coords();
        point2 = data_structure.nodes[*(node_ref.end()-1)]->coords();

        draw_subway_line(point1, point2, color);
    }
}

// draw subway lines with color and two points specified
void draw_subway_line(LatLon& point1, LatLon& point2, color_types color){
    // x = lon * cos(lat_avg)
    double x1 = point1.lon() * cos(avg_lat) * DEG_TO_RAD;
    double x2 = point2.lon() * cos(avg_lat) * DEG_TO_RAD;
    // y = lat
    double y1 = point1.lat() * DEG_TO_RAD;
    double y2 = point2.lat() * DEG_TO_RAD;
    
    setlinestyle(SOLID);
    setlinewidth(8); // subway
    setcolor(color); // subway
    drawline(x1, y1, x2, y2);
}


// draw subway station with station name, icon is within function
void draw_subway_station(LatLon& point, std::string name){
    Surface icon = load_png_from_file("libstreetmap/resources/subway_icon.png");
    
    double x = point.lon() * cos(avg_lat) * DEG_TO_RAD;
    double y = point.lat() * DEG_TO_RAD;
    
    draw_surface(icon, x, y);
    
    setcolor(BLACK);
    settextrotation(0);
    drawtext(x, y+5.0e-8, name, 0.5); 
    
}


void subway_button(void (*drawscreen_ptr) (void)){//turn on and off subway routes
    if(if_draw_subway){
        if_draw_subway = false;
    }
    else{
        if_draw_subway = true;
    }
    
    drawscreen_ptr();
}


void attraction_button(void (*drawscreen_ptr) (void)){//trun on and off attractions
    if(if_draw_attraction){
        if_draw_attraction = false;
    }
    else{
        if_draw_attraction = true;
    }
    
    drawscreen_ptr();
}


// if searchbar_button is pressed, then set if_search to false
void searchbar_button(void (*drawscreen_ptr) (void)){
    if_road_found = false;
     if(if_search){//turn of search bar, clear data
        if_search = false;
        set_keypress_input(if_search);
        data_structure.input_string.clear();
        input = "";
        output = "";
        select_auto_search = -1;
    }
     else{//turn on search bar, clear data
        if_search = true;
        set_keypress_input(if_search);
        data_structure.input_string.clear();
        input = "";
        output = "";
        select_auto_search = -1;

    }
    
    drawscreen_ptr();
}

void act_on_key_press(char c, int keysym){
    

    switch (keysym) {
        case XK_BackSpace:
            if(data_structure.input_string.size() != 0){
                //if input not empty, remove the last character
                data_structure.input_string.pop_back();
            }
            select_auto_search = -1;
            break;
        case XK_Delete://clear every thing including input, outtput, and markers
            input = "";
            output = "";
            if_road_found = false;
            data_structure.input_string.clear();
            data_structure.ids_of_intersections.clear();
            data_structure.id_of_poi.clear();
            select_auto_search = -1;
            found_roads.clear();
            break;
        case XK_Return:
            input_calculation();
            break;
        case XK_Caps_Lock:
            break;
        case XK_Shift_L:
            break;
        case XK_Shift_R:
            break;
        case XK_Left: /* Move left, left arrow */
            break;
        case XK_Up:/* Move up, up arrow */
            if(select_auto_search > -1 && select_auto_search < limit_of_auto_search && input != ""){
                select_auto_search-=1;
            }
            break;
        case XK_Right:/* Move right, right arrow */
            break;
        case XK_Down:/* Move down, down arrow */
            if(select_auto_search >= -1 && select_auto_search < limit_of_auto_search - 1&& input != ""){
                select_auto_search+=1;
            }
            break;
        default://if no other command, read character from input
            data_structure.input_string.push_back(c);
            break;
    }
    
    drawscreen();
}

void input_calculation(){
    
    std::string first_street;
    std::string second_street;
    output = "";//clear the output statement
    int if_char_exist = input.find('&');//check oif searching for intersections
    
    if_draw_pois = false;//clear flag
    data_structure.id_of_poi.clear();//clear vector of poi
    
    if(if_char_exist != -1){//calculation for
        
        
        /*separate two street names*/
        first_street = input.substr(0, if_char_exist - 1);
        second_street = input.substr(if_char_exist + 2, input.length() - if_char_exist - 2);
         
        //search for intersections
        data_structure.ids_of_intersections = 
                find_intersection_ids_from_street_names(first_street, second_street);

        if(data_structure.ids_of_intersections.size() != 0){//nothing found
             if_draw_intersection = true;
             output = "Intersection Found!";
         }
         else{//intersections found
             output = "Intersection Not Found!";
        }
        
    }
    else{//search for poi
        
        //search for id of poi from name
        data_structure.id_of_poi = data_structure.nameToPOI[input];
        if(data_structure.id_of_poi.size() != 0){//nothing found
            if_draw_pois = true;
            output = "Points Of Interest Found!";
        }
        else{//poi found
            
            output = "No Point Of Interest Found!" ;
        }

    }

    drawscreen();
}

void thumbnail(){
   thumbnail_draw = true; // raise up a global bool flag to check if we should draw the thumbnail 
   t_bound_box word_box = get_visible_world(); 
   setcolor(251,  245,  226, 255);
   
  fillrect(word_box.right(), word_box.bottom(), 
           word_box.right()- word_box.get_width()/4,word_box.bottom() + word_box.get_height()/4);
   //draw a rectangle for the background colour of the thumbnail 
   
   draw_feature(); // redraw the features for thumbnail 
  
   thumbnail_draw = false; // drop down the flag when finished 
}


bool if_in_box(t_bound_box & bb, double x, double y){
    
    if( bb.left() <= x && x <= bb.right() &&  bb.bottom() <= y && y <= bb.top()){
        return true;
    }
   
    return false;
    
}



void auto_poi_search(std::string & str){
    
    auto_str_poi.clear();
    auto_str_intersection.clear();
    unsigned input_length = str.length();
    bool if_usable = true;
    
    
    if(str.length() > 0){
    for(unsigned i = 0; i < data_structure.poi_names.size(); i++){
        std::string temp = data_structure.poi_names[i];
        
        
        if(temp.length() < input_length){
            for(unsigned j = 0; j < temp.length(); j++){
                if(str[j] != temp[j]){
                    if_usable = false;
                }
            }
            
        }
        else{
            for(unsigned j = 0; j < input_length; j++){
                if(str[j] != temp[j]){
                    if_usable = false;
                }
            }
        }
  
        if(if_usable){
            auto_str_poi.push_back(temp);
        }
        if_usable = true;        
        
    }
    }    

    if(str.length() > 0){
    for(unsigned i = 0; i < data_structure.intersection_names.size(); i++){
        std::string temp = data_structure.intersection_names[i];
        
        
        if(temp.length() < input_length){
            for(unsigned j = 0; j < temp.length(); j++){
                if(str[j] != temp[j]){
                    if_usable = false;
                }
            }
            
        }
        else{
            for(unsigned j = 0; j < input_length; j++){
                if(str[j] != temp[j]){
                    if_usable = false;
                }
            }
        }
  
        if(if_usable){
            auto_str_intersection.push_back(temp);
        }
        
        if_usable = true;        
        
    }
   
    }    

}

// set from 
void set_from(void (*drawscreen_ptr) (void)){
    if_road_found = false;
    from = input;
    drawscreen_ptr();    
}


void set_to(void (*drawscreen_ptr) (void)){
    if_road_found = false;
    to = input;
    drawscreen_ptr();
}
        
// if press navigate button the from and to connected 
void navigate(void (*drawscreen_ptr) (void)){
   
    std::vector<unsigned> from_inte_id_vector = data_structure.intername_interid[from];
    std::vector<unsigned> to_id_vector;
    
    if(from_inte_id_vector.size() != 0){
        
        unsigned from_inte_id = from_inte_id_vector[0];
        int if_char_exist = to.find('&');
        
    
    if(if_char_exist != -1 ){//to intersection
        to_id_vector = data_structure.intername_interid[to];
        
        if(to_id_vector.size() != 0){
            std::vector<unsigned> possible_found_road;

            possible_found_road = find_path_between_intersections(from_inte_id, to_id_vector[0], 15);
        
            if(possible_found_road.size() != 0){
                if_road_found = true;
                found_roads = possible_found_road;
                
                std::vector<std::pair <double, int>> navig = navigation(found_roads);
          std::vector<std::pair <std::string,double>> result = navigation_handler(navig);
    
   for (unsigned i = 0; i<result.size();i++){
       std::cout <<std::get<0>(result[i])<<std::endl;
       std::cout <<std::get<1>(result[i])<<std::endl;
   }
        std::cout <<"done"<<std::endl;
        std::cout <<"****************"<<std::endl;
            }
            else{
                 if_road_found = false;
            }
        }
    }
    else{// to poi
        
        std::vector<unsigned> possible_found_road;
        possible_found_road = find_path_to_point_of_interest(from_inte_id, to, 15);
        
        if(possible_found_road.size() != 0){
            if_road_found = true;
            found_roads = possible_found_road;
            
            std::vector<std::pair <double, int>> navig = navigation(found_roads);
          std::vector<std::pair <std::string,double>> result = navigation_handler(navig);
    
   for (unsigned i = 0; i<result.size();i++){
       std::cout <<std::get<0>(result[i])<<std::endl;
       std::cout <<std::get<1>(result[i])<<std::endl;
   }
        std::cout <<"done"<<std::endl;
        std::cout <<"****************"<<std::endl;
        }
        else{
             if_road_found = false;
        }
        
        
    }
    }

    drawscreen_ptr();

}

// if enable clicking searching mode 
void click_searching(void (*drawscreen_ptr) (void)){
    if(if_click_searching){
        if_click_searching = false;
        click_searching_1 = false;
        click_searching_2 = false;
        if_road_found = false;
        click = false;
        found_roads.clear();
        
    }
    else{
        if_click_searching = true;
        click_searching_1 = false;
        click_searching_2 = false;
        if_road_found = false;
        click = false;
        found_roads.clear();
        
    }
    
    drawscreen_ptr();
}

void click_search_calc(){
    
    std::vector<unsigned> possible_found_road;
     possible_found_road = find_path_between_intersections(click_1, click_2, 15);

    if(possible_found_road.size() != 0){
        if_road_found = true;
        found_roads = possible_found_road;
          std::vector<std::pair <double, int>> navig = navigation(found_roads);
          std::vector<std::pair <std::string,double>> result = navigation_handler(navig);
    
   for (unsigned i = 0; i<result.size();i++){
       std::cout <<std::get<0>(result[i])<<std::endl;
       std::cout <<std::get<1>(result[i])<<std::endl;
   }
        std::cout <<"done"<<std::endl;
        std::cout <<"****************"<<std::endl;
    }
    else{
        if_road_found = false;
    }
     
     drawscreen();
    
}


// handeling the information from the navigation function 
std::vector<std::pair <std::string,double>> navigation_handler(std::vector<std::pair<double, int>> n){
    std::vector<std::pair <std::string,double>> return_value;
    std::string intro;
    int status = 0;
    double distance;
    std::pair <std::string,double> temp;
    int num = 0;
    
    std::vector<std::pair <std::string,int>> nav_instructions;  
    for (unsigned i = 0; i < n.size(); i++ ){
        if (status == std::get<1>(n[i])){
            distance = std::get<0>(n[i]) + std::get<1>(return_value[num-1]);
             temp = std::make_pair(intro,distance); 
             return_value[num-1]= temp;
        }
        else {     
            
                  status = std::get<1>(n[i]);
            
              switch (status) {
                case 1:
                    intro = "Go straight";
                    break;
                case 2:
                    intro = "Turn right" ;
                    break;
                case 3:
                    intro = "Turn hard right";
                    break;
                case 4:
                    intro = "Turn left";
                    break;
                case 5:
                    intro = "Turn hard left";
                    break;
                case 6:
                    intro = "Turn slight right ";
                    break;
                case 7:
                    intro = "Turn slight left";
                    break;
                  default:
                    intro = "error";
                    
                    break;
                 }
              temp = std::make_pair(intro,std::get<0>(n[i])); 
              return_value.push_back(temp);
              num++;
              
        }
        
    }
    return return_value;
} 

// the function that find the navigation of the best searched road
std::vector<std::pair <double, int>> navigation(std::vector<unsigned> paths) {
    std::vector <std::pair<double, int>> navi; 
    std::pair<unsigned, int> temp; 
    unsigned numCurve;  
    double temp_x1, temp_x2, temp_y1, temp_y2, temp_x3, temp_y3;
    LatLon temp_point1, temp_point2, temp_point3;
    LatLon edgepoint; 
    double dotresult, crossresult; 
    double mag; 
    double distance;
    
    
    for (unsigned i = 0; i<paths.size(); i++) {
        numCurve = getStreetSegmentInfo(paths[i]).curvePointCount;
//    
        if (numCurve != 0){
            edgepoint = data_structure.intersection[getStreetSegmentInfo(paths[i]).from];
            for (unsigned j = 0; j <numCurve; j++){
                
                 // then loop street segment, find curve length between two curve points
                
                if (j == 0) {
                    temp_point1= edgepoint;   
                }

                if (j == (numCurve-1)){
                     temp_point3 =  data_structure.intersection[getStreetSegmentInfo(paths[i]).to]; 
                   }
                else {
                     temp_point3 = getStreetSegmentCurvePoint(paths[i], j+1); // second curve poi
                }
                   
                  temp_point2 = getStreetSegmentCurvePoint(paths[i], j); // first curve pointnt
                  //double avg_lat3 = (temp_point1.lat()+temp_point2.lat())/2; 
            
                    temp_x1 = temp_point1.lon() * cos(avg_lat) * DEG_TO_RAD *100000; 
                    temp_x2 = temp_point2.lon() * cos(avg_lat) * DEG_TO_RAD*100000;
                    temp_x3 = temp_point3.lon() * cos(avg_lat) * DEG_TO_RAD*100000;

                    temp_y1 = temp_point1.lat() * DEG_TO_RAD*100000;
                    temp_y2 = temp_point2.lat() * DEG_TO_RAD*100000;
                    temp_y3 = temp_point3.lat() * DEG_TO_RAD*100000;
                   
                    double first_vec []{temp_x2-temp_x1,temp_y2-temp_y1};
                    double second_vec []{temp_x3-temp_x2,temp_y3-temp_y2};
                    double third_vec []{temp_x3-temp_x1, temp_y3-temp_y1};
                    
                    dotresult = first_vec[0]*second_vec[0]+first_vec[1]*second_vec[1]; 
                    mag = sqrt(first_vec[0]*first_vec[0]+first_vec[1]*first_vec[1])*sqrt(second_vec[0]*second_vec[0]+second_vec[1]*second_vec[1]); 
                    crossresult =third_vec[0]*first_vec[1]-first_vec[0]*third_vec[1]; 
                    
                    if (crossresult > 0){
                          if (dotresult == 0) {
                              distance = find_distance_between_two_points(temp_point1,temp_point2);
                         temp = std::make_pair(distance,2);
                         navi.push_back(temp); 
                    } 
                    if (dotresult > 0){
                        if ((acos(abs(dotresult/mag))*180/PI)>45) {
                             distance = find_distance_between_two_points(temp_point1,temp_point2);
                         temp = std::make_pair(distance,2);
                             navi.push_back(temp); 
                        }
                        else if ((acos(abs(dotresult/mag))*180/PI)>10) {
                             distance = find_distance_between_two_points(temp_point1,temp_point2);
                         temp = std::make_pair(distance,1);
                            
                             navi.push_back(temp); 
                        }
                        else {
                             distance = find_distance_between_two_points(temp_point1,temp_point2);
                         temp = std::make_pair(distance,1);
                            
                              navi.push_back(temp);
                        }
                    }
                    else {
                          if ((acos(abs(dotresult/mag))*180/PI)>45) {
                               distance = find_distance_between_two_points(temp_point1,temp_point2);
                         temp = std::make_pair(distance,2);
                             
                             navi.push_back(temp); 
                        }
                        else if ((acos(abs(dotresult/mag))*180/PI)>10) {
                             distance = find_distance_between_two_points(temp_point1,temp_point2);
                              temp = std::make_pair(distance,2);
                      
                             navi.push_back(temp); 
                        }
                        else {
                             distance = find_distance_between_two_points(temp_point1,temp_point2);
                             temp = std::make_pair(distance,1);
                             
                              navi.push_back(temp);
                        }
                    }
                    }
                    else if (crossresult < 0){
                                if (dotresult == 0) {
                                     distance = find_distance_between_two_points(temp_point1,temp_point2);
                         temp = std::make_pair(distance,4);
                        
                         navi.push_back(temp); 
                    } 
                    if (dotresult > 0){
                        if ((acos(abs(dotresult/mag))*180/PI)>45) {
                             distance = find_distance_between_two_points(temp_point1,temp_point2);
                         temp = std::make_pair(distance,4);
                             
                             navi.push_back(temp); 
                        }
                        else if ((acos(abs(dotresult/mag))*180/PI)>10) {
                             distance = find_distance_between_two_points(temp_point1,temp_point2);
                         temp = std::make_pair(distance,1);
           
                             navi.push_back(temp); 
                        }
                        else {
                             distance = find_distance_between_two_points(temp_point1,temp_point2);
                         temp = std::make_pair(distance,1);
                             
                             navi.push_back(temp);
                        }
                    }
                    else {
                          if ((acos(abs(dotresult/mag))*180/PI)>45) {
                               distance = find_distance_between_two_points(temp_point1,temp_point2);
                         temp = std::make_pair(distance,4);
                             
                             navi.push_back(temp); 
                        }
                        else if ((acos(abs(dotresult/mag))*180/PI)>10) {
                             distance = find_distance_between_two_points(temp_point1,temp_point2);
                         temp = std::make_pair(distance,4);
                             
                             navi.push_back(temp); 
                        }
                        else {
                             distance = find_distance_between_two_points(temp_point1,temp_point2);
                         temp = std::make_pair(distance,1);
                          
                             navi.push_back(temp);
                        }
                    }
                    }
                    else {
                         distance = find_distance_between_two_points(temp_point1,temp_point2);
                         temp = std::make_pair(distance,1);
                         
                          navi.push_back(temp);  
                    }
             
                    temp_point1 =  temp_point2 ;

            }
        }
        // checking for the junction between two streetsegments
        if (i != (paths.size()-1)){
            StreetSegmentInfo next_seg = data_structure.streetsegmentinfo[paths[i+1]];
            StreetSegmentInfo current_seg = data_structure.streetsegmentinfo[paths[i]];
            if (current_seg.streetID == next_seg.streetID){
                  if (current_seg.curvePointCount == 0) {
                     temp_point1 =data_structure.intersection[current_seg.from];
                    }
                  else {
                      temp_point1 = getStreetSegmentCurvePoint(paths[i], numCurve-1);
                   }
                  
                    temp_point2 = data_structure.intersection[current_seg.to];
            
                    if (next_seg.curvePointCount == 0){
                        temp_point3 = data_structure.intersection[next_seg.to];
                    }
                    else{
                        temp_point3 = getStreetSegmentCurvePoint(paths[i+1], next_seg.curvePointCount-1);
                    }
                
                distance = find_distance_between_two_points(temp_point1,temp_point2);
                         temp = std::make_pair(distance,1);
                         navi.push_back(temp);
                
            }
            else {
                   if (current_seg.curvePointCount == 0) {
                    temp_point1 =data_structure.intersection[current_seg.from];
            }
            else {
                temp_point1 = getStreetSegmentCurvePoint(paths[i], numCurve-1);
            }
            temp_point2 = data_structure.intersection[current_seg.to];
            
            if (next_seg.curvePointCount == 0){
                temp_point3 = data_structure.intersection[next_seg.to];
            }
            else{
                temp_point3 = getStreetSegmentCurvePoint(paths[i+1], next_seg.curvePointCount-1);
            }
            
                    temp_x1 = temp_point1.lon() * cos(avg_lat) * DEG_TO_RAD*100000; 
                    temp_x2 = temp_point2.lon() * cos(avg_lat) * DEG_TO_RAD*100000;
                    temp_x3 = temp_point3.lon() * cos(avg_lat) * DEG_TO_RAD*100000;

                    temp_y1 = temp_point1.lat() * DEG_TO_RAD*100000;
                    temp_y2 = temp_point2.lat() * DEG_TO_RAD*100000;
                    temp_y3 = temp_point3.lat() * DEG_TO_RAD*100000;
                   
                    
                    double first_vec []{temp_x2-temp_x1,temp_y2-temp_y1};
                    double second_vec []{temp_x3-temp_x2,temp_y3-temp_y2};
                    double third_vec []{temp_x3-temp_x1, temp_y3-temp_y1};
                    
                    dotresult = first_vec[0]*second_vec[0]+first_vec[1]*second_vec[1]; 
                    mag = sqrt(first_vec[0]*first_vec[0]+first_vec[1]*first_vec[1])*sqrt(second_vec[0]*second_vec[0]+second_vec[1]*second_vec[1]); 
                    crossresult =third_vec[0]*first_vec[1]-first_vec[0]*third_vec[1]; 
                    
                    if (crossresult > 0){
                          if (dotresult == 0) {
                         distance = find_distance_between_two_points(temp_point1,temp_point2);
                         temp = std::make_pair(distance,2);
                         navi.push_back(temp); 
                         } 
                        if (dotresult > 0){
                        if ((acos(abs(dotresult/mag))*180/PI)>45) {
                             distance = find_distance_between_two_points(temp_point1,temp_point2);
                             temp = std::make_pair(distance,2);
                             navi.push_back(temp); 
                        }
                        else if ((acos(abs(dotresult/mag))*180/PI)>20) {
                             distance = find_distance_between_two_points(temp_point1,temp_point2);
                             temp = std::make_pair(distance,2);
                             navi.push_back(temp); 
                        }
                        else {
                              distance = find_distance_between_two_points(temp_point1,temp_point2);
                             temp = std::make_pair(distance,2); 
                              navi.push_back(temp);
                        }
                     }
                    else {
                       
                          if ((acos(abs(dotresult/mag))*180/PI)>45) {
                               distance = find_distance_between_two_points(temp_point1,temp_point2);
                             temp = std::make_pair(distance,2);
                             navi.push_back(temp); 
                        }
                        else if ((acos(abs(dotresult/mag))*180/PI)>20) {
                             distance = find_distance_between_two_points(temp_point1,temp_point2);
                             temp = std::make_pair(distance,2);
                             navi.push_back(temp); 
                        }
                        else {
                             distance = find_distance_between_two_points(temp_point1,temp_point2);
                             temp = std::make_pair(distance,2); 
                              navi.push_back(temp);
                        }
                    }
                    }
                    else if (crossresult <0){
                         if (dotresult == 0) {
                                      distance = find_distance_between_two_points(temp_point1,temp_point2);
                                       temp = std::make_pair(distance,4);
                                      navi.push_back(temp); 
                               } 
                        if (dotresult > 0){
                         if ((acos(abs(dotresult/mag))*180/PI)>45){
                               distance = find_distance_between_two_points(temp_point1,temp_point2);
                             temp = std::make_pair(distance,4);
                             navi.push_back(temp); 
                         }
                        else {
                             temp = std::make_pair(distance,4); 
                             navi.push_back(temp);
                        }
                    }
                    else {
                            double angle =(acos(abs(dotresult/mag))*180/PI);
                          if (angle>45) {
                              distance = find_distance_between_two_points(temp_point1,temp_point2);
                             temp = std::make_pair(distance,4);
                             navi.push_back(temp); 
                        }
                          else if (angle>30) {
                             distance = find_distance_between_two_points(temp_point1,temp_point2);
                             temp = std::make_pair(distance,5);
                             navi.push_back(temp); 
                        }
                        else {
                             distance = find_distance_between_two_points(temp_point1,temp_point2);
                             temp = std::make_pair(distance,4); 
                              navi.push_back(temp);
                        }
                     }
                    }
                    else {
                         distance = find_distance_between_two_points(temp_point1,temp_point2);
                         temp = std::make_pair(distance,1); 
                          navi.push_back(temp);  
                    }
            }
            }
    
    }
    
    return navi;
   
}


void check_which_button_pressed(){
    
    
    
          if ((x_click <=(current_box.left() + current_box.get_width() /6 ) )&&(x_click >= current_box.left())
         &&(y_click <= (current_box.top() - current_box.get_height()*3/20))&&(y_click >= (current_box.top() - current_box.get_height()*4/20))){
              if (!planner_click){
                  planner_click = true;
                  information_click = false;
              }
              else planner_click = false;    
               planner_layer = true;
               
            }
          if ((x_click<=(current_box.left() + current_box.get_width() /6 ) )&&(x_click >= current_box.left())
         &&(y_click <= (current_box.top() - current_box.get_height()*2/20))&&(y_click >= (current_box.top() - current_box.get_height()*3/20))){
             if (!information_click) {
                 information_click = true;
                 planner_click = false;
             }
             else information_click = false;   
             information_layer = true;
          }
        
    
}