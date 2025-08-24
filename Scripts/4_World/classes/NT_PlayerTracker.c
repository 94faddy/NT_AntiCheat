class NT_PlayerTracker
{
    string steamId;
    string playerName;
    vector lastPosition;
    vector currentPosition;
    float lastCheckTime;
    float currentCheckTime;
    float lastSpeed;
    float heightAboveGround;
    float teleportDistance;
    bool isWhitelisted;
    bool isConnected;
    
    int speedViolations;
    int flyViolations;
    int teleportViolations;
    int godModeViolations;
    int noClipViolations;
    int totalViolations;
    
    float connectionTime;
    int checksPerformed;
    
    float lastHealthCheck;
    float lastBloodCheck;
    int consecutiveFullHealth;
    int consecutiveFullBlood;
    
    void NT_PlayerTracker()
    {
        steamId = "";
        playerName = "";
        lastPosition = "0 0 0";
        currentPosition = "0 0 0";
        lastCheckTime = 0.0;
        currentCheckTime = 0.0;
        lastSpeed = 0.0;
        heightAboveGround = 0.0;
        teleportDistance = 0.0;
        isWhitelisted = false;
        isConnected = false;
        
        speedViolations = 0;
        flyViolations = 0;
        teleportViolations = 0;
        godModeViolations = 0;
        noClipViolations = 0;
        totalViolations = 0;
        
        connectionTime = 0.0;
        checksPerformed = 0;
        
        lastHealthCheck = 1.0;
        lastBloodCheck = 1.0;
        consecutiveFullHealth = 0;
        consecutiveFullBlood = 0;
    }
    
    void InitializeTracker(string pSteamId, string pPlayerName, vector initialPosition, bool whitelisted)
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
    }
    
    void UpdatePosition(vector newPosition)
    {
        lastPosition = currentPosition;
        currentPosition = newPosition;
        lastCheckTime = currentCheckTime;
        currentCheckTime = GetGame().GetTime();
        
        CalculateMovementData();
        checksPerformed = checksPerformed + 1;
    }
    
    void CalculateMovementData()
    {
        float deltaTime = currentCheckTime - lastCheckTime;
        
        if (deltaTime > 0.0)
        {
            if (deltaTime < 10.0)
            {
                teleportDistance = vector.Distance(currentPosition, lastPosition);
                lastSpeed = teleportDistance / deltaTime;
                
                float surfaceY = GetGame().SurfaceY(currentPosition[0], currentPosition[2]);
                heightAboveGround = currentPosition[1] - surfaceY;
            }
        }
    }
    
    void UpdateHealthData(float health, float blood)
    {
        if (health >= 1.0)
        {
            consecutiveFullHealth = consecutiveFullHealth + 1;
        }
        else
        {
            consecutiveFullHealth = 0;
        }
        
        if (blood >= 1.0)
        {
            consecutiveFullBlood = consecutiveFullBlood + 1;
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
        totalViolations = totalViolations + 1;
        
        if (violationType == "SPEED")
        {
            speedViolations = speedViolations + 1;
        }
        if (violationType == "FLY")
        {
            flyViolations = flyViolations + 1;
        }
        if (violationType == "TELEPORT")
        {
            teleportViolations = teleportViolations + 1;
        }
        if (violationType == "GODMODE")
        {
            godModeViolations = godModeViolations + 1;
        }
        if (violationType == "NOCLIP")
        {
            noClipViolations = noClipViolations + 1;
        }
    }
    
    void ResetViolation(string violationType)
    {
        if (violationType == "SPEED")
        {
            speedViolations = 0;
        }
        if (violationType == "FLY")
        {
            flyViolations = 0;
        }
        if (violationType == "TELEPORT")
        {
            teleportViolations = 0;
        }
        if (violationType == "GODMODE")
        {
            godModeViolations = 0;
        }
        if (violationType == "NOCLIP")
        {
            noClipViolations = 0;
        }
    }
    
    bool ShouldBeBanned(int maxViolations)
    {
        if (speedViolations >= maxViolations)
            return true;
        if (flyViolations >= maxViolations)
            return true;
        if (teleportViolations >= maxViolations)
            return true;
        if (godModeViolations >= maxViolations)
            return true;
        if (noClipViolations >= maxViolations)
            return true;
        return false;
    }
    
    void Disconnect()
    {
        isConnected = false;
    }
    
    bool IsValid()
    {
        if (steamId == "")
            return false;
        if (playerName == "")
            return false;
        if (!isConnected)
            return false;
        return true;
    }
};