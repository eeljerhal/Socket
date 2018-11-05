/*
 *   C++ sockets on Unix and Windows
 *   Copyright (C) 2002
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "YASL.h"      // For Socket and SocketException
#include "checkArgs.h"
#include <iostream>    // For cerr and cout
#include <cstdlib>     // For atoi()

const uint32_t RCVBUFSIZE = 32;    // Size of receive buffer

int main(int argc, char *argv[]) {

	checkArgs* argumentos = new checkArgs(argc, argv);
	
    std::string servAddress; 
	uint16_t    echoServPort;
    std::string echoString [2];		//http request
	uint32_t echoStringLen [2];   // Determine input length
	
	servAddress   = argumentos->getArgs().SERVER;
	echoServPort  = argumentos->getArgs().PORT;
	echoString[0] = "GET / HTTP/1.1\r\n";	//http request 
	echoString[1] = "Host: " + servAddress + "\r\n\r\n";	//host
	echoStringLen[0] = echoString[0].length();	//largo de ambos strings
	echoStringLen[1] = echoString[1].length();
	
	delete argumentos;

	try {
		// Establish connection with the echo server
		TCPSocket sock(servAddress, echoServPort);

		// Send the string to the echo server
		sock.send(echoString[0].c_str(), echoStringLen[0]);	//Se envía el request de http
		sock.send(echoString[1].c_str(), echoStringLen[1]);	//y el host

		char echoBuffer[RCVBUFSIZE + 1];    // Buffer for echo string + \0
		uint32_t bytesReceived = 0;              // Bytes read on each recv()
		uint32_t totalBytesReceived = 0;         // Total bytes read

		// Receive the same string back from the server
		std::cout << "Received: ";               // Setup to print the echoed string
		while (true) {
			// Receive up to the buffer size bytes from the sender
			if ((bytesReceived = (sock.recv(echoBuffer, RCVBUFSIZE))) <= 0) {
				std::cerr << "Unable to read";
				exit(EXIT_FAILURE);
			}
			totalBytesReceived += bytesReceived;     // Keep tally of total bytes
			echoBuffer[bytesReceived] = '\0';        // Terminate the string!
			std::cout << echoBuffer;                      // Print the echo buffer
		}
		std::cout << std::endl;

		// Destructor closes the socket

	} catch(SocketException &e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
