modded class MissionServer
{
    protected ref NT_AntiCheatManager m_AntiCheatManager;
    protected bool m_AntiCheatInitialized;
    
    override void OnInit()
    {
        super.OnInit();
        
        Print("=================================================");
        Print("[NT_AntiCheat] Initializing AntiCheat System...");
        Print("=================================================");
        
        // Initialize AntiCheat System
        m_AntiCheatManager = new NT_AntiCheatManager();
        m_AntiCheatInitialized = false;
        
        if (m_AntiCheatManager)
        {
            m_AntiCheatInitialized = true;
            Print("[NT_AntiCheat] AntiCheat Manager created successfully!");
            
            // Delay the config initialization to ensure proper server startup
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
            
            // Display system information
            string systemInfo = m_AntiCheatManager.GetSystemInfo();
            Print(systemInfo);
        }
        else
        {
            Print("[NT_AntiCheat] ERROR: AntiCheat Manager not available for delayed initialization!");
        }
    }
    
    override void OnClientConnectedEvent(PlayerIdentity identity, PlayerBase player)
    {
        super.OnClientConnectedEvent(identity, player);
        
        if (m_AntiCheatManager && m_AntiCheatInitialized && identity && player)
        {
            // Add a small delay to ensure player is fully loaded
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.HandlePlayerConnection, 1000, false, identity, player);
        }
        else if (!m_AntiCheatInitialized)
        {
            Print("[NT_AntiCheat] WARNING: Player connected but AntiCheat not yet initialized");
        }
    }
    
    void HandlePlayerConnection(PlayerIdentity identity, PlayerBase player)
    {
        if (m_AntiCheatManager && identity && player)
        {
            try
            {
                m_AntiCheatManager.OnPlayerConnect(identity, player);
            }
            catch
            {
                Print("[NT_AntiCheat] ERROR: Exception occurred during player connection handling");
            }
        }
    }
    
    override void OnClientDisconnectedEvent(PlayerIdentity identity, PlayerBase player, int logoutTime, bool authFailed)
    {
        super.OnClientDisconnectedEvent(identity, player, logoutTime, authFailed);
        
        if (m_AntiCheatManager && m_AntiCheatInitialized && identity)
        {
            try
            {
                m_AntiCheatManager.OnPlayerDisconnect(identity);
            }
            catch
            {
                Print("[NT_AntiCheat] ERROR: Exception occurred during player disconnection handling");
            }
        }
    }
    
    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
        
        if (m_AntiCheatManager && m_AntiCheatInitialized)
        {
            try
            {
                m_AntiCheatManager.Update(timeslice);
            }
            catch
            {
                Print("[NT_AntiCheat] ERROR: Exception occurred during update cycle");
                // Optionally disable the system if there are persistent errors
                // m_AntiCheatManager.SetSystemEnabled(false);
            }
        }
    }
    
    // Admin commands integration (optional)
    override void OnClientRespawnEvent(PlayerIdentity identity, PlayerBase player)
    {
        super.OnClientRespawnEvent(identity, player);
        
        if (m_AntiCheatManager && m_AntiCheatInitialized && identity && player)
        {
            // Re-initialize tracking for respawned player
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.HandlePlayerRespawn, 2000, false, identity, player);
        }
    }
    
    void HandlePlayerRespawn(PlayerIdentity identity, PlayerBase player)
    {
        if (m_AntiCheatManager && identity && player)
        {
            string steamId = identity.GetPlainId();
            Print(string.Format("[NT_AntiCheat] Player respawned, reinitializing tracker: %1 (%2)", 
                identity.GetName(), steamId));
                
            // The tracker will be updated automatically on the next update cycle
        }
    }
    
    // Shutdown handler
    override void OnMissionFinish()
    {
        if (m_AntiCheatManager)
        {
            Print("[NT_AntiCheat] Mission ending - saving final data...");
            
            // Force save all data before shutdown
            try
            {
                // This would need to be implemented in the manager if needed
                // m_AntiCheatManager.ForceDataSave();
            }
            catch
            {
                Print("[NT_AntiCheat] ERROR: Exception during final data save");
            }
        }
        
        super.OnMissionFinish();
    }
    
    // Public methods for admin commands or other mods to interact with
    NT_AntiCheatManager GetAntiCheatManager()
    {
        return m_AntiCheatManager;
    }
    
    bool IsAntiCheatInitialized()
    {
        return m_AntiCheatInitialized && m_AntiCheatManager;
    }
    
    void AdminCommand_ReloadAntiCheat(PlayerIdentity identity)
    {
        if (m_AntiCheatManager && IsAdminPlayer(identity))
        {
            Print("[NT_AntiCheat] Admin " + identity.GetName() + " requested configuration reload");
            m_AntiCheatManager.ReloadConfiguration();
        }
    }
    
    void AdminCommand_AntiCheatStatus(PlayerIdentity identity)
    {
        if (m_AntiCheatManager && IsAdminPlayer(identity))
        {
            string info = m_AntiCheatManager.GetSystemInfo();
            Print(info);
            // You could also send this info directly to the admin if you have a messaging system
        }
    }
    
    void AdminCommand_ToggleAntiCheat(PlayerIdentity identity, bool enabled)
    {
        if (m_AntiCheatManager && IsAdminPlayer(identity))
        {
            m_AntiCheatManager.SetSystemEnabled(enabled);
            Print(string.Format("[NT_AntiCheat] Admin %1 %2 the AntiCheat system", 
                identity.GetName(), enabled ? "enabled" : "disabled"));
        }
    }
    
    void AdminCommand_WhitelistPlayer(PlayerIdentity identity, string targetSteamId, string targetPlayerName, string reason)
    {
        if (m_AntiCheatManager && IsAdminPlayer(identity))
        {
            m_AntiCheatManager.AddPlayerToWhitelist(targetSteamId, targetPlayerName, reason, identity.GetName());
        }
    }
    
    void AdminCommand_UnWhitelistPlayer(PlayerIdentity identity, string targetSteamId)
    {
        if (m_AntiCheatManager && IsAdminPlayer(identity))
        {
            m_AntiCheatManager.RemovePlayerFromWhitelist(targetSteamId);
        }
    }
    
    void AdminCommand_UnbanPlayer(PlayerIdentity identity, string targetSteamId)
    {
        if (m_AntiCheatManager && IsAdminPlayer(identity))
        {
            m_AntiCheatManager.UnbanPlayer(targetSteamId);
        }
    }
    
    // Helper method to check if player is admin - you might need to adjust this based on your admin system
    bool IsAdminPlayer(PlayerIdentity identity)
    {
        if (!identity)
            return false;
            
        string steamId = identity.GetPlainId();
        
        // Check if player is in whitelist (assuming whitelisted = admin)
        if (m_AntiCheatManager)
        {
            return m_AntiCheatManager.IsPlayerWhitelisted(steamId);
        }
        
        // Alternative: Check against server admin list or your custom admin system
        // return GetGame().IsPlayerListAdmin(identity);
        
        return false;
    }
};