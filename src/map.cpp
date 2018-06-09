#include <iostream>
#include <vector>
#include <math.h>
#include "map.h"
#include "helpers.h"

using namespace std;

// Map::Map(){}


Map& Map::getInstance()
{
    // Guaranteed to be instantiated on first use
    static Map instance;
    return instance;
}

void Map::addWaypoint(double x, double y, double s, double dx, double dy)
{
    this->map_waypoints_x.push_back(x);
    this->map_waypoints_y.push_back(y);
    this->map_waypoints_s.push_back(s);
    this->map_waypoints_dx.push_back(dx);
    this->map_waypoints_dy.push_back(dy);
}

int Map::closestWaypoint(double x, double y)
{
    double closestLen = 100000; //large number
	int closestWaypoint = 0;

	for(int i = 0; i < this->map_waypoints_x.size(); i++)
	{
		double map_x = this->map_waypoints_x[i];
		double map_y = this->map_waypoints_y[i];
		double dist = distance(x,y,map_x,map_y);
		if(dist < closestLen)
		{
			closestLen = dist;
			closestWaypoint = i;
		}

	}

	return closestWaypoint;
}

int Map::nextWaypoint(double x, double y, double theta)
{
    int closestWaypoint = this->closestWaypoint(x, y);

	double map_x = this->map_waypoints_x[closestWaypoint];
	double map_y = this->map_waypoints_y[closestWaypoint];

	double heading = atan2((map_y-y),(map_x-x));

    double angle = fabs(theta-heading);
    angle = min(2*pi() - angle, angle);

    if(angle > pi()/4)
    {
        closestWaypoint++;
        if (closestWaypoint == this->map_waypoints_x.size())
        {
            closestWaypoint = 0;
        }
    }

    return closestWaypoint;
}

vector<double> Map::toFrenet(double x, double y, double theta)
{
    int next_wp = this->nextWaypoint(x,y, theta);

	int prev_wp;
	prev_wp = next_wp-1;
	if(next_wp == 0)
	{
		prev_wp  = this->map_waypoints_x.size()-1;
	}

	double n_x = this->map_waypoints_x[next_wp]-this->map_waypoints_x[prev_wp];
	double n_y = this->map_waypoints_y[next_wp]-this->map_waypoints_y[prev_wp];
	double x_x = x - this->map_waypoints_x[prev_wp];
	double x_y = y - this->map_waypoints_y[prev_wp];

	// find the projection of x onto n
	double proj_norm = (x_x*n_x+x_y*n_y)/(n_x*n_x+n_y*n_y);
	double proj_x = proj_norm*n_x;
	double proj_y = proj_norm*n_y;

	double frenet_d = distance(x_x,x_y,proj_x,proj_y);

	//see if d value is positive or negative by comparing it to a center point

	double center_x = 1000-this->map_waypoints_x[prev_wp];
	double center_y = 2000-this->map_waypoints_y[prev_wp];
	double centerToPos = distance(center_x,center_y,x_x,x_y);
	double centerToRef = distance(center_x,center_y,proj_x,proj_y);

	if(centerToPos <= centerToRef)
	{
		frenet_d *= -1;
	}

	// calculate s value
	double frenet_s = 0;
	for(int i = 0; i < prev_wp; i++)
	{
		frenet_s += distance(this->map_waypoints_x[i],this->map_waypoints_y[i],this->map_waypoints_x[i+1],this->map_waypoints_y[i+1]);
	}

	frenet_s += distance(0,0,proj_x,proj_y);

	return {frenet_s,frenet_d};
}

vector<double> Map::toRealWorldXY(double s, double d)
{
    // cout << "TO READLWORLD - MAP-SIZE=" << this->map_waypoints_x.size() << endl;
    int prev_wp = -1;

	while(s > this->map_waypoints_s[prev_wp+1] && (prev_wp < (int)(this->map_waypoints_s.size()-1) ))
	{
		prev_wp++;
	}

	int wp2 = (prev_wp+1)%this->map_waypoints_x.size();

	double heading = atan2((this->map_waypoints_y[wp2]-this->map_waypoints_y[prev_wp]),(this->map_waypoints_x[wp2]-this->map_waypoints_x[prev_wp]));
	// the x,y,s along the segment
	double seg_s = (s-this->map_waypoints_s[prev_wp]);

	double seg_x = this->map_waypoints_x[prev_wp]+seg_s*cos(heading);
	double seg_y = this->map_waypoints_y[prev_wp]+seg_s*sin(heading);

	double perp_heading = heading-pi()/2;

	double x = seg_x + d*cos(perp_heading);
	double y = seg_y + d*sin(perp_heading);

	return {x,y};

}

