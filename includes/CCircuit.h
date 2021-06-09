
#ifndef CCIRCUIT_H
#define CCIRCUIT_H

#include "CUnit.h"
/// This is a class of circuit.
/// 
///a circuit has circuit feed components, judging criteria
/// We can calculate the performance value of a circuit and check the Validity of it.
class CCircuit {
public:

    // INITIAL PARAMETERS
    double cost ; // the penalty for waste every kg
    double profit ; // the profit for gormanium every kg
    double input_gor ; // the initial valuable material of the circuit feed
    double input_waste; //the initial waste material of the circuit feed
    double tol ; // the tolerance for the converge of mass balance


    // Circuit simulation PARAMETERS
    int num_units; // the units in this circuit
    CUnit* units = nullptr; // the array for units in circuit


    /// create a circuit
    /// @param num_units    number of units in this circuit
    CCircuit(int num_units); // default constructor
    ~CCircuit();

    /// evaluate the circuit with a performance value
    /// @param vector    the vector represented the connection method
    /// @param tolerance    the tolerance
    /// @param max_iteration    the max iteration time
    /// @return performance value
    double Evaluate(int *vector, double tolerance, int max_iteration);

    /// check the circuit is valid.
    /// @param circuit_vector    the vector represented the connection method
    /// @param tolerance    the tolerance
    /// @return true or false
    bool Check_Validity(int *circuit_vector);

    //double circuit_feed[2];

    /// mark the units in the circuit
    /// @param unit_num the unit id
    void mark_units(int unit_num);

    /// mark the tailing stream of this unit
    /// @param unit_num the unit id
    void mark_tailings(int unit_num);

    /// mark the concentrate stream of this unit
    /// @param unit_num the unit id
    void mark_concentrate(int unit_num);

    /// create our circuit, find the connection method of each units
    /// @param circuit_vector    the vector represented the connection method
    void init_allunits(int *circuit_vector);

    /// calculate the difference of feed in unit i
    /// @param i    the unit id
    /// @return difference
    double cal_rel_tol(int i); //relative change
};
#endif
