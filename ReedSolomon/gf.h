#ifndef GF_H
#define GF_H

#include <vector>
#include <iostream>
#include "gf.cpp"

namespace gf {
	//auxiliary output functions
	void print(std::vector<int> vec);

	void print(std::vector<int> vec, int len);

	void print(unsigned char* vec, int len);

	void print(char* vec, int len);

	void print(int x);

	void print(std::string x);


	/* ################################
	 * # OPERATIONS OVER GALUA FIELDS #
	 * ################################ */

	 /* @brief Addition in Galua Fields
	  * @param x - left operand
	  * @param y - right operand
	  * @return x + y */
	inline int add(int x, int y);

	/* ##### GF substraction ###### */
	/* @brief Substraction in Galua Fields
	 * @param x - left operand
	 * @param y - right operand
	 * @return x - y */
	inline int sub(int x, int y);


	/* @brief Multiplication in Galua Fields
	 * @param x - left operand
	 * @param y - rifht operand
	 * @return x * y */
	inline int mult(int x, int y);

	/* @brief Division in Galua Fields
	 * @param x - dividend
	 * @param y - divisor
	 * @return x / y */
	inline int div_my(int x, int y);

	/* @brief X in power Y w
	 * @param x     - operand
	 * @param power - power
	 * @return x^power */
	inline int pow_my(int x, int power);

	/* @brief Inversion in Galua Fields
	 * @param x - number
	 * @return inversion of x */
	inline int inverse(int x);



	/* ##########################
	 * # POLYNOMIALS OPERATIONS #
	 * ########################## */

	 /* @brief Multiplication polynomial by scalar
	  * @param &p    - source polynomial
	  * @param x     - scalar
	  * @return      - result  polynomial */
	std::vector<int> gf_poly_scale(std::vector<int>& p, int x);

	/* @brief Addition of two polynomials
	 * @param &p    - right operand polynomial
	 * @param &q    - left operand polynomial
	 * @return destination polynomial */
	std::vector<int> gf_poly_add(std::vector<int> p, std::vector<int> q);

	/* @brief Multiplication of two polynomials
	 * @param &p    - right operand polynomial
	 * @param &q    - left operand polynomial
	 * @return destination polynomial */
	std::vector<int> gf_poly_mult(std::vector<int>& p, std::vector<int>& q);

	/* @brief Division of two polynomials
	 * @param &divident    - right operand polynomial
	 * @param &divisor    - left operand polynomial
	 * @param &quotient - destination polynomial
	 * @param &remainder - destination polynomial */
	void gf_poly_div(std::vector<int>& divident, std::vector<int>& divisor, std::vector<int>& quotient, std::vector<int>& remainder);

	// Calculate the polynomial in the field given by the value x
	/* @brief Evaluation of polynomial in x
	 * @param &p - polynomial to evaluate
	 * @param x  - evaluation point */
	int gf_poly_eval(std::vector<int>& poly, int x);
}

#endif