#include <iostream>
#include <signal.h>

#include "Sluice.h"

void ctrlCHandler(int sig){
    std::cout << "\nPICNIC!"
    		  << "\n\nAlso fuck sluice"
    		  << std::endl;
}

int main(int argc, char const *argv[])
{
	signal (SIGINT,&ctrlCHandler);

	Sluice normalSluice1 = Sluice(5555);
	// Sluice normalSluice2 = Sluice(5556);
	// Sluice fastLockSluice = Sluice(5557);
	// Sluice pulseMotorSluice = Sluice(5558);

	int choice = ' ';
    char line[80];
    while (choice != '0')
    {
    	std::cout << "\n==Menu==\n"
    	<< "TESTING - [q]	allowEntry Sluice1 (left)\n"
    	<< "TESTING - [a]	allowEntry Sluice1 (right)\n"
    	// << "TESTING - [w]	allowEntry Sluice2 (left)\n"
    	// << "TESTING - [s]	allowEntry Sluice2 (right)\n"
    	// << "TESTING - [e]	allowEntry lockSluice (left)\n"
    	// << "TESTING - [d]	allowEntry lockSluice (right)\n"
    	// << "TESTING - [r]	allowEntry shittyMotorSluice (left)\n"
    	// << "TESTING - [f]	allowEntry shittyMotorSluice (right)\n"
    	<< "Enter your choice: ";
    	std::cin >> line;
        choice = line[0];
        std::cout << std::endl;

        switch (choice)
        {
        	case 'q':
        		std::cout << "Function returned " << normalSluice1.allowEntry(left) << std::endl;
        		break;
        	case 'a':
        		std::cout << "Function returned " << normalSluice1.allowEntry(right) << std::endl;
        		break;
        	// case 'w':
        	// 	std::cout << "Function returned " << normalSluice2.allowEntry(left) << std::endl;
        	// 	break;
        	// case 's':
        	// 	std::cout << "Function returned " << normalSluice2.allowEntry(right) << std::endl;
        	// 	break;
        	// case 'e':
        	// 	std::cout << "Function returned " << fastLockSluice.allowEntry(left) << std::endl;
        	// 	break;
        	// case 'd':
        	// 	std::cout << "Function returned " << fastLockSluice.allowEntry(right) << std::endl;
        	// 	break;
        	// case 'r':
        	// 	std::cout << "Function returned " << pulseMotorSluice.allowEntry(left) << std::endl;
        	// 	break;
        	// case 'f':
        	// 	std::cout << "Function returned " << pulseMotorSluice.allowEntry(right) << std::endl;
        	// 	break;
        }
    }
	return 0;
}