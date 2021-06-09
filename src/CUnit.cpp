#include "../includes/CUnit.h"
#include<cstring>
#include <cmath>


CUnit::CUnit() {
    for (int i = 0; i < 2; i++) {
        conc_send[i] = 0;
        tail_send[i] = 0;
        cur_feed[i] = 0;
        old_feed[i] = 0;
    }
    mark = false;
};

CUnit::~CUnit() {
}

void CUnit::cal_con_tail() {
    // conc_send[0] represent gormanium send to next concentrate stream
    conc_send[0] = cur_feed[0] * 0.2;
    // conc_send[1] represent waste send to next concentrate stream
    conc_send[1] = cur_feed[1] * 0.05;
    // tail_send[0] represent gormanium send to next tailing stream
    tail_send[0] = cur_feed[0] - conc_send[0];
    // tail_send[1] represent waste send to next tailing stream
    tail_send[1] = cur_feed[1] - conc_send[1];
}

void CUnit::store_feed() {
    for (int i = 0; i < 2; i++) {
        old_feed[i] = cur_feed[i];
    }

}

void CUnit::clean_feed() {
    for (int i = 0; i < 2; i++) {
        cur_feed[i] = 0;
    }
}
