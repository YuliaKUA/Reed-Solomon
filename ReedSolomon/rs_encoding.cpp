#include <iostream>
#include <string.h>
#include <stdint.h>
#include <vector>
#include "gf.h"


#define GENERATOR 2 //replace alpha in the classical theory

// A function that computes a generator polynomial for a given number of redundant characters
   /* @brief Ñomputes a generator polynomial 
	* @param red_code_len - the number of characters representing the redundant code
	* @return generator polynomial*/
	std::vector<int> rs_generator_poly(int &red_code_len) {
		std::vector<int> g, temp;
		g.reserve(red_code_len);
		temp.reserve(red_code_len);

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
	std::vector<int> rs_encode_msg(std::vector<int>& msg_in, int red_code_len) {
		try {
			if (msg_in.size() + red_code_len < 256) {
				int msg_in_size = msg_in.size();
				std::vector<int> gen;
				gen.reserve(red_code_len);

				gen = rs_generator_poly(red_code_len);

				std::vector<int> msg_out;
				int msg_out_size = msg_in_size + red_code_len;
				msg_out.reserve(msg_out_size);

				for (int i = 0; i < msg_out_size; i++) { // initialize msg_out: len = msg_in.size() + red_code_len
					if (i < msg_in_size)
						msg_out.push_back(msg_in[i]);                      // the highest k characters contain the original message
					else
					{
						msg_out.push_back(0);
					}
				}

				std::vector<int> quotient, remainder;
				quotient.reserve(msg_out_size);
				remainder.reserve(msg_out_size);

				gf::gf_poly_div(msg_out, gen, quotient, remainder);        // divide our original polynomial by the generator polynomial and use the remainder
				reverse(remainder.begin(), remainder.end());

				for (int i = 0; i < msg_out_size; i++) { 
					if (i < msg_in_size)
						msg_out[i] = msg_in[i];                      
					else
					{
						msg_out[i] = remainder[msg_out_size - 1 - i];
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
				throw "\"The total number of characters - messages + redundant code - exceeds 256\"";
			}
		}
		catch (const char* ex) {
			std::cerr << std::endl << std::endl << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =";
			std::cerr << std::endl << "Error: " << ex << std::endl;
			std::cerr << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =" << std::endl;
		}
	}