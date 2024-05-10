//#pragma once
//#include <gmock/gmock.h>
//
//int Factorial(int n) { // Returns the factorial of n
//    if (n <= 0) //基本情况返回1;
//        return 1;
//    else
//        return n * Factorial(n - 1);
//}
//
//// Tests factorial of 0.
//TEST(FactorialTest, HandlesZeroInput) {
//    EXPECT_EQ(Factorial(0), 1);
//}
//
//// Tests factorial of positive numbers.
//TEST(FactorialTest, HandlesPositiveInput) {
//    EXPECT_EQ(Factorial(1), 1);
//    EXPECT_EQ(Factorial(2), 2);
//    EXPECT_EQ(Factorial(3), 6);
//    EXPECT_EQ(Factorial(8), 40320);
//}
//
//// Tests factorial of negative numbers.
//TEST(FactorialTest, HandlesNegativeInput) {
//    EXPECT_EQ(Factorial(-1), 1);
//    EXPECT_EQ(Factorial(-2), 1);
//    EXPECT_EQ(Factorial(-3), 1);
//    EXPECT_EQ(Factorial(-8), 1);
//}