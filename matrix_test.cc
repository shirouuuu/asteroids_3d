#include "matrix.h"
#include "gtest/gtest.h"

namespace {
    TEST(MATRIX, ListInitialization2df) {
        SquareMatrix2df matrix = {
            Vector2df{1.0, 0.0},
            Vector2df{0.0, 1.0}
        };

        EXPECT_NEAR(1.0, matrix.at(0,0), 0.00001);
        EXPECT_NEAR(0.0, matrix.at(0,1), 0.00001);
        EXPECT_NEAR(0.0, matrix.at(1,0), 0.00001);
        EXPECT_NEAR(1.0, matrix.at(1,1), 0.00001);
    }

    TEST(MATRIX, ListInitialization3df) {
        SquareMatrix3df matrix = {
            Vector3df{1.0, 0.0, 0.0},
            Vector3df{0.0, 1.0, 0.0},
            Vector3df{0.0, 0.0, 1.0}
        };

        EXPECT_NEAR(1.0, matrix[0][0], 0.00001);
        EXPECT_NEAR(0.0, matrix[0][1], 0.00001);
        EXPECT_NEAR(0.0, matrix[0][2], 0.00001);
        EXPECT_NEAR(0.0, matrix[1][0], 0.00001);
        EXPECT_NEAR(1.0, matrix[1][1], 0.00001);
        EXPECT_NEAR(0.0, matrix[1][2], 0.00001);
        EXPECT_NEAR(0.0, matrix[2][0], 0.00001);
        EXPECT_NEAR(0.0, matrix[2][1], 0.00001);
        EXPECT_NEAR(1.0, matrix[2][2], 0.00001);
    }

    TEST(MATRIX, ListInitialization4df) {
        SquareMatrix4df matrix = {
            {1.0, 5.0, 9.0, 13.0},
            {2.0, 6.0, 10.0, 14.0},
            {3.0, 7.0, 11.0, 15.0},
            {4.0, 8.0, 12.0, 16.0}
        };

        float v = 1.0f;
        for (size_t row = 0; row < 4; row++) {
            for (size_t column = 0; column < 4; column++) {
                EXPECT_NEAR(v++, matrix.at(row, column), 0.00001);
            }
        }
    }


    TEST(MATRIX, ProductWithVector3df) {
        SquareMatrix3df matrix = {
            {1.0, 0.0, 1.0},
            {0.0, 1.0, 1.0},
            {0.0, 0.0, 1.0}
        };
        Vector3df vector = {-6.0, 3.0, 1.0};
        Vector3df product = matrix * vector;

        EXPECT_NEAR(-6.0, product[0], 0.00001);
        EXPECT_NEAR(3.0, product[1], 0.00001);
        EXPECT_NEAR(-2.0, product[2], 0.00001);
    }


    TEST(MATRIX, ProductWithMatrix2df) {
        SquareMatrix3df matrix1 = {
            {1.0, 2.0},
            {-1.0, 1.5}
        };
        SquareMatrix3df matrix2 = {
            {2.0, -1.0},
            {1.0, 0.0}
        };
        SquareMatrix3df matrix = matrix1 * matrix2;

        EXPECT_NEAR(3.0, matrix.at(0,0), 0.00001);
        EXPECT_NEAR(1.0, matrix.at(0,1), 0.00001);
        EXPECT_NEAR(2.5, matrix.at(1,0), 0.00001);
        EXPECT_NEAR(2.0, matrix.at(1,1), 0.00001);
    }


    TEST(MATRIX, ProductWithMatrix3df) {
        SquareMatrix3df matrix1 = {
            {2, 3, 2},
            {5, 4, 8}, {6, 6, 4}
        };
        SquareMatrix3df matrix2 = {
            {2, 2, 7}, {-3, -4, 4}, {5, 3, 2}
        };
        SquareMatrix3df matrix = matrix1 * matrix2;
        // Row 1
        EXPECT_NEAR(56, matrix.at(0,0), 0.00001);
        EXPECT_NEAR(-2, matrix.at(0,1), 0.00001);
        EXPECT_NEAR(37, matrix.at(0,2), 0.00001);

        // Row 2
        EXPECT_NEAR(56, matrix.at(1,0), 0.00001);
        EXPECT_NEAR(-1, matrix.at(1,1), 0.00001);
        EXPECT_NEAR(39, matrix.at(1,2), 0.00001);

        // Row 3
        EXPECT_NEAR(48, matrix.at(2,0), 0.00001);
        EXPECT_NEAR(-22, matrix.at(2,1), 0.00001);
        EXPECT_NEAR(42, matrix.at(2,2), 0.00001);
    }

    TEST(MATRIX, ProductWithMatrix2df2) {
        SquareMatrix2df matrix1 = {
            {
                {1, 2},
                {3, 4}
            }
        };
        SquareMatrix2df matrix2 = {
            {
                {5, 6},
                {7, 8}
            }
        };
        SquareMatrix2df matrix = matrix1 * matrix2;

        EXPECT_NEAR(23, matrix.at(0, 0), 0.00001);
        EXPECT_NEAR(31, matrix.at(0, 1), 0.00001);
        EXPECT_NEAR(34, matrix.at(1, 0), 0.00001);
        EXPECT_NEAR(46, matrix.at(1, 1), 0.00001);
    }

    TEST(MATRIX, ProductWithMatrix3df2) {
        SquareMatrix3df matrix1 = {
            {
                {2, 5, 6}, // Spalte 0
                {3, 4, 6}, // Spalte 1
                {2, 8, 4} // Spalte 2
            }
        };
        SquareMatrix3df matrix2 = {
            {
                {2, -3, 5}, // Spalte 0
                {2, -4, 3}, // Spalte 1
                {7, 4, 2} // Spalte 2
            }
        };
        SquareMatrix3df matrix = matrix1 * matrix2;

        // Zeile 0
        EXPECT_NEAR(5, matrix.at(0, 0), 0.00001);
        EXPECT_NEAR(-2, matrix.at(0, 1), 0.00001);
        EXPECT_NEAR(30, matrix.at(0, 2), 0.00001);

        // Zeile 1
        EXPECT_NEAR(38, matrix.at(1, 0), 0.00001);
        EXPECT_NEAR(18, matrix.at(1, 1), 0.00001);
        EXPECT_NEAR(67, matrix.at(1, 2), 0.00001);

        // Zeile 2
        EXPECT_NEAR(14, matrix.at(2, 0), 0.00001);
        EXPECT_NEAR(0, matrix.at(2, 1), 0.00001);
        EXPECT_NEAR(74, matrix.at(2, 2), 0.00001);
    }

    TEST(MATRIX, IdentityProduct3x3) {
        SquareMatrix3df identity = {
            {
                {1, 0, 0},
                {0, 1, 0},
                {0, 0, 1}
            }
        };
        SquareMatrix3df matrix = {
            {
                {2, 5, 8},
                {3, 6, 9},
                {4, 7, 10}
            }
        };
        SquareMatrix3df result = identity * matrix;

        EXPECT_NEAR(2.000000, result.at(0, 0), 0.00001);
        EXPECT_NEAR(3.000000, result.at(0, 1), 0.00001);
        EXPECT_NEAR(4.000000, result.at(0, 2), 0.00001);
        EXPECT_NEAR(5.000000, result.at(1, 0), 0.00001);
        EXPECT_NEAR(6.000000, result.at(1, 1), 0.00001);
        EXPECT_NEAR(7.000000, result.at(1, 2), 0.00001);
        EXPECT_NEAR(8.000000, result.at(2, 0), 0.00001);
        EXPECT_NEAR(9.000000, result.at(2, 1), 0.00001);
        EXPECT_NEAR(10.000000, result.at(2, 2), 0.00001);
    }

    TEST(MATRIX, ZeroMatrix3x3) {
        SquareMatrix3df zero = {
            {
                {0, 0, 0},
                {0, 0, 0},
                {0, 0, 0}
            }
        };
        SquareMatrix3df matrix = {
            {
                {1, 4, 7},
                {2, 5, 8},
                {3, 6, 9}
            }
        };
        SquareMatrix3df result = zero * matrix;

        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                EXPECT_NEAR(0.0, result.at(i, j), 0.00001);
    }

    TEST(MATRIX, ColumnAccessTest3x3) {
        SquareMatrix3df matrix = {
            {
                {1, 4, 7}, // Spalte 0
                {2, 5, 8}, // Spalte 1
                {3, 6, 9} // Spalte 2
            }
        };

        // Prüfe einzelne Elemente
        EXPECT_NEAR(1, matrix.at(0, 0), 0.00001);
        EXPECT_NEAR(4, matrix.at(1, 0), 0.00001);
        EXPECT_NEAR(7, matrix.at(2, 0), 0.00001);
        EXPECT_NEAR(2, matrix.at(0, 1), 0.00001);
        EXPECT_NEAR(5, matrix.at(1, 1), 0.00001);
        EXPECT_NEAR(8, matrix.at(2, 1), 0.00001);
        EXPECT_NEAR(3, matrix.at(0, 2), 0.00001);
        EXPECT_NEAR(6, matrix.at(1, 2), 0.00001);
        EXPECT_NEAR(9, matrix.at(2, 2), 0.00001);
    }
}