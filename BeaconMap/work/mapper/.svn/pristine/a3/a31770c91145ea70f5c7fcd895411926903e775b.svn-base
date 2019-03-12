/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m2_header.h
 * Author: liuyixu6
 *
 * Created on February 23, 2018, 4:41 PM
 */

#ifndef M2_HEADER_H
#define M2_HEADER_H


#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>

#include <string>
#include <math.h>

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <map>

#include "m2.h"
#include "m3.h"
#include "m1.h"
#include "graphics.h"
#include "dataStructure.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"

std::string output;//input in search bar
std::string input;//output in result bar
std::string from;
std::string to;
std::vector<std::string> auto_str_poi;
std::vector<std::string> auto_str_intersection;

bool mouse_move = false; 
float x_coordinate, y_coordinate;
bool if_draw_intersection = false;//flag control if draw intersection found in searching
double avg_lat = 0; // average latitude
bool thumbnail_draw = false;
t_bound_box first_world;
int select_auto_search = -1;
int limit_of_auto_search = 0;
unsigned click_1, click_2;

t_bound_box search_bar;//bound box of search bar
t_bound_box current_box;//bound box of current window
t_bound_box result_bar;//bound box of result bar


std::vector<unsigned> found_roads;
void draw_found_roads();

bool click_intersection = false; //flag control if draw intersection when left clicking
double x_click,y_click; 
bool if_draw_pois = false;
bool if_draw_subway = false;
bool if_draw_attraction = false;
bool if_draw_highway = false;
bool if_road_found = false;
bool click_searching_1 = false;
bool click_searching_2 = false;
bool if_click_searching = false;

Surface arrow = load_png_from_file("libstreetmap/resources/arrow.png");
Surface pin = load_png_from_file("libstreetmap/resources/marker.png");
bool click = false;

void draw_click();
bool if_on_search_icon = false;

bool if_navi= false;

bool click_menu = false;
bool mouse_move_draw = false;
std::string now;
// mouse move 
//void act_on_mousemove(float x, float y);
float activeMouse_x;
float activeMouse_y;
bool planner_layer = false;
bool information_layer = false;
bool planner_click = false;
bool information_click =false;
bool navi_start = false;
int current_page =0;



t_bound_box mission_prompt;//the dark_blue menu
t_bound_box submission; //submenu 
t_bound_box planning_area;
t_bound_box navi_area;

t_bound_box gray_layer;
t_bound_box journey_planner;
t_bound_box journey_planner_str;
t_bound_box information;
Surface journey_icon =  load_png_from_file("libstreetmap/resources/journey.png");
Surface information_icon = load_png_from_file("libstreetmap/resources/information.png");
Surface introduction_icon = load_png_from_file("libstreetmap/resources/introduction1.png");
Surface right =  load_png_from_file("libstreetmap/resources/right.png");
Surface left = load_png_from_file("libstreetmap/resources/left.png"); 
Surface straight = load_png_from_file("libstreetmap/resources/straight.png");

// global data structure
extern dataStructure data_structure;

void drawscreen(void);
void search_button(void (*drawscreen_ptr) (void));
void act_on_button_press(float x, float y, t_event_buttonPressed event);
void act_on_mouse_move(float x, float y);
void act_on_key_press(char c, int keysym);
void delay(long milliseconds);

void act_on_mousebutton(float x, float y, t_event_buttonPressed button_info);
unsigned close_inter (double lon, double lat);

// display the thumbnail 
void thumbnail();


void input_calculation();

// draw street segment name
void draw_street_name();

// draw features such as buildings and lakes
void draw_feature();
void draw_poi_and_name();

bool if_in_box(t_bound_box & bb, double x, double y );


// draw osm features with osm database
// it includes draw_relation, draw_way, draw_node
void draw_osm();

// traverse OSMRelation, draw subway
void draw_relation();
// traverse OSMWay, draw highway
void draw_way();
// traverse OSMNode, draw tourism points
void draw_node();


// draw subway routes with different colors, and mark all stations
void draw_osm_subway_and_station(const OSMWay* w, color_types color);

// draw subway lines with color and two points specified
void draw_subway_line(LatLon& point1, LatLon& point2, color_types color);

// draw highway with particular color
// road is drawn as orange color outside and yellow color inside
void draw_osm_highway(const OSMWay* w, int R, int G, int B, int width);

// draw highway lines with two points specified
void draw_highway_line(LatLon& point1, LatLon& point2, int R, int G, int B, int width);

// draw subway station with station name, icon is within function
void draw_subway_station(LatLon& point, std::string name);

// draw tourism point, icon is within function
void draw_tourism_point(LatLon& point, std::string name);

void draw_found_road(std::vector<unsigned> & seg);

void draw_click_poi_and_intersection();

// whether user clicks button
bool if_draw_osm = false;
// if osm button is pressed, then set if_draw_osm to false
void osm_button(void (*drawscreen_ptr) (void));

// whether user clicks button
bool if_search = false;

void draw_search();
// if searchbar_button is pressed, then set if_search to false
void searchbar_button(void (*drawscreen_ptr) (void));

//function control the on and off of attractions
void attraction_button(void (*drawscreen_ptr) (void));

//function control the on and off of subways
void subway_button(void (*drawscreen_ptr) (void));

void set_from(void (*drawscreen_ptr) (void));

void set_to(void (*drawscreen_ptr) (void));

void navigate(void (*drawscreen_ptr) (void));

void click_searching(void (*drawscreen_ptr) (void));

void auto_poi_search(std::string & str);

void click_search_calc();

std::vector<std::pair <double, int>> navigation(std::vector<unsigned> paths);

std::vector<std::pair <std::string,double>> navigation_handler(std::vector<std::pair<double, int>> n);

void act_on_mousemove(float x, float y);


void check_which_button_pressed();

#endif /* M2_HEADER_H */

