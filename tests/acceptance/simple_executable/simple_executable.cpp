/*
 * simple_executable.cpp
 *
 * Writes "output" to the stdout and "error" to stderr
 *
 * See LICENSE.md for Copyright information.
 */

#include <iostream>

int main ()
{
    std::cout << "output" << std::endl;
    std::cerr << "error" << std::endl;
    return 0;
}
