class NT_AntiCheatManager
{
    protected ref array<ref NT_BannedPlayer> m_BannedPlayers;
    protected ref array<ref NT_AdminWhitelist> m_AdminWhitelist;
    protected ref NT_AntiCheatConfig m_Config;
    protected ref map<string, ref NT_PlayerTracker> m_PlayerTrackers;
    
    // File paths
    protected string m_BanListPath = "$profile:/nt-anticheat/banlist.json";
    protected string m_WhitelistPath = "$profile:/nt-anticheat/whitelist.json";
    protected string m_ConfigPath = "$profile:/nt-anticheat/config.json";
    protected string m_LogPath = "$profile:/nt-anticheat/logs/";
    
    // Timers
    protected float m_CheckTimer;
    protected float m_SaveTimer;
    protected float m_CleanupTimer;
    protected float m_LogTimer;
    
    // Status flags
    protected bool m_IsInitialized;
    protected bool m_SystemEnabled;
    protected int m_TotalPlayersChecked;
    protected int m_TotalViolationsDetected;
    protected int m_TotalPlayersBanned;
    
    void NT_AntiCheatManager()
    {
        m_BannedPlayers = new array<ref NT_BannedPlayer>;
        m_AdminWhitelist = new array<ref NT_AdminWhitelist>;
        m_PlayerTrackers = new map<string, ref NT_PlayerTracker>;
        m_Config = new NT_AntiCheatConfig();
        
        // Initialize timers
        m_CheckTimer = 0;
        m_SaveTimer = 0;
        m_CleanupTimer = 0;
        m_LogTimer = 0;
        
        // Initialize status
        m_IsInitialized = false;
        m_SystemEnabled = true;
        m_TotalPlayersChecked = 0;
        m_TotalViolationsDetected = 0;
        m_TotalPlayersBanned = 0;
        
        // Create directory structure
        CreateDirectoryStructure();
        
        // Load all configurations
        LoadAllConfigurations();
        
        m_IsInitialized = true;
        
        Print("[NT_AntiCheat] AntiCheat Manager initialized successfully");
    }
    
    void CreateDirectoryStructure()
    {
        // Create main directory
        if (!FileExist("$profile:/nt-anticheat/"))
        {
            MakeDirectory("$profile:/nt-anticheat/");
            Print("[NT_AntiCheat] Created main directory: nt-anticheat");
        }
        
        // Create logs directory
        if (!FileExist("$profile:/nt-anticheat/logs/"))
        {
            MakeDirectory("$profile:/nt-anticheat/logs/");
            Print("[NT_AntiCheat] Created logs directory");
        }
        
        // Create backups directory
        if (!FileExist("$profile:/nt-anticheat/backups/"))
        {
            MakeDirectory("$profile:/nt-anticheat/backups/");
            Print("[NT_AntiCheat] Created backups directory");
        }
    }
    
    void InitializeDefaultConfigs()
    {
        // Create default whitelist if not exists
        if (!FileExist(m_WhitelistPath))
        {
            CreateDefaultWhitelist();
        }
        
        // Create default config if not exists  
        if (!FileExist(m_ConfigPath))
        {
            CreateDefaultConfig();
        }
        
        // Create readme file
        CreateReadmeFile();
        
        Print("[NT_AntiCheat] Configuration files initialized");
    }
    
    void CreateDefaultWhitelist()
    {
        Print("[NT_AntiCheat] Creating default whitelist file...");
        
        // Add example admin entries with detailed information
        NT_AdminWhitelist admin1 = new NT_AdminWhitelist();
        admin1.InitializeWhitelist("76561198123456789", "Example_Admin1", "Server Administrator - Replace with real SteamID", "System", "Head Admin");
        admin1.AddNotes("This is an example entry - Replace with actual admin SteamID64");
        m_AdminWhitelist.Insert(admin1);
        
        NT_AdminWhitelist admin2 = new NT_AdminWhitelist();
        admin2.InitializeWhitelist("76561198987654321", "Example_Admin2", "Server Moderator - Replace with real SteamID", "System", "Moderator");
        admin2.AddNotes("This is an example entry - Replace with actual admin SteamID64");
        m_AdminWhitelist.Insert(admin2);
        
        SaveWhitelist();
        Print("[NT_AntiCheat] Default whitelist created with " + m_AdminWhitelist.Count().ToString() + " example entries");
    }
    
    void CreateDefaultConfig()
    {
        Print("[NT_AntiCheat] Creating default configuration file...");
        
        m_Config.checkInterval = 2.5;
        m_Config.maxSpeed = 10.0;
        m_Config.maxHeight = 80.0;
        m_Config.maxTeleportDistance = 300.0;
        m_Config.enableSpeedCheck = true;
        m_Config.enableFlyCheck = true;
        m_Config.enableTeleportCheck = true;
        m_Config.enableGodModeCheck = false;
        m_Config.enableNoClipCheck = false;
        m_Config.autoKickEnabled = true;
        m_Config.autoBanEnabled = true;
        m_Config.logLevel = 2;
        m_Config.maxViolationsBeforeBan = 3;
        m_Config.saveInterval = 30.0;
        
        // Validate and save config
        m_Config.ValidateConfig();
        SaveConfig();
        Print("[NT_AntiCheat] Default configuration created and validated");
    }
    
    void CreateReadmeFile()
    {
        string readmePath = "$profile:/nt-anticheat/README.txt";
        
        if (!FileExist(readmePath))
        {
            FileHandle file = OpenFile(readmePath, FileMode.WRITE);
            if (file)
            {
                string content = "";
                content += "==============================================\n";
                content += "     NT ANTICHEAT SYSTEM v2.0\n";
                content += "==============================================\n\n";
                content += "FILES EXPLANATION:\n\n";
                content += "1. config.json - Main configuration file\n";
                content += "   - checkInterval: How often to check players (seconds)\n";
                content += "   - maxSpeed: Maximum allowed speed (m/s)\n";
                content += "   - maxHeight: Maximum height above ground (meters)\n";
                content += "   - enableXXXCheck: Enable/disable specific checks\n\n";
                content += "2. whitelist.json - Admin whitelist file\n";
                content += "   - Add admin SteamID64 here to bypass all checks\n";
                content += "   - Replace example SteamIDs with real ones!\n\n";
                content += "3. banlist.json - Banned players database\n";
                content += "   - Automatically generated when players are banned\n";
                content += "   - Contains all ban information\n\n";
                content += "IMPORTANT NOTES:\n";
                content += "- Replace example SteamID64 in whitelist.json with real admin IDs\n";
                content += "- Adjust config.json values according to your server needs\n";
                content += "- Check logs/ folder for detailed violation logs\n";
                content += "- Backup files are created in backups/ folder\n\n";
                content += "For support, contact NT Development\n";
                content += "==============================================\n";
                
                FPrintln(file, content);
                CloseFile(file);
                Print("[NT_AntiCheat] README.txt created");
            }
        }
    }
    
    void LoadAllConfigurations()
    {
        LoadBanList();
        LoadWhitelist();
        LoadConfig();
        
        Print("[NT_AntiCheat] All configurations loaded successfully");
    }
    
    void LoadBanList()
    {
        if (FileExist(m_BanListPath))
        {
            if (JsonFileLoader<array<ref NT_BannedPlayer>>.JsonLoadFile(m_BanListPath, m_BannedPlayers))
            {
                Print("[NT_AntiCheat] Loaded " + m_BannedPlayers.Count().ToString() + " banned players");
                
                // Validate loaded bans
                int validBans = 0;
                foreach (NT_BannedPlayer ban : m_BannedPlayers)
                {
                    if (ban && ban.IsValidBan())
                    {
                        validBans++;
                    }
                }
                
                if (validBans != m_BannedPlayers.Count())
                {
                    Print("[NT_AntiCheat] WARNING: " + (m_BannedPlayers.Count() - validBans).ToString() + " invalid ban entries found");
                }
            }
            else
            {
                Print("[NT_AntiCheat] ERROR: Failed to load ban list from " + m_BanListPath);
            }
        }
        else
        {
            Print("[NT_AntiCheat] Ban list file not found - will be created when first ban occurs");
        }
    }
    
    void LoadWhitelist()
    {
        if (FileExist(m_WhitelistPath))
        {
            if (JsonFileLoader<array<ref NT_AdminWhitelist>>.JsonLoadFile(m_WhitelistPath, m_AdminWhitelist))
            {
                Print("[NT_AntiCheat] Loaded " + m_AdminWhitelist.Count().ToString() + " whitelisted admins");
                
                // Validate loaded whitelist entries
                int validEntries = 0;
                foreach (NT_AdminWhitelist entry : m_AdminWhitelist)
                {
                    if (entry && entry.IsValid())
                    {
                        validEntries++;
                        if (entry.isActive)
                        {
                            Print("[NT_AntiCheat] Active admin: " + entry.playerName + " (" + entry.steamId + ")");
                        }
                    }
                }
                
                if (validEntries != m_AdminWhitelist.Count())
                {
                    Print("[NT_AntiCheat] WARNING: " + (m_AdminWhitelist.Count() - validEntries).ToString() + " invalid whitelist entries found");
                }
            }
            else
            {
                Print("[NT_AntiCheat] ERROR: Failed to load whitelist from " + m_WhitelistPath);
            }
        }
        else
        {
            Print("[NT_AntiCheat] Whitelist file not found - will be created with default entries");
        }
    }
    
    void LoadConfig()
    {
        if (FileExist(m_ConfigPath))
        {
            if (JsonFileLoader<NT_AntiCheatConfig>.JsonLoadFile(m_ConfigPath, m_Config))
            {
                // Validate loaded configuration
                m_Config.ValidateConfig();
                Print("[NT_AntiCheat] Configuration loaded and validated successfully");
                PrintConfigInfo();
            }
            else
            {
                Print("[NT_AntiCheat] ERROR: Failed to load config from " + m_ConfigPath + " - using defaults");
                m_Config = new NT_AntiCheatConfig();
                m_Config.ValidateConfig();
            }
        }
        else
        {
            Print("[NT_AntiCheat] Config file not found - will be created with default settings");
        }
    }
    
    void PrintConfigInfo()
    {
        Print("=== NT ANTICHEAT CONFIGURATION ===");
        Print("Check Interval: " + m_Config.checkInterval.ToString() + "s");
        Print("Max Speed: " + m_Config.maxSpeed.ToString() + " m/s");
        Print("Max Height: " + m_Config.maxHeight.ToString() + "m");
        Print("Max Teleport Distance: " + m_Config.maxTeleportDistance.ToString() + "m");
        Print("Speed Check: " + (m_Config.enableSpeedCheck ? "ENABLED" : "DISABLED"));
        Print("Fly Check: " + (m_Config.enableFlyCheck ? "ENABLED" : "DISABLED"));
        Print("Teleport Check: " + (m_Config.enableTeleportCheck ? "ENABLED" : "DISABLED"));
        Print("God Mode Check: " + (m_Config.enableGodModeCheck ? "ENABLED" : "DISABLED"));
        Print("Auto Ban: " + (m_Config.autoBanEnabled ? "ENABLED" : "DISABLED"));
        Print("Max Violations Before Ban: " + m_Config.maxViolationsBeforeBan.ToString());
        Print("Log Level: " + m_Config.logLevel.ToString());
        Print("==================================");
    }
    
    void SaveBanList()
    {
        if (JsonFileLoader<array<ref NT_BannedPlayer>>.JsonSaveFile(m_BanListPath, m_BannedPlayers))
        {
            if (m_Config.logLevel >= 1)
            {
                Print("[NT_AntiCheat] Ban list saved successfully (" + m_BannedPlayers.Count().ToString() + " entries)");
            }
        }
        else
        {
            Print("[NT_AntiCheat] ERROR: Failed to save ban list to " + m_BanListPath);
        }
    }
    
    void SaveWhitelist()
    {
        if (JsonFileLoader<array<ref NT_AdminWhitelist>>.JsonSaveFile(m_WhitelistPath, m_AdminWhitelist))
        {
            if (m_Config.logLevel >= 1)
            {
                Print("[NT_AntiCheat] Whitelist saved successfully (" + m_AdminWhitelist.Count().ToString() + " entries)");
            }
        }
        else
        {
            Print("[NT_AntiCheat] ERROR: Failed to save whitelist to " + m_WhitelistPath);
        }
    }
    
    void SaveConfig()
    {
        if (JsonFileLoader<NT_AntiCheatConfig>.JsonSaveFile(m_ConfigPath, m_Config))
        {
            if (m_Config.logLevel >= 1)
            {
                Print("[NT_AntiCheat] Configuration saved successfully");
            }
        }
        else
        {
            Print("[NT_AntiCheat] ERROR: Failed to save config to " + m_ConfigPath);
        }
    }
    
    void Update(float timeslice)
    {
        if (!m_IsInitialized || !m_SystemEnabled)
            return;
            
        // Update timers
        m_CheckTimer += timeslice;
        m_SaveTimer += timeslice;
        m_CleanupTimer += timeslice;
        m_LogTimer += timeslice;
        
        // Perform anticheat checks
        if (m_CheckTimer >= m_Config.checkInterval)
        {
            m_CheckTimer = 0;
            PerformAntiCheatChecks();
        }
        
        // Save data periodically
        if (m_SaveTimer >= m_Config.saveInterval)
        {
            m_SaveTimer = 0;
            SaveBanList();
            BackupFiles();
        }
        
        // Cleanup disconnected players
        if (m_CleanupTimer >= 60.0)
        {
            m_CleanupTimer = 0;
            CleanupDisconnectedPlayers();
        }
        
        // Log statistics
        if (m_LogTimer >= 300.0 && m_Config.logLevel >= 1) // Every 5 minutes
        {
            m_LogTimer = 0;
            LogStatistics();
        }
    }
    
    void OnPlayerConnect(PlayerIdentity identity, PlayerBase player)
    {
        if (!identity || !player)
        {
            Print("[NT_AntiCheat] ERROR: Invalid player data on connection");
            return;
        }
        
        string steamId = identity.GetPlainId();
        string playerName = identity.GetName();
        
        if (steamId == "" || playerName == "")
        {
            Print("[NT_AntiCheat] ERROR: Empty SteamID or player name");
            return;
        }
        
        // Check if player is banned
        if (IsPlayerBanned(steamId))
        {
            NT_BannedPlayer banned = GetBannedPlayer(steamId);
            if (banned)
            {
                string kickMessage = banned.GetFullBanInfo();
                
                Print(string.Format("[NT_AntiCheat] BLOCKED banned player connection: %1 (%2) - Ban ID: %3", 
                    playerName, steamId, banned.banId));
                    
                LogToFile("BLOCKED_CONNECTION", playerName, steamId, "Attempted connection while banned - " + banned.reason);
                
                GetGame().AdminKick(identity, kickMessage);
                return;
            }
        }
        
        // Check if player is whitelisted
        bool isWhitelisted = IsPlayerWhitelisted(steamId);
        
        // Create player tracker
        NT_PlayerTracker tracker = new NT_PlayerTracker();
        tracker.InitializeTracker(steamId, playerName, player.GetPosition(), isWhitelisted);
        
        m_PlayerTrackers.Set(steamId, tracker);
        m_TotalPlayersChecked++;
        
        if (isWhitelisted)
        {
            NT_AdminWhitelist adminInfo = GetWhitelistEntry(steamId);
            string adminLevel = adminInfo ? adminInfo.adminLevel : "Administrator";
            
            Print(string.Format("[NT_AntiCheat] Whitelisted %1 connected: %2 (%3)", 
                adminLevel, playerName, steamId));
                
            LogToFile("ADMIN_CONNECTION", playerName, steamId, "Whitelisted admin connected - " + adminLevel);
        }
        else
        {
            Print(string.Format("[NT_AntiCheat] Player connected and tracking enabled: %1 (%2)", 
                playerName, steamId));
                
            if (m_Config.logLevel >= 2)
            {
                LogToFile("PLAYER_CONNECTION", playerName, steamId, "Player connected - monitoring active");
            }
        }
    }
    
    void OnPlayerDisconnect(PlayerIdentity identity)
    {
        if (!identity)
            return;
            
        string steamId = identity.GetPlainId();
        string playerName = identity.GetName();
        
        NT_PlayerTracker tracker = m_PlayerTrackers.Get(steamId);
        if (tracker)
        {
            // Log disconnect stats if player had violations
            if (tracker.totalViolations > 0 && m_Config.logLevel >= 1)
            {
                Print(string.Format("[NT_AntiCheat] Player disconnected with %1 violations: %2 (%3)", 
                    tracker.totalViolations.ToString(), playerName, steamId));
                    
                LogToFile("PLAYER_DISCONNECT", playerName, steamId, 
                    string.Format("Disconnected with %1 violations", tracker.totalViolations.ToString()));
            }
            
            tracker.Disconnect();
        }
        
        // Don't remove immediately - cleanup will handle it
        if (m_Config.logLevel >= 2)
        {
            Print(string.Format("[NT_AntiCheat] Player disconnected: %1 (%2)", playerName, steamId));
        }
    }
    
    void CleanupDisconnectedPlayers()
    {
        array<string> toRemove = new array<string>;
        
        foreach (string steamId, NT_PlayerTracker tracker : m_PlayerTrackers)
        {
            PlayerIdentity identity = GetGame().GetPlayerIdentityBySteamId(steamId);
            if (!identity)
            {
                toRemove.Insert(steamId);
            }
        }
        
        foreach (string removeSteamId : toRemove)
        {
            m_PlayerTrackers.Remove(removeSteamId);
        }
        
        if (toRemove.Count() > 0 && m_Config.logLevel >= 1)
        {
            Print(string.Format("[NT_AntiCheat] Cleaned up %1 disconnected player trackers", 
                toRemove.Count().ToString()));
        }
    }
    
    void PerformAntiCheatChecks()
    {
        if (!m_SystemEnabled)
            return;
            
        array<Man> players = new array<Man>;
        GetGame().GetWorld().GetPlayerList(players);
        
        foreach (Man player : players)
        {
            PlayerBase pb = PlayerBase.Cast(player);
            if (pb && pb.GetIdentity())
            {
                CheckPlayer(pb);
            }
        }
    }
    
    void CheckPlayer(PlayerBase player)
    {
        if (!player || !player.GetIdentity())
            return;
            
        PlayerIdentity identity = player.GetIdentity();
        string steamId = identity.GetPlainId();
        
        // Skip if whitelisted
        if (IsPlayerWhitelisted(steamId))
        {
            return;
        }
        
        NT_PlayerTracker tracker = m_PlayerTrackers.Get(steamId);
        if (!tracker || !tracker.IsValid())
        {
            return;
        }
        
        // Update tracker with current position
        tracker.UpdatePosition(player.GetPosition());
        
        // Update health data for God Mode detection
        if (m_Config.enableGodModeCheck)
        {
            float health = player.GetHealth01("", "");
            float blood = player.GetHealth01("", "Blood");
            tracker.UpdateHealthData(health, blood);
        }
        
        // Perform checks based on configuration
        bool violationDetected = false;
        string violationType = "";
        float detectedValue = 0;
        
        // Check Speed Hack
        if (m_Config.enableSpeedCheck && !player.IsInVehicle())
        {
            if (CheckSpeedHack(player, tracker))
            {
                violationDetected = true;
                violationType = "SPEED_HACK";
                detectedValue = tracker.lastSpeed;
            }
        }
        
        // Check Fly Hack
        if (!violationDetected && m_Config.enableFlyCheck && !player.IsInVehicle() && !player.IsUnconscious())
        {
            if (CheckFlyHack(player, tracker))
            {
                violationDetected = true;
                violationType = "FLY_HACK";
                detectedValue = tracker.heightAboveGround;
            }
        }
        
        // Check Teleport Hack
        if (!violationDetected && m_Config.enableTeleportCheck)
        {
            if (CheckTeleportHack(player, tracker))
            {
                violationDetected = true;
                violationType = "TELEPORT_HACK";
                detectedValue = tracker.teleportDistance;
            }
        }
        
        // Check God Mode
        if (!violationDetected && m_Config.enableGodModeCheck)
        {
            if (CheckGodMode(player, tracker))
            {
                violationDetected = true;
                violationType = "GOD_MODE";
                detectedValue = tracker.consecutiveFullHealth;
            }
        }
        
        // Handle violation if detected
        if (violationDetected)
        {
            HandleViolation(identity, tracker, violationType, detectedValue);
        }
    }
    
    bool CheckSpeedHack(PlayerBase player, NT_PlayerTracker tracker)
    {
        if (tracker.lastSpeed > m_Config.maxSpeed)
        {
            tracker.AddViolation("SPEED");
            
            if (tracker.speedViolations >= m_Config.maxViolationsBeforeBan)
            {
                LogViolation("SPEED_HACK", tracker.playerName, tracker.steamId, 
                    string.Format("Speed: %.2f m/s (Max: %.2f) - Violation #%1", 
                    tracker.lastSpeed, m_Config.maxSpeed, tracker.speedViolations.ToString()));
                return true;
            }
            else
            {
                LogViolation("SPEED_WARNING", tracker.playerName, tracker.steamId, 
                    string.Format("Speed: %.2f m/s (Max: %.2f) - Warning %1/%2", 
                    tracker.lastSpeed, m_Config.maxSpeed, tracker.speedViolations.ToString(), m_Config.maxViolationsBeforeBan.ToString()));
            }
        }
        else if (tracker.speedViolations > 0)
        {
            // Reset violation counter if speed is now within limits
            tracker.ResetViolation("SPEED");
            if (m_Config.logLevel >= 2)
            {
                LogViolation("SPEED_CLEARED", tracker.playerName, tracker.steamId, "Speed violations cleared");
            }
        }
        
        return false;
    }
    
    bool CheckFlyHack(PlayerBase player, NT_PlayerTracker tracker)
    {
        if (tracker.heightAboveGround > m_Config.maxHeight)
        {
            tracker.AddViolation("FLY");
            
            if (tracker.flyViolations >= m_Config.maxViolationsBeforeBan)
            {
                LogViolation("FLY_HACK", tracker.playerName, tracker.steamId,
                    string.Format("Height: %.2f m (Max: %.2f) - Violation #%1", 
                    tracker.heightAboveGround, m_Config.maxHeight, tracker.flyViolations.ToString()));
                return true;
            }
            else
            {
                LogViolation("FLY_WARNING", tracker.playerName, tracker.steamId,
                    string.Format("Height: %.2f m (Max: %.2f) - Warning %1/%2", 
                    tracker.heightAboveGround, m_Config.maxHeight, tracker.flyViolations.ToString(), m_Config.maxViolationsBeforeBan.ToString()));
            }
        }
        else if (tracker.flyViolations > 0)
        {
            tracker.ResetViolation("FLY");
            if (m_Config.logLevel >= 2)
            {
                LogViolation("FLY_CLEARED", tracker.playerName, tracker.steamId, "Fly violations cleared");
            }
        }
        
        return false;
    }
    
    bool CheckTeleportHack(PlayerBase player, NT_PlayerTracker tracker)
    {
        float deltaTime = tracker.currentCheckTime - tracker.lastCheckTime;
        
        if (deltaTime > 0 && deltaTime < 5.0) // Ignore large time gaps
        {
            float maxPossibleDistance = m_Config.maxSpeed * deltaTime * 1.5; // Allow 50% tolerance
            
            if (tracker.teleportDistance > maxPossibleDistance && tracker.teleportDistance > m_Config.maxTeleportDistance)
            {
                tracker.AddViolation("TELEPORT");
                
                if (tracker.teleportViolations >= m_Config.maxViolationsBeforeBan)
                {
                    LogViolation("TELEPORT_HACK", tracker.playerName, tracker.steamId,
                        string.Format("Distance: %.2f m in %.2fs (Max possible: %.2f) - Violation #%1", 
                        tracker.teleportDistance, deltaTime, maxPossibleDistance, tracker.teleportViolations.ToString()));
                    return true;
                }
                else
                {
                    LogViolation("TELEPORT_WARNING", tracker.playerName, tracker.steamId,
                        string.Format("Distance: %.2f m in %.2fs (Max possible: %.2f) - Warning %1/%2", 
                        tracker.teleportDistance, deltaTime, maxPossibleDistance, tracker.teleportViolations.ToString(), m_Config.maxViolationsBeforeBan.ToString()));
                }
            }
        }
        
        return false;
    }
    
    bool CheckGodMode(PlayerBase player, NT_PlayerTracker tracker)
    {
        // God Mode detection is complex and can have false positives
        // This is a basic implementation
        if (tracker.consecutiveFullHealth > 50 && tracker.consecutiveFullBlood > 50) // 50+ checks with full health/blood
        {
            tracker.AddViolation("GODMODE");
            
            if (tracker.godModeViolations >= m_Config.maxViolationsBeforeBan * 2) // Require more violations for God Mode
            {
                LogViolation("GOD_MODE", tracker.playerName, tracker.steamId,
                    string.Format("Consecutive full health: %1, full blood: %2", 
                    tracker.consecutiveFullHealth.ToString(), tracker.consecutiveFullBlood.ToString()));
                return true;
            }
        }
        
        return false;
    }
    
    void HandleViolation(PlayerIdentity identity, NT_PlayerTracker tracker, string violationType, float detectedValue)
    {
        m_TotalViolationsDetected++;
        
        // Check if player should be banned
        if (tracker.ShouldBeBanned(m_Config.maxViolationsBeforeBan))
        {
            if (m_Config.autoBanEnabled)
            {
                BanPlayer(identity, GetViolationReason(violationType), violationType, detectedValue, tracker.totalViolations);
            }
            else if (m_Config.autoKickEnabled)
            {
                KickPlayer(identity, GetViolationReason(violationType));
            }
        }
    }
    
    string GetViolationReason(string violationType)
    {
        switch (violationType)
        {
            case "SPEED_HACK":
                return "Speed Hack Detected - Moving at impossible speed";
            case "FLY_HACK":
                return "Fly Hack Detected - Flying/Hovering without vehicle";
            case "TELEPORT_HACK":
                return "Teleport Hack Detected - Instant position changes";
            case "GOD_MODE":
                return "God Mode Detected - Invincibility/Immortality hack";
            case "NOCLIP_HACK":
                return "NoClip Hack Detected - Clipping through objects";
            default:
                return "Cheat/Hack Detected - Unauthorized game modification";
        }
    }
    
    void BanPlayer(PlayerIdentity identity, string reason, string banType, float detectedValue, int violationCount = 0)
    {
        string steamId = identity.GetPlainId();
        string playerName = identity.GetName();
        
        // Double check whitelist before banning
        if (IsPlayerWhitelisted(steamId))
        {
            Print(string.Format("[NT_AntiCheat] CRITICAL ERROR: Attempted to ban whitelisted admin: %1 (%2) - This should never happen!", 
                playerName, steamId));
            LogToFile("SYSTEM_ERROR", playerName, steamId, "Attempted to ban whitelisted admin - BLOCKED");
            return;
        }
        
        // Create ban record
        NT_BannedPlayer newBan = new NT_BannedPlayer();
        newBan.InitializeBan(steamId, playerName, reason, banType, detectedValue);
        newBan.violationCount = violationCount;
        
        m_BannedPlayers.Insert(newBan);
        m_TotalPlayersBanned++;
        
        // Save ban list immediately
        SaveBanList();
        
        // Create detailed kick message
        string kickMessage = newBan.GetFullBanInfo();
        
        // Log the ban
        string logMessage = string.Format("[NT_AntiCheat] *** PLAYER BANNED *** %1 (%2) | Reason: %3 | Type: %4 | Value: %.2f | Violations: %5 | Ban ID: %6",
            playerName, steamId, reason, banType, detectedValue, violationCount.ToString(), newBan.banId);
        
        Print(logMessage);
        GetGame().AdminLog(logMessage);
        LogToFile("PLAYER_BANNED", playerName, steamId, 
            string.Format("Ban ID: %1 | Reason: %2 | Violations: %3 | Value: %.2f", 
            newBan.banId, reason, violationCount.ToString(), detectedValue));
        
        // Kick player
        GetGame().AdminKick(identity, kickMessage);
        
        // Remove tracker
        m_PlayerTrackers.Remove(steamId);
        
        Print(string.Format("[NT_AntiCheat] Player %1 (%2) has been permanently banned - Ban ID: %3", 
            playerName, steamId, newBan.banId));
    }
    
    void KickPlayer(PlayerIdentity identity, string reason)
    {
        string steamId = identity.GetPlainId();
        string playerName = identity.GetName();
        
        string kickMessage = string.Format("=== NT ANTICHEAT WARNING ===\nYou have been kicked from the server\n\nPlayer: %1\nSteamID: %2\nReason: %3\n\nYou may reconnect, but continued violations will result in a permanent ban.", 
            playerName, steamId, reason);
        
        Print(string.Format("[NT_AntiCheat] Player kicked: %1 (%2) - %3", playerName, steamId, reason));
        LogToFile("PLAYER_KICKED", playerName, steamId, reason);
        
        GetGame().AdminKick(identity, kickMessage);
    }
    
    bool IsPlayerWhitelisted(string steamId)
    {
        foreach (NT_AdminWhitelist admin : m_AdminWhitelist)
        {
            if (admin && admin.steamId == steamId && admin.isActive)
            {
                return true;
            }
        }
        return false;
    }
    
    NT_AdminWhitelist GetWhitelistEntry(string steamId)
    {
        foreach (NT_AdminWhitelist admin : m_AdminWhitelist)
        {
            if (admin && admin.steamId == steamId)
            {
                return admin;
            }
        }
        return null;
    }
    
    bool IsPlayerBanned(string steamId)
    {
        foreach (NT_BannedPlayer banned : m_BannedPlayers)
        {
            if (banned && banned.steamId == steamId && banned.IsValidBan())
            {
                return true;
            }
        }
        return false;
    }
    
    NT_BannedPlayer GetBannedPlayer(string steamId)
    {
        foreach (NT_BannedPlayer banned : m_BannedPlayers)
        {
            if (banned && banned.steamId == steamId && banned.IsValidBan())
            {
                return banned;
            }
        }
        return null;
    }
    
    void LogViolation(string violationType, string playerName, string steamId, string details)
    {
        if (m_Config.logLevel >= 1)
        {
            string logMsg = string.Format("[NT_AntiCheat] %1: %2 (%3) - %4", 
                violationType, playerName, steamId, details);
            Print(logMsg);
            
            if (m_Config.logLevel >= 2)
            {
                GetGame().AdminLog(logMsg);
                LogToFile("VIOLATION_" + violationType, playerName, steamId, details);
            }
        }
    }
    
    void LogToFile(string eventType, string playerName, string steamId, string details)
    {
        if (m_Config.logLevel < 2)
            return;
            
        // Create log filename with current date
        int year, month, day;
        GetYearMonthDay(year, month, day);
        
        string logFileName = string.Format("anticheat_%1-%2-%3.log", 
            year.ToString(), AddLeadingZero(month), AddLeadingZero(day));
        string logFilePath = m_LogPath + logFileName;
        
        // Create log entry
        int hour, minute, second;
        GetHourMinuteSecond(hour, minute, second);
        
        string timestamp = string.Format("%1-%2-%3 %4:%5:%6", 
            year.ToString(), AddLeadingZero(month), AddLeadingZero(day),
            AddLeadingZero(hour), AddLeadingZero(minute), AddLeadingZero(second));
        
        string logEntry = string.Format("[%1] %2 | Player: %3 | SteamID: %4 | Details: %5\n",
            timestamp, eventType, playerName, steamId, details);
        
        // Write to file
        FileHandle file = OpenFile(logFilePath, FileMode.APPEND);
        if (file)
        {
            FPrintln(file, logEntry);
            CloseFile(file);
        }
    }
    
    string AddLeadingZero(int value)
    {
        if (value < 10)
            return "0" + value.ToString();
        return value.ToString();
    }
    
    void LogStatistics()
    {
        int activeTrackers = 0;
        int totalViolations = 0;
        int whitelistedPlayers = 0;
        
        foreach (string steamId, NT_PlayerTracker tracker : m_PlayerTrackers)
        {
            if (tracker && tracker.IsValid())
            {
                activeTrackers++;
                totalViolations += tracker.totalViolations;
                if (tracker.isWhitelisted)
                {
                    whitelistedPlayers++;
                }
            }
        }
        
        string statsMsg = string.Format("[NT_AntiCheat] Statistics - Players Online: %1 | Whitelisted: %2 | Total Violations: %3 | Players Banned: %4 | System Uptime: %.1f min",
            activeTrackers.ToString(), whitelistedPlayers.ToString(), 
            totalViolations.ToString(), m_TotalPlayersBanned.ToString(),
            (GetGame().GetTime() / 60.0));
        
        Print(statsMsg);
        
        if (m_Config.logLevel >= 2)
        {
            LogToFile("SYSTEM_STATISTICS", "SYSTEM", "N/A", statsMsg);
        }
    }
    
    void BackupFiles()
    {
        // Create backup timestamp
        int year, month, day, hour, minute, second;
        GetYearMonthDay(year, month, day);
        GetHourMinuteSecond(hour, minute, second);
        
        string timestamp = string.Format("%1%2%3_%4%5%6", 
            year.ToString(), AddLeadingZero(month), AddLeadingZero(day),
            AddLeadingZero(hour), AddLeadingZero(minute), AddLeadingZero(second));
        
        string backupPath = "$profile:/nt-anticheat/backups/";
        
        // Backup ban list
        if (FileExist(m_BanListPath))
        {
            string backupBanFile = backupPath + "banlist_" + timestamp + ".json";
            CopyFile(m_BanListPath, backupBanFile);
        }
        
        // Backup whitelist  
        if (FileExist(m_WhitelistPath))
        {
            string backupWhitelistFile = backupPath + "whitelist_" + timestamp + ".json";
            CopyFile(m_WhitelistPath, backupWhitelistFile);
        }
        
        // Backup config
        if (FileExist(m_ConfigPath))
        {
            string backupConfigFile = backupPath + "config_" + timestamp + ".json";
            CopyFile(m_ConfigPath, backupConfigFile);
        }
        
        if (m_Config.logLevel >= 2)
        {
            Print("[NT_AntiCheat] Backup files created with timestamp: " + timestamp);
        }
    }
    
    // Public methods for external access
    bool IsSystemEnabled()
    {
        return m_SystemEnabled;
    }
    
    void SetSystemEnabled(bool enabled)
    {
        m_SystemEnabled = enabled;
        string status = enabled ? "ENABLED" : "DISABLED";
        Print("[NT_AntiCheat] System status changed to: " + status);
        LogToFile("SYSTEM_STATUS_CHANGE", "ADMIN", "N/A", "System " + status);
    }
    
    void ReloadConfiguration()
    {
        Print("[NT_AntiCheat] Reloading configuration...");
        LoadAllConfigurations();
        Print("[NT_AntiCheat] Configuration reloaded successfully");
    }
    
    void AddPlayerToWhitelist(string steamId, string playerName, string reason, string addedBy = "Admin")
    {
        // Check if already exists
        if (IsPlayerWhitelisted(steamId))
        {
            Print("[NT_AntiCheat] Player already in whitelist: " + steamId);
            return;
        }
        
        NT_AdminWhitelist newAdmin = new NT_AdminWhitelist();
        newAdmin.InitializeWhitelist(steamId, playerName, reason, addedBy);
        
        m_AdminWhitelist.Insert(newAdmin);
        SaveWhitelist();
        
        Print(string.Format("[NT_AntiCheat] Added to whitelist: %1 (%2) by %3", 
            playerName, steamId, addedBy));
            
        LogToFile("WHITELIST_ADDED", playerName, steamId, 
            string.Format("Added by: %1 | Reason: %2", addedBy, reason));
    }
    
    void RemovePlayerFromWhitelist(string steamId)
    {
        for (int i = m_AdminWhitelist.Count() - 1; i >= 0; i--)
        {
            NT_AdminWhitelist admin = m_AdminWhitelist.Get(i);
            if (admin && admin.steamId == steamId)
            {
                string playerName = admin.playerName;
                m_AdminWhitelist.RemoveOrdered(i);
                SaveWhitelist();
                
                Print(string.Format("[NT_AntiCheat] Removed from whitelist: %1 (%2)", 
                    playerName, steamId));
                    
                LogToFile("WHITELIST_REMOVED", playerName, steamId, "Removed from whitelist");
                return;
            }
        }
        
        Print("[NT_AntiCheat] Player not found in whitelist: " + steamId);
    }
    
    void UnbanPlayer(string steamId)
    {
        for (int i = m_BannedPlayers.Count() - 1; i >= 0; i--)
        {
            NT_BannedPlayer banned = m_BannedPlayers.Get(i);
            if (banned && banned.steamId == steamId)
            {
                string playerName = banned.playerName;
                string banId = banned.banId;
                
                m_BannedPlayers.RemoveOrdered(i);
                SaveBanList();
                
                Print(string.Format("[NT_AntiCheat] Unbanned player: %1 (%2) - Ban ID: %3", 
                    playerName, steamId, banId));
                    
                LogToFile("PLAYER_UNBANNED", playerName, steamId, 
                    string.Format("Ban ID: %1 | Unbanned by admin", banId));
                return;
            }
        }
        
        Print("[NT_AntiCheat] Player not found in ban list: " + steamId);
    }
    
    string GetSystemInfo()
    {
        string info = "=== NT ANTICHEAT SYSTEM INFO ===\n";
        info += "Version: 2.0\n";
        info += "Status: " + (m_SystemEnabled ? "ENABLED" : "DISABLED") + "\n";
        info += "Total Players Checked: " + m_TotalPlayersChecked.ToString() + "\n";
        info += "Total Violations Detected: " + m_TotalViolationsDetected.ToString() + "\n";
        info += "Total Players Banned: " + m_TotalPlayersBanned.ToString() + "\n";
        info += "Active Player Trackers: " + m_PlayerTrackers.Count().ToString() + "\n";
        info += "Whitelisted Admins: " + m_AdminWhitelist.Count().ToString() + "\n";
        info += "Banned Players: " + m_BannedPlayers.Count().ToString() + "\n";
        info += "Check Interval: " + m_Config.checkInterval.ToString() + " seconds\n";
        info += "Max Speed: " + m_Config.maxSpeed.ToString() + " m/s\n";
        info += "Max Height: " + m_Config.maxHeight.ToString() + " meters\n";
        info += "Log Level: " + m_Config.logLevel.ToString() + "\n";
        info += "Auto Ban: " + (m_Config.autoBanEnabled ? "ENABLED" : "DISABLED") + "\n";
        info += "===============================\n";
        
        return info;
    }
};