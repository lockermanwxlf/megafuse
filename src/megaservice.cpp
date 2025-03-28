#include "megaservice.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <termios.h>
#include <unistd.h>

megafuse::MegaService::MegaService(std::string cache_dir)
    : cache_dir{cache_dir}
    , api{"megafuse", cache_dir.c_str()}
    , listener{}
{
    api.addRequestListener(&listener);
}

void megafuse::MegaService::login()
{
    std::string session_key_path{cache_dir + "/session.txt"};

    /* Use session key if it's stored locally. */
    if (std::filesystem::exists(session_key_path)) {
        /* Get session key. */
        std::ifstream fin{session_key_path};
        std::string session_key;
        fin >> session_key;

        /* Login with session key. */
        api.fastLogin(session_key.c_str());
        listener.wait();
    }
    /* Otherwise, ask for email and password. */
    else {
        /* Disable terminal echo. */
        termios old_term, new_term;
        std::string email, password;
        tcgetattr(STDIN_FILENO, &old_term);
        new_term = old_term;
        new_term.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

        /* Prompt for email and password. */
        std::cout << "Enter email: ";
        std::cin >> email;
        std::cout << "\nEnter password: ";
        std::cin >> password;

        /* Revert terminal echo. */
        tcsetattr(STDIN_FILENO, TCSANOW, &old_term);

        /* Login with email and password. */
        std::cout << "\nLogging in with email and password." << std::endl;
        api.login(email.c_str(), password.c_str());
        listener.wait();

        /* Save session key. */
        std::cout << "Saving session key." << std::endl;
        std::ofstream fout{session_key_path};
        std::string session_key{api.dumpSession()};
        fout << session_key;
    }

    /* Fetch nodes. */
    std::cout << "Fetching nodes." << std::endl;
    api.fetchNodes();
    listener.wait();
}

void megafuse::MegaService::add_mount(std::string mountpoint)
{
    /* Get root node. */
    mega::MegaNode *root_node{api.getRootNode()};

    /* Create mount flags. */
    mega::MegaMountFlags *flags{mega::MegaMountFlags::create()};
    flags->setName("megafuse");
    flags->setPersistent(false);

    /* Create mount. */
    mega::MegaMount *mount{mega::MegaMount::create()};
    mount->setPath(mountpoint.c_str());
    mount->setHandle(root_node->getHandle());
    mount->setFlags(flags);

    /* Add mount. */
    api.addMount(mount, &listener);
    listener.wait();

    /* Enable mount. */
    api.enableMount("megafuse", &listener, false);
    listener.wait();
}
