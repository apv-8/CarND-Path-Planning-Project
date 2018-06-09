#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <vector>

using namespace std;
class Map
{
    public:
        static Map& getInstance();

        // C++ 11 technique for singleton
        Map(Map const&)               = delete;
        void operator=(Map const&)    = delete;

        void addWaypoint(double x, double y, double s, double dx, double dy);
        
        vector<double> toFrenet(double x, double y, double theta);

        vector<double> toRealWorldXY(double s, double d);

    private:
        // Private constructor
        Map(){}

        vector<double> map_waypoints_x;
        vector<double> map_waypoints_y;
        vector<double> map_waypoints_s;
        vector<double> map_waypoints_dx;
        vector<double> map_waypoints_dy;

        int closestWaypoint(double x, double y);
        int nextWaypoint(double x, double y, double theta);


};

#endif