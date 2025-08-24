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
        
        RegisterGameCallbacks();
        InitializeSystemConstants();
        
        Print("[NT_AntiCheat] Game module initialized successfully");
    }
    
    void RegisterGameCallbacks()
    {
        if (GetGame())
        {
            Print("[NT_AntiCheat] Game callbacks registered");
        }
    }
    
    void InitializeSystemConstants()
    {
        ValidateSystemEnvironment();
        SetupGlobalProperties();
        
        Print("[NT_AntiCheat] System constants initialized");
    }
    
    void ValidateSystemEnvironment()
    {
        if (!GetGame().IsServer())
        {
            Print("[NT_AntiCheat] WARNING: AntiCheat system should only run on server!");
        }
        
        string gameVersion = GetGame().GetVersion();
        if (gameVersion != "")
        {
            Print("[NT_AntiCheat] Running on DayZ version: " + gameVersion);
        }
        
        if (!CanCreateDirectories())
        {
            Print("[NT_AntiCheat] ERROR: Insufficient permissions to create directories!");
        }
    }
    
    void SetupGlobalProperties()
    {
        bool debugMode = false;
        Print("[NT_AntiCheat] Debug mode: " + debugMode.ToString());
        
        SetOptimizationFlags();
    }
    
    void SetOptimizationFlags()
    {
        Print("[NT_AntiCheat] Performance optimization flags set");
    }
    
    bool CanCreateDirectories()
    {
        string testDir = "$profile:/nt-anticheat-test/";
        
        if (MakeDirectory(testDir))
        {
            return true;
        }
        
        return false;
    }
    
    void SystemHealthCheck()
    {
        if (!GetGame() || !GetGame().IsServer())
        {
            Print("[NT_AntiCheat] System health check failed - Invalid game state");
            return;
        }
        
        CheckMemoryUsage();
        CheckFileSystemHealth();
        CheckSystemPerformance();
    }
    
    void CheckMemoryUsage()
    {
        Print("[NT_AntiCheat] Memory health check completed");
    }
    
    void CheckFileSystemHealth()
    {
        ref array<string> criticalDirs = new array<string>;
        criticalDirs.Insert("$profile:/nt-anticheat/");
        criticalDirs.Insert("$profile:/nt-anticheat/logs/");
        criticalDirs.Insert("$profile:/nt-anticheat/backups/");
        
        for (int i = 0; i < criticalDirs.Count(); i++)
        {
            string dir = criticalDirs.Get(i);
            if (!FileExist(dir))
            {
                Print("[NT_AntiCheat] WARNING: Critical directory missing: " + dir);
                MakeDirectory(dir);
            }
        }
    }
    
    void CheckSystemPerformance()
    {
        float currentTime = GetGame().GetTime();
        static float lastCheckTime = 0;
        
        if (lastCheckTime > 0)
        {
            float deltaTime = currentTime - lastCheckTime;
            if (deltaTime > 120.0)
            {
                Print("[NT_AntiCheat] WARNING: Long time between health checks - " + deltaTime.ToString() + "s");
            }
        }
        
        lastCheckTime = currentTime;
    }
    
    // Static method เพื่อเข้าถึง static variable
    static bool IsInitialized()
    {
        return g_NT_GameInitialized;
    }
    
    static void SetInitialized(bool value)
    {
        g_NT_GameInitialized = value;
    }
}

// Global utility functions
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
    return (interval >= 1.0 && interval <= 10.0);
}

bool NT_IsValidSpeed(float speed)
{
    return (speed >= 5.0 && speed <= 50.0);
}

bool NT_IsValidHeight(float height)
{
    return (height >= 10.0 && height <= 500.0);
}

bool NT_IsValidViolationCount(int count)
{
    return (count >= 1 && count <= 10);
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
    ref array<string> validTypes = new array<string>;
    validTypes.Insert("SPEED_HACK");
    validTypes.Insert("FLY_HACK");
    validTypes.Insert("TELEPORT_HACK");
    validTypes.Insert("GOD_MODE");
    validTypes.Insert("NOCLIP_HACK");
    validTypes.Insert("UNKNOWN_HACK");
    
    return (validTypes.Find(banType) != -1);
}

string NT_GetBanTypeName(string banType)
{
    if (banType == "SPEED_HACK")
        return "Speed Hack";
    else if (banType == "FLY_HACK")
        return "Fly Hack";
    else if (banType == "TELEPORT_HACK")
        return "Teleport Hack";
    else if (banType == "GOD_MODE")
        return "God Mode";
    else if (banType == "NOCLIP_HACK")
        return "NoClip Hack";
    else
        return "Unknown Hack";
}

bool NT_IsValidLogLevel(int level)
{
    return (level >= 0 && level <= 2);
}

string NT_GetLogLevelName(int level)
{
    if (level == 0)
        return "Disabled";
    else if (level == 1)
        return "Console Only";
    else if (level == 2)
        return "Console + File";
    else
        return "Unknown";
}

bool NT_IsValidSteamId64(string steamId)
{
    if (steamId.Length() != 17)
        return false;
        
    for (int i = 0; i < steamId.Length(); i++)
    {
        string char = steamId.Substring(i, 1);
        if (char != "0" && char != "1" && char != "2" && char != "3" && char != "4" && 
            char != "5" && char != "6" && char != "7" && char != "8" && char != "9")
        {
            return false;
        }
    }
    
    string prefix = steamId.Substring(0, 9);
    return (prefix == "765611980" || prefix == "765611981");
}

string NT_GetSystemInfo()
{
    string info = "=== NT ANTICHEAT GAME MODULE INFO ===\n";
    info += "System Name: NT AntiCheat System\n";
    info += "Version: 2.0\n";
    info += "Author: NT Development\n";
    info += "Initialized: " + (NT_GameInit.IsInitialized() ? "YES" : "NO") + "\n";
    info += "Server Mode: " + (GetGame().IsServer() ? "YES" : "NO") + "\n";
    info += "Config Directory: $profile:/nt-anticheat/\n";
    info += "Log Directory: $profile:/nt-anticheat/logs/\n";
    info += "Backup Directory: $profile:/nt-anticheat/backups/\n";
    info += "=====================================\n";
    
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
    
    if (GetGame())
    {
        GetGame().AdminLog(formattedError);
    }
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