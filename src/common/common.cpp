#include "common.h"
#include <algorithm>
#include <sstream>
#include <thread>
#include <limits>

bool isPalindrome(const std::string& str) {
    std::string reversed_str = str;
    std::reverse(reversed_str.begin(), reversed_str.end());
    return str == reversed_str;
}

bool hasDigit(const std::string& str) {
    return std::any_of(str.begin(), str.end(), ::isdigit);
}

int sumOfThreeNumbers(const std::string& str) {
    int a, b, c;
    sscanf_s(str.c_str(), "%d %d %d", &a, &b, &c);
    return a + b + c;
}

std::vector<int> parseNumbers(const std::string& str) {
    std::vector<int> numbers;
    std::stringstream ss(str);
    int number;
    while (ss >> number) {
        numbers.push_back(number);
    }
    return numbers;
}

void findMin(const std::vector<int>& numbers, int& min_value) {
    min_value = *std::min_element(numbers.begin(), numbers.end());
}

void findMax(const std::vector<int>& numbers, int& max_value) {
    max_value = *std::max_element(numbers.begin(), numbers.end());
}

std::pair<int, int> findMinMax(const std::vector<int>& numbers) {
    int min_value = std::numeric_limits<int>::max();
    int max_value = std::numeric_limits<int>::min();

    std::thread t1(findMin, std::cref(numbers), std::ref(min_value));
    std::thread t2(findMax, std::cref(numbers), std::ref(max_value));

    t1.join();
    t2.join();

    return {min_value, max_value};
}
