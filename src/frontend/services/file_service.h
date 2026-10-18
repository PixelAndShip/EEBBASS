/*
File helpers for the frontend: listing the save-file and log folders and
opening a file in the system's text editor. No drawing and no raylib in here.
*/

#pragma once
#include <string>
#include <vector>
#include <memory>
#include <atomic>
#include <cstdint>
#include <ctime>

struct FileEntry
{
    std::string name;
    std::string path;
    std::uintmax_t size = 0;
    std::time_t modified = 0;
};

namespace FileService
{
    inline constexpr const char *SaveDirectory = "environments";
    inline constexpr const char *LogDirectory = "logs";

    // regular files of a directory, newest first (empty if it does not exist)
    std::vector<FileEntry> list(const std::string &directory);

    // "12.3 KB - modified 2026-10-18 08:01"
    std::string describe(const FileEntry &file);
    std::string formatSize(std::uintmax_t bytes);

    // Finds a save file from what the user typed: the name as given, or inside
    // environments/, with or without ".txt". Returns "" when nothing matches.
    std::string resolveSaveFile(const std::string &typed);
}

// Opens a file with the system text editor on its own thread, so the
// frontend keeps running. running() turns false when the editor process ends.
//
// Editor used: the EEBBASS_EDITOR environment variable if set, otherwise
// notepad (Windows), open -W -t (macOS) or xdg-open (Linux).
class EditorLauncher
{
public:
    void open(const std::string &path);
    bool running() const;
    const std::string &file() const { return current; }
    // why the editor could not be started ("" if it started fine)
    std::string error() const;

private:
    struct Shared;
    std::shared_ptr<Shared> shared;
    std::string current;
};
