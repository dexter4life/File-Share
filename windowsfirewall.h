#ifndef WINDOWSFIREWALL_H
#define WINDOWSFIREWALL_H

#include <windows.h>
#include <crtdbg.h>
#include <netfw.h>
#include <objbase.h>
#include <oleauto.h>
#include <stdio.h>

class WindowsFirewall
{
public:
    WindowsFirewall() = delete;
    ~WindowsFirewall() = delete;
    static HRESULT WindowsFirewallInitialize(INetFwProfile **fwProfile);
    static HRESULT WindowsFirewallIsOn(INetFwProfile *fwProfile, BOOL *fwOn);
    static HRESULT WindowsFirewallTurnOn(INetFwProfile *fwProfile);
    static HRESULT WindowsFirewallTurnOff(INetFwProfile *fwProfile);
    static HRESULT WindowsFirewallAppIsEnabled(INetFwProfile *fwProfile, const wchar_t *fwProcessImageFileName, BOOL *fwAppEnabled);
    static HRESULT WindowsFirewallAddApp(INetFwProfile *fwProfile, const wchar_t *fwProcessImageFileName, const wchar_t *fwName);
    static HRESULT WindowsFirewallPortIsEnabled(INetFwProfile *fwProfile, LONG portNumber, NET_FW_IP_PROTOCOL ipProtocol, BOOL *fwPortEnabled);
    static HRESULT WindowsFirewallPortAdd(INetFwProfile *fwProfile, LONG portNumber, NET_FW_IP_PROTOCOL ipProtocol, const wchar_t *name);
    static void WindowsFirewallCleanup(INetFwProfile *fwProfile);
};

#endif // WINDOWSFIREWALL_H
