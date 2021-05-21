#include <stdint.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <assert.h>

namespace gf {

    //auxiliary output functions
    void print(std::vector<int> vec) {
        for (int i = 0; i < vec.size(); i++) {
            std::cout << vec[i] << " ";
        }
        std::cout << std::endl;
    }

    void print(std::vector<int> vec, int len) {
        //std::cout << std::endl;
        for (int i = 0; i < len; i++) {
            std::cout << char(vec[i]);
        }
        std::cout << std::endl;
    }

    void print(unsigned char* vec, int len) {
        for (int i = 0; i < len ; i++) {
            std::cout << vec[i];
        }
        std::cout << std::endl;
    }

    void print(char* vec, int len) {
        for (int i = 0; i < len; i++) {
            std::cout << vec[i];
        }
        std::cout << std::endl;
    }

    void print(int x) {
        std::cout << x << std::endl;
    }

    void print(std::string x)
    {
        for (int i = 0; i < x.size(); i++) {
            std::cout << x[i];
        }
        std::cout << std::endl;
    }


    //Read more about calculating tables, log, and exp you can see in help_gf.hpp. 
    //For clarity, they are explicitly defined.

    const int exp_[512] = {
        1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180, 117, 234,
        201, 143, 3, 6, 12, 24, 48, 96, 192, 157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238,
        193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161, 95, 190,
        97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240, 253, 231, 211, 187, 107, 214,
        177, 127, 254, 225, 223, 163, 91, 182, 113, 226, 217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52,
        104, 208, 189, 103, 206, 129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102,
        204, 133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84, 168, 77, 154, 41, 82,
        164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115, 230, 209, 191, 99, 198, 145, 63, 126, 252,
        229, 215, 179, 123, 246, 241, 255, 227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165,
        87, 174, 65, 130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166, 81, 162, 89,
        178, 121, 242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9, 18, 36, 72, 144, 61, 122, 244, 245,
        247, 243, 251, 235, 203, 139, 11, 22, 44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173,
        71, 142, 1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180,
        117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192, 157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181,
        119, 238, 193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161, 95,
        190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240, 253, 231, 211, 187, 107, 214,
        177, 127, 254, 225, 223, 163, 91, 182, 113, 226, 217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52,
        104, 208, 189, 103, 206, 129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102,
        204, 133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84, 168, 77, 154, 41, 82,
        164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115, 230, 209, 191, 99, 198, 145, 63, 126, 252, 229,
        215, 179, 123, 246, 241, 255, 227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165, 87, 174,
        65, 130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166, 81, 162, 89, 178, 121,
        242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9, 18, 36, 72, 144, 61, 122, 244, 245, 247, 243,
        251, 235, 203, 139, 11, 22, 44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173, 71, 142
    };

    const int log_[256] = {
        0, 0, 1, 25, 2, 50, 26, 198, 3, 223, 51, 238, 27, 104, 199, 75, 4, 100, 224, 14, 52, 141, 239,
        129, 28, 193, 105, 248, 200, 8, 76, 113, 5, 138, 101, 47, 225, 36, 15, 33, 53, 147, 142, 218, 240,
        18, 130, 69, 29, 181, 194, 125, 106, 39, 249, 185, 201, 154, 9, 120, 77, 228, 114, 166, 6, 191,
        139, 98, 102, 221, 48, 253, 226, 152, 37, 179, 16, 145, 34, 136, 54, 208, 148, 206, 143, 150, 219,
        189, 241, 210, 19, 92, 131, 56, 70, 64, 30, 66, 182, 163, 195, 72, 126, 110, 107, 58, 40, 84, 250,
        133, 186, 61, 202, 94, 155, 159, 10, 21, 121, 43, 78, 212, 229, 172, 115, 243, 167, 87, 7, 112, 192,
        247, 140, 128, 99, 13, 103, 74, 222, 237, 49, 197, 254, 24, 227, 165, 153, 119, 38, 184, 180, 124,
        17, 68, 146, 217, 35, 32, 137, 46, 55, 63, 209, 91, 149, 188, 207, 205, 144, 135, 151, 178, 220, 252,
        190, 97, 242, 86, 211, 171, 20, 42, 93, 158, 132, 60, 57, 83, 71, 109, 65, 162, 31, 45, 67, 216, 183,
        123, 164, 118, 196, 23, 73, 236, 127, 12, 111, 246, 108, 161, 59, 82, 41, 157, 85, 170, 251, 96, 134,
        177, 187, 204, 62, 90, 203, 89, 95, 176, 156, 169, 160, 81, 11, 245, 22, 235, 122, 117, 44, 215, 79,
        174, 213, 233, 230, 231, 173, 232, 116, 214, 244, 234, 168, 80, 88, 175
    };



    /* ################################
     * # OPERATIONS OVER GALUA FIELDS #
     * ################################ */

     /* @brief Addition in Galua Fields
      * @param x - left operand
      * @param y - right operand
      * @return x + y */
    inline int add(int x, int y) {
        return x ^ y;
    }

    /* ##### GF substraction ###### */
    /* @brief Substraction in Galua Fields
     * @param x - left operand
     * @param y - right operand
     * @return x - y */
    inline int sub(int x, int y) {
        return x ^ y;
    }

   
    /* @brief Multiplication in Galua Fields
     * @param x - left operand
     * @param y - rifht operand
     * @return x * y */
    inline int mult(int x, int y) {
        if (x == 0 || y == 0)
            return 0;
        return exp_[log_[x] + log_[y]];
    }

    /* @brief Division in Galua Fields
     * @param x - dividend
     * @param y - divisor
     * @return x / y */
    inline int div_my(int x, int y) {
        assert(y != 0);
        if (x == 0) return 0;
        return exp_[(log_[x] + 255 - log_[y]) % 255]; //add 255 to make sure the difference is not negative
    }

    /* @brief X in power Y w
     * @param x     - operand
     * @param power - power
     * @return x^power */
    inline int pow_my(int x, int power) {
        int i = log_[x];
        //std::cout << "log" << i << std::endl;
        i *= power;
        i %= 255;
        
        if (i < 0) i = i + 255;

        return exp_[i];
    }

    /* @brief Inversion in Galua Fields
     * @param x - number
     * @return inversion of x */
    inline int inverse(int x) {
        return exp_[255 - log_[x]]; /* == div(1, x); */
    }



    /* ##########################
     * # POLYNOMIALS OPERATIONS #
     * ########################## */


     /* @brief Multiplication polynomial by scalar
      * @param &p    - source polynomial
      * @param x     - scalar
      * @return      - result  polynomial */
    std::vector<int> gf_poly_scale(std::vector<int>& p, int x) {
        std::vector<int> res;
        res.reserve(p.size());

        for (int i = 0; i < p.size(); i++) {
            res.push_back(mult(p[i], x));
        }
        return res;
    }

    /* @brief Addition of two polynomials
     * @param &p    - right operand polynomial
     * @param &q    - left operand polynomial
     * @return destination polynomial */
    std::vector<int> gf_poly_add(std::vector<int> p, std::vector<int> q) {
        std::vector<int> res;
        int max = std::max(p.size(), q.size());
        res.reserve(max);

        for (int i = 0; i < max; i++) {
            res.push_back(0);
        }

        for (int i = 0; i < p.size(); i++) {
            res[i + res.size() - p.size()] = p[i];
        }
        for (int i = 0; i < q.size(); i++) {
            res[i + res.size() - q.size()] ^= q[i];
        }

        return res;
    }

    /* @brief Multiplication of two polynomials
     * @param &p    - right operand polynomial
     * @param &q    - left operand polynomial
     * @return destination polynomial */
    std::vector<int> gf_poly_mult(std::vector<int>& p, std::vector<int>& q) {
        std::vector<int> res;
        res.reserve(p.size() + q.size() - 1);

        for (int i = 0; i < p.size() + q.size() - 1; i++) {
            res.push_back(0);
        }

        //multiply each coefficient p by all coefficients q
        for (int j = 0; j < q.size(); j++) {
            for (int i = 0; i < p.size(); i++) {
                res[i + j] ^= mult(p[i], q[j]);      //~res[i + j] = add(res[i+j], mul(p[i], q[j]))
            }
        }

        return res;
    }

    /* @brief Division of two polynomials
     * @param &divident    - right operand polynomial
     * @param &divisor    - left operand polynomial
     * @param &quotient - destination polynomial
     * @param &remainder - destination polynomial */
    void gf_poly_div(std::vector<int>& divident, std::vector<int>& divisor, std::vector<int>& quotient, std::vector<int>& remainder) {
        std::vector<int> msg_out;
        msg_out.reserve(divident.size());

        for (int i = 0; i < divident.size(); i++) {
            msg_out.push_back(divident[i]);
        }

        //use the synthetic division algorithm
        for (int i = 0; i < divident.size() - (divisor.size() - 1); i++) {
            int coef = msg_out[i];
            if (coef != 0) {                                          //log[0] is undefined, so we need to manually check for this case
                for (int j = 1; j < divisor.size(); j++) {
                    if (divisor[j] != 0) {
                        msg_out[i + j] ^= mult(divisor[j], coef);      //~ msg_out[i + j] += - divisor[j] * coef, but xor is faster
                    }
                }
            }
        }

        for (int i = 0; i < msg_out.size(); i++) {
            if (i < msg_out.size() - (divisor.size() - 1))
                quotient.push_back(msg_out[i]);
            else
                remainder.push_back(msg_out[i]);
        }
    }

    // Calculate the polynomial in the field given by the value x
    /* @brief Evaluation of polynomial in x
     * @param &p - polynomial to evaluate
     * @param x  - evaluation point */
    int gf_poly_eval(std::vector<int>& poly, int x) {
        int y = poly[0];
        for (int i = 1; i < poly.size(); i++) {
            y = mult(y, x) ^ poly[i];
        }
        return y;
    }

}

