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
#include <fstream>

const uint32_t RCVBUFSIZE = 32;    // Size of receive buffer

int main(int argc, char *argv[]) {

	checkArgs* argumentos = new checkArgs(argc, argv);
	
    std::string servAddress; 
	uint16_t    echoServPort;
    std::string echoString;		//http request
	uint32_t echoStringLen;   // Determine input length
	std::string nomArchivo;
	
	servAddress   = argumentos->getArgs().SERVER;
	echoServPort  = argumentos->getArgs().PORT;
	nomArchivo = argumentos->getArgs().ARCHIVO;

	echoString = "GET / HTTP/1.1\r\nHost: " + servAddress + "\r\nConnection: close\r\n\r\n";	//http request con coneccion close.
	echoStringLen = echoString.length();	//largo de ambos strings
	
	delete argumentos;

	try {
		// Establish connection with the echo server
		TCPSocket sock(servAddress, echoServPort);
		// Instancia y creación del archivo
		std::ofstream archivo;
		archivo.open (nomArchivo);

		// Send the string to the echo server
		sock.send(echoString.c_str(), echoStringLen);	//Se envía el request de http

		char echoBuffer[RCVBUFSIZE + 1];    // Buffer for echo string + \0
		uint32_t bytesReceived = 0;              // Bytes read on each recv()
		uint32_t totalBytesReceived = 0;         // Total bytes read

		// Receive the same string back from the server
		std::cout << "Iniciando carga del contenido web..." << std::endl;               // Setup to print the echoed string
		while ((bytesReceived = (sock.recv(echoBuffer, RCVBUFSIZE))) != 0) {
			
			if (bytesReceived < 0) {
				std::cerr << "No se puede leer." << std::endl;
				archivo.close();	//cerrar archivo antes de salir
				return(EXIT_FAILURE);
			}

			totalBytesReceived += bytesReceived;     // Keep tally of total bytes
			echoBuffer[bytesReceived] = '\0';        // Terminate the string!
			archivo << echoBuffer;	//Printear texto en el archivo
		}
		
		archivo.close();		//cerrar archivo antes de salir
		std::cout << "Se guardó el contenido en el archivo: " << nomArchivo << "\n" << std::endl;
		return(EXIT_SUCCESS);
		// Destructor closes the socket

	} catch(SocketException &e) {
		std::cerr << e.what() << std::endl;
		return(EXIT_FAILURE);
	}
}
