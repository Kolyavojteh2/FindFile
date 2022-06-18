#ifndef FINDMANAGER_H
#define FINDMANAGER_H

#include <queue>
#include <vector>
#include <string>
#include <iostream>

#include <thread>
#include <filesystem>
#include <mutex>
#include <list>
#include <exception>
#include <system_error>

class FindManager
{
private:
    std::vector<std::thread*> _threads;

    std::list<std::string> _paths;
    std::string _resultPath;
    std::mutex mtx_result;
    std::mutex mtx_paths;

    std::string targetName;

public:
    FindManager();

    std::string findFile(const std::string name);

};


#endif // FINDMANAGER_H
