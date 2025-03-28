#include "megaservice.h"
#include <filesystem>
#include <glib-2.0/glib.h>
#include <iostream>
#include <megaapi.h>
#include <termios.h>
#include <unistd.h>

std::string ensure_cache_dir()
{
    std::string user_cache_dir{g_get_user_cache_dir()};
    std::string app_cache_dir{user_cache_dir + "/megafuse"};
    std::filesystem::create_directory(app_cache_dir);
    return app_cache_dir;
}

int main(int argc, char *argv[])
{
    /* Provide usage message. */
    if (argc != 2) {
        std::cout << "Usage: megafuse <path>\n\
Megafuse will mount your root MEGA folder to the local <path>. \
This mount will not be unmounted upon termination."
                  << std::endl;
        exit(1);
    }

    /* Validate local path */
    std::string local_path{argv[1]};
    if (!std::filesystem::exists(local_path)) {
        std::cerr << "<path> does not exist." << std::endl;
        exit(1);
    }

    /* Create MegaService. */
    std::string app_cache_dir{ensure_cache_dir()};
    megafuse::MegaService service{app_cache_dir};
    service.login();

    /* Mount filesystem. */
    service.add_mount(local_path);

    std::cout << "Successfully mounted." << std::endl;

    /* Run forever. */
    std::mutex mtx;
    std::condition_variable cv;
    std::unique_lock lock{mtx};
    cv.wait(lock, [] { return false; });
}
