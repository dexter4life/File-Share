#ifndef NETWORK_SHARING_H
#define NETWORK_SHARING_H

#include <NetCon.h>
#include <Windows.h>
#include <vector>
#include <tuple>
#include <map>
#include <string>

#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Oleaut32.lib")

using std::vector;
using std::string;

class NetworkSharing{
    // typedef for handling INetConnection Pointers
    typedef std::vector<INetConnection *> NetworkConnection;

    NetworkConnection ExistingNetwork;

	INetSharingManager * pNSM;
	HRESULT hr;
	INetConnection * pNC;
	INetSharingConfiguration * pSC;
	INetSharingEveryConnectionCollection *pNSECC;

public:
	NetworkSharing();
    ~NetworkSharing();
    void enableSharing(__in INetConnection *connection, __in SHARINGCONNECTIONTYPE sharingType = ICSSHARINGTYPE_PRIVATE);

    void disableSharing(__in INetConnection *connection);
    // get the existing connection list
    const NetworkConnection *getConnectionList() const { return &ExistingNetwork;}
    // return the connection for giving index
    INetConnection *connectionAt(int index) const;
};

#endif
