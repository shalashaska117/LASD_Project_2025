#include "zlasdtest/test.hpp"  // official tests
#include "zmytest/test.hpp"    // my tests

#include <iostream>
#include <string>

int main() {
  std::cout << "=== LASD Libraries 2025 ===\n" << std::endl;

  bool keepGoing = true;

  while (keepGoing) {
    std::cout << "Choose the test to run:\n";
    std::cout << "1. Official tests (zlasdtest)\n";
    std::cout << "2. Custom tests - Part 1 [Vector, List, Set, SetVec, SetLst]\n";
    std::cout << "3. Custom tests - Part 2 [Heap, HeapVec, PQ, PQHeap]\n";
    std::cout << "4. Custom tests - All\n";
    std::cout << "0. Exit\n";
    std::cout << ">> ";

    int choice;
    std::cin >> choice;

    switch (choice) {
      case 1:
        std::cout << "\n== Starting official tests ==\n" << std::endl;
        lasdtest();
        break;

      case 2:
        std::cout << "\n== Starting custom tests - Part 1 ==\n" << std::endl;
        lasd::RunAllTests1();
        break;

      case 3:
        std::cout << "\n== Starting custom tests - Part 2 ==\n" << std::endl;
        lasd::RunAllTests2();
        break;

      case 4:
        std::cout << "\n== Starting all custom tests ==\n" << std::endl;
        lasd::RunAllTests();
        break;

      case 0:
        std::cout << "Exiting the program." << std::endl;
        keepGoing = false;
        break;

      default:
        std::cout << "Invalid choice. Try again." << std::endl;
        break;
    }

    if (choice >= 1 && choice <= 4) {
      std::cout << "\nDo you want to run another test? (y/n): ";
      std::string answer;
      std::cin >> answer;
      if (answer != "y" && answer != "Y") {
        keepGoing = false;
        std::cout << "Closing the program." << std::endl;
      }
    }
  }
  

  return 0;
}
