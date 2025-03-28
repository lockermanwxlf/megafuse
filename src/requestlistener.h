#ifndef REQUESTLISTENER_H
#define REQUESTLISTENER_H

#include <condition_variable>
#include <megaapi.h>

namespace megafuse {
class RequestListener : public mega::MegaRequestListener
{
    std::mutex mutex;
    std::condition_variable cv;
    bool request_finished;
    bool request_success;

public:
    RequestListener()
        : mutex{}
        , cv{}
        , request_finished{false}
        , request_success{false}
    {}

    bool wait();

    // MegaRequestListener interface
public:
    void onRequestFinish(mega::MegaApi *api,
                         mega::MegaRequest *request,
                         mega::MegaError *e) override;
};
} // namespace megafuse

#endif // REQUESTLISTENER_H
