/* Author: Yulia Kuznetsova (YuliaKUA)
 * Date: 01.01.2021
 *
 * See LICENSE */

#include <iostream>
#include "help_gf.hpp"
#include "gf.h"
#include "rs_encoding.h"
#include "rs_decoding.h"


int main() {

    //std::cout << gf_mult(61, 64, 0);
    //std::cout << gf_mult(61, 64, 255);

    //find_prime_polys(2, 8);

   /* std::vector<int> correct_primes = find_prime_polys();
    for (int i = 0; i < correct_primes.size(); i++) {
        std::cout << correct_primes[i] << " ";
    }*/
  
     char message[] = "hello world";
     const int msglen = sizeof(message) - 1;

     int k = msglen;
     int N = k + 10;

     std::cout << "Message:   ";
     gf::print(message, msglen);

     std::vector<int> original, encoded, erroneous, decoded;
     std::cout << "Original:  ";
     for (int i = 0; i < msglen; i++) {
         original.push_back(message[i]);
         std::cout << original[i] << " ";
     }
     encoded = rs_encode_msg(original, k);

     std::cout << std::endl << "Encoded:   ";
     for (int i = 0; i < encoded.size(); i++) {
         std::cout << encoded[i] << " ";
     }

     std::vector<char> encoded_str;
     for (int i = 0; i < encoded.size(); i++) {
         encoded_str.push_back(encoded[i]);
        // std::cout << encoded_str[i];
         erroneous.push_back(encoded[i]);
     }

     std::cout << std::endl << "Erroneous: ";
     srand(time(0));
     for (int i = 0; i < encoded.size(); i++) {
         if (i < (N - k) / 2) {
             int k = rand() % (msglen);
             erroneous[k] = '?';    // == 32
             //erroneous[i] = ' ';    // == 32
         }
         //erroneous[0] = 0;
         std::cout << erroneous[i] << " ";
     }
     
     decoded = rs_decode_msg(erroneous, k, {});
     std::cout << std::endl << "Decoded:   ";

     for (int i = 0; i < decoded.size(); i++) {
         std::cout << decoded[i] << " ";
     }

     std::cout << std::endl << "Message:   ";
     gf::print(decoded, msglen);
     std::cout << std::endl;

    system("pause");
    return 0;
}
