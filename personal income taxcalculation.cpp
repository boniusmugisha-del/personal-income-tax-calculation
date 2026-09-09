/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>

int main() {
    double gross = 12000000.0;
    double nssf = gross * 0.05; // 5% exempt
    double taxable = gross - nssf;
    double tax = 0.0;

    // Ugandan PAYE Monthly Brackets
    if (taxable > 235000)  tax += (std::min(taxable, 335000.0) - 235000) * 0.10;
    if (taxable > 335000)  tax += (std::min(taxable, 410000.0) - 335000) * 0.20;
    if (taxable > 410000)  tax += (std::min(taxable, 10000000.0) - 410000) * 0.30;
    if (taxable > 10000000) tax += (taxable - 10000000.0) * 0.40;

    std::cout << "Gross Income:   " << gross << "\n";
    std::cout << "NSSF Deduction: " << nssf << "\n";
    std::cout << "Taxable Income: " << taxable << "\n";
    std::cout << "PAYE Tax Due:   " << tax << "\n";
    std::cout << "Net Take-Home:  " << gross - tax << "\n";

    return 0;
}