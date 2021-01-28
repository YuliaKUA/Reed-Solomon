#include <iostream>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <algorithm>

#define GENERATOR 2 //replace alpha in the classical theory

//auxiliary output functions
void print(std::vector<int> vec) {
	//std::cout << std::endl;
	for (int i = 0; i < vec.size(); i++) {
		std::cout << vec[i] << " ";
	}
	std::cout << std::endl;
}

void print(int x) {
	std::cout << x << std::endl;
}

/* @brief Function calculates the polynomial of error syndromes
	* @param msg - an incoming message that is represented by a vector of polynomials(in this case, integers)
	* @param red_code_len - the number of characters representing the redundant code
	* @return generator polynomial */
std::vector<int> rs_calc_syndromes(std::vector<int> msg, int red_code_len) {
	std::vector<int> synd;
	
	for (int i = 0; i < red_code_len + 1; i++) {     //initialization
		synd.push_back(0);
	}

	for (int i = 1; i < red_code_len + 1; i++) {     // We need the first element 0 for mathematical accuracy, otherwise errors occur
		int temp = gf::pow_my(GENERATOR, i - 1);
		synd[i] = gf::gf_poly_eval(msg, temp);       //S[i+1] = C(a^(i)) 
	}

	
	return synd;
}

/* @brief Function find the error locator polynomial L(x) according to the known location of the error (for correct errarta)
	* @param err_pos - vector with the positions of the erroneous characters
	* @return error locator polynomial L(x) */
std::vector<int> rs_find_errarta_locator(std::vector<int> err_pos) {
	
	std::vector<int> e_loc, temp, temp2, add;
	e_loc.push_back(1);

	temp.push_back(1);

	temp2.push_back(0);
	temp2.push_back(0);

	for (int i = 0; i < err_pos.size(); i++) {
		temp2[0] = gf::pow_my(GENERATOR, err_pos[i]);         // since we know location of the error, we can find L(x) as
		add = gf::gf_poly_add(temp, temp2);                   // L(x) = Ï(1 + x*alpha^(i))
		e_loc = gf::gf_poly_mult(e_loc, add);
	}

	return e_loc;
}


/* @brief Function find the error polynomial W(x) = S(x)*L(x)
	* @param synd - polynomial of error syndromes (vector int) S(x)
	* @param err_loc - error locator polynomial L(x)
	* @param err_loc_size - size of L(x)
	* @return error polynomial W(x) */
std::vector<int> rs_find_error_evaluator(std::vector<int> synd, std::vector<int> err_loc, int err_loc_size) {
	std::vector<int> poly_mul, help, quotient, remainder;
	//print(synd);
	//print(err_loc);
	poly_mul = gf::gf_poly_mult(synd, err_loc);
	
	help.push_back(1);
	for (int i = 0; i < err_loc_size; i++) {
		help.push_back(0);
	}

	// Since W(x) cannot exceed u-1, where u is the number of errors, we will use division to discard the extra part
	gf::gf_poly_div(poly_mul, help, quotient, remainder);

	return remainder;
}


/* @brief Correction of the input message using the Forney algorithm, which calculates the error value
	* @param msg_in - incoming message that is represented by a vector of polynomials(in this case, integers)
	* @param synd - polynomial of error syndromes (vector int) S(x)
	* @param err_pos - vector with the positions of the erroneous characters
	* @return corrected input message */
std::vector<int> rs_correct_errata(std::vector<int> msg_in, std::vector<int> synd, std::vector<int> err_pos, int fcr = 0) {
	std::vector<int> coef_pos, err_loc, err_eval, x;
	int len = msg_in.size();
	for (int i = 0; i < err_pos.size(); i++) {                         // convert err positions in the coefficients of degree
		coef_pos.push_back(len - 1 - err_pos[i]);
	}
	print(coef_pos);

	// find the error locator polynomial L(x) according to the known location of the error
	err_loc = rs_find_errarta_locator(coef_pos);
	
	//find the error polynomial W(x)
	reverse(synd.begin(), synd.end());
	err_eval = rs_find_error_evaluator(synd, err_loc, err_loc.size());
	reverse(err_eval.begin(), err_eval.end());

	//x - will storage the position of the errors
	//we need get the error location polynomial X from the error positions in err_pos 
	//(the roots of the error locator polynomial, ie, where it evaluates to 0)
	for (int i = 0; i < coef_pos.size(); i++) {
		int l = 255 - coef_pos[i];
		x.push_back(gf::pow_my(GENERATOR, -l));
	}

	//using the Forney algorithm, we find the error magnitudes
	std::vector<int> E, err_loc_prime_tmp;
	for (int i = 0; i < msg_in.size(); i++) {
		E.push_back(0);                                 //will store the values that need to be corrected to the original message with errors
	}


	int Xlength = x.size();
	reverse(err_eval.begin(), err_eval.end());

	for (int i = 0; i < x.size(); i++) {
		int Xi_inv = gf::inverse(x[i]);

		//Find the formal derivative of the error locator polynomial
		//the formal derivative of the errata locator is used as the denominator of the Forney Algorithm, 
		//which simply says that the ith error value is given by error_evaluator(gf_inverse(Xi)) / error_locator_derivative(gf_inverse(Xi)).
		for (int j = 0; j < Xlength; j++) {
			if (j != i) {
				err_loc_prime_tmp.push_back(gf::sub(1, gf::mult(Xi_inv, x[j])));
			}
		}

		//error polynomial Yi = W(Xi^(-1))/L'(Xi^(-1))
		//calculate the product that is the denominator of the Forney algorithm(the derivative of the error locator)
		int err_loc_prime = 1;

		for (int coef = 0; coef < err_loc_prime_tmp.size(); coef++) {
			err_loc_prime = gf::mult(err_loc_prime, err_loc_prime_tmp[coef]);
		}
		
		err_loc_prime_tmp.clear();

		int y;                                              
		y = gf::gf_poly_eval(err_eval, Xi_inv);                          // numerator
		y = gf::mult(gf::pow_my(x[i], 1 - fcr), y);

		try {
			if (err_loc_prime == 0) {                                     // divisor should not be 0
				throw "Could not find error magnitude";
			}
		}
		catch (std::string ex) {
			std::cout << ex << std::endl;
		}

		int magnitude = gf::div_my(y, err_loc_prime);                      //The value of the error value calculated by the Forney algorithm
		                                                                   //Dividing the error estimator by the derivative of the error locator 
		E[err_pos[i]] = magnitude;                                         //gives us the error value, that is, the value for recovering the symbol
	}

	//correct our erroneous polynomial
	msg_in = gf::gf_poly_add(msg_in, E);                                   // C(x) = C'(x) + E(x) (xor)
	return msg_in;
}


/* @brief Function find the error locator polynomial L(x) using the Berlekamp-Messi algorithm
	* @param synd - polynomial of error syndromes (vector int) S(x)
	* @param red_code_len - the number of characters representing the redundant code
	* @param erase_count - count of known errors
	* @return error locator polynomial L(x)*/
std::vector<int> rs_find_error_locator(std::vector<int> synd, int red_code_len, int erase_count = 0) {
	std::vector<int> err_loc, old_loc;
	err_loc.push_back(1);                               //error locator polynomial (sigma) C(x)
	old_loc.push_back(1);                               //the error locator polynomial of the previous iteration
	
	
	int synd_shift = synd.size() - red_code_len;

	//The Berlekamp–Massey algorithm is an alternative to the Reed–Solomon Peterson decoder for solving the set of linear equations.
	//The main idea is that the algorithm iteratively evaluates the error locator polynomial.To do this, it calculates the delta discrepancy, 
	//by which we can determine whether we need to update the error locator or not
	int k = 0, delta = 0;
	for (int i = 0; i < red_code_len - erase_count; i++) {
		k = i + synd_shift;
		
		//calculating the delta of the discrepancy
		delta = synd[k];
				
		for (int j = 1; j < err_loc.size() ; j++) {
			delta ^= gf::mult(err_loc[err_loc.size() - 1 - j], synd[k - j]);    // delta = Sn + C1*Sn-1 +..+ Cj*Sk-j
		}

		//shift the polynomials to calculate the next degree
		old_loc.push_back(0);
		
		std::vector<int> new_loc;
		if(delta != 0){                                                         //if delta == 0, algorithm assumed that C(x) and L are correct for the moment and continues
			if (old_loc.size() > err_loc.size()) {                              //~2*L <= k + erase_count
				//Computing errata locator polynomial Sigma
				new_loc = gf::gf_poly_scale(old_loc, delta);
				old_loc = gf::gf_poly_scale(err_loc, gf::inverse(delta));
				err_loc = new_loc;
			}

			//Update with the discrepancy
			err_loc = gf::gf_poly_add(err_loc, gf::gf_poly_scale(old_loc, delta));

		}
	}

	while (err_loc.size() && err_loc[0] == 0) {
		err_loc.erase(err_loc.begin());
	}


	int errs = err_loc.size() - 1 - erase_count;
	try {
		if (errs*2 + erase_count > red_code_len) {
			throw "Too many errors to correct";
		}
	}
	catch (std::string ex) {
		std::cout << ex << std::endl;
	}

	return err_loc;
}


/* @brief Function find the zeros of this polynomial
	* @param err_loc - error locator polynomial L(x)
	* @param nmess - message size
	* @return  vector of the index of the characters that need to be corrected */
std::vector<int> rs_find_errors(std::vector<int> err_loc, int nmess) {
	// eraseusing the error locator polynomial, we simply use trial substitution to find the zeros of this polynomial, 
	//which identifies the error locations(ie, the index of the characters that need to be corrected)
	std::vector<int> err_pos;
	int errs = err_loc.size() - 1;
	for (int i = 0; i < nmess; i++) {
		if (gf::gf_poly_eval(err_loc, gf::pow_my(GENERATOR, i)) == 0) {   // if 0 then it is a root of the error locator polynomial
			err_pos.push_back(nmess - 1 - i);
		}
	}

	try {
		if (err_pos.size() != errs) {
			throw "Too many (or few) errors found for the errata locator polynomial!";
		}
	}
	catch (std::string ex) {
		std::cout << ex << std::endl;
	}
	
	return err_pos;
}


/* @brief Function of decoding message
	* @param msg_in - encoded message vector
	* @param red_code_len - the number of characters representing the redundant code
	* @param erase_pos - known errors positions
	* @return  decoded message*/
std::vector<int> rs_decode_msg(std::vector<int> msg_in, int red_code_len, std::vector<int> erase_pos) {
	try {
		if (msg_in.size() > 255) {
			throw "Message is too long";
		}
	}
	catch (std::string ex) {
		std::cout << ex << std::endl;
	}

	std::vector<int> msg_out = msg_in;
	if (erase_pos.size() != 0) {
		for (int e_pos = 0; e_pos < erase_pos.size(); e_pos++) {
			msg_out[e_pos] = 0;
		}
	}

	try {
		if (erase_pos.size() > red_code_len) {
			throw "Too many erasures to correct";
		}
	}
	catch (std::string ex) {
		std::cout << ex << std::endl;
	}

	//so that we do not count the generator polynomial several times and do not divide, 
	//we immediately count the error syndrome polynomial, and if there is not at least 
	//one non-0 value in it, then the message is not distorted
	std::vector<int> synd = rs_calc_syndromes(msg_out, red_code_len);
	int max = *max_element(synd.begin(), synd.end());

	if (max == 0) {
		return msg_out;
	}


	std::vector<int> err_pos, err_loc;

		//Find the error locator polynomial L(x)
		err_loc = rs_find_error_locator(synd, red_code_len, erase_pos.size());
		
		reverse(err_loc.begin(), err_loc.end());

		//find the vector of the index of the characters that need to be corrected
		err_pos = rs_find_errors(err_loc, msg_out.size());
		
		try {
			if (err_pos.size() == 0) {
				throw "Could not locate error";
			}
		}
		catch (std::string ex) {
			std::cout << ex << std::endl;
		}
	

	std::vector<int> help_err = erase_pos;
	for (int i = 0; i < err_pos.size(); i++) {
		help_err.push_back(err_pos[i]);
	}
	
	//Find errors values and apply them to correct the message
	//compute errata evaluatorand errata magnitude polynomials, then correct errorsand erasures
	msg_out = rs_correct_errata(msg_out, synd, help_err, 0);
	
	//count the error syndrome polynomial, and if there is not at least 
	//one non-0 value in it, then the message is decoded successful
	synd = rs_calc_syndromes(msg_out, red_code_len);
	max = *max_element(synd.begin(), synd.end());
	try {
		if (max > 0) {
			throw "Could not correct message";
		}
	}
	catch (std::string ex) {
		std::cout << ex << std::endl;
	}

	return msg_out;
}
