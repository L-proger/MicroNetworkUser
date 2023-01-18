#pragma once

#include "AbstractLink.h"

namespace MicroNetwork::User {

template<typename TLinkDescriptor>
class Link : public AbstractLink {
public:
    using LinkDescriptor = TLinkDescriptor;

    void setDataReceiver(LFramework::ComPtr<Common::IDataReceiver> receiver) override final{
        auto serializer = LFramework::ComPtr<typename LinkDescriptor::OutInterface>::template create<typename LinkDescriptor::OutInterfaceSerializer>(receiver);
        setOutputStream(serializer);
    }

    virtual void setOutputStream(LFramework::ComPtr<typename LinkDescriptor::OutInterface> outStream) = 0;
};

}
