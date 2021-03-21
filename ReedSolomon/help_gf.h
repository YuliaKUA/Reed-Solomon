#ifndef RS_DECODING_H
#define RS_DECODING_H

#include <vector>
#include "help_gf.cpp"

//Here we use the multiplication algorithm according to the method of "Russian peasants" to find a non-reduced polynomial
/* @brief Multiplication in Galua Fields
	 * @param x - left operand
	 * @param y - rifht operand
	 * @param prim - primitive binary polynomial
	 * @param field_charac_full - the number of elements in Galois field
	 * @return x * y */
int gf_mult(int x, int y, int prim = 0, int field_charac_full = 256);


/* @brief Finding a list of all simple polynomials for a given generator(2) and the characteristic exponent of the Galois field(8)
	 * @param c_exp - Galois field exponent
	 * @return [irreducible polynomial] */
std::vector<int> find_prime_polys(int c_exp = 8);

/* @brief Calculating the "logarithm" and "exponent" tables for further use in arithmetic operations
	 * @param gf_exp - the vector passed by reference, in which the values will be placed exp
	 * @param gf_log - the vector passed by reference, in which the values will be placed log
	 * @param prim - primitive binary polynomial
	 * @param c_exp - Galois field exponent */
void init_tables(std::vector<int>& gf_exp, std::vector<int>& gf_log, int prim = 285, int c_exp = 8);


#endif