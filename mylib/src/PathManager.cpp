#include "PathManager.h"
#include <iostream>

std::filesystem::path PathManager::s_resourceDirectory;

std::filesystem::path PathManager::findProjectRoot(const std::filesystem::path& startPath)
{
    auto currentPath = startPath;

    while (currentPath.has_parent_path() && !std::filesystem::exists(currentPath / "resources"))
    {
        currentPath = currentPath.parent_path();
    }

    if (std::filesystem::exists(currentPath / "resources"))
        return currentPath;

    throw std::runtime_error("Could not find resources directory in parent directories");
}

void PathManager::initialize(const std::string& executablePath)
{
    std::filesystem::path execPath(executablePath);
    auto projectRoot = findProjectRoot(execPath.parent_path());
    s_resourceDirectory = projectRoot / "resources";
}

std::string PathManager::getResourcePath(const std::string& relativePath)
{
    std::string fullPath = (PathManager::s_resourceDirectory / relativePath).string();
    std::cout << "Loading resource: " << fullPath << std::endl; // Debug
    return fullPath;
}
