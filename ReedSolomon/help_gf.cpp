#include <cmath>
#include <vector>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include "gf.h"


uint8_t exp_h[512]; //For efficiency, gf_exp[] has size 2 * GF_SIZE, so that a simple multiplication of two numbers can be resolved without calling % 255
uint8_t log_h[256];

#define GENERATOR 2 //replace alpha in the classical theory

//Here we use the multiplication algorithm according to the method of "Russian peasants" to find a non-reduced polynomial
/* @brief Multiplication in Galua Fields
	 * @param x - left operand
     * @param y - rifht operand
	 * @param prim - primitive binary polynomial
	 * @param field_charac_full - the number of elements in Galois field
     * @return x * y */
int gf_mult(int x, int y, int prim = 0, int field_charac_full=256) {
	int res = 0;

	while (y) {
		if ((y & 1) == 1) {
			res = res ^ x;
		}
		y = y >> 1; //~ y/2
		x = x << 1; //~ x*2

		if ((prim > 0) && (x & field_charac_full)) { // if x ~ 256 use xor
			x = x ^ prim;
		}
		
	}

	return res;
}


/* @brief Finding a list of all simple polynomials for a given generator(2) and the characteristic exponent of the Galois field(8)
	 * @param c_exp - Galois field exponent
	 * @return [irreducible polynomial] */
std::vector<int> find_prime_polys(int c_exp = 8) {
	int root_charac = 2;
	int field_charac = pow(root_charac, c_exp) - 1;             //255
	int field_charac_next = pow(root_charac, c_exp + 1) - 1;    // 511

	std::vector<int> prim_candidates;
	std::vector<int> irreducible_polynomial;
	std::vector<int> seen;

	for (int j = 0; j < field_charac + 1; j++) {
		seen.push_back(0);
	}

	for (int i = field_charac + 2; i < field_charac_next; i += root_charac) { //we need the number > 256 to avoid repetitions
		prim_candidates.push_back(i);                                         // try every simple polynomial, excluding even ones
	}

	//Here is implemented a bruteforce approach to find all these prime polynomials, by generating every possible 
	//prime polynomials(ie, every integers between field_charac + 1 and field_charac * 2), and then we build 
	//the whole Galois Field, and we reject the candidate prime polynomial if it duplicates even one value or if it 
	//generates a value above field_charac(ie, cause an overflow).
	for (int i = 0; i < prim_candidates.size(); i++) {
		int prim = prim_candidates[i];
		for (int j = 0; j < field_charac + 1; j++) {
			seen[j] = 0;                                       // specifies whether the value in the field has already been generated (seen[x] ?== 1) or not
		}

		bool conflict = false;                                 // to find out if there was at least 1 conflict
		int x = 1;

		for (int j = 0; j < field_charac; j++) {
			x = gf_mult(x, GENERATOR, prim, field_charac + 1); //compute the next value in the field

			if (x > field_charac || seen[x] == 1) {            //if this number is a duplicate, then we reject it
				//std::cout << x << std::endl;
				conflict = true;
				break;
			}
			else {
				seen[x] = 1;                                   //remember this value to detect future duplicates
			}
		}

		if (!conflict) {                                       //if there was no conflict, then this is a simple polynomial
			irreducible_polynomial.push_back(prim);
		}

	}

	return irreducible_polynomial;
}

/* @brief Calculating the "logarithm" and "exponent" tables for further use in arithmetic operations
	 * @param gf_exp - the vector passed by reference, in which the values will be placed exp
	 * @param gf_log - the vector passed by reference, in which the values will be placed log
	 * @param prim - primitive binary polynomial
	 * @param c_exp - Galois field exponent */
void init_tables(std::vector<int>& gf_exp, std::vector<int>& gf_log, int prim = 285, int c_exp = 8) {
	int field_charac = pow(2, c_exp) - 1;        //255

	for (int i = 0; i < field_charac * 2; i++) { //initialization
		gf_exp.push_back(0);
	}
	for (int i = 0; i < field_charac + 1; i++) {
		gf_log.push_back(0);
	}

	//For each element from the Galois field, we calculate the log and exp
	int x = 1;
	for (int i = 0; i < field_charac; i++) {
		gf_exp[i] = x;
		gf_log[x] = i;
		x = gf_mult(x, GENERATOR, prim, field_charac + 1);
	}

	for (int i = field_charac; i < field_charac * 2; i++) {
		gf_exp[i] = gf_exp[i - field_charac];
	}
}



