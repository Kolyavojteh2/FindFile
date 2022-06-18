#include <iostream>
#include "findmanager.h"
#include <filesystem>

using namespace std;

int main()
{
    FindManager mgr;
    
    std::string str;
    
    cout << "Enter a filename: ";
    cin >> str;
    cout << mgr.findFile(str) << endl;

    return 0;
}
