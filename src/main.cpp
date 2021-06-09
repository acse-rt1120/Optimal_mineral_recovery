#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "../includes/Test.h"
#include "../includes/CUnit.h"
#include "../includes/CCircuit.h"
#include "../includes/Genetic_Algorithm.h"

//#include "CCircuit.cpp"
//#include "CUnit.cpp"
//#include "Test.cpp"
//#include "Genetic_Algorithm.cpp"

using namespace std;

void read_int(ifstream &input_file, int &value) {
    string s;
    stringstream ss;
    getline(input_file, s, '\n');
    ss << s;
    getline(ss, s, ',');
    getline(ss, s, ',');

    if (s == "") {
        throw "Please reformat the file";
    }
    value = stoi(s);

}

void read_double(ifstream &input_file, double &value) {
    string s;
    stringstream ss;
    getline(input_file, s, '\n');
    ss << s;
    getline(ss, s, ',');
    getline(ss, s, ',');
    if (s == "") {
        throw "Please reformat the file";
    }
    value = stod(s);
}

void run_experiment(string filename, bool save) {
    ifstream input_file;
    input_file.open(filename);
    if (!input_file.is_open()) {
        cout << "Error could not find config file\n";
        run_experiment(filename, save);
    }

    // load the parameters
    int num_unit, cir_iter;
    double cir_tol;

    int mute_step, num_offspring, ga_iter;
    double profit, cost;
    double cross_prob, mute_rate, gen_tol;

    double gor_feed, waste_feed;

    read_int(input_file, num_unit);
    read_double(input_file, profit);
    read_double(input_file, cost);

    read_double(input_file, cir_tol);
    read_int(input_file, cir_iter);
    read_double(input_file, cross_prob);

    read_double(input_file, mute_rate);
    read_int(input_file, mute_step);
    read_int(input_file, num_offspring);

    read_int(input_file, ga_iter);
    read_double(input_file, gen_tol);

    read_double(input_file, gor_feed);
    read_double(input_file, waste_feed);

    cout << "-----------INPUT SUCCESS----------\n";
    cout << "number of units: " << num_unit << endl;
    cout << "profit: " << profit << endl;
    cout << "cost: " << cost << endl;
    cout << "circuit tolerance: " << cir_tol << endl;
    cout << "circuit iteration: " << cir_iter << endl;
    cout << "crossing probability: " << cross_prob << endl;
    cout << "mutation probability: " << mute_rate << endl;
    cout << "mutate step: " << mute_step << endl;
    cout << "num offspring: " << num_offspring << endl;
    cout << "GA iteration: " << ga_iter << endl;
    cout << "GA tolerance: " << gen_tol << endl;
    cout << "gormanium feed: " << gor_feed << endl;
    cout << "waste feed: " << waste_feed << endl;

    if (save == true){
        double start = clock();
        iteration_save(mute_rate, mute_step, num_offspring, cross_prob, num_unit, ga_iter, cir_tol, cir_iter, profit, cost,
                  gor_feed, waste_feed);
        double end = clock();
        cout << "Time used for Experiment (save): " << (double) (end - start) / CLOCKS_PER_SEC << endl;

    }
    if (save == false){
        double start = clock();
        iteration(mute_rate, mute_step, num_offspring, cross_prob, num_unit, ga_iter, cir_tol, cir_iter, profit, cost,
                  gor_feed, waste_feed);
        double end = clock();

        cout << "Time used for Experiment (without save): " << (double) (end - start) / CLOCKS_PER_SEC << endl;

    }

}

int main(int argc, char *argv[]) {
    int save;
    save = atoi(argv[1]);

    if (save == 1) {
        cout << "************************* START EXPERIMENT-1 *************************" << endl;
        run_experiment("./input_file/input_1.csv", true);
        cout << "************************* END EXPERIMENT-1  **************************" << endl << endl;

        cout << "************************* START EXPERIMENT-2 *************************" << endl;
        run_experiment("./input_file/input_2.csv", true);
        cout << "************************* END EXPERIMENT-2 *************************" << endl << endl;

        cout << "************************* START EXPERIMENT-3 *************************" << endl;
        run_experiment("./input_file/input_3.csv", true);
        cout << "************************* END EXPERIMENT-3 *************************" << endl;
    } else {
        cout << "************************* START EXPERIMENT-1 *************************" << endl;
        run_experiment("./input_file/input_1.csv", false);
        cout << "************************* START EXPERIMENT-1 *************************" << endl;

        cout << "************************* START EXPERIMENT-2 *************************" << endl;
        run_experiment("./input_file/input_2.csv", false);
        cout << "************************* END EXPERIMENT-2 *************************" << endl << endl;

        cout << "************************* START EXPERIMENT-3 *************************" << endl;
        run_experiment("./input_file/input_3.csv", false);
        cout << "************************* END EXPERIMENT-3 *************************" << endl;

    }
    return 0;
}
