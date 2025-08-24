class NT_GameInit
{
    static bool g_NT_GameInitialized = false;
    
    void NT_GameInit()
    {
        if (!g_NT_GameInitialized)
        {
            InitializeGameModule();
            g_NT_GameInitialized = true;
        }
    }
    
    void InitializeGameModule()
    {
        Print("=================================================");
        Print("[NT_AntiCheat] Game Module Initialization...");
        Print("[NT_AntiCheat] Version: 2.0");
        Print("[NT_AntiCheat] Author: NT Development");
        Print("=================================================");
        Print("[NT_AntiCheat] Game module initialized successfully");
    }
    
    static bool IsInitialized()
    {
        return g_NT_GameInitialized;
    }
    
    static void SetInitialized(bool value)
    {
        g_NT_GameInitialized = value;
    }
}

string NT_GetSystemVersion()
{
    return "2.0";
}

string NT_GetSystemName()
{
    return "NT AntiCheat System";
}

string NT_GetSystemAuthor()
{
    return "NT Development";
}

bool NT_IsValidCheckInterval(float interval)
{
    if (interval >= 1.0 && interval <= 10.0)
        return true;
    return false;
}

bool NT_IsValidSpeed(float speed)
{
    if (speed >= 5.0 && speed <= 50.0)
        return true;
    return false;
}

bool NT_IsValidHeight(float height)
{
    if (height >= 10.0 && height <= 500.0)
        return true;
    return false;
}

bool NT_IsValidViolationCount(int count)
{
    if (count >= 1 && count <= 10)
        return true;
    return false;
}

string NT_GetConfigDirectory()
{
    return "$profile:/nt-anticheat/";
}

string NT_GetLogDirectory()
{
    return "$profile:/nt-anticheat/logs/";
}

string NT_GetBackupDirectory()
{
    return "$profile:/nt-anticheat/backups/";
}

bool NT_IsValidBanType(string banType)
{
    if (banType == "SPEED_HACK") return true;
    if (banType == "FLY_HACK") return true;
    if (banType == "TELEPORT_HACK") return true;
    if (banType == "GOD_MODE") return true;
    if (banType == "NOCLIP_HACK") return true;
    if (banType == "UNKNOWN_HACK") return true;
    return false;
}

string NT_GetBanTypeName(string banType)
{
    if (banType == "SPEED_HACK") return "Speed Hack";
    if (banType == "FLY_HACK") return "Fly Hack";
    if (banType == "TELEPORT_HACK") return "Teleport Hack";
    if (banType == "GOD_MODE") return "God Mode";
    if (banType == "NOCLIP_HACK") return "NoClip Hack";
    return "Unknown Hack";
}

bool NT_IsValidLogLevel(int level)
{
    if (level >= 0 && level <= 2)
        return true;
    return false;
}

string NT_GetLogLevelName(int level)
{
    if (level == 0) return "Disabled";
    if (level == 1) return "Console Only";
    if (level == 2) return "Console + File";
    return "Unknown";
}

bool NT_IsValidSteamId64(string steamId)
{
    if (steamId.Length() != 17) 
        return false;
    return true;
}

string NT_GetSystemInfo()
{
    string info = "=== NT ANTICHEAT GAME MODULE INFO ===";
    return info;
}

void NT_Shutdown()
{
    if (NT_GameInit.IsInitialized())
    {
        Print("[NT_AntiCheat] Game module shutting down...");
        NT_CleanupResources();
        NT_GameInit.SetInitialized(false);
        Print("[NT_AntiCheat] Game module shutdown completed");
    }
}

void NT_CleanupResources()
{
    Print("[NT_AntiCheat] Resources cleaned up");
}

void NT_LogError(string errorMessage)
{
    string formattedError = "[NT_AntiCheat] GAME MODULE ERROR: " + errorMessage;
    Print(formattedError);
}

void NT_LogWarning(string warningMessage)
{
    string formattedWarning = "[NT_AntiCheat] GAME MODULE WARNING: " + warningMessage;
    Print(formattedWarning);
}

void NT_LogInfo(string infoMessage)
{
    string formattedInfo = "[NT_AntiCheat] GAME MODULE INFO: " + infoMessage;
    Print(formattedInfo);
}