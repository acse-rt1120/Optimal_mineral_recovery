#include <stdio.h>
#include <cmath>
#include <time.h>
#include <ctime>
#include <vector>
#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>
#include <string>
#include "../includes/CUnit.h"
#include "../includes/CCircuit.h"
#include "../includes/Genetic_Algorithm.h"

// find smallest performance in the fitness list
double find_smallest_performance(double *performance_list, int num_offspring) {
    double smallest_performance = performance_list[0];
    for (int j = 1; j < num_offspring; j++) {
        if (performance_list[j] < smallest_performance) {
            smallest_performance = performance_list[j];
        }
    }
    return smallest_performance;
}

// determine whether to stop with given fitness list
bool stopping_iteration(double *performance_list, int num_offspring) {
    for (int j = 1; j < num_offspring; j++) {
        if (performance_list[j] != performance_list[j - 1])
            return false;
    }
    return true;
}

// flatten the performance list, i.e. 0 for smallest performance, max - min for largest performance
void flatten_performance_list(double *performance_list, double smallest_performance, int num_offspring) {
    for (int j = 0; j < num_offspring; j++) {
        performance_list[j] = log10(performance_list[j] - smallest_performance + 10);
    }
}

// calculate the sum of the flattened performance list. this is used for randomly pick vectors for crossover
int sum_flattened_performance(double *flattened_performance_list, int num_offspring) {
    double flattened_performance_sum = 0;
    for (int j = 0; j < num_offspring; j++) {
        flattened_performance_sum += flattened_performance_list[j];
    }
    return flattened_performance_sum;
}


// generate an random index with weighted probability to be crossovered
int pick_vector_idx_for_crossover(double *flattened_performance_list, double flattened_performance_sum) {
    double random_acc_performance = flattened_performance_sum * rand() / RAND_MAX;
    int index = -1;
    while (random_acc_performance >= 0) {
        index++;
        random_acc_performance -= flattened_performance_list[index];
    }
    return index;
}

double iteration(double mutate_rate, int mutate_step, int num_offspring, double crossing_probability, int num_units,
                 int max_iter,
                 double cir_tol, int cir_iter, double profit, double cost, double gor_feed, double was_feed) {

    srand((unsigned) time(NULL));
    int vector_size = 2 * num_units + 1;
    double max_performance;

    CCircuit *circuit = new CCircuit(num_units);
    circuit->cost = cost;
    circuit->profit = profit;
    circuit->input_waste = was_feed;
    circuit->input_gor = gor_feed;
    circuit->tol = cir_tol;

    double *fitness_list = new double[num_offspring];
    int *parent_list = new int[num_offspring * vector_size];

    int *son_vector = new int[vector_size];                      // initialize 2 child_vector
    int *daughter_vector = new int[vector_size];

    int *child_list = new int[num_offspring * vector_size];      // initialise a child list to be filled
    double *child_fitness_list = new double[num_offspring];      // initialise a fitness list for child generation
    int *random_vector = new int[vector_size];

    for (int i = 0; i < num_offspring; i++) {
        for (int j = 0; j < vector_size; j++) {                  // initialize a vector
            random_vector[j] = rand() % (num_units + 2);
        }

        // check its validity: keep generate new vector if it is not valid. 
        while (!circuit->Check_Validity(random_vector)) {
            for (int j = 0; j < vector_size; j++) {
                random_vector[j] = rand() % (num_units + 2);
            }
        }
        // fill the child_list
        for (int j = 0; j < vector_size; j++) {
            parent_list[i * vector_size + j] = random_vector[j];
        }
        // calculate the performance (fitness value) and fit the fitness list
        fitness_list[i] = circuit->Evaluate(random_vector, cir_tol, cir_iter);
    }

    //
    for (int iter = 0; iter < max_iter; iter++) {
        if (stopping_iteration(fitness_list, num_offspring)) break;
        std::cerr << "Iteration: " << iter << std::endl;
        int idx_fittest = 0;  // find the index of the vector with highest performance in the parent list
        for (int j = 0; j < num_offspring; j++) {
            if (fitness_list[j] > fitness_list[idx_fittest]) {
                idx_fittest = j;
            }
        }
        // fill the fisrt position with child_list with fittest vector
        for (int i = 0; i < vector_size; i++) {
            child_list[i] = parent_list[idx_fittest * vector_size + i];
        }
        // fill the first position with the highest performance
        child_fitness_list[0] = fitness_list[idx_fittest];

        // highest fitness value
        max_performance = child_fitness_list[0];

        //=== PRINT TO TEST
        std::cout << "Highest Performance = " <<max_performance << "\nVector with highest performance"
                  << std::endl;
        for (int i = 0; i < vector_size; i++) {
            std::cout << child_list[i] << ", ";
        }
        std::cerr << std::endl;

        // find the smallest fitness
        double smallest_performance = find_smallest_performance(fitness_list, num_offspring);

        // flatten the performance list, i.e. 0 for smallest performance, max - min for largest performance
        flatten_performance_list(fitness_list, smallest_performance, num_offspring);
        double flattened_performance_sum = sum_flattened_performance(fitness_list, num_offspring);

        // mutate and crossover
        int counter = 1; // start from 1 because the 0 position is already occupied with the vector with highest fitness value
        while (counter < num_offspring) {
            // randomly pick 2 vector index;
            int random_index_father = pick_vector_idx_for_crossover(fitness_list, flattened_performance_sum);
            int random_index_mother = pick_vector_idx_for_crossover(fitness_list, flattened_performance_sum);
            while (random_index_father == random_index_mother) {
                random_index_mother = pick_vector_idx_for_crossover(fitness_list, flattened_performance_sum);
            }
            // crossover
            // first decide whether to crossover
            bool to_crossover = (bool) ((crossing_probability * 100) >= (rand() % 100));
            if (to_crossover) {
                // randomly pick a point to crossover
                int crossover_point = (rand() % (vector_size - 1)) + 1;

                // crossover part
                for (int i = 0; i < crossover_point; i++) {
                    son_vector[i] = parent_list[random_index_father * vector_size + i];
                    daughter_vector[i] = parent_list[random_index_mother * vector_size + i];
                }
                // remain part
                for (int i = crossover_point; i < vector_size; i++) {
                    daughter_vector[i] = parent_list[random_index_father * vector_size + i];
                    son_vector[i] = parent_list[random_index_mother * vector_size + i];
                }
            } else {
                for (int i = 0; i < vector_size; i++) {
                    son_vector[i] = parent_list[random_index_father * vector_size + i];
                    daughter_vector[i] = parent_list[random_index_mother * vector_size + i];
                }
            }
            // mutation
            // decide whether to mutate
            for (int i = 0; i < vector_size; i++) {
                int mutate_direction;
                bool to_mutate = (bool) ((mutate_rate * 100) >= (rand() % 100));
                if (to_mutate) {

                    // mutate a vector
                    mutate_direction = (rand() % 2) * 2 - 1; // decide plus the step or minus the step
                    son_vector[i] = (son_vector[i] + num_units + 2 + mutate_direction * mutate_step) %
                                    (num_units + 2); // modulus to make sure within right number of unit
                }
                to_mutate = (bool) ((mutate_rate * 100) >= (rand() % 100));
                if (to_mutate) {

                    // mutate another vector
                    mutate_direction = (rand() % 2) * 2 - 1; // decide plus the step or minus the step
                    daughter_vector[i] = (daughter_vector[i] + num_units + 2 + mutate_direction * mutate_step) %
                                         (num_units + 2); // modulus to make sure within right number of unit
                }
            }
            // check validity
            if (circuit->Check_Validity(son_vector)) {
                // store into the child_list
                for (int i = 0; i < vector_size; i++) {
                    child_list[counter * vector_size + i] = son_vector[i];
                }
                child_fitness_list[counter] = circuit->Evaluate(son_vector, 10e-6, 10000);
                counter++;
            }
            if (circuit->Check_Validity(daughter_vector) &&
                counter < num_offspring) { // to prevent overflow of child_list
                // store into the child_list
                for (int i = 0; i < vector_size; i++) {
                    child_list[counter * vector_size + i] = daughter_vector[i];
                }
                child_fitness_list[counter] = circuit->Evaluate(daughter_vector, 10e-6, 10000);
                counter++;
            }

        }

        // fill and replace the parent list with child list
        for (int n = 0; n < num_offspring * vector_size; n++) {
            parent_list[n] = child_list[n];
        }
        // fill and replace the fitness list with child fitness list
        for (int j = 0; j < num_offspring; j++) {
            fitness_list[j] = child_fitness_list[j];
        }
    }

    delete circuit;
    delete[]fitness_list;
    delete[]parent_list;
    delete[]son_vector;
    delete[]daughter_vector;
    delete[]child_list;
    delete[]child_fitness_list;
    delete[]random_vector;

    return max_performance;
}

double iteration_save(double mutate_rate, int mutate_step, int num_offspring, double crossing_probability, int num_units,
                 int max_iter,
                 double cir_tol, int cir_iter, double profit, double cost, double gor_feed, double was_feed) {

    // write a running log
    struct tm t;
    time_t now;
    time(&now);

    //localtime_r(&now, &t); // command used for linux / MacOS
    localtime_s(&t, &now); // command used for Windows

    std::ofstream log;
    std::string file_name = "./log/Runninglog";
    file_name += std::to_string(t.tm_year + 1900);
    file_name += std::to_string(t.tm_mon);
    file_name += std::to_string(t.tm_mday);
    file_name += std::to_string(t.tm_hour);
    file_name += std::to_string(t.tm_min);
    file_name += std::to_string(t.tm_sec);
    file_name += ".log";
    log.open(file_name, std::ios::app);
    log << "====PARAMETER====\n";
    log << "Number_of_Units = " << num_units << std::endl;
    log << "Number_of_Offspring = " << num_offspring << std::endl;
    log << "Max_Iteration = " << max_iter << std::endl;
    log << "Crossover_Probability = " << crossing_probability << std::endl;
    log << "Mutation_Rate = " << mutate_rate << std::endl;
    log << "Mutation_Step = " << mutate_step << std::endl;
    log << "====INFO====\n";
    log << "Iteration; " << "Highest Performance; " << "Vector with Highest Performance; " << "Performance List" << std::endl;
    log << "====OUTPUT====" << std::endl;
    log.close();

    srand((unsigned) time(NULL));
    int vector_size = 2 * num_units + 1;
    double max_performance;

    CCircuit *circuit = new CCircuit(num_units);
    circuit->cost = cost;
    circuit->profit = profit;
    circuit->input_waste = was_feed;
    circuit->input_gor = gor_feed;
    circuit->tol = cir_tol;

    double *fitness_list = new double[num_offspring];
    int *parent_list = new int[num_offspring * vector_size];

    int *son_vector = new int[vector_size];                      // initialize 2 child_vector
    int *daughter_vector = new int[vector_size];

    int *child_list = new int[num_offspring * vector_size];      // initialise a child list to be filled
    double *child_fitness_list = new double[num_offspring];      // initialise a fitness list for child generation
    int *random_vector = new int[vector_size];

    for (int i = 0; i < num_offspring; i++) {
        for (int j = 0; j < vector_size; j++) {                  // initialize a vector
            random_vector[j] = rand() % (num_units + 2);
        }

        // check its validity: keep generate new vector if it is not valid.
        while (!circuit->Check_Validity(random_vector)) {
            for (int j = 0; j < vector_size; j++) {
                random_vector[j] = rand() % (num_units + 2);
            }
        }
        // fill the child_list
        for (int j = 0; j < vector_size; j++) {
            parent_list[i * vector_size + j] = random_vector[j];
        }
        // calculate the performance (fitness value) and fit the fitness list
        fitness_list[i] = circuit->Evaluate(random_vector, cir_tol, cir_iter);
    }

    //
    for (int iter = 0; iter < max_iter; iter++) {
        if (stopping_iteration(fitness_list, num_offspring)) break;
        std::cerr << "Iteration: " << iter << std::endl;
        int idx_fittest = 0;  // find the index of the vector with highest performance in the parent list
        for (int j = 0; j < num_offspring; j++) {
            if (fitness_list[j] > fitness_list[idx_fittest]) {
                idx_fittest = j;
            }
        }
        // fill the fisrt position with child_list with fittest vector
        for (int i = 0; i < vector_size; i++) {
            child_list[i] = parent_list[idx_fittest * vector_size + i];
        }
        // fill the first position with the highest performance
        child_fitness_list[0] = fitness_list[idx_fittest];

        // highest fitness value
        max_performance = child_fitness_list[0];

        //=== PRINT TO TEST
        std::cout << "Highest Performance = " <<max_performance << "\nVector with highest performance"
                  << std::endl;
        for (int i = 0; i < vector_size; i++) {
            std::cout << child_list[i] << ", ";
        }
        std::cerr << std::endl;

        //=== PRINT TO TEST
        log.open(file_name, std::ios::app);
        log << iter << "," << max_performance << ",";
        for (int i = 0; i < vector_size; i++) {
            log << child_list[i] << " ";
        }
        log << ",";
        for (int i = 0; i < num_offspring; i++) {
            log << fitness_list[i] << " ";
        }
        log << std::endl;
        log.close();

        // find the smallest fitness
        double smallest_performance = find_smallest_performance(fitness_list, num_offspring);

        // flatten the performance list, i.e. 0 for smallest performance, max - min for largest performance
        flatten_performance_list(fitness_list, smallest_performance, num_offspring);
        double flattened_performance_sum = sum_flattened_performance(fitness_list, num_offspring);

        // mutate and crossover
        int counter = 1; // start from 1 because the 0 position is already occupied with the vector with highest fitness value
        while (counter < num_offspring) {
            // randomly pick 2 vector index;
            int random_index_father = pick_vector_idx_for_crossover(fitness_list, flattened_performance_sum);
            int random_index_mother = pick_vector_idx_for_crossover(fitness_list, flattened_performance_sum);
            while (random_index_father == random_index_mother) {
                random_index_mother = pick_vector_idx_for_crossover(fitness_list, flattened_performance_sum);
            }
            // crossover
            // first decide whether to crossover
            bool to_crossover = (bool) ((crossing_probability * 100) >= (rand() % 100));
            if (to_crossover) {
                // randomly pick a point to crossover
                int crossover_point = (rand() % (vector_size - 1)) + 1;

                // crossover part
                for (int i = 0; i < crossover_point; i++) {
                    son_vector[i] = parent_list[random_index_father * vector_size + i];
                    daughter_vector[i] = parent_list[random_index_mother * vector_size + i];
                }
                // remain part
                for (int i = crossover_point; i < vector_size; i++) {
                    daughter_vector[i] = parent_list[random_index_father * vector_size + i];
                    son_vector[i] = parent_list[random_index_mother * vector_size + i];
                }
            } else {
                for (int i = 0; i < vector_size; i++) {
                    son_vector[i] = parent_list[random_index_father * vector_size + i];
                    daughter_vector[i] = parent_list[random_index_mother * vector_size + i];
                }
            }
            // mutation
            // decide whether to mutate
            for (int i = 0; i < vector_size; i++) {
                int mutate_direction;
                bool to_mutate = (bool) ((mutate_rate * 100) >= (rand() % 100));
                if (to_mutate) {

                    // mutate a vector
                    mutate_direction = (rand() % 2) * 2 - 1; // decide plus the step or minus the step
                    son_vector[i] = (son_vector[i] + num_units + 2 + mutate_direction * mutate_step) %
                                    (num_units + 2); // modulus to make sure within right number of unit
                }
                to_mutate = (bool) ((mutate_rate * 100) >= (rand() % 100));
                if (to_mutate) {

                    // mutate another vector
                    mutate_direction = (rand() % 2) * 2 - 1; // decide plus the step or minus the step
                    daughter_vector[i] = (daughter_vector[i] + num_units + 2 + mutate_direction * mutate_step) %
                                         (num_units + 2); // modulus to make sure within right number of unit
                }
            }
            // check validity
            if (circuit->Check_Validity(son_vector)) {
                // store into the child_list
                for (int i = 0; i < vector_size; i++) {
                    child_list[counter * vector_size + i] = son_vector[i];
                }
                child_fitness_list[counter] = circuit->Evaluate(son_vector, 10e-6, 10000);
                counter++;
            }
            if (circuit->Check_Validity(daughter_vector) &&
                counter < num_offspring) { // to prevent overflow of child_list
                // store into the child_list
                for (int i = 0; i < vector_size; i++) {
                    child_list[counter * vector_size + i] = daughter_vector[i];
                }
                child_fitness_list[counter] = circuit->Evaluate(daughter_vector, 10e-6, 10000);
                counter++;
            }

        }

        // fill and replace the parent list with child list
        for (int n = 0; n < num_offspring * vector_size; n++) {
            parent_list[n] = child_list[n];
        }
        // fill and replace the fitness list with child fitness list
        for (int j = 0; j < num_offspring; j++) {
            fitness_list[j] = child_fitness_list[j];
        }
    }

    delete circuit;
    delete[]fitness_list;
    delete[]parent_list;
    delete[]son_vector;
    delete[]daughter_vector;
    delete[]child_list;
    delete[]child_fitness_list;
    delete[]random_vector;

    return max_performance;
}


