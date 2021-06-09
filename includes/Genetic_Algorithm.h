#pragma once

/// find smallest performance in the fitness list
/// @param performance_list	performance list
/// @return smallest performance value
double find_smallest_performance(double* performance_list, int num_offspring);
/// calculate the sum of the flattened performance list. 
/// @param flattened_performance_list	flattened performance list
/// @return	the sum of the flattened performance list
/// @note	this is used for randomly pick vectors for crossover// calculate the sum of the flattened performance list. this is used for randomly pick vectors for crossover
int sum_flattened_performance(double* flattened_performance_list, int num_offspring);

/// make sure there is no negative value in the performance list
/// @param performance_list	performance list
/// @param smallest_performance	smallest performance value
/// @note	use the log function to make the better parents (higher performance) parents have a higher probability to generate the next generation.
/// @note	i.e. 0 for smallest performance, max - min for largest performance
void flatten_performance_list(double* performance_list, double smallest_performance, int num_offspring);

/// generate an random index with weighted probability to be crossovered
/// @return	the parent position in the all parent list 
/// @note	we can choose this father/mother at next step
int pick_vector_idx_for_crossover(double* flattened_performance_list, double flattened_performance_sum);


/// judge whether the iteration should stop or not
/// @param performance_list	performance list
/// @return	true or false
/// @note	when every children are the same and cannot produce a new generation individual, jump out of the iteration
bool stopping_iteration(double* performance_list, int num_offspring);

/// simulate the whole process for the generatic process
/// @param mutate_rate	mutation rate
/// @param mutate_step	mutation rate
/// @param num_offspring	number of offspring
/// @param crossing_probability	performance list
/// @param num_units	number of units
/// @param max_iter	maxmum iteration
/// @param cir_tol	circuit tolerance
/// @param cir_iter	circuit max iteration time
/// @param profit	profit
/// @param cost	panelty
/// @param gor_feed	the initial valuable material of the circuit feed
/// @param was_feed	the initial waste material of the circuit feed
/// @return	the highest performance for this group
/// @note	this function will simulate the whole process for the generatic process 
/// @note	which includes generate parents, crossover, mutate, generate children over and over again
/// @note	it will stop: first when rearch to the iteration time; 
/// @note	second all the children have the same performance (will remain itself without the iteration times changing)
double iteration(double mutate_rate, int mutate_step, int num_offspring, double crossing_probability, int num_units, int max_iter,
                 double cir_tol, int cir_iter, double profit, double cost, double gor_feed, double was_feed);
/// save the result to log files
/// @param mutate_rate	mutation rate
/// @param mutate_step	mutation rate
/// @param num_offspring	number of offspring
/// @param crossing_probability	performance list
/// @param num_units	number of units
/// @param max_iter	maxmum iteration
/// @param cir_tol	circuit tolerance
/// @param cir_iter	circuit max iteration time
/// @param profit	profit
/// @param cost	panelty
/// @param gor_feed	the initial valuable material of the circuit feed
/// @param was_feed	the initial waste material of the circuit feed
/// @return	the highest performance for this group
double iteration_save(double mutate_rate, int mutate_step, int num_offspring, double crossing_probability, int num_units,
               int max_iter,
               double cir_tol, int cir_iter, double profit, double cost, double gor_feed, double was_feed);

