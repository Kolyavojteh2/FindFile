#include "findmanager.h"

FindManager::FindManager()
{
    int maxThreads = std::min(std::thread::hardware_concurrency(), static_cast<unsigned int>(8));
    if (maxThreads == 0)
        maxThreads = 1;
    _threads.resize(maxThreads, nullptr);
}


std::string FindManager::findFile(const std::string name)
{
    _paths.push_back("/");
    targetName = name;

    auto func = [&]()
    {
        while (1)
        {
            mtx_result.lock();
            if (!_resultPath.empty())
            {
                mtx_result.unlock();
                return;
            }
            mtx_result.unlock();

            mtx_paths.lock();

            if (_paths.empty())
            {
                mtx_paths.unlock();
                continue;
            }

            std::filesystem::path currentPath = _paths.front();
            _paths.pop_front();
            mtx_paths.unlock();

            try
            {

                for (auto &it : std::filesystem::directory_iterator(currentPath))
                {
                    if (it.path().filename() == "." || it.path().filename() == ".." || it.path().filename() == "")
                        continue;

                    if (it.is_regular_file() && it.path().filename().string() == targetName)
                    {
                        mtx_result.lock();
                        if (!_resultPath.empty())
                        {
                            mtx_result.unlock();
                            return;
                        }

                        _resultPath = it.path().string();
                        mtx_result.unlock();

                        mtx_paths.lock();
                        _paths.clear();
                        mtx_paths.unlock();

                        return;
                    }

                    if (it.is_directory())
                    {
                        mtx_result.lock();
                        if (!_resultPath.empty())
                        {
                            mtx_result.unlock();
                            return;
                        }
                        mtx_result.unlock();

                        mtx_paths.lock();
                        _paths.push_back(it.path());
                        mtx_paths.unlock();
                    }

                }

            }
            catch (std::exception &ex) {}
            catch (...) {}
        }
    };

    while (1)
    {
        try {
            for (unsigned int i = 0; i < _threads.size(); i++)
            {
                _threads[i] = new std::thread(func);
            }


            for (unsigned int i = 0; i < _threads.size(); i++)
            {
                _threads[i]->join();
            }

        }
        catch (std::exception &ex)
        {
            std::cout << ex.what() << std::endl;
        }

        if (!_resultPath.empty())
            break;

    }


    return _resultPath;
}
