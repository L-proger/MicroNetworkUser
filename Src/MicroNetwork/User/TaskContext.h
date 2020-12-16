#pragma once

#include <LFramework/COM/ComObject.h>
#include <MicroNetwork/Common/IDataReceiver.h>

namespace MicroNetwork::User {

class TaskContext : public LFramework::RefCountedObject {
public:
    virtual void setDataReceiver(LFramework::ComPtr<Common::IDataReceiver> receiver) = 0;
    virtual void onUserRelease() = 0;
    virtual void onNetworkRelease() {
        _isConnected = false;
    }
    LFramework::Result isConnected(bool& result) {
        result = _isConnected;
        return LFramework::Result::Ok;
    }
private:
    bool _isConnected = true;
};

}
