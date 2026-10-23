#include <iostream> //everything should be working, need to link github and finish makefile
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
using namespace std;

void splitNumber(const string& s, string& intPart, string& fracPart) {
    size_t dot = s.find('.');
    if (dot == string::npos) {
        intPart = s;
        fracPart = "";
    } else {
        intPart = s.substr(0, dot);
        fracPart = s.substr(dot + 1);
    }
}

string stripLeadingZeros(const string& s) {
    size_t i = s.find_first_not_of('0');
    if (i == string::npos) return "0";   // string was all zeros
    return s.substr(i);
}

void padToMatch(string& intA, string& fracA, string& intB, string& fracB) {
    while (intA.length() < intB.length()) intA = "0" + intA;
    while (intB.length() < intA.length()) intB = "0" + intB;
    while (fracA.length() < fracB.length()) fracA += "0";
    while (fracB.length() < fracA.length()) fracB += "0";
}

string cleanup(const string& s) {
    string intPart, fracPart;
    splitNumber(s, intPart, fracPart);
    intPart = stripLeadingZeros(intPart);
    while (!fracPart.empty() && fracPart.back() == '0') fracPart.pop_back();
    if (fracPart.empty()) return intPart;
    return intPart + "." + fracPart;
}

int compareMagnitudes(const string& a, const string& b) {
    string intA, fracA, intB, fracB;
    splitNumber(a, intA, fracA);
    splitNumber(b, intB, fracB);
    padToMatch(intA, fracA, intB, fracB);
    string ka = intA + fracA;
    string kb = intB + fracB;
    if (ka == kb) return 0;
    if (ka > kb)  return 1;
    return -1;
}

string addMagnitudes(const string& a, const string& b) {
    string intA, fracA, intB, fracB;
    splitNumber(a, intA, fracA);
    splitNumber(b, intB, fracB);
    padToMatch(intA, fracA, intB, fracB);

    string numA = intA + fracA;   
    string numB = intB + fracB;

    string result = "";
    int carry = 0;
    for (int i = (int)numA.length() - 1; i >= 0; i--) {
        int sum = (numA[i] - '0') + (numB[i] - '0') + carry;
        result = char('0' + (sum % 10)) + result;   
        carry = sum / 10;
    }
    if (carry > 0) result = char('0' + carry) + result;

    if (!fracA.empty()) {
        int pos = (int)result.length() - (int)fracA.length();
        result = result.substr(0, pos) + "." + result.substr(pos);
    }
    return cleanup(result);
}

string subtractMagnitudes(const string& a, const string& b) {
    string intA, fracA, intB, fracB;
    splitNumber(a, intA, fracA);
    splitNumber(b, intB, fracB);
    padToMatch(intA, fracA, intB, fracB);

    string numA = intA + fracA;
    string numB = intB + fracB;

    string result = "";
    int borrow = 0;
    for (int i = (int)numA.length() - 1; i >= 0; i--) {
        int d = (numA[i] - '0') - (numB[i] - '0') - borrow;
        if (d < 0) { d += 10; borrow = 1; }
        else       { borrow = 0; }
        result = char('0' + d) + result;
    }

    if (!fracA.empty()) {
        int pos = (int)result.length() - (int)fracA.length();
        result = result.substr(0, pos) + "." + result.substr(pos);
    }
    return cleanup(result);
}

string addStrings(const string& a, const string& b) {
    bool negA = (a[0] == '-');
    bool negB = (b[0] == '-');
    string magA = (a[0] == '+' || a[0] == '-') ? a.substr(1) : a;
    string magB = (b[0] == '+' || b[0] == '-') ? b.substr(1) : b;

    if (negA == negB) {
        string sum = addMagnitudes(magA, magB);
        if (negA && sum != "0") return "-" + sum;
        return sum;
    }

    int cmp = compareMagnitudes(magA, magB);
    if (cmp == 0) return "0";
    if (cmp > 0) {
        string diff = subtractMagnitudes(magA, magB);
        return negA ? "-" + diff : diff;
    }
    string diff = subtractMagnitudes(magB, magA);
    return negB ? "-" + diff : diff;
}


bool isValid(const string& s) {
    int i = 0;
    int len = s.length();

    if (i < len && (s[i] == '+' || s[i] == '-')) i++;

    int digits = 0;
    while (i < len && isdigit(s[i])) { i++; digits++; }
    if (digits == 0) return false;

    if (i < len && s[i] == '.') {
        i++;
        digits = 0;
        while (i < len && isdigit(s[i])) { i++; digits++; }
        if (digits == 0) return false;
    }

    return i == len;
}


void fileLoop() {
    string input;
    cout << "Enter the name of the file: ";
    cin >> input;

    ifstream readFile(input);
    if (!readFile.is_open()) {
        cerr << "Could not open file: " << input << "\n";
        return;
    }

    string line;
    while (getline(readFile, line)) {
        istringstream iss(line);
        string a, b, extra;

        if (!(iss >> a >> b) || (iss >> extra)) {
            cerr << "Error: expected exactly two values on line: " << line << "\n";
            continue;
        }
        
        if (!isValid(a) || !isValid(b)) {
            cerr << "Error: invalid number on line: " << line << "\n";
            continue;
        }
        cout << addStrings(a, b) << "\n";
    }
}

int main() {
    fileLoop();
    return 0;
}