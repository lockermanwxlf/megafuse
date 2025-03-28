#ifndef MEGASERVICE_H
#define MEGASERVICE_H

#include "requestlistener.h"

#include <megaapi.h>
#include <string>

namespace megafuse {
class MegaService
{
    mega::MegaApi api;
    std::string cache_dir;
    megafuse::RequestListener listener;

public:
    MegaService(std::string cache_dir);

    void login();

    void add_mount(std::string mountpoint);
};
} // namespace megafuse
#endif // MEGASERVICE_H
