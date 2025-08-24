modded class MissionServer
{
    protected ref NT_AntiCheatManager m_AntiCheatManager;
    protected bool m_AntiCheatInitialized;
    
    override void OnInit()
    {
        super.OnInit();
        
        Print("[NT_AntiCheat] Initializing AntiCheat System...");
        
        m_AntiCheatManager = new NT_AntiCheatManager();
        m_AntiCheatInitialized = false;
        
        if (m_AntiCheatManager)
        {
            m_AntiCheatInitialized = true;
            Print("[NT_AntiCheat] AntiCheat Manager created successfully!");
            
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.DelayedInitialization, 3000, false);
        }
        else
        {
            Print("[NT_AntiCheat] ERROR: Failed to create AntiCheat Manager!");
        }
    }
    
    void DelayedInitialization()
    {
        if (m_AntiCheatManager)
        {
            m_AntiCheatManager.InitializeDefaultConfigs();
            
            Print("=================================================");
            Print("[NT_AntiCheat] System Successfully Initialized!");
            Print("[NT_AntiCheat] Version: 2.0");
            Print("[NT_AntiCheat] Author: NT Development");
            Print("[NT_AntiCheat] Status: ACTIVE");
            Print("=================================================");
            
            string systemInfo = m_AntiCheatManager.GetSystemInfo();
            Print(systemInfo);
        }
        else
        {
            Print("[NT_AntiCheat] ERROR: AntiCheat Manager not available!");
        }
    }
    
    override PlayerBase OnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx)
    {
        PlayerBase player = super.OnClientNewEvent(identity, pos, ctx);
        
        if (m_AntiCheatManager)
        {
            if (m_AntiCheatInitialized)
            {
                if (identity)
                {
                    if (player)
                    {
                        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.HandlePlayerConnection, 2000, false, identity, player);
                    }
                }
            }
        }
        
        return player;
    }
    
    void HandlePlayerConnection(PlayerIdentity identity, PlayerBase player)
    {
        if (m_AntiCheatManager)
        {
            if (identity)
            {
                if (player)
                {
                    m_AntiCheatManager.OnPlayerConnect(identity, player);
                }
            }
        }
    }
    
    override void OnClientDisconnectedEvent(PlayerIdentity identity, PlayerBase player, int logoutTime, bool authFailed)
    {
        super.OnClientDisconnectedEvent(identity, player, logoutTime, authFailed);
        
        if (m_AntiCheatManager)
        {
            if (m_AntiCheatInitialized)
            {
                if (identity)
                {
                    m_AntiCheatManager.OnPlayerDisconnect(identity);
                }
            }
        }
    }
    
    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        
        if (m_AntiCheatManager)
        {
            if (m_AntiCheatInitialized)
            {
                m_AntiCheatManager.Update(timeslice);
            }
        }
    }
    
    override void OnClientRespawnEvent(PlayerIdentity identity, PlayerBase player)
    {
        super.OnClientRespawnEvent(identity, player);
        
        if (m_AntiCheatManager)
        {
            if (m_AntiCheatInitialized)
            {
                if (identity)
                {
                    if (player)
                    {
                        string steamId = identity.GetPlainId();
                        string playerName = identity.GetName();
                        Print("[NT_AntiCheat] Player respawned: " + playerName + " (" + steamId + ")");
                    }
                }
            }
        }
    }
    
    override void OnMissionFinish()
    {
        if (m_AntiCheatManager)
        {
            Print("[NT_AntiCheat] Mission ending - saving final data...");
        }
        
        super.OnMissionFinish();
    }
    
    NT_AntiCheatManager GetAntiCheatManager()
    {
        return m_AntiCheatManager;
    }
    
    bool IsAntiCheatInitialized()
    {
        if (m_AntiCheatInitialized)
        {
            if (m_AntiCheatManager)
            {
                return true;
            }
        }
        return false;
    }
    
    void AdminCommand_ReloadAntiCheat(PlayerIdentity identity)
    {
        if (m_AntiCheatManager)
        {
            if (IsAdminPlayer(identity))
            {
                string adminName = identity.GetName();
                Print("[NT_AntiCheat] Admin " + adminName + " requested configuration reload");
                m_AntiCheatManager.ReloadConfiguration();
            }
        }
    }
    
    void AdminCommand_AntiCheatStatus(PlayerIdentity identity)
    {
        if (m_AntiCheatManager)
        {
            if (IsAdminPlayer(identity))
            {
                string info = m_AntiCheatManager.GetSystemInfo();
                Print(info);
            }
        }
    }
    
    void AdminCommand_ToggleAntiCheat(PlayerIdentity identity, bool enabled)
    {
        if (m_AntiCheatManager)
        {
            if (IsAdminPlayer(identity))
            {
                m_AntiCheatManager.SetSystemEnabled(enabled);
                string adminName = identity.GetName();
                string status;
                if (enabled)
                {
                    status = "enabled";
                }
                else
                {
                    status = "disabled";
                }
                Print("[NT_AntiCheat] Admin " + adminName + " " + status + " the AntiCheat system");
            }
        }
    }
    
    void AdminCommand_WhitelistPlayer(PlayerIdentity identity, string targetSteamId, string targetPlayerName, string reason)
    {
        if (m_AntiCheatManager)
        {
            if (IsAdminPlayer(identity))
            {
                string adminName = identity.GetName();
                m_AntiCheatManager.AddPlayerToWhitelist(targetSteamId, targetPlayerName, reason, adminName);
            }
        }
    }
    
    void AdminCommand_UnWhitelistPlayer(PlayerIdentity identity, string targetSteamId)
    {
        if (m_AntiCheatManager)
        {
            if (IsAdminPlayer(identity))
            {
                m_AntiCheatManager.RemovePlayerFromWhitelist(targetSteamId);
            }
        }
    }
    
    void AdminCommand_UnbanPlayer(PlayerIdentity identity, string targetSteamId)
    {
        if (m_AntiCheatManager)
        {
            if (IsAdminPlayer(identity))
            {
                m_AntiCheatManager.UnbanPlayer(targetSteamId);
            }
        }
    }
    
    bool IsAdminPlayer(PlayerIdentity identity)
    {
        if (!identity)
            return false;
            
        string steamId = identity.GetPlainId();
        
        if (m_AntiCheatManager)
        {
            return m_AntiCheatManager.IsPlayerWhitelisted(steamId);
        }
        
        return false;
    }
};