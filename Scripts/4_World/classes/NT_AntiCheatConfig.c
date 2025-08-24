class NT_AntiCheatConfig
{
    float checkInterval;
    float maxSpeed;
    float maxHeight;
    float maxTeleportDistance;
    bool enableSpeedCheck;
    bool enableFlyCheck;
    bool enableTeleportCheck;
    bool enableGodModeCheck;
    bool enableNoClipCheck;
    bool autoKickEnabled;
    bool autoBanEnabled;
    int logLevel;
    int maxViolationsBeforeBan;
    float saveInterval;
    
    void NT_AntiCheatConfig()
    {
        checkInterval = 2.5;
        maxSpeed = 10.0;
        maxHeight = 80.0;
        maxTeleportDistance = 300.0;
        enableSpeedCheck = true;
        enableFlyCheck = true;
        enableTeleportCheck = true;
        enableGodModeCheck = false;
        enableNoClipCheck = false;
        autoKickEnabled = true;
        autoBanEnabled = true;
        logLevel = 2;
        maxViolationsBeforeBan = 3;
        saveInterval = 30.0;
    }
    
    void ValidateConfig()
    {
        if (checkInterval < 1.0) checkInterval = 1.0;
        if (checkInterval > 10.0) checkInterval = 10.0;
        
        if (maxSpeed < 5.0) maxSpeed = 5.0;
        if (maxSpeed > 50.0) maxSpeed = 50.0;
        
        if (maxHeight < 10.0) maxHeight = 10.0;
        if (maxHeight > 500.0) maxHeight = 500.0;
        
        if (maxTeleportDistance < 50.0) maxTeleportDistance = 50.0;
        if (maxTeleportDistance > 1000.0) maxTeleportDistance = 1000.0;
        
        if (logLevel < 0) logLevel = 0;
        if (logLevel > 2) logLevel = 2;
        
        if (maxViolationsBeforeBan < 1) maxViolationsBeforeBan = 1;
        if (maxViolationsBeforeBan > 10) maxViolationsBeforeBan = 10;
        
        if (saveInterval < 10.0) saveInterval = 10.0;
        if (saveInterval > 300.0) saveInterval = 300.0;
    }
};