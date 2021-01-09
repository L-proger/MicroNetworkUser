#pragma once

#include <LFramework/COM/ComObject.h>
#include <MicroNetwork.Common.h>
#include <MicroNetwork/Host/INetwork.h>

namespace MicroNetwork::User {

template<class TUserInterface, class TImplementer>
class TaskContextConstructor {
public:
    template<typename ... TArgs>
    static LFramework::ComPtr<TUserInterface> construct(LFramework::ComPtr<MicroNetwork::Host::INetwork> network, MicroNetwork::Host::NodeHandle node, TArgs&& ... args) {
        auto* taskContext = new TImplementer(std::move(args)...);
        auto contextUserInterface = LFramework::makeComDelegate<TUserInterface>(taskContext, &TImplementer::onUserRelease);
        auto contextNetworkInterface = LFramework::makeComDelegate<MicroNetwork::Common::IDataReceiver>(taskContext, &TImplementer::onNetworkRelease);
        auto networkInterface = network->startTask(node, TImplementer::ID, contextNetworkInterface);
        if (networkInterface == nullptr) { return {}; }
        taskContext->setDataReceiver(networkInterface);
        return contextUserInterface;
    }
};

}
