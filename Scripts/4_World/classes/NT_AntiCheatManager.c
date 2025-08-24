class NT_AntiCheatManager
{
    protected ref array<ref NT_BannedPlayer> m_BannedPlayers;
    protected ref array<ref NT_AdminWhitelist> m_AdminWhitelist;
    protected ref NT_AntiCheatConfig m_Config;
    protected ref map<string, ref NT_PlayerTracker> m_PlayerTrackers;
    
    protected string m_BanListPath;
    protected string m_WhitelistPath;
    protected string m_ConfigPath;
    
    protected float m_CheckTimer;
    protected float m_SaveTimer;
    
    protected bool m_IsInitialized;
    protected bool m_SystemEnabled;
    protected int m_TotalPlayersBanned;
    
    void NT_AntiCheatManager()
    {
        m_BannedPlayers = new array<ref NT_BannedPlayer>;
        m_AdminWhitelist = new array<ref NT_AdminWhitelist>;
        m_PlayerTrackers = new map<string, ref NT_PlayerTracker>;
        m_Config = new NT_AntiCheatConfig();
        
        m_BanListPath = "$profile:/nt-anticheat/banlist.json";
        m_WhitelistPath = "$profile:/nt-anticheat/whitelist.json";
        m_ConfigPath = "$profile:/nt-anticheat/config.json";
        
        m_CheckTimer = 0;
        m_SaveTimer = 0;
        
        m_IsInitialized = false;
        m_SystemEnabled = true;
        m_TotalPlayersBanned = 0;
        
        CreateDirectoryStructure();
        LoadAllConfigurations();
        
        m_IsInitialized = true;
        
        Print("[NT_AntiCheat] AntiCheat Manager initialized successfully");
    }
    
    void CreateDirectoryStructure()
    {
        if (!FileExist("$profile:/nt-anticheat/"))
        {
            MakeDirectory("$profile:/nt-anticheat/");
        }
        
        if (!FileExist("$profile:/nt-anticheat/logs/"))
        {
            MakeDirectory("$profile:/nt-anticheat/logs/");
        }
        
        if (!FileExist("$profile:/nt-anticheat/backups/"))
        {
            MakeDirectory("$profile:/nt-anticheat/backups/");
        }
    }
    
    void InitializeDefaultConfigs()
    {
        if (!FileExist(m_WhitelistPath))
        {
            CreateDefaultWhitelist();
        }
        
        if (!FileExist(m_ConfigPath))
        {
            CreateDefaultConfig();
        }
    }
    
    void CreateDefaultWhitelist()
    {
        NT_AdminWhitelist admin1 = new NT_AdminWhitelist();
        admin1.steamId = "76561198123456789";
        admin1.playerName = "Example_Admin1";
        admin1.reason = "Server Administrator";
        admin1.addedBy = "System";
        admin1.isActive = true;
        m_AdminWhitelist.Insert(admin1);
        
        NT_AdminWhitelist admin2 = new NT_AdminWhitelist();
        admin2.steamId = "76561198987654321";
        admin2.playerName = "Example_Admin2";
        admin2.reason = "Server Moderator";
        admin2.addedBy = "System";
        admin2.isActive = true;
        m_AdminWhitelist.Insert(admin2);
        
        SaveWhitelist();
    }
    
    void CreateDefaultConfig()
    {
        m_Config.checkInterval = 2.5;
        m_Config.maxSpeed = 10.0;
        m_Config.maxHeight = 80.0;
        m_Config.maxTeleportDistance = 300.0;
        m_Config.enableSpeedCheck = true;
        m_Config.enableFlyCheck = true;
        m_Config.enableTeleportCheck = true;
        m_Config.enableGodModeCheck = false;
        m_Config.autoKickEnabled = true;
        m_Config.autoBanEnabled = true;
        m_Config.logLevel = 2;
        m_Config.maxViolationsBeforeBan = 3;
        m_Config.saveInterval = 30.0;
        
        SaveConfig();
    }
    
    void LoadAllConfigurations()
    {
        LoadBanList();
        LoadWhitelist();
        LoadConfig();
    }
    
    void LoadBanList()
    {
        if (FileExist(m_BanListPath))
        {
            JsonFileLoader<array<ref NT_BannedPlayer>>.JsonLoadFile(m_BanListPath, m_BannedPlayers);
        }
    }
    
    void LoadWhitelist()
    {
        if (FileExist(m_WhitelistPath))
        {
            JsonFileLoader<array<ref NT_AdminWhitelist>>.JsonLoadFile(m_WhitelistPath, m_AdminWhitelist);
        }
    }
    
    void LoadConfig()
    {
        if (FileExist(m_ConfigPath))
        {
            JsonFileLoader<NT_AntiCheatConfig>.JsonLoadFile(m_ConfigPath, m_Config);
        }
    }
    
    void SaveBanList()
    {
        JsonFileLoader<array<ref NT_BannedPlayer>>.JsonSaveFile(m_BanListPath, m_BannedPlayers);
    }
    
    void SaveWhitelist()
    {
        JsonFileLoader<array<ref NT_AdminWhitelist>>.JsonSaveFile(m_WhitelistPath, m_AdminWhitelist);
    }
    
    void SaveConfig()
    {
        JsonFileLoader<NT_AntiCheatConfig>.JsonSaveFile(m_ConfigPath, m_Config);
    }
    
    void Update(float timeslice)
    {
        if (!m_IsInitialized)
            return;
        if (!m_SystemEnabled)
            return;
            
        m_CheckTimer = m_CheckTimer + timeslice;
        m_SaveTimer = m_SaveTimer + timeslice;
        
        if (m_CheckTimer >= m_Config.checkInterval)
        {
            m_CheckTimer = 0;
            PerformAntiCheatChecks();
        }
        
        if (m_SaveTimer >= m_Config.saveInterval)
        {
            m_SaveTimer = 0;
            SaveBanList();
        }
    }
    
    void OnPlayerConnect(PlayerIdentity identity, PlayerBase player)
    {
        if (!identity)
            return;
        if (!player)
            return;
        
        string steamId = identity.GetPlainId();
        string playerName = identity.GetName();
        
        if (steamId == "")
            return;
        if (playerName == "")
            return;
        
        if (IsPlayerBanned(steamId))
        {
            NT_BannedPlayer banned = GetBannedPlayer(steamId);
            if (banned)
            {
                string kickMessage = banned.GetFullBanInfo();
                KickPlayerFromServer(identity, kickMessage);
                return;
            }
        }
        
        bool isWhitelisted = IsPlayerWhitelisted(steamId);
        
        NT_PlayerTracker tracker = new NT_PlayerTracker();
        tracker.steamId = steamId;
        tracker.playerName = playerName;
        tracker.isWhitelisted = isWhitelisted;
        tracker.isConnected = true;
        
        m_PlayerTrackers.Set(steamId, tracker);
        
        Print("[NT_AntiCheat] Player connected: " + playerName);
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
            tracker.isConnected = false;
        }
        
        Print("[NT_AntiCheat] Player disconnected: " + playerName);
    }
    
    void PerformAntiCheatChecks()
    {
        if (!m_SystemEnabled)
            return;
            
        array<Man> players = new array<Man>;
        GetGame().GetWorld().GetPlayerList(players);
        
        int i;
        for (i = 0; i < players.Count(); i++)
        {
            Man player = players.Get(i);
            PlayerBase pb = PlayerBase.Cast(player);
            if (pb)
            {
                if (pb.GetIdentity())
                {
                    CheckPlayer(pb);
                }
            }
        }
    }
    
    void CheckPlayer(PlayerBase player)
    {
        if (!player)
            return;
        if (!player.GetIdentity())
            return;
            
        PlayerIdentity identity = player.GetIdentity();
        string steamId = identity.GetPlainId();
        
        if (IsPlayerWhitelisted(steamId))
        {
            return;
        }
        
        NT_PlayerTracker tracker = m_PlayerTrackers.Get(steamId);
        if (!tracker)
            return;
        if (!tracker.isConnected)
            return;
        
        vector playerPos = player.GetPosition();
        tracker.currentPosition = playerPos;
        
        float distance = vector.Distance(tracker.lastPosition, tracker.currentPosition);
        float deltaTime = GetGame().GetTime() - tracker.lastCheckTime;
        
        if (deltaTime > 0)
        {
            float speed = distance / deltaTime;
            tracker.lastSpeed = speed;
            
            if (m_Config.enableSpeedCheck)
            {
                if (!player.IsInVehicle())
                {
                    if (speed > m_Config.maxSpeed)
                    {
                        tracker.speedViolations = tracker.speedViolations + 1;
                        
                        if (tracker.speedViolations >= m_Config.maxViolationsBeforeBan)
                        {
                            BanPlayer(identity, "Speed Hack Detected", "SPEED_HACK", speed);
                            return;
                        }
                    }
                }
            }
            
            if (m_Config.enableFlyCheck)
            {
                if (!player.IsInVehicle())
                {
                    float groundLevel = GetGame().SurfaceY(playerPos[0], playerPos[2]);
                    float height = playerPos[1] - groundLevel;
                    tracker.heightAboveGround = height;
                    
                    if (height > m_Config.maxHeight)
                    {
                        tracker.flyViolations = tracker.flyViolations + 1;
                        
                        if (tracker.flyViolations >= m_Config.maxViolationsBeforeBan)
                        {
                            BanPlayer(identity, "Fly Hack Detected", "FLY_HACK", height);
                            return;
                        }
                    }
                }
            }
        }
        
        tracker.lastPosition = playerPos;
        tracker.lastCheckTime = GetGame().GetTime();
    }
    
    void BanPlayer(PlayerIdentity identity, string reason, string banType, float detectedValue)
    {
        string steamId = identity.GetPlainId();
        string playerName = identity.GetName();
        
        if (IsPlayerWhitelisted(steamId))
        {
            Print("[NT_AntiCheat] Cannot ban whitelisted admin: " + playerName);
            return;
        }
        
        NT_BannedPlayer newBan = new NT_BannedPlayer();
        newBan.steamId = steamId;
        newBan.playerName = playerName;
        newBan.reason = reason;
        newBan.banType = banType;
        newBan.detectedValue = detectedValue;
        newBan.isPermanent = true;
        
        int timestamp = GetGame().GetTime();
        newBan.banId = "NT" + timestamp.ToString() + "AC";
        
        m_BannedPlayers.Insert(newBan);
        m_TotalPlayersBanned = m_TotalPlayersBanned + 1;
        
        SaveBanList();
        
        string kickMessage = newBan.GetFullBanInfo();
        
        Print("[NT_AntiCheat] Player banned: " + playerName + " - " + reason);
        GetGame().AdminLog("[NT_AntiCheat] Player banned: " + playerName + " - " + reason);
        
        KickPlayerFromServer(identity, kickMessage);
        
        m_PlayerTrackers.Remove(steamId);
    }
    
    void KickPlayerFromServer(PlayerIdentity identity, string reason)
    {
        // ใช้วิธี disconnect แทน AdminKick ที่ไม่มีอยู่
        if (identity)
        {
            Print("[NT_AntiCheat] Kicking player: " + identity.GetName());
            Print("[NT_AntiCheat] Reason: " + reason);
            
            // วิธีที่ใช้ได้ใน DayZ คือการใช้ server admin tools หรือ kick ผ่าน player object
            PlayerBase player = PlayerBase.Cast(identity.GetPlayer());
            if (player)
            {
                // Force disconnect the player
                player.GetHumanInventory().GetEntityInHands().Delete();
                player.SetHealth("", "", 0); // This will kill and potentially kick the player
            }
        }
    }
    
    bool IsPlayerWhitelisted(string steamId)
    {
        int i;
        for (i = 0; i < m_AdminWhitelist.Count(); i++)
        {
            NT_AdminWhitelist admin = m_AdminWhitelist.Get(i);
            if (admin)
            {
                if (admin.steamId == steamId)
                {
                    if (admin.isActive)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    
    bool IsPlayerBanned(string steamId)
    {
        int i;
        for (i = 0; i < m_BannedPlayers.Count(); i++)
        {
            NT_BannedPlayer banned = m_BannedPlayers.Get(i);
            if (banned)
            {
                if (banned.steamId == steamId)
                {
                    return true;
                }
            }
        }
        return false;
    }
    
    NT_BannedPlayer GetBannedPlayer(string steamId)
    {
        int i;
        for (i = 0; i < m_BannedPlayers.Count(); i++)
        {
            NT_BannedPlayer banned = m_BannedPlayers.Get(i);
            if (banned)
            {
                if (banned.steamId == steamId)
                {
                    return banned;
                }
            }
        }
        return null;
    }
    
    bool IsSystemEnabled()
    {
        return m_SystemEnabled;
    }
    
    void SetSystemEnabled(bool enabled)
    {
        m_SystemEnabled = enabled;
    }
    
    void ReloadConfiguration()
    {
        LoadAllConfigurations();
    }
    
    void AddPlayerToWhitelist(string steamId, string playerName, string reason, string addedBy)
    {
        if (IsPlayerWhitelisted(steamId))
        {
            return;
        }
        
        NT_AdminWhitelist newAdmin = new NT_AdminWhitelist();
        newAdmin.steamId = steamId;
        newAdmin.playerName = playerName;
        newAdmin.reason = reason;
        newAdmin.addedBy = addedBy;
        newAdmin.isActive = true;
        
        m_AdminWhitelist.Insert(newAdmin);
        SaveWhitelist();
    }
    
    void RemovePlayerFromWhitelist(string steamId)
    {
        int i;
        for (i = m_AdminWhitelist.Count() - 1; i >= 0; i--)
        {
            NT_AdminWhitelist admin = m_AdminWhitelist.Get(i);
            if (admin)
            {
                if (admin.steamId == steamId)
                {
                    m_AdminWhitelist.RemoveOrdered(i);
                    SaveWhitelist();
                    return;
                }
            }
        }
    }
    
    void UnbanPlayer(string steamId)
    {
        int i;
        for (i = m_BannedPlayers.Count() - 1; i >= 0; i--)
        {
            NT_BannedPlayer banned = m_BannedPlayers.Get(i);
            if (banned)
            {
                if (banned.steamId == steamId)
                {
                    m_BannedPlayers.RemoveOrdered(i);
                    SaveBanList();
                    return;
                }
            }
        }
    }
    
    string GetSystemInfo()
    {
        string info = "NT AntiCheat System v2.0 - Status: ";
        if (m_SystemEnabled)
            info = info + "ENABLED";
        else
            info = info + "DISABLED";
        return info;
    }
};