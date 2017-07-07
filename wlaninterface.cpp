/*********************************************************************************************
 *  This software is designed and maintained by Peter Nwanosike, and is not for sale or any  *
 *  public distribution. This software is release as an opensource project, under the GPL    *
 *  License. Thus, you are free to modify this code or make changes as require but reference *
 *  must be made to the owner, Peter. E-Mail: dexter4life@gmail.com                          *
 * ******************************************************************************************/

#include "WlanInterface.h"
#include <string>

using std::string;

#pragma comment(lib, "Wlanapi.lib")
#define BUFFER_SIZE 100

char *ssidToString(UCHAR ssid[], size_t len)
{
    char *buffer = (char*)malloc(len + 1);
    memcpy(buffer, ssid, len + 1);
    return buffer;
}

const char *_catstr(const char *source, char *errormsg){
    int len = strlen(source) + strlen(errormsg) + 1;
    char *des = (char*)malloc(len);
    strcpy_s(des, len, source);
    strcat_s(des, len, errormsg);
    return des;
}

void msg(string message )
{
    MessageBoxA(NULL, message.c_str(), "Error: ", MB_OK);
}

#define NetFreeMemory(n)\
    WlanFreeMemory(n)

LPSTR NetWlanHostedNetworkReasonCodeToString(PWLAN_HOSTED_NETWORK_REASON pReasonCode)
{
    switch (*pReasonCode)
    {
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_ap_start_failed:
        return LPSTR("The driver for the wireless Hosted Network failed to start.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_bad_parameters:
        return LPSTR("Bad parameter(s).");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_client_abort:
        return LPSTR("A state change occurred that was caused by client abort.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_crypt_error:
        return LPSTR("A cryptographic error occurred.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_device_change:
        return LPSTR("The wireless adapter used by the wireless Hosted Network changed.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_elevation_required:
        return LPSTR("This operation requires elevation.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_gp_denied:
        return LPSTR("The operation was denied by group policy.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_impersonation:
        return LPSTR("User impersonation failed.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_incompatible_connection_started:
        return LPSTR("An incompatible connection started.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_incompatible_connection_stopped:
        return LPSTR("An incompatible connection stop.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_insufficient_resources:
        return LPSTR("Service is out of resources.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_interface_available:
        return LPSTR("A wireless interface has become available");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_interface_unavailable:
        return LPSTR("A wireless interface has become unavailable.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_miniport_started:
        return LPSTR("The wireless miniport driver status changed.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_miniport_stopped:
        return LPSTR("The wireless miniport driver stopped the Hosted Network.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_peer_arrived:
        return LPSTR("A peer connected to the wireless Hosted Network.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_peer_departed:
        return LPSTR("A peer disconnected from the wireless Hosted Network.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_peer_timeout:
        return LPSTR("A peer timed out.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_persistence_failed:
        return LPSTR("Data persistence failed.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_properties_change:
        return LPSTR("The properties of the wireless Hosted Network changed.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_read_only:
        return LPSTR("An attempt was made to write read-only data.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_service_available_on_virtual_station:
        return LPSTR("An identical service is available on a virtual station.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_service_shutting_down:
        return LPSTR("Service is shutting down.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_service_unavailable:
        return LPSTR("The Wireless LAN service is not running.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_stop_before_start:
        return LPSTR("An incorrect function call sequence was made.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_success:
        return LPSTR("The operation was successful.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_unspecified:
        return LPSTR("Unknown error.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_user_action:
        return LPSTR("A state change occurred that was caused by explicit user action.");
    case WLAN_HOSTED_NETWORK_REASON::wlan_hosted_network_reason_virtual_station_blocking_use:
        return LPSTR("A virtual station is active and blocking operation.");
    default:
        return LPSTR("Unknown error.");
    }
}

const char *interfaceStateToString(WLAN_INTERFACE_STATE isState)
{
    switch (isState)
    {
    case wlan_interface_state_not_ready:
        return "Not ready";
    case wlan_interface_state_connected:
        return "Connected";
    case wlan_interface_state_ad_hoc_network_formed:
        return "No peer has connected.";

    case wlan_interface_state_disconnecting:
        return "Disconnecting...";

    case wlan_interface_state_disconnected:
        return "Not connected.";

    case wlan_interface_state_associating:
        return "Associating with a network.";

    case wlan_interface_state_discovering:
        return "Discovering network settings";
    case wlan_interface_state_authenticating:
        return "Authenticating.";
    }
    return "";
}

const char *MsmNoficationToString(DWORD dwState)
{
    switch (dwState)
    {
    case wlan_notification_msm_associating:
        return "wlan_notification_msm_associating";
    case wlan_notification_msm_associated:
        return "wlan_notification_msm_associating";
    case wlan_notification_msm_authenticating:
        return "wlan_notification_msm_authenticating";
    case wlan_notification_msm_connected:
        return "wlan_notification_msm_connected";
    case wlan_notification_msm_roaming_start:
        return "wlan_notification_msm_roaming_start";
    case wlan_notification_msm_roaming_end:
        return "wlan_notification_msm_roaming_end";
    case wlan_notification_msm_radio_state_change:
        return "wlan_notification_msm_radio_state_change";
    case wlan_notification_msm_signal_quality_change:
        return "wlan_notification_msm_signal_quality_change";
    case wlan_notification_msm_disassociating:
        return "wlan_notification_msm_disassociating";
    case wlan_notification_msm_disconnected:
        return "wlan_notification_msm_disconnected";
    case wlan_notification_msm_peer_join:
        return "wlan_notification_msm_peer_join";
    case wlan_notification_msm_peer_leave:
        return "wlan_notification_msm_peer_leave";
    case wlan_notification_msm_adapter_removal:
        return "wlan_notification_msm_adapter_removal";
    case wlan_notification_msm_adapter_operation_mode_change:
        return "wlan_notification_msm_adapter_operation_mode_change";
    }
    return "None";
}
const char *peerStateNotification(DWORD dwState)
{
    switch (dwState)
    {
    case wlan_hosted_network_peer_state_change:
        return "wlan_hosted_network_peer_state_change";
    case wlan_hosted_network_radio_state_change:
        return "wlan_hosted_network_radio_state_change";
    case wlan_hosted_network_state_change:
        return "wlan_hosted_network_state_change";
    }
    return "None";
}
const char *OneXTypeNotification(DWORD dwState)
{
    switch (dwState)
    {
    case OneXNotificationTypeResultUpdate:
        return "OneXNotificationTypeResultUpdate";
    case OneXNotificationTypeAuthRestarted:
        return "OneXNotificationTypeAuthRestarted";
    }
    return "None";
}
const char *AcmNoficationToString(DWORD dwState)
{
    switch (dwState)
    {
    case wlan_notification_acm_autoconf_enabled:
        return "Autoconf enabled";
    case wlan_notification_acm_autoconf_disabled:
        return "Autoconf disabled";
    case wlan_notification_acm_background_scan_enabled:
        return "Background scanenabled";
    case wlan_notification_acm_background_scan_disabled:
        return "Background scan disabled";
    case wlan_notification_acm_bss_type_change:
        return "Bss type changed";
    case wlan_notification_acm_power_setting_change:
        return "Power setting change";
    case wlan_notification_acm_scan_complete:
        return "Scan complete";
    case wlan_notification_acm_scan_fail:
        return "Scan fail";
    case wlan_notification_acm_connection_start:
        return "Connection start";
    case wlan_notification_acm_connection_complete:
        return "Connection complete";
    case wlan_notification_acm_connection_attempt_fail:
        return "Connection attempt failed";
    case wlan_notification_acm_filter_list_change:
        return "Filter list change";
    case wlan_notification_acm_interface_arrival:
        return "Interface arrival";
    case wlan_notification_acm_interface_removal:
        return "Interface removal";
    case wlan_notification_acm_profile_change:
        return "Profile changed";
    case wlan_notification_acm_profile_name_change:
        return "Name changed";
    case wlan_notification_acm_profiles_exhausted:
        return "Profiles exhausted";
    case wlan_notification_acm_network_not_available:
        return "Not available";
    case wlan_notification_acm_network_available:
        return "Network available";
    case wlan_notification_acm_disconnecting:
        return "Disconnecting";
    case wlan_notification_acm_disconnected:
        return "Disconnected";
    case wlan_notification_acm_adhoc_network_state_change:
        return "State changed";
    }
    return "None";
}
char *wcharToString(const wchar_t *wstr, size_t len)
{
    char *szBuffer = (char*)malloc(len + 1);
    size_t i;
    wcstombs_s(&i, szBuffer, (size_t)len + 1, wstr, (size_t)len + 1);
    return szBuffer;
}

TCHAR *WlanErrorCodeToString(DWORD dwMessageID)
{
    LPTSTR _tBuffer = NULL;
    if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
        dwMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&_tBuffer, _MAX_LENGTH, NULL))
    {
        msg("FormatMessage Function Failure");
    }
    return _tBuffer;
}

#define WTOSTR(n) wcharToString(n, wcslen(n))

HANDLE  NetOpenWlanHandle()
{
    HANDLE hClientHandle = 0;
    DWORD dwFuncResult = 0, pdwNegVer = 0;
    if ((dwFuncResult != WlanOpenHandle(WLAN_API_VERSION_2_0, NULL, &pdwNegVer, &hClientHandle)))
    {
        msg(_catstr("NetOpenWlanHandle: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
    if (WLAN_API_VERSION_MAJOR(pdwNegVer) < WLAN_API_VERSION_MAJOR(WLAN_API_VERSION_2_0)){
        WlanCloseHandle(hClientHandle, NULL);
    }
    return hClientHandle;
}

void NetSetWlanHostedNetworkEnable(HANDLE hClientHandle, PWLAN_HOSTED_NETWORK_REASON pFailReason, BOOL bEnable)
{
    DWORD dwFuncResult = 0;
    if (ERROR_SUCCESS != (dwFuncResult = WlanHostedNetworkSetProperty(hClientHandle, wlan_hosted_network_opcode_enable,
        sizeof(BOOL), &bEnable, pFailReason, NULL)))
    {
        msg(_catstr("NetSetWlanHostedNetworkEnable: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
        return;
    }
}
void NetSetWlanHostedNetworkSettings(HANDLE hClientHandle, std::string ssid, DWORD dwMaxClient, PWLAN_HOSTED_NETWORK_REASON pFailReason)
{
    WLAN_HOSTED_NETWORK_CONNECTION_SETTINGS wlanConSettings;
    ZeroMemory(&wlanConSettings, sizeof(WLAN_HOSTED_NETWORK_CONNECTION_SETTINGS));

    memcpy_s(wlanConSettings.hostedNetworkSSID.ucSSID, ssid.length()* sizeof(UCHAR), ssid.c_str(), ssid.length() *
        sizeof(char));

    wlanConSettings.hostedNetworkSSID.uSSIDLength = ssid.length();

    wlanConSettings.dwMaxNumberOfPeers = dwMaxClient;
    DWORD dwFuncResult = 0;

    dwFuncResult = WlanHostedNetworkSetProperty(hClientHandle, wlan_hosted_network_opcode_connection_settings,
        sizeof(WLAN_HOSTED_NETWORK_CONNECTION_SETTINGS), &wlanConSettings, pFailReason, NULL);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetSetWlanHostedNetworkSettings: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}
void NetCloseWlanHandle(HANDLE hClientHandle)
{
    DWORD dwFuncResult = WlanCloseHandle(hClientHandle, NULL);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetCloseWlanHandle: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}
void NetStartHostedNetwork(HANDLE hClientHandle, PWLAN_HOSTED_NETWORK_REASON pFailReason)
{
    DWORD dwFuncResult = 0;
    dwFuncResult = WlanHostedNetworkStartUsing(hClientHandle, pFailReason, NULL);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetStartHostedNetwork: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}
BOOL NetQueryWlanShowDeniedNetworkStatus(HANDLE hClientHandle, PWLAN_OPCODE_VALUE_TYPE pValueType)
{
    BOOL pValue = 0;
    DWORD pDataSize = sizeof(BOOL);
    DWORD dwFuncResult = WlanQueryAutoConfigParameter(hClientHandle, wlan_autoconf_opcode_show_denied_networks, NULL,
        &pDataSize, (void **)&pValue, pValueType);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetQueryWlanAutoConfigInformation: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
    return pValue;
}
void NetQueryWlanPowerSettings(HANDLE hClientHandle, PWLAN_POWER_SETTING pPowerSetting, PWLAN_OPCODE_VALUE_TYPE pValueType)
{
    DWORD dwDataSize = sizeof(PWLAN_POWER_SETTING);
    DWORD dwFuncResult = WlanQueryAutoConfigParameter(hClientHandle, wlan_autoconf_opcode_power_setting, NULL, &dwDataSize,
        (void**)&pPowerSetting, pValueType);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetQueryWlanPowerSettings: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}
void NetSetShowDeniedNetwork(HANDLE hClientHandle, BOOL pData)
{
    DWORD dwFuncResult = 0;
    if (ERROR_SUCCESS != (dwFuncResult = WlanSetAutoConfigParameter(hClientHandle,
        wlan_autoconf_opcode_show_denied_networks, sizeof(BOOL), &pData, NULL)))
    {
        msg(_catstr("NetSetShowDeniedNetwork: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}
void NetSetWlanAllowExplicitCreds(HANDLE hClientHandle, BOOL pData)
{
    DWORD dwFuncResult = 0;
    if (ERROR_SUCCESS != (dwFuncResult = WlanSetAutoConfigParameter(hClientHandle,
        wlan_autoconf_opcode_allow_explicit_creds, sizeof(BOOL), &pData, NULL)))
    {
        msg(_catstr("NetSetWlanAllowExplicitCreds: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}
void NetSetWlanAutoConfigBlockPeriod(HANDLE hClientHandle, DWORD pData)
{
    DWORD dwFuncResult = 0;
    if (ERROR_SUCCESS != (dwFuncResult = WlanSetAutoConfigParameter(hClientHandle,
        wlan_autoconf_opcode_block_period, sizeof(DWORD), &pData, NULL)))
    {
        msg(_catstr("NetSetWlanAutoConfigBlockPeriod: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}
// Set Wlan Virtual Station Extensibility
void NetSetWlanAllowVirtualStationExtensibility(HANDLE hClientHandle, BOOL pData)
{
    DWORD dwFuncResult = 0;
    if (ERROR_SUCCESS != (dwFuncResult = WlanSetAutoConfigParameter(hClientHandle,
        wlan_autoconf_opcode_allow_virtual_station_extensibility, sizeof(BOOL), &pData, NULL)))
    {
        msg(_catstr("NetSetWlanAllowVirtualStationExtensibility: ",
            WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}
//Wlan Scan
void NetScanWifi(HANDLE hClientHandle, const GUID *pInterfaceGuid, const std::string ssid, PWLAN_RAW_DATA pRawData)
{
    DWORD dwFuncResult = 0;
    if (ssid.empty()){
        dwFuncResult = WlanScan(hClientHandle, pInterfaceGuid, NULL, pRawData, NULL);
    }
    else
    {
        DOT11_SSID SSID;
        SSID.uSSIDLength = ssid.length();
        memcpy_s(SSID.ucSSID, sizeof(UCHAR)*ssid.length(), ssid.c_str(), ssid.length());
        dwFuncResult = WlanScan(hClientHandle, pInterfaceGuid, &SSID, pRawData, NULL);
        if (dwFuncResult != ERROR_SUCCESS){
            msg(_catstr("NetScanWifi: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
        }
    }
}

void NetConnectToWlan(HANDLE hClientHandle, const GUID *interfaceGuid, PWLAN_CONNECTION_PARAMETERS pConnectionParameter)
{
    DWORD dwFuncResult = WlanConnect(hClientHandle, interfaceGuid, pConnectionParameter, NULL);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetConnectToWlan: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}
void NetDeleteWlanProfile(HANDLE hClientHandle, const GUID *interfaceGuid, LPCWSTR strProfileName)
{
    DWORD dwFuncResult = WlanDeleteProfile(hClientHandle, interfaceGuid, strProfileName, NULL);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetDeleteWlanProfile: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}
void NetDisconnectWlan(HANDLE hClientHandle, const GUID *interfaceGuid)
{
    DWORD dwFuncResult = WlanDisconnect(hClientHandle, interfaceGuid, NULL);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetDisconnectWlan: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}
void NetQueryWlanInterface(HANDLE hClientHandle, const GUID *pInterfaceGuid, WLAN_INTF_OPCODE pCode,
    PDWORD pDataSize, PVOID *pData, PWLAN_OPCODE_VALUE_TYPE pValueType)
{
    DWORD dwFuncResult = WlanQueryInterface(hClientHandle, pInterfaceGuid, pCode, NULL, pDataSize, pData, pValueType);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetQueryWlanInterface: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}

void NetGetWlanAvailableNetworkList(HANDLE hClientHandle, const GUID *pInterfaceGuid, DWORD dwFlag,
    PWLAN_AVAILABLE_NETWORK_LIST *ppAvailableNetworkList)
{
    DWORD dwFuncResult = WlanGetAvailableNetworkList(hClientHandle, pInterfaceGuid, dwFlag, NULL, ppAvailableNetworkList);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetGetWlanAvailableNetworkList: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}
void NetEnumWlanInterfaces(HANDLE hClientHandle, PWLAN_INTERFACE_INFO_LIST *ppInterfaceList)
{
    DWORD dwFuncResult = WlanEnumInterfaces(hClientHandle, NULL, ppInterfaceList);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetEnumWlanInterfaces: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}

#define NetStringFromGUID2(guid, buffer, sz) StringFromGUID2(guid, (LPOLESTR)buffer, sz)

void NetQueryWlanConnectionSettings(HANDLE hClientHandle, PWLAN_HOSTED_NETWORK_CONNECTION_SETTINGS *connectionSettings,
    PDWORD dwConSize, PWLAN_OPCODE_VALUE_TYPE valueType)
{
    DWORD dwFuncResult = WlanHostedNetworkQueryProperty(hClientHandle, wlan_hosted_network_opcode_connection_settings
        , dwConSize, (void **)connectionSettings, valueType, NULL);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetQueryWlanConnectionSettings: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}

const char *notificationString(DWORD dwCode, DWORD dwSource){
    switch (dwSource){
    case WLAN_NOTIFICATION_SOURCE_ACM:
        return AcmNoficationToString(dwCode);
    case WLAN_NOTIFICATION_SOURCE_MSM:
        return MsmNoficationToString(dwCode);
    }
    return "None";
}
LPWSTR NetGetWlanProfile(HANDLE hClientHandle, const GUID *interfaceGuid, LPWSTR strProfileName, DWORD *pdwFlags,
    PDWORD pdwGrantedAccess
    )
{
    LPWSTR lpProfileXML = (LPWSTR)malloc(100);
    DWORD dwFuncResult = WlanGetProfile(hClientHandle, interfaceGuid, strProfileName, NULL, &lpProfileXML, pdwFlags,
        pdwGrantedAccess);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetGetWlanProfile: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
    return lpProfileXML;
}
void NetGetWlanProfileList(HANDLE hClientHandle, const GUID *pInterfaceGuid, PWLAN_PROFILE_INFO_LIST *pProfileInfoList)
{
    DWORD dwFuncResult = WlanGetProfileList(hClientHandle, pInterfaceGuid, NULL, pProfileInfoList);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetGetWlanProfileList: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}
void NetQueryWlanSecondaryKey(HANDLE hClientHandle, PDWORD pdwKeyLength, PUCHAR *ppucKeyData,
    PBOOL pbIsPassPhrase, PBOOL pbPersistent, PWLAN_HOSTED_NETWORK_REASON pFailReason)
{
    pbPersistent = 0;
    DWORD dwFuncResult = WlanHostedNetworkQuerySecondaryKey(hClientHandle, pdwKeyLength, ppucKeyData, pbIsPassPhrase,
        pbIsPassPhrase, pFailReason, NULL);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetQueryWlanSecondaryKey: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}
void NetInitWlanSettings(HANDLE hClientHandle, PWLAN_HOSTED_NETWORK_REASON pFailReason)
{
    DWORD dwFuncResult = WlanHostedNetworkInitSettings(hClientHandle, pFailReason, NULL);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetInitWlanSettings: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}
void NetSaveTemporaryProfile(__in HANDLE hClientHandle, __in const GUID *pInterfaceGuid, __in LPCWSTR strProfileName,
    __in LPCWSTR strAllUserProfileSecurity, __in DWORD dwFlags, __in BOOL bOverWrite)
{
    DWORD dwFuncResult = WlanSaveTemporaryProfile(hClientHandle, pInterfaceGuid, strProfileName, strAllUserProfileSecurity,
        dwFlags, bOverWrite, NULL);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetSaveTemporaryProfile: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}
void NetRenameProfile(HANDLE hClientHandle, const GUID *pInterfaceGuid, LPCWSTR strOldProfileName, LPCWSTR strNewProfileName)
{
    DWORD dwFuncResult = WlanRenameProfile(hClientHandle, pInterfaceGuid, strOldProfileName, strNewProfileName, NULL);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetRenameProfile: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}

void NetWlanHostedNetworkSetSecondaryKey(HANDLE hClientHandle, std::string key, BOOL passphrase, BOOL persistent,
    PWLAN_HOSTED_NETWORK_REASON pFailReason)
{
    DWORD dwFuncResult = 0;
    UCHAR *_tmpKey = (UCHAR *)malloc(key.length() + 1);
    ZeroMemory(_tmpKey, key.length() + 1);
    memcpy_s(_tmpKey, key.length() * sizeof(UCHAR), key.c_str(), key.length());
    dwFuncResult = WlanHostedNetworkSetSecondaryKey(hClientHandle, key.length() + 1, _tmpKey, passphrase,
        persistent, pFailReason, NULL);
    if (dwFuncResult != ERROR_SUCCESS){
        msg(_catstr("NetWlanHostedNetworkSetSecondaryKey: ", WTOSTR(WlanErrorCodeToString(dwFuncResult))));
    }
}

