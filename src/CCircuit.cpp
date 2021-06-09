#include <vector>
#include <iostream>
#include <stdio.h>
//#include "../includes/CUnit.h"
#include "../includes/CCircuit.h"

using namespace std;

CCircuit::CCircuit(int num_units) {
    this->num_units = num_units;
    units = new CUnit[num_units + 2];
    for (int i = 0; i < num_units + 2; i++) {
        units[i].num_node = num_units;
        units[i].id = i;
    }
}

CCircuit::~CCircuit() {}

void CCircuit::init_allunits(int *circuit_vector) {
    for (int i = 0; i < num_units + 2; i++) {
        units[i].conc_num = circuit_vector[1 + i * 2]; // get the destination of concen
        units[i].tails_num = circuit_vector[2 + i * 2]; // get the destination of tail
        units[i].cur_feed[0] = input_gor;
        units[i].cur_feed[1] = input_waste;
        units[i].id = i;
        units[i].mark = -1;
        if (i == num_units || i == num_units + 1) { // set the final two inexistent units
            units[i].input_gor = this->input_gor;
            units[i].input_waste = this->input_gor;
            units[i].conc_num = -1;
            units[i].tails_num = -1;
            units[i].id = i;
        }
    }
}


double CCircuit::Evaluate(int *circuit_vector, double tolerance, int max_iterations) {

    // set the initial state
    init_allunits(circuit_vector);

    //do iteration
    int iter = 0;                       // store maximum iteration
    bool converged = false;             // check whether it was converged
    double max_rel_tol;

    while (iter < max_iterations &&
           converged == false) {     // Calculate Conc and Tail streams of all units for this time step

        for (int i = 0; i < num_units; i++) {
            units[i].cal_con_tail();                        // calculate the output conc and tail for each cell
            units[i].store_feed();                          // store the current value of the feed to each cell as an old feed value
            units[i].clean_feed();                          // set the current value for all components to zero
        }

        units[num_units].clean_feed();                      // set the feed of final conc unit (inexistent) to 0
        units[num_units + 1].clean_feed();                  // set the feed of final tail unit (inexistent) to 0

        units[circuit_vector[0]].cur_feed[0] = input_gor;   // set the gormanium of input unit to 10
        units[circuit_vector[0]].cur_feed[1] = input_waste; // set the waste of input unit to 100

        // Go over each unit and add the conc and tail flows to the appropriate unit’s feed
        for (int i = 0; i < num_units; i++) {
            for (int j = 0; j < 2; j++) {
                units[units[i].conc_num].cur_feed[j] += units[i].conc_send[j];
                units[units[i].tails_num].cur_feed[j] += units[i].tail_send[j];
            }
        }

        // relative change
        max_rel_tol = cal_rel_tol(0);
        for (int i = 0; i < num_units; i++) {
            max_rel_tol = max(max_rel_tol, cal_rel_tol(i));
        }
        if (max_rel_tol < tolerance) {
            converged = true;
        }

        iter++;
    };

    double Performance = 0;         //calculate the performance
    if (!converged) {
        Performance = cost * (-input_waste);
    } else {
        Performance = units[num_units].cur_feed[0] * profit - units[num_units].cur_feed[1] * cost;
    }

    return Performance;
}

double CCircuit::cal_rel_tol(int i) {
    double rel_tol =
            abs((units[i].cur_feed[0] + units[i].cur_feed[1]) - (units[i].old_feed[0] + units[i].old_feed[1])) /
            abs((units[i].old_feed[0] + units[i].old_feed[1]));
    return rel_tol;
}


bool CCircuit::Check_Validity(int *circuit_vector) {
    init_allunits(circuit_vector);
    // get the feed number from int array
    int start_unit = circuit_vector[0];
    if (start_unit>=num_units)
        return false;
    // make all mark be false at first
    for (int i = 0; i < num_units; i++)
        units[i].mark = false;
    // Mark every cell that start_unit can see
    mark_units(start_unit);
    for (int i = 0; i < num_units; i++) {
        // 01 make sure every unit must be accessible from the feed
        if (!units[i].mark) {
            //cout << endl << i <<" unit 01 failed" << endl;
            return false;
        }

        // 03 make sure no self-recycle
        if (units[i].conc_num == i || units[i].tails_num == i) {
            //cout << endl << i <<" unit 03 failed" << endl;
            return false;
        }

        // 04 make sure the destination for both products from a unit is not the same unit.
        if (units[i].tails_num == units[i].conc_num) {
            //cout << endl << i <<" unit 04 failed" << endl;
            return false;
        }
    }
    // 02 make sure every unit must have a route forward to both of the outlet streams
    for (int i = 0; i < num_units; i++) {
        //reset all mark
        for (int i = 0; i < num_units + 2; i++) {
            units[i].mark = false;
        }
        mark_tailings(i);
        mark_concentrate(i);
        if (!units[num_units].mark || !units[num_units + 1].mark) {
            //cout << i << " unit 02 failed" << endl;
            return false;
        }

    }
    return true;
}

void CCircuit::mark_units(int unit_num) {
    //If we have seen this unit already exit
    if (units[unit_num].mark)
        return;

    //Mark that we have now seen the unit
    units[unit_num].mark = true;
    //If conc_num does not point at a circuit outlet recursively call the function
    if (units[unit_num].conc_num < num_units)
        mark_units(units[unit_num].conc_num);
    else
        units[units[unit_num].conc_num].mark = true;

    //If tails_num does not point at a circuit outlet recursively call the function
    if (units[unit_num].tails_num < num_units)
        mark_units(units[unit_num].tails_num);
    else
        units[units[unit_num].tails_num].mark = true;
}

void CCircuit::mark_tailings(int unit_num) {
    // If we have seen this unit already exit
    if (units[unit_num].mark && units[units[unit_num].tails_num].mark)
        return;

    //Mark that we have now seen the unit
    units[unit_num].mark = true;

    //If tails_num does not point at a circuit outlet recursively call the function
    if (units[unit_num].tails_num < num_units) {
        mark_tailings(units[unit_num].tails_num);
    } else
        units[units[unit_num].tails_num].mark = true;
}

void CCircuit::mark_concentrate(int unit_num) {
    // If we have seen this unit already exit
    if (units[unit_num].mark && units[units[unit_num].conc_num].mark)
        return;

    //Mark that we have now seen the unit
    units[unit_num].mark = true;

    //If conc_num does not point at a circuit outlet recursively call the function
    if (units[unit_num].conc_num < num_units)
        mark_concentrate(units[unit_num].conc_num);
    else
        units[units[unit_num].conc_num].mark = true;

}
