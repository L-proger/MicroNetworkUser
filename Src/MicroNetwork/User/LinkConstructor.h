#pragma once

#include <LFramework/COM/ComObject.h>
#include <MicroNetwork.Common.h>
#include <MicroNetwork.Host.h>

namespace MicroNetwork::User {

template<class TUserInterface, class TImplementer>
class LinkConstructor {
public:
    template<typename ... TArgs>
    static LFramework::ComPtr<TUserInterface> construct(LFramework::ComPtr<MicroNetwork::Host::INetwork> network, MicroNetwork::Host::NodeHandle node, TArgs&& ... args) {
        auto* taskContext = new TImplementer(std::move(args)...);
        auto contextUserInterface = LFramework::makeComDelegate<TUserInterface>(taskContext, &TImplementer::onUserRelease);
        auto contextNetworkInterface = LFramework::makeComDelegate<MicroNetwork::Common::IDataReceiver>(taskContext, &TImplementer::onNetworkRelease);
        auto networkInterface = network->startTask(node, TImplementer::LinkDescriptor::ID(), contextNetworkInterface);
        if (networkInterface == nullptr) { return {}; }
        taskContext->setDataReceiver(networkInterface);
        return contextUserInterface;
    }

    template<typename ... TArgs>
    static LFramework::ComPtr<TUserInterface> constructLink(LFramework::ComPtr<MicroNetwork::Host::INetwork> network, MicroNetwork::Host::NodeHandle node, TArgs&& ... args) {
        static_assert(LFramework::IsInterfaceSupported<TImplementer, typename TImplementer::LinkDescriptor::InInterface>(), "InInterface is not supported");

        auto* taskContext = new TImplementer(std::move(args)...);
        auto contextUserInterface = LFramework::makeComDelegate<TUserInterface>(taskContext, &TImplementer::onUserRelease);
        auto contextNetworkInterface = LFramework::makeComDelegate<typename TImplementer::LinkDescriptor::InInterface>(taskContext, &TImplementer::onNetworkRelease);

        auto contextNetworkInterfaceSerializer = LFramework::ComPtr<MicroNetwork::Common::IDataReceiver>
            ::create<typename TImplementer::LinkDescriptor::InInterfaceSerializer>(contextNetworkInterface);

        //

        auto i0 = contextNetworkInterfaceSerializer->addRef();
        auto i1 = contextNetworkInterfaceSerializer->release();
        auto networkInterface = network->startTask(node, TImplementer::LinkDescriptor::ID(), contextNetworkInterfaceSerializer);

        auto i2 = contextNetworkInterfaceSerializer->addRef();
        auto i3 = contextNetworkInterfaceSerializer->release();

        if (networkInterface == nullptr) { return {}; }
        taskContext->setDataReceiver(networkInterface);
        return contextUserInterface;
    }
};

}
