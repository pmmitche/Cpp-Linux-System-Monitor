#include <string>
#include <sstream>
#include <iostream>

#include "format.h"

using std::string;
using std::cout;
using std::endl;

string Format::ElapsedTime(long seconds) {
  	int hours, minutes;
  	string shours, sminutes, sseconds;
	std::ostringstream formatted;
  	minutes = seconds / 60;
  	hours = minutes / 60;
  
  	shours = std::to_string(hours);
  	sminutes = std::to_string(minutes % 60);
    sseconds = std::to_string(seconds % 60);

  	if (hours < 10) {
    	shours = "0" + std::to_string(hours);
    }
  	if ((minutes % 60) < 10) {
    	sminutes = "0" + std::to_string(minutes % 60);
    }
  	if ((seconds % 60) < 10) {
    	sseconds = "0" + std::to_string(seconds % 60);
    }
  	
  	formatted << shours << ":" << sminutes << ":" << sseconds;
  	return formatted.str();
}