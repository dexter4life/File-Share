
#ifndef WLAN_INTERFACE
#define WLAN_INTERFACE

#include <Windows.h>
#include <wlanihv.h>
#include <wlanapi.h>
#include <wlanihvtypes.h>
#include <wlantypes.h>
#include <string>
#include <objbase.h>
#include <exception>
#include <NetCon.h>

//Ip info
#include <Iphlpapi.h>
#include <Iptypes.h>

#ifndef _MAX_LENGTH
#define _MAX_LENGTH _MAX_PATH
#endif

#pragma comment(lib, "Wlanapi.lib")

char *ssidToString(UCHAR ssid[], size_t len);

const char *_catstr(const char *source, char *errormsg);

#define NetFreeMemory(n)\
	WlanFreeMemory(n)

LPSTR NetWlanHostedNetworkReasonCodeToString(PWLAN_HOSTED_NETWORK_REASON pReasonCode);

const char *interfaceStateToString(WLAN_INTERFACE_STATE isState);

const char *MsmNoficationToString(DWORD dwState);

const char *peerStateNotification(DWORD dwState);

const char *OneXTypeNotification(DWORD dwState);

const char *AcmNoficationToString(DWORD dwState);

char *wcharToString(const wchar_t *wstr, size_t len);

TCHAR *WlanErrorCodeToString(DWORD dwMessageID);

#define WTOSTR(n) wcharToString(n, wcslen(n))

HANDLE NetOpenWlanHandle();

void NetSetWlanHostedNetworkEnable(__in HANDLE hClientHandle, __in PWLAN_HOSTED_NETWORK_REASON pFailReason, __in BOOL bEnable);

void NetSetWlanHostedNetworkSettings(HANDLE hClientHandle, std::string ssid,
                                     DWORD dwMaxClient, PWLAN_HOSTED_NETWORK_REASON pFailReason);

void NetCloseWlanHandle(HANDLE hClientHandle);

void NetStartHostedNetwork(HANDLE hClientHandle, PWLAN_HOSTED_NETWORK_REASON pFailReason);

BOOL NetQueryWlanShowDeniedNetworkStatus(HANDLE hClientHandle, PWLAN_OPCODE_VALUE_TYPE pValueType);

void NetQueryWlanPowerSettings(HANDLE hClientHandle, PWLAN_POWER_SETTING pPowerSetting,
                               PWLAN_OPCODE_VALUE_TYPE pValueType);

void NetSetShowDeniedNetwork(HANDLE hClientHandle, BOOL pData);

void NetSetWlanAllowExplicitCreds(HANDLE hClientHandle, BOOL pData);

void NetSetWlanAutoConfigBlockPeriod(HANDLE hClientHandle, DWORD pData);
// Set Wlan Virtual Station Extensibility
void NetSetWlanAllowVirtualStationExtensibility(HANDLE hClientHandle, BOOL pData);
//Wlan Scan
void NetScanWifi(HANDLE hClientHandle, const GUID *pInterfaceGuid, const std::string ssid, PWLAN_RAW_DATA pRawData);

void NetConnectToWlan(HANDLE hClientHandle, const GUID *interfaceGuid,
                      PWLAN_CONNECTION_PARAMETERS pConnectionParameter);

void NetDeleteWlanProfile(HANDLE hClientHandle, const GUID *interfaceGuid, LPCWSTR strProfileName);

void NetDisconnectWlan(HANDLE hClientHandle, const GUID *interfaceGuid);

void NetQueryWlanInterface(HANDLE hClientHandle, const GUID *pInterfaceGuid, WLAN_INTF_OPCODE pCode,
    PDWORD pDataSize, PVOID *pData, PWLAN_OPCODE_VALUE_TYPE pValueType);

void NetGetWlanAvailableNetworkList(HANDLE hClientHandle, const GUID *pInterfaceGuid, DWORD dwFlag,
    PWLAN_AVAILABLE_NETWORK_LIST *ppAvailableNetworkList);

void NetEnumWlanInterfaces(HANDLE hClientHandle, PWLAN_INTERFACE_INFO_LIST *ppInterfaceList);

#define NetStringFromGUID2(guid, buffer, sz) StringFromGUID2(guid, (LPOLESTR)buffer, sz)


void NetQueryWlanConnectionSettings(HANDLE hClientHandle, PWLAN_HOSTED_NETWORK_CONNECTION_SETTINGS *connectionSettings,
    PDWORD dwConSize, PWLAN_OPCODE_VALUE_TYPE valueType);

const char *notificationString(DWORD dwCode, DWORD dwSource);

LPWSTR NetGetWlanProfile(HANDLE hClientHandle, const GUID *interfaceGuid, LPWSTR strProfileName, DWORD *pdwFlags,PDWORD pdwGrantedAccess );

void NetGetWlanProfileList(HANDLE hClientHandle, const GUID *pInterfaceGuid, PWLAN_PROFILE_INFO_LIST *pProfileInfoList);

void NetQueryWlanSecondaryKey(HANDLE hClientHandle, PDWORD pdwKeyLength, PUCHAR *ppucKeyData,
    PBOOL pbIsPassPhrase, PBOOL pbPersistent, PWLAN_HOSTED_NETWORK_REASON pFailReason);

void NetInitWlanSettings(HANDLE hClientHandle, PWLAN_HOSTED_NETWORK_REASON pFailReason);

void NetSaveTemporaryProfile(__in HANDLE hClientHandle, __in const GUID *pInterfaceGuid,
                             __in LPCWSTR strProfileName,__in LPCWSTR strAllUserProfileSecurity, __in DWORD dwFlags, __in BOOL bOverWrite);

void NetRenameProfile(HANDLE hClientHandle, const GUID *pInterfaceGuid,
                      LPCWSTR strOldProfileName, LPCWSTR strNewProfileName);

void NetWlanHostedNetworkSetSecondaryKey(HANDLE hClientHandle, std::string key, BOOL passphrase, BOOL persistent,
    PWLAN_HOSTED_NETWORK_REASON pFailReason);
#endif
