#include <iostream>
#include <sstream>
#include <vector>
#include <boost/algorithm/string.hpp>
using namespace boost::algorithm;

using namespace std;

// class Employee
// {
//     int id;
//     string name;

// public:
//     void getdata();
//     void putdata();
// };

// void Employee::getdata()
// {
//     cout << "Enter Id : ";
//     cin >> id;
//     cout << "Enter Name : ";
//     cin >> name;
// }

// void Employee::putdata()
// {
//     cout << id << " ";
//     cout << name << " ";
//     cout << endl;
// }

int main()
{
    // string line = "GeeksForGeeks is a must try";
    string line = "";
    getline(cin, line);

    // Vector of string to save tokens
    vector<string> tokens;

    // stringstream class check1
    stringstream check1(line);

    string intermediate;

    // Tokenizing w.r.t. space ' '
    while (getline(check1, intermediate, '|'))
    {
        trim(intermediate);
        tokens.push_back(intermediate);
    }

    // Printing the token vector
    for (int i = 0; i < tokens.size(); i++)
    {
        cout << tokens[i] << '\n';
    }
}

// string s = "ahmed";
// printf("\"%s\" \t", s);
// vector<Employee> employees;
// int n, i;

// employees.clear();
// while (true)
// {
//     cout << "Enter Number of Employees - " << endl;
//     cin >> n;
//     for (int k = 0; k < n; k++)
//     {
//         Employee emp;
//         employees.push_back(emp);
//     }

//     for (i = 0; i < n; i++)
//         employees[i].getdata();

//     cout << "Employee Data - " << endl;

//     for (i = 0; i < n; i++)
//         employees[i].putdata();
// }
