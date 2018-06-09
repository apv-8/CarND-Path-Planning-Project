#ifndef PATH_GENERATOR_H
#define PATH_GENERATOR_H
#include <iostream>
#include <vector>
#include "trajectory.h"
#include "vehicle.h"

using namespace std;

class PathGenerator
{
    public:

        const Vehicle& ego;
        Trajectory& current_trajectory;

        
        PathGenerator(const Vehicle& ego, Trajectory& trajectory);

        ~PathGenerator();

        /**
         * Generates a number of paths given a set of target conditions
         * 
         */ 
        vector<Trajectory> generatePaths(double target_s, double target_d, 
                                         double target_s_speed, double target_d_speed,
                                         double target_s_acc, double target_d_acc,
                                         double std_s, double std_d, 
                                         int count, int from_point_index, double time_interval);
    
    private:

        /**
         * @brief Computes the coefficients of the quintic polynomial for a jerk
         * minimized trajectory
         * 
         * @param start the start position, velocity and acceleration
         * @param end the desired end position, velocity and acceleration
         * @param T the time period in seconds
         * @return vector<double> the 6 coefficients vector of the quintic polynomial that minimizes jerk
         */
        vector<double> JMT(vector< double> start, vector <double> end, double T);
};


#endif