#include <iostream>
#include <signal.h>

#include "Sluice.h"

int currentSluice = 0;
Sluice normalSluice1 = Sluice(5555);
Sluice normalSluice2 = Sluice(5556);
Sluice fastLockSluice = Sluice(5557);
Sluice pulseMotorSluice = Sluice(5558);

void ctrlCHandler(int sig){
    switch(currentSluice)
    {
    	case 1:
    		normalSluice1.passInterrupt();
    		std::cout << "Stopping sluice 1 (standard)." << std::endl;
    		break;
    	case 2:
    		normalSluice2.passInterrupt();
    		std::cout << "Stopping sluice 2 (standard)." << std::endl;
    		break;
    	case 3:
    		fastLockSluice.passInterrupt();
    		std::cout << "Stopping sluice 3 (locking doors)." << std::endl;
    		break;
    	case 4:
    		pulseMotorSluice.passInterrupt();
    		std::cout << "Stopping sluice 4 (different motor)." << std::endl;
    		break;
    	default:
    		std::cout << "No sluice selected to stop." << std::endl;
    }
}

int main(int argc, char const *argv[])
{
	signal (SIGINT,&ctrlCHandler);

	int choice = ' ';
	
    int rtnval;
    char line[1];
    while (choice != 'q')
    {
    	std::cout << "\n==Menu==\n"
    	<< "[1] Manage sluice 1 (standard)\n"
    	<< "[2] Manage sluice 2 (standard)\n"
    	<< "[3] Manage sluice 3 (locking doors)\n"
    	<< "[4] Manage sluice 4 (different motor)\n"
    	<< "[q] Quit\n"
    	<< "Enter your choice: ";
    	std::cin >> line;
        choice = line[0];
        std::cout << std::endl;

        switch (choice)
        {
        	case '1':
        		currentSluice = 1;
                std::cout << "\n==Sluice 1 (standard)==\n";
        		while (choice != '9')
        		{
        			
	        		std::cout << "[1]	Allow entry from the left\n"
                              << "[2]   Move boat up/down\n"
	        		          << "[3]   Allow entry from the right\n"
	        		          << "[9]   Return to main menu\n"
	        		          << "Enter your choice: ";
			    	std::cin >> line;
			        choice = line[0];
			        std::cout << std::endl;
			        switch (choice)
			        {
			        	case '1':
			        		std::cout << "Allowing entry into sluice from the left side." << std::endl;
                            rtnval = normalSluice1.allowEntry(left);
			        		switch(rtnval)
			        		{
			        			case 0:
			        				break;
			        			//TODO: add all other possible return values
			        			default:
			        				std::cout << "Warning - sluice returned an unknown value: " << rtnval << std::endl;
			        				break;
			        		}
			        		break;
                        case '3':
                            std::cout << "Allowing entry into sluice from the right side." << std::endl;
                            rtnval = normalSluice1.allowEntry(right);
                            switch(rtnval)
                            {
                                case 0:
                                    break;
                                //TODO: add all other possible return values
                                default:
                                    std::cout << "Warning - sluice returned an unknown value: " << rtnval << std::endl;
                                    break;
                            }
                            break;
			        	//TODO: add other possible inputs
			        	default:
			        		std::cout << "Invalid input: " << choice << std::endl;
			        		break;
					}
        		}
        		break;
        	case '2':
        		currentSluice = 2;
        		std::cout << "\n==Sluice 2 (standard)==\n";
                while (choice != '9')
                {
                    
                    std::cout << "[1]   Allow entry from the left\n"
                              << "[2]   Move boat up/down\n"
                              << "[3]   Allow entry from the right\n"
                              << "[9]   Return to main menu\n"
                              << "Enter your choice: ";
                    std::cin >> line;
                    choice = line[0];
                    std::cout << std::endl;
                    switch (choice)
                    {
                        case '1':
                            std::cout << "Allowing entry into sluice from the left side." << std::endl;
                            rtnval = normalSluice2.allowEntry(left);
                            switch(rtnval)
                            {
                                case 0:
                                    break;
                                //TODO: add all other possible return values
                                default:
                                    std::cout << "Warning - sluice returned an unknown value: " << rtnval << std::endl;
                                    break;
                            }
                            break;
                        case '3':
                            std::cout << "Allowing entry into sluice from the right side." << std::endl;
                            rtnval = normalSluice2.allowEntry(right);
                            switch(rtnval)
                            {
                                case 0:
                                    break;
                                //TODO: add all other possible return values
                                default:
                                    std::cout << "Warning - sluice returned an unknown value: " << rtnval << std::endl;
                                    break;
                            }
                            break;
                        //TODO: add other possible inputs
                        default:
                            std::cout << "Invalid input: " << choice << std::endl;
                            break;
                    }
                }
                break;
        	case '3':
        		currentSluice = 3;
        		std::cout << "\n==Sluice 3 (locking doors)==\n";
                while (choice != '9')
                {
                    
                    std::cout << "[1]   Allow entry from the left\n"
                              << "[2]   Move boat up/down\n"
                              << "[3]   Allow entry from the right\n"
                              << "[9]   Return to main menu\n"
                              << "Enter your choice: ";
                    std::cin >> line;
                    choice = line[0];
                    std::cout << std::endl;
                    switch (choice)
                    {
                        case '1':
                            std::cout << "Allowing entry into sluice from the left side." << std::endl;
                            rtnval = fastLockSluice.allowEntry(left);
                            switch(rtnval)
                            {
                                case 0:
                                    break;
                                //TODO: add all other possible return values
                                default:
                                    std::cout << "Warning - sluice returned an unknown value: " << rtnval << std::endl;
                                    break;
                            }
                            break;
                        case '3':
                            std::cout << "Allowing entry into sluice from the right side." << std::endl;
                            rtnval = fastLockSluice.allowEntry(right);
                            switch(rtnval)
                            {
                                case 0:
                                    break;
                                //TODO: add all other possible return values
                                default:
                                    std::cout << "Warning - sluice returned an unknown value: " << rtnval << std::endl;
                                    break;
                            }
                            break;
                        //TODO: add other possible inputs
                        default:
                            std::cout << "Invalid input: " << choice << std::endl;
                            break;
                    }
                }
                break;
        	case '4':
        		currentSluice = 4;
        		std::cout << "\n==Sluice 4 (different motor)==\n";
                while (choice != '9')
                {
                    
                    std::cout << "[1]   Allow entry from the left\n"
                              << "[2]   Move boat up/down\n"
                              << "[3]   Allow entry from the right\n"
                              << "[9]   Return to main menu\n"
                              << "Enter your choice: ";
                    std::cin >> line;
                    choice = line[0];
                    std::cout << std::endl;
                    switch (choice)
                    {
                        case '1':
                            std::cout << "Allowing entry into sluice from the left side." << std::endl;
                            rtnval = pulseMotorSluice.allowEntry(left);
                            switch(rtnval)
                            {
                                case 0:
                                    break;
                                //TODO: add all other possible return values
                                default:
                                    std::cout << "Warning - sluice returned an unknown value: " << rtnval << std::endl;
                                    break;
                            }
                            break;
                        case '3':
                            std::cout << "Allowing entry into sluice from the right side." << std::endl;
                            rtnval = pulseMotorSluice.allowEntry(right);
                            switch(rtnval)
                            {
                                case 0:
                                    break;
                                //TODO: add all other possible return values
                                default:
                                    std::cout << "Warning - sluice returned an unknown value: " << rtnval << std::endl;
                                    break;
                            }
                            break;
                        //TODO: add other possible inputs
                        default:
                            std::cout << "Invalid input: " << choice << std::endl;
                            break;
                    }
                }
                break;
        	default:
        		std::cout << "Invalid input." << std::endl;
        		break;
        }
    }
	return 0;
}