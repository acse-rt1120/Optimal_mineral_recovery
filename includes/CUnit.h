/// This is a class that represents a unit
/// 
/// a unit has id, feed composition, feed rate, product streams. 
/// We can calculate the performance of a Unit and manipulate streams.
#pragma once

class CUnit {  // member functions and variables of CUnit
public:

    int id; ///< set the id for this unit 
    int conc_num; ///<index of the unit to which this unit’s concentrate stream is connected
    int tails_num; ///<index of the unit to which this unit’s tail stream is connected
    bool mark;     ///<A Boolean that is changed to true if the unit has been seen
    double conc_send[2]; ///< the concentrate  send out from this unit
    double tail_send[2]; ///< the tail send out from this unit
    double cur_feed[2]; ///< current feed (new feed), store new gormanium and waste for update
    double old_feed[2]; ///< old feed, used for store the gormanium and waste
    int num_node;
    double input_gor;
    double input_waste;

    CUnit();

    /// create a unit with given id, destination concentrate and tailing stream
    /// @param id   The id of unit.
    /// @param dest_con The destination concentrate of unit
    /// @param dest_tail    The tailing stream of unit
    ~CUnit();

    /// calculate the concentrate and tailing stream in this unit
    void cal_con_tail();

    /// store the current feed stream
    void store_feed();

    /// clean the current feed stream
    void clean_feed();

    void reset_cur_feed();

    double cal_rel_tol();
};


