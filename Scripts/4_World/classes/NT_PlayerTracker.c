class NT_PlayerTracker
{
    string steamId;
    string playerName;
    vector lastPosition;
    vector currentPosition;
    float lastCheckTime;
    float currentCheckTime;
    float lastSpeed;
    float maxSpeedRecorded;
    float heightAboveGround;
    float maxHeightRecorded;
    float teleportDistance;
    float maxTeleportDistance;
    bool isWhitelisted;
    bool isConnected;
    
    // Violation counters
    int speedViolations;
    int flyViolations;
    int teleportViolations;
    int godModeViolations;
    int noClipViolations;
    int totalViolations;
    
    // Tracking data
    float connectionTime;
    int checksPerformed;
    float averageSpeed;
    ref array<float> speedHistory;
    ref array<vector> positionHistory;
    
    // Health tracking for God Mode detection
    float lastHealthCheck;
    float lastBloodCheck;
    int consecutiveFullHealth;
    int consecutiveFullBlood;
    
    void NT_PlayerTracker()
    {
        steamId = "";
        playerName = "";
        lastPosition = Vector(0, 0, 0);
        currentPosition = Vector(0, 0, 0);
        lastCheckTime = 0;
        currentCheckTime = 0;
        lastSpeed = 0;
        maxSpeedRecorded = 0;
        heightAboveGround = 0;
        maxHeightRecorded = 0;
        teleportDistance = 0;
        maxTeleportDistance = 0;
        isWhitelisted = false;
        isConnected = false;
        
        // Initialize violation counters
        speedViolations = 0;
        flyViolations = 0;
        teleportViolations = 0;
        godModeViolations = 0;
        noClipViolations = 0;
        totalViolations = 0;
        
        // Initialize tracking data
        connectionTime = GetGame().GetTime();
        checksPerformed = 0;
        averageSpeed = 0;
        speedHistory = new array<float>;
        positionHistory = new array<vector>;
        
        // Initialize health tracking
        lastHealthCheck = 1.0;
        lastBloodCheck = 1.0;
        consecutiveFullHealth = 0;
        consecutiveFullBlood = 0;
    }
    
    void InitializeTracker(string pSteamId, string pPlayerName, vector initialPosition, bool whitelisted = false)
    {
        steamId = pSteamId;
        playerName = pPlayerName;
        lastPosition = initialPosition;
        currentPosition = initialPosition;
        isWhitelisted = whitelisted;
        isConnected = true;
        
        float currentTime = GetGame().GetTime();
        lastCheckTime = currentTime;
        currentCheckTime = currentTime;
        connectionTime = currentTime;
        
        // Add initial position to history
        if (positionHistory.Count() >= 10)
        {
            positionHistory.RemoveOrdered(0);
        }
        positionHistory.Insert(initialPosition);
    }
    
    void UpdatePosition(vector newPosition)
    {
        lastPosition = currentPosition;
        currentPosition = newPosition;
        lastCheckTime = currentCheckTime;
        currentCheckTime = GetGame().GetTime();
        
        // Calculate movement data
        CalculateMovementData();
        
        // Add to position history
        if (positionHistory.Count() >= 10)
        {
            positionHistory.RemoveOrdered(0);
        }
        positionHistory.Insert(newPosition);
        
        // Update checks counter
        checksPerformed++;
    }
    
    void CalculateMovementData()
    {
        float deltaTime = currentCheckTime - lastCheckTime;
        
        if (deltaTime > 0 && deltaTime < 10.0) // Ignore if delta time is too large (disconnect/reconnect)
        {
            // Calculate speed
            teleportDistance = vector.Distance(currentPosition, lastPosition);
            lastSpeed = teleportDistance / deltaTime;
            
            // Update max speed
            if (lastSpeed > maxSpeedRecorded)
            {
                maxSpeedRecorded = lastSpeed;
            }
            
            // Calculate height above ground
            heightAboveGround = currentPosition[1] - GetGame().SurfaceY(currentPosition[0], currentPosition[2]);
            
            // Update max height
            if (heightAboveGround > maxHeightRecorded)
            {
                maxHeightRecorded = heightAboveGround;
            }
            
            // Update max teleport distance
            if (teleportDistance > maxTeleportDistance)
            {
                maxTeleportDistance = teleportDistance;
            }
            
            // Update speed history for average calculation
            if (speedHistory.Count() >= 20)
            {
                speedHistory.RemoveOrdered(0);
            }
            speedHistory.Insert(lastSpeed);
            
            // Calculate average speed
            CalculateAverageSpeed();
        }
    }
    
    void CalculateAverageSpeed()
    {
        if (speedHistory.Count() > 0)
        {
            float totalSpeed = 0;
            for (int i = 0; i < speedHistory.Count(); i++)
            {
                totalSpeed += speedHistory.Get(i);
            }
            averageSpeed = totalSpeed / speedHistory.Count();
        }
    }
    
    void UpdateHealthData(float health, float blood)
    {
        // Track consecutive full health/blood for God Mode detection
        if (health >= 1.0)
        {
            consecutiveFullHealth++;
        }
        else
        {
            consecutiveFullHealth = 0;
        }
        
        if (blood >= 1.0)
        {
            consecutiveFullBlood++;
        }
        else
        {
            consecutiveFullBlood = 0;
        }
        
        lastHealthCheck = health;
        lastBloodCheck = blood;
    }
    
    void AddViolation(string violationType)
    {
        totalViolations++;
        
        if (violationType == "SPEED")
        {
            speedViolations++;
        }
        else if (violationType == "FLY")
        {
            flyViolations++;
        }
        else if (violationType == "TELEPORT")
        {
            teleportViolations++;
        }
        else if (violationType == "GODMODE")
        {
            godModeViolations++;
        }
        else if (violationType == "NOCLIP")
        {
            noClipViolations++;
        }
    }
    
    void ResetViolation(string violationType)
    {
        if (violationType == "SPEED")
        {
            speedViolations = 0;
        }
        else if (violationType == "FLY")
        {
            flyViolations = 0;
        }
        else if (violationType == "TELEPORT")
        {
            teleportViolations = 0;
        }
        else if (violationType == "GODMODE")
        {
            godModeViolations = 0;
        }
        else if (violationType == "NOCLIP")
        {
            noClipViolations = 0;
        }
    }
    
    bool ShouldBeBanned(int maxViolations)
    {
        return (speedViolations >= maxViolations || 
                flyViolations >= maxViolations || 
                teleportViolations >= maxViolations || 
                godModeViolations >= maxViolations ||
                noClipViolations >= maxViolations);
    }
    
    string GetViolationType(int maxViolations)
    {
        if (speedViolations >= maxViolations) return "SPEED_HACK";
        if (flyViolations >= maxViolations) return "FLY_HACK";
        if (teleportViolations >= maxViolations) return "TELEPORT_HACK";
        if (godModeViolations >= maxViolations) return "GOD_MODE";
        if (noClipViolations >= maxViolations) return "NOCLIP_HACK";
        return "UNKNOWN";
    }
    
    float GetConnectionDuration()
    {
        return GetGame().GetTime() - connectionTime;
    }
    
    string GetTrackerStats()
    {
        string stats = "=== PLAYER TRACKER STATS ===\n";
        stats += "Player: " + playerName + " (" + steamId + ")\n";
        stats += "Connection Duration: " + (GetConnectionDuration() / 60.0).ToString() + " minutes\n";
        stats += "Checks Performed: " + checksPerformed.ToString() + "\n";
        stats += "Current Speed: " + lastSpeed.ToString() + " m/s\n";
        stats += "Average Speed: " + averageSpeed.ToString() + " m/s\n";
        stats += "Max Speed Recorded: " + maxSpeedRecorded.ToString() + " m/s\n";
        stats += "Current Height: " + heightAboveGround.ToString() + " m\n";
        stats += "Max Height Recorded: " + maxHeightRecorded.ToString() + " m\n";
        stats += "Max Teleport Distance: " + maxTeleportDistance.ToString() + " m\n";
        stats += "Total Violations: " + totalViolations.ToString() + "\n";
        stats += "Speed Violations: " + speedViolations.ToString() + "\n";
        stats += "Fly Violations: " + flyViolations.ToString() + "\n";
        stats += "Teleport Violations: " + teleportViolations.ToString() + "\n";
        stats += "God Mode Violations: " + godModeViolations.ToString() + "\n";
        stats += "NoClip Violations: " + noClipViolations.ToString() + "\n";
        stats += "Whitelisted: " + (isWhitelisted ? "YES" : "NO") + "\n";
        
        return stats;
    }
    
    void Disconnect()
    {
        isConnected = false;
    }
    
    bool IsValid()
    {
        return (steamId != "" && playerName != "" && isConnected);
    }
    
    // Additional utility methods for detailed tracking
    bool HasRecentViolations(int timeWindowSeconds = 60)
    {
        float currentTime = GetGame().GetTime();
        float timeSinceConnection = currentTime - connectionTime;
        
        if (timeSinceConnection < timeWindowSeconds)
        {
            return (totalViolations > 0);
        }
        
        // This is a simplified check - in a more complex implementation,
        // you could track violation timestamps
        return (totalViolations > 0);
    }
    
    string GetViolationSummary()
    {
        string summary = "Violations Summary: ";
        
        if (totalViolations == 0)
        {
            return summary + "Clean record";
        }
        
        bool hasViolations = false;
        
        if (speedViolations > 0)
        {
            if (hasViolations) summary += ", ";
            summary += "Speed(" + speedViolations.ToString() + ")";
            hasViolations = true;
        }
        
        if (flyViolations > 0)
        {
            if (hasViolations) summary += ", ";
            summary += "Fly(" + flyViolations.ToString() + ")";
            hasViolations = true;
        }
        
        if (teleportViolations > 0)
        {
            if (hasViolations) summary += ", ";
            summary += "Teleport(" + teleportViolations.ToString() + ")";
            hasViolations = true;
        }
        
        if (godModeViolations > 0)
        {
            if (hasViolations) summary += ", ";
            summary += "GodMode(" + godModeViolations.ToString() + ")";
            hasViolations = true;
        }
        
        if (noClipViolations > 0)
        {
            if (hasViolations) summary += ", ";
            summary += "NoClip(" + noClipViolations.ToString() + ")";
            hasViolations = true;
        }
        
        return summary;
    }
    
    void ResetAllViolations()
    {
        speedViolations = 0;
        flyViolations = 0;
        teleportViolations = 0;
        godModeViolations = 0;
        noClipViolations = 0;
        totalViolations = 0;
    }
    
    bool IsHealthSuspicious()
    {
        // Check if health/blood patterns are suspicious for God Mode
        return (consecutiveFullHealth > 30 && consecutiveFullBlood > 30);
    }
    
    bool IsSpeedSuspicious()
    {
        // Check if speed patterns are suspicious
        return (maxSpeedRecorded > 20.0 && averageSpeed > 15.0);
    }
    
    bool IsMovementSuspicious()
    {
        // Check if movement patterns are suspicious (teleporting)
        return (maxTeleportDistance > 1000.0);
    }
    
    string GetSuspiciousActivities()
    {
        string activities = "";
        bool hasSuspicious = false;
        
        if (IsHealthSuspicious())
        {
            activities += "Suspicious health patterns";
            hasSuspicious = true;
        }
        
        if (IsSpeedSuspicious())
        {
            if (hasSuspicious) activities += ", ";
            activities += "Suspicious speed patterns";
            hasSuspicious = true;
        }
        
        if (IsMovementSuspicious())
        {
            if (hasSuspicious) activities += ", ";
            activities += "Suspicious movement patterns";
            hasSuspicious = true;
        }
        
        if (!hasSuspicious)
        {
            activities = "No suspicious activities detected";
        }
        
        return activities;
    }
    
    void LogTrackerState(string reason = "")
    {
        string logMessage = "[NT_AntiCheat] Tracker State - " + playerName + " (" + steamId + ")";
        if (reason != "")
        {
            logMessage += " - " + reason;
        }
        
        Print(logMessage);
        Print("  Position: " + currentPosition.ToString());
        Print("  Speed: " + lastSpeed.ToString() + " m/s");
        Print("  Height: " + heightAboveGround.ToString() + " m");
        Print("  Violations: " + GetViolationSummary());
        Print("  Suspicious: " + GetSuspiciousActivities());
    }
};