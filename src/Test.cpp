#include <iostream>
#include "../includes/Test.h"
#include"../includes/CCircuit.h"
#include "../includes/CUnit.h"
#include<math.h>

using namespace std;


Test::Test()
{}


Test::~Test()
{}

bool Test::test_circuit_simulation(double tol)
{
	cout << endl << "Check if Circuit Simulation works well." << endl;
	CCircuit circuit(15);
	int vector[31] = {12,7,12,7,0,7,14,10,1,10,3,14,6,13,5,10,4,1,11,1,8,15,7,1,5,7,2,0,16,1,9 };
	double performance = circuit.Evaluate(vector, 1e-6, 500);
	bool testPass = false;
	if (abs(performance - 408.546) < tol)
		testPass = true;
	if (testPass) {
		cout << "Test Passed" << endl;
	}
	else {
		cout << "Test Failed" << endl;
	}
	return testPass;
}

bool Test::test_Check_Validity()
{
	cout << endl << "Check if Check_Validity works well." << endl;
	CCircuit circuit(5);
	//int vector[31] = {12,7,12,7,0,7,14,10,1,10,3,14,6,13,5,10,4,1,11,1,8,15,7,1,5,7,2,0,16,1,9 }; 02 failed
	int vector[11] = { 0,4,3,2,0,5,4,4,6,2,1 }; 
	// int vector[11] = { 0,4,3,2,0,5,4,4,3,2,1 }; 03 failed
	// int vector[11] = { 0,4,3,2,0,5,4,4,4,2,1 }; 04 failed
	// int vector[11] = { 0,4,1,2,6,5,4,4,3,2,1 }; 01 failed
	bool testPass = circuit.Check_Validity(vector);
	if (testPass) {
		cout << "Test Passed" << endl;
	}
	else {
		cout << "Test Failed" << endl;
	}
	return testPass;
}