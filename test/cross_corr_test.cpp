#include <gtest/gtest.h>

#include "cross_corr_host.hpp"

using namespace emida;

void cross_corr_data_load(std::string name, matrix<int> & a, matrix<int> & b, matrix<int> & result)
{
    std::string test_location = "test/res/cross_corr/";
    a = matrix<int>::from_file(test_location + name + "_A.txt");
    b = matrix<int>::from_file(test_location + name + "_B.txt");
    result = matrix<int>::from_file(test_location + name + "_res.txt");
}

TEST(cross_corr, matrix_3x3)
{
    algorithm_cross_corr<int> alg;

    std::vector<int> a =
    {
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
    };

    std::vector<int> res =
    {
        9, 26, 50, 38, 21,
        42, 94, 154, 106, 54,
        90, 186, 285, 186, 90,
        54, 106, 154, 94, 42,
        21, 38, 50, 26, 9
    };


    alg.prepare(a.data(), a.data(), 3, 3, 1);
    alg.run();
    alg.finalize();

    EXPECT_EQ(res, alg.result());
}

TEST(cross_corr, matrix_4x3)
{
    algorithm_cross_corr<int> alg;

    std::vector<int> a =
    {
        1, 2, 3, 4 ,
        5, 6, 7, 8,
        9, 10, 11, 12
    };

    std::vector<int> b =
    {
        8, 7, 6, 5,
        16, 15, 14, 13,
        4, 3, 2, 1
    };

    std::vector<int> res =
    {
        1, 4, 10, 20, 25, 24, 16,
        18, 56, 116, 200, 194, 160, 96,
        79, 192, 342, 532, 471, 364, 208,
        142, 316, 524, 768, 638, 468, 256,
        45, 104, 178, 268, 229, 172, 96
    };


    alg.prepare(a.data(), b.data(), 4, 3, 1);
    alg.run();
    alg.finalize();

    EXPECT_EQ(res, alg.result());
}


TEST(cross_corr, matrix_64x64)
{
    algorithm_cross_corr<int> alg;
    matrix<int> a, b, res;
    cross_corr_data_load("64", a, b, res);

    alg.prepare(a.data.data(), b.data.data(), a.n, a.n, 1);
    alg.run();
    alg.finalize();

    EXPECT_EQ(res.data, alg.result());
}