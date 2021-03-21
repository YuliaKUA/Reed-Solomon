#ifndef RS_DECODING_H
#define RS_DECODING_H

#include <vector>
#include "rs_decoding.cpp"

/* @brief Function calculates the polynomial of error syndromes
	* @param msg - an incoming message that is represented by a vector of polynomials(in this case, integers)
	* @param red_code_len - the number of characters representing the redundant code
	* @return generator polynomial */
std::vector<int> rs_calc_syndromes(std::vector<int>& msg, int& red_code_len);

/* @brief Function find the error locator polynomial L(x) according to the known location of the error (for correct errarta)
	* @param err_pos - vector with the positions of the erroneous characters
	* @return error locator polynomial L(x) */
std::vector<int> rs_find_errarta_locator(std::vector<int>& err_pos);


/* @brief Function find the error polynomial W(x) = S(x)*L(x)
	* @param synd - polynomial of error syndromes (vector int) S(x)
	* @param err_loc - error locator polynomial L(x)
	* @param err_loc_size - size of L(x)
	* @return error polynomial W(x) */
std::vector<int> rs_find_error_evaluator(std::vector<int> synd, std::vector<int> err_loc, int err_loc_size);


/* @brief Correction of the input message using the Forney algorithm, which calculates the error value
	* @param msg_in - incoming message that is represented by a vector of polynomials(in this case, integers)
	* @param synd - polynomial of error syndromes (vector int) S(x)
	* @param err_pos - vector with the positions of the erroneous characters
	* @return corrected input message */
std::vector<int> rs_correct_errata(std::vector<int> msg_in, std::vector<int> synd, std::vector<int> err_pos);


/* @brief Function find the error locator polynomial L(x) using the Berlekamp-Messi algorithm
	* @param synd - polynomial of error syndromes (vector int) S(x)
	* @param red_code_len - the number of characters representing the redundant code
	* @return error locator polynomial L(x)*/
std::vector<int> rs_find_error_locator(std::vector<int> synd, int red_code_len);


/* @brief Function find the zeros of this polynomial
	* @param err_loc - error locator polynomial L(x)
	* @param nmess - message size
	* @return  vector of the index of the characters that need to be corrected */
std::vector<int> rs_find_errors(std::vector<int> err_loc, int nmess);

/* @brief Function of decoding message
	* @param msg_in - encoded message vector
	* @param red_code_len - the number of characters representing the redundant code
	* @param erase_pos - known errors positions
	* @return  decoded message*/
std::vector<int> rs_decode_msg(std::vector<int> msg_in, int red_code_len);


#endif