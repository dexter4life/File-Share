/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include "NetworkSharing.h"
#include <cstring>

#include <map>
using std::map;

#include <QDebug>

#include "platform.h" // for FileShare purpose

// as in winsock.h
#define NAT_PROTOCOL_TCP 6

NetworkSharing::NetworkSharing(){
	//initialize com ( common object module )
	CoInitialize(NULL);

	// init security to enum RAS connections
	CoInitializeSecurity(NULL, -1, NULL, NULL,
		RPC_C_AUTHN_LEVEL_PKT,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL, EOAC_NONE, NULL);

	
	hr = ::CoCreateInstance(__uuidof(NetSharingManager),
		NULL,
		CLSCTX_ALL,
		__uuidof(INetSharingManager),
		(void**)&pNSM);

	if (!pNSM)
		throw std::runtime_error("failed to create NetSharingManager object\r\n");

		HRESULT hr = pNSM->get_EnumEveryConnection(&pNSECC);
		if (!pNSECC){
			throw std::runtime_error("Failure: can't get pointer to INetSharingEveryConnectionCollection");
		}
		IUnknown *iUk = NULL;
		IEnumVARIANT *iEV = NULL;

		hr = pNSECC->get__NewEnum(&iUk);
		if (!iUk){
			throw std::runtime_error("Failure: can't get pointer to iUnknown");
		}

		hr = iUk->QueryInterface(__uuidof(IEnumVARIANT), (void**)&iEV);
		iUk->Release();

		if (!iEV){
			throw std::runtime_error("Failure: can't get pointer to IEnumVariant");
		}
		VARIANT v;
		VariantInit(&v);
		while (S_OK == iEV->Next(1, &v, NULL)){
			if (V_VT(&v) == VT_UNKNOWN){
				V_UNKNOWN(&v)->QueryInterface(__uuidof(INetConnection), (void**)&pNC);
				if (pNC){
                    // get the property for a particular connection
                    NETCON_PROPERTIES *ppProps = 0;
                    pNC->GetProperties(&ppProps);

                    //save existing connections
                    ExistingNetwork.push_back(pNC);
				}
			}
		}
}

NetworkSharing::~NetworkSharing() {
    // free the memory for the Net sharing
    pNSECC->Release();

    // free the Net Sharing Manager
    pNSM->Release();

    //release the memory when the program is terminated
    for(size_t i = 0; i < ExistingNetwork.size(); i++ ){
        ExistingNetwork.at(i)->Release();
    }

    CoUninitialize();
}

INetConnection *NetworkSharing::connectionAt(int index) const
{
    if( index < 0)
        return NULL;

    return ExistingNetwork.at(index);
}
//default to public sharing type
void NetworkSharing::enableSharing(__in INetConnection *connection, __in SHARINGCONNECTIONTYPE sharingType){
     pNSM->get_INetSharingConfigurationForINetConnection(connection, &pSC);
     VARIANT_BOOL pbEnabled = 0;
     pSC->get_SharingEnabled(&pbEnabled);

     //if sharing is already enabled then
    if( !pbEnabled ){
        pSC->EnableSharing(sharingType);
    }

//    pSC->EnableInternetFirewall();
//    //include the port mapping or port forwarding for the connection to work
    INetSharingPortMapping *pSPM = NULL;

    //verify if sharing is supported in the system
    VARIANT_BOOL pbInstalled = 0;
    pNSM->get_SharingInstalled(&pbInstalled);


}

void NetworkSharing::disableSharing(__in INetConnection *connection){
    this->pNSM->get_INetSharingConfigurationForINetConnection(connection, &pSC);
    pSC->DisableSharing();
}

