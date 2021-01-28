#include <iostream>
#include <string.h>
#include <stdint.h>
#include <vector>
//#include "gf.h"

#define GENERATOR 2 //replace alpha in the classical theory

// A function that computes a generator polynomial for a given number of redundant characters
   /* @brief Ñomputes a generator polynomial 
	* @param red_code_len - the number of characters representing the redundant code
	* @return generator polynomial*/
	std::vector<int> rs_generator_poly(int red_code_len) {
		std::vector<int> g, temp;

		g.push_back(1);
		temp.push_back(1);
		temp.push_back(0);

		for (uint8_t i = 0; i < red_code_len; i++) {
			temp[1] = gf::pow_my(GENERATOR, i);
			g = gf::gf_poly_mult(g, temp);
		}
		return g;                                         //(x-1)*(x-2^(1))*..*(x-2^(red_code_len-1))
	}


	/* @brief encoding of the message
	* @param msg_in - an incoming message that is represented by a vector of polynomials(in this case, integers)
	* @param red_code_len - the number of characters representing the redundant code
	* @return encoded message = vector [msg_in] + [redundant information] (in this case, integers)*/ 
	std::vector<int> rs_encode_msg(std::vector<int> msg_in, int red_code_len) {
		try {
			if (msg_in.size() + red_code_len < 256) {
				std::vector<int> gen;
				gen = rs_generator_poly(red_code_len);

				std::vector<int> msg_out;

				for (int i = 0; i < msg_in.size() + gen.size() - 1; i++) { // initialize msg_out: len = msg_in.size() + red_code_len
					if (i < msg_in.size())
						msg_out.push_back(msg_in[i]);                      // the highest k characters contain the original message
					else
					{
						msg_out.push_back(0);
					}
				}

				std::vector<int> quotient, remainder;
				gf::gf_poly_div(msg_out, gen, quotient, remainder);        // divide our original polynomial by the generator polynomial and use the remainder
				reverse(remainder.begin(), remainder.end());

				for (int i = 0; i < msg_in.size() + gen.size() - 1; i++) { 
					if (i < msg_in.size())
						msg_out[i] = msg_in[i];                      
					else
					{
						msg_out[i] = remainder[msg_in.size() + gen.size() - 2 - i];
					}
				}

				//I wrote the division function for polynomials later, so this was the first version of the algorithm, but it's also working!
				
				//// the method of synthetic division
				//for (int i = 0; i < msg_in.size(); i++) {
				//	int coef = msg_out[i];
				//	if (coef != 0) {                                        //log[0] is undefined, so we need to manually check for this case
				//		for (int j = 1; j < gen.size(); j++) {
				//			msg_out[i + j] ^= gf::mul(gen[j], coef);        //~ '+='
				//		}
				//	}
				//}

				//for (int i = 0; i < msg_in.size(); i++) {                    //in the higher k characters, there is now a quotient, 
				//	msg_out[i] = msg_in[i];                                  //we do not need it for encoding, so we rewrite the msg_in here
				//}

				return msg_out;
			}
			else {
				throw "The message is too long";
			}
		}
		catch (std::string ex) {
			std::cout << ex << std::endl;
		}
	}