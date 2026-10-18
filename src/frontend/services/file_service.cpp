#include "file_service.h"
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <mutex>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <process.h>
#else
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
extern char **environ;
#endif

namespace fs = std::filesystem;

namespace FileService
{
    std::vector<FileEntry> list(const std::string &directory)
    {
        std::vector<FileEntry> files;
        std::error_code ec;

        for (const fs::directory_entry &entry : fs::directory_iterator(directory, ec))
        {
            std::error_code entryError;
            if (!entry.is_regular_file(entryError))
            {
                continue;
            }

            FileEntry file;
            file.name = entry.path().filename().string();
            file.path = entry.path().string();
            file.size = entry.file_size(entryError);

            fs::file_time_type ftime = entry.last_write_time(entryError);
            if (!entryError)
            {
                auto asSystem = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                    ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
                file.modified = std::chrono::system_clock::to_time_t(asSystem);
            }
            files.push_back(file);
        }

        std::sort(files.begin(), files.end(), [](const FileEntry &a, const FileEntry &b)
                  { return a.modified != b.modified ? a.modified > b.modified : a.name < b.name; });
        return files;
    }

    std::string formatSize(std::uintmax_t bytes)
    {
        char buffer[32];
        if (bytes < 1024)
        {
            std::snprintf(buffer, sizeof buffer, "%ju B", bytes);
        }
        else if (bytes < 1024 * 1024)
        {
            std::snprintf(buffer, sizeof buffer, "%.1f KB", bytes / 1024.0);
        }
        else
        {
            std::snprintf(buffer, sizeof buffer, "%.1f MB", bytes / (1024.0 * 1024.0));
        }
        return buffer;
    }

    std::string describe(const FileEntry &file)
    {
        char when[32] = "unknown";
        std::tm *local = std::localtime(&file.modified);
        if (local)
        {
            std::strftime(when, sizeof when, "%Y-%m-%d %H:%M", local);
        }
        return "Size: " + formatSize(file.size) + "\nModified: " + when;
    }

    std::string resolveSaveFile(const std::string &typed)
    {
        size_t first = typed.find_first_not_of(" \t");
        size_t last = typed.find_last_not_of(" \t");
        if (first == std::string::npos)
        {
            return "";
        }
        std::string name = typed.substr(first, last - first + 1);

        const std::string candidates[] = {
            name,
            std::string(SaveDirectory) + "/" + name,
            name + ".txt",
            std::string(SaveDirectory) + "/" + name + ".txt"};

        for (const std::string &candidate : candidates)
        {
            std::error_code ec;
            if (fs::is_regular_file(candidate, ec))
            {
                return candidate;
            }
        }
        return "";
    }
}

// ---------------------------------------------------------------- editor

namespace
{
    // Runs the editor and waits for it to end. Returns an error text or "".
    std::string runEditorAndWait(const std::string &path)
    {
        const char *custom = std::getenv("EEBBASS_EDITOR");
#ifdef _WIN32
        std::string editor = (custom and *custom) ? custom : "notepad.exe";
        std::string quoted = "\"" + path + "\"";
        int result = _spawnlp(_P_WAIT, editor.c_str(), editor.c_str(), quoted.c_str(), (char *)nullptr);
        if (result == -1)
        {
            return "Could not start " + editor;
        }
        return "";
#else
#ifdef __APPLE__
        std::string editor = (custom and *custom) ? custom : "open";
        std::vector<std::string> args = {editor};
        if (!(custom and *custom))
        {
            args.push_back("-W");
            args.push_back("-t");
        }
#else
        std::string editor = (custom and *custom) ? custom : "xdg-open";
        std::vector<std::string> args = {editor};
#endif
        args.push_back(path);

        std::vector<char *> argv;
        for (std::string &arg : args)
        {
            argv.push_back(arg.data());
        }
        argv.push_back(nullptr);

        pid_t pid = 0;
        int rc = posix_spawnp(&pid, editor.c_str(), nullptr, nullptr, argv.data(), environ);
        if (rc != 0)
        {
            return "Could not start " + editor;
        }
        int status = 0;
        waitpid(pid, &status, 0);
        return "";
#endif
    }
}

struct EditorLauncher::Shared
{
    std::atomic<bool> running{true};
    mutable std::mutex mutex;
    std::string error;
};

bool EditorLauncher::running() const
{
    return shared and shared->running;
}

std::string EditorLauncher::error() const
{
    if (!shared)
    {
        return "";
    }
    std::lock_guard<std::mutex> lock(shared->mutex);
    return shared->error;
}

void EditorLauncher::open(const std::string &path)
{
    if (running())
    {
        return;
    }

    current = path;
    shared = std::make_shared<Shared>();

    // The thread owns a copy of the shared state, so it stays valid even if
    // the program is closed while the editor is still open.
    std::thread([state = shared, path]()
                {
                    std::string problem = runEditorAndWait(path);
                    {
                        std::lock_guard<std::mutex> lock(state->mutex);
                        state->error = problem;
                    }
                    state->running = false; })
        .detach();
}
