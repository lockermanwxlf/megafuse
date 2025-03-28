#include "requestlistener.h"
#include <iostream>

bool megafuse::RequestListener::wait()
{
    std::unique_lock lock{mutex};
    request_finished = false;
    cv.wait(lock, [&] { return request_finished; });
    return request_success;
}

void megafuse::RequestListener::onRequestFinish(mega::MegaApi *api,
                                                mega::MegaRequest *request,
                                                mega::MegaError *e)
{
    std::lock_guard lock{mutex};
    if (e && e->getErrorCode() != e->API_OK) {
        request_success = false;
        std::cerr << "ERROR: " << e->getErrorString() << " (" << e->getErrorCode() << ")"
                  << std::endl;
    } else {
        request_success = true;
    }
    request_finished = true;
    cv.notify_one();
}
