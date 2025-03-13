#pragma once
#include <string>
#include <filesystem>

class PathManager
{
public:
    static void initialize(const std::string& executablePath);
    static std::string getResourcePath(const std::string& relativePath);

private:
    static std::filesystem::path s_resourceDirectory;
    static std::filesystem::path findProjectRoot(const std::filesystem::path& startPath);
};