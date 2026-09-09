#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>

struct TaxBracket {
    double lowerBound;
    double upperBound; // Use std::numeric_limits<double>::infinity() for top tier
    double rate;       // Decimal format (e.g., 0.10 for 10%)
};

struct BracketBreakdown {
    std::string label;
    double taxableInBracket;
    double taxDue;
};

struct TaxResult {
    double grossIncome;
    double allowances;
    double taxableIncome;
    double totalTax;
    double netIncome;
    double effectiveRate;
    std::vector<BracketBreakdown> breakdown;
};

class IncomeTaxCalculator {
private:
    std::vector<TaxBracket> brackets;

public:
    IncomeTaxCalculator(const std::vector<TaxBracket>& taxBrackets) 
        : brackets(taxBrackets) {}

    TaxResult calculate(double grossIncome, double allowances = 0.0) const {
        double taxableIncome = std::max(0.0, grossIncome - allowances);
        double totalTax = 0.0;
        std::vector<BracketBreakdown> breakdownList;

        for (size_t i = 0; i < brackets.size(); ++i) {
            const auto& bracket = brackets[i];

            if (taxableIncome > bracket.lowerBound) {
                double upper = std::min(taxableIncome, bracket.upperBound);
                double taxableInBracket = upper - bracket.lowerBound;
                double taxDue = taxableInBracket * bracket.rate;

                totalTax += taxDue;

                std::string label = "Tier " + std::to_string(i + 1) + " (" + 
                                    std::to_string(static_cast<int>(bracket.rate * 100)) + "%)";

                breakdownList.push_back({label, taxableInBracket, taxDue});
            } else {
                break;
            }
        }

        double netIncome = grossIncome - totalTax;
        double effectiveRate = (grossIncome > 0.0) ? (totalTax / grossIncome) * 100.0 : 0.0;

        return {
            grossIncome,
            allowances,
            taxableIncome,
            totalTax,
            netIncome,
            effectiveRate,
            breakdownList
        };
    }
};

int main() {
    // Define standard monthly PAYE tax brackets (Example values in local currency)
    std::vector<TaxBracket> payeBrackets = {
        {0.0, 235000.0, 0.00},
        {235000.0, 335000.0, 0.10},
        {335000.0, 410000.0, 0.20},
        {410000.0, 10000000.0, 0.30},
        {10000000.0, std::numeric_limits<double>::infinity(), 0.40}
    };

    IncomeTaxCalculator taxCalc(payeBrackets);

    // Input values
    double grossSalary = 12000000.0;
    double nssfDeduction = grossSalary * 0.05; // 5% exempt pension contribution

    // Compute Tax
    TaxResult result = taxCalc.calculate(grossSalary, nssfDeduction);

    // Format console output
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "===========================================\n";
    std::cout << "        PAYE TAX CALCULATION REPORT        \n";
    std::cout << "===========================================\n";
    std::cout << "Gross Monthly Income : " << result.grossIncome << "\n";
    std::cout << "Exemptions / NSSF    : " << result.allowances << "\n";
    std::cout << "Taxable Income       : " << result.taxableIncome << "\n";
    std::cout << "-------------------------------------------\n";
    std::cout << "Total PAYE Tax Due   : " << result.totalTax << "\n";
    std::cout << "Net Take-Home Pay    : " << result.netIncome << "\n";
    std::cout << "Effective Tax Rate   : " << result.effectiveRate << "%\n";
    std::cout << "===========================================\n\n";

    std::cout << "--- TAX BRACKET BREAKDOWN ---\n";
    for (const auto& item : result.breakdown) {
        std::cout << std::left << std::setw(15) << item.label 
                  << " | Taxable: " << std::right << std::setw(12) << item.taxableInBracket 
                  << " | Tax: " << std::right << std::setw(10) << item.taxDue << "\n";
    }

    return 0;
}
