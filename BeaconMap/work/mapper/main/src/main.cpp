/* 
 * Copyright 2018 University of Toronto
 *
 * Permission is hereby granted, to use this software and associated 
 * documentation files (the "Software") in course work at the University 
 * of Toronto, or for personal use. Other uses are prohibited, in 
 * particular the distribution of the Software either publicly or to third 
 * parties.
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <iostream>
#include <string>
#include "m1.h"
#include "m2.h"
#include "m4.h"
#include "StreetsDatabaseAPI.h"
#include<algorithm>
#include <stdio.h>      
#include <stdlib.h>   


//Program exit codes
constexpr int SUCCESS_EXIT_CODE = 0;        //Everyting went OK
constexpr int ERROR_EXIT_CODE = 1;          //An error occured
constexpr int BAD_ARGUMENTS_EXIT_CODE = 2;  //Invalid command-line usage

//The default map to load if none is specified
std::string default_map_path;

std::string default_map_path_toronto = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";

std::string default_map_path_beijing = "/cad2/ece297s/public/maps/beijing_china.streets.bin";

std::string default_map_path_tokyo =  "/cad2/ece297s/public/maps/tokyo_japan.streets.bin";

std::string default_map_path_tehran =  "/cad2/ece297s/public/maps/tehran_iran.streets.bin";

std::string default_map_path_sydney =  "/cad2/ece297s/public/maps/sydney_australia.streets.bin";

std::string default_map_path_singapore =  "/cad2/ece297s/public/maps/singapore.streets.bin";

std::string default_map_path_saint_helena =  "/cad2/ece297s/public/maps/saint_helena.streets.bin";

std::string default_map_path_brazil =  "/cad2/ece297s/public/maps/rio-de-janeiro_brazil.streets.bin";

std::string default_map_path_new_york =  "/cad2/ece297s/public/maps/new-york_usa.streets.bin";

std::string default_map_path_new_delhi =  "/cad2/ece297s/public/maps/new-new-delhi_india.streets.bin";

std::string default_map_path_moscow =  "/cad2/ece297s/public/maps/moscow_russia.streets.bin";

std::string default_map_path_london =  "/cad2/ece297s/public/maps/london_england.streets.bin";

std::string default_map_path_interlaken =  "/cad2/ece297s/public/maps/interlaken_switzerland.streets.bin";

std::string default_map_path_iceland =  "/cad2/ece297s/public/maps/iceland.streets.bin";

std::string default_map_path_hong_kong =  "/cad2/ece297s/public/maps/hong-kong_china.streets.bin";

std::string default_map_path_hamilton =  "/cad2/ece297s/public/maps/hamilton_canada.streets.bin";

std::string default_map_path_golden_horseshoe =  "/cad2/ece297s/public/maps/golden-horseshoe_canada.streets.bin";

std::string default_map_path_capetown =  "/cad2/ece297s/public/maps/cape-town_south-africa.streets.bin";

std::string default_map_path_cairo =  "/cad2/ece297s/public/maps/cairo_egypt.streets.bin";




int main(int argc, char** argv) {


    
    std::string map_name;


    

    std::cout << "1.toronto" << std::endl;
    std::cout << "2.beijing" << std::endl;
    std::cout << "3.tokyo" << std::endl;
    std::cout << "4.tehran" << std::endl;
    std::cout << "5.sydney" << std::endl;
    std::cout << "6.singapore" << std::endl;
    std::cout << "7.saint_helena" << std::endl;
    std::cout << "8.rio_de_janeiro" << std::endl;
    std::cout << "9.new_york" << std::endl;
    std::cout << "10.new_delhi" << std::endl;
    std::cout << "11.moscow" << std::endl;
    std::cout << "12.london" << std::endl;
    std::cout << "13.interlaken" << std::endl;
    std::cout << "14.iceland" << std::endl;
    std::cout << "15.hong_kong" << std::endl;
    std::cout << "16.hamilton" << std::endl;
    std::cout << "17.golden_horseshoe" << std::endl;
    std::cout << "18.capetown" << std::endl;
    std::cout << "19.cairo" << std::endl;
    std::cout << "Please select the ID of the city that you want to load:" << std::endl;
    
    std::cin >> map_name;
    
    

    int id = std::stoi(map_name);
   

    switch(id){
        case 1:
            default_map_path = default_map_path_toronto;
            break;
        case 2:
            default_map_path = default_map_path_beijing;
            break;
        case 3:
            default_map_path = default_map_path_tokyo;
            break;
        case 4:
            default_map_path = default_map_path_tehran;
            break;
        case 5:
            default_map_path = default_map_path_sydney;
            break;
        case 6:
            default_map_path = default_map_path_singapore;
            break;
        case 7:
            default_map_path = default_map_path_saint_helena;
            break;
        case 8:
            default_map_path = default_map_path_brazil;
            break;
        case 9:
            default_map_path = default_map_path_new_york;
            break;
        case 10:
            default_map_path = default_map_path_new_delhi;
            break;
        case 11:
            default_map_path = default_map_path_moscow;
            break;
        case 12:
            default_map_path = default_map_path_london;
            break;
        case 13:
            default_map_path = default_map_path_interlaken;
            break;
        case 14:
            default_map_path = default_map_path_iceland;
            break;
        case 15:
            default_map_path = default_map_path_hong_kong;
            break;
        case 16:
            default_map_path = default_map_path_hamilton;
            break;   
        case 17:
            default_map_path = default_map_path_golden_horseshoe;
            break;
        case 18:
            default_map_path = default_map_path_capetown;
            break;
        case 19:
            default_map_path = default_map_path_cairo;
            break;
        default:
            break;
    }

    //Load the map and related data structures
    bool load_success = load_map(default_map_path);
    if(!load_success) {
        std::cerr << "Failed to load map '" << default_map_path << "'\n";
        return ERROR_EXIT_CODE;
    }

    std::cout << "Successfully loaded map '" <<default_map_path << "'\n";
    
    


    //You can now do something with the map data
     draw_map();
    
    //Clean-up the map data and related data structures
    std::cout << "Closing map\n" ;
    close_map(); 

    return SUCCESS_EXIT_CODE;
}
