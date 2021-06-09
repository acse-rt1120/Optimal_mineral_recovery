#ifndef TEST_H
#define TEST_H
/// This is a class contain test.
/// 
/// This is for developer to manual test our algorithm.
class Test
{
public:
    /// construct Test
    Test();
    /// distruct Test
    virtual ~Test();
    /// test the circuit simulation function Evaluate with a test circuit
    /// @note   test has a default tolerance of 1e-3
    /// @return Test Passed or Test Failed
    bool test_circuit_simulation(double tol = 1e-3);
    /// test the circuit Validity function Check_Validity with a test circuit
    /// @return Test Passed or Test Failed
    bool test_Check_Validity();
};
#endif