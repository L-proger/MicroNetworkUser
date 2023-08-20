#pragma once

#include <LFramework/COM/ComObject.h>
#include <MicroNetwork.Common.h>
#include <MicroNetwork.User.h>

namespace MicroNetwork::User {



class AbstractLink : public LFramework::ComImplement<AbstractLink, LFramework::ComObject, ILink>    {
public:
    virtual void setDataReceiver(LFramework::ComPtr<Common::IDataReceiver> receiver) = 0;
    virtual void onUserRelease() = 0;
    virtual void onNetworkRelease() {
        _isConnected = false;
    }
    bool isConnected() {
        return _isConnected;
    }
private:
    bool _isConnected = true;
};

}
