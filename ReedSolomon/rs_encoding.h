#ifndef RS_ENCODING_H
#define RS_ENCODING_H

#include <vector>
#include "rs_encoding.cpp"

// A function that computes a generator polynomial for a given number of redundant characters
   /* @brief Ñomputes a generator polynomial
	* @param red_code_len - the number of characters representing the redundant code
	* @return generator polynomial*/
std::vector<int> rs_generator_poly(int& red_code_len);

/* @brief encoding of the message
* @param msg_in - an incoming message that is represented by a vector of polynomials(in this case, integers)
* @param red_code_len - the number of characters representing the redundant code
* @return encoded message = vector [msg_in] + [redundant information] (in this case, integers)*/
std::vector<int> rs_encode_msg(std::vector<int>& msg_in, int red_code_len);

#endif