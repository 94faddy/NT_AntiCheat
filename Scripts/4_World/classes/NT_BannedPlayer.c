class NT_BannedPlayer
{
    string steamId;
    string playerName;
    string reason;
    string banType;
    string banDate;
    string banTime;
    string banId;
    float detectedValue;
    string serverName;
    string adminBannedBy;
    bool isPermanent;
    string additionalInfo;
    int violationCount;
    
    void NT_BannedPlayer()
    {
        steamId = "";
        playerName = "";
        reason = "";
        banType = "";
        banDate = "";
        banTime = "";
        banId = "";
        detectedValue = 0.0;
        serverName = "";
        adminBannedBy = "NT_AntiCheat_System";
        isPermanent = true;
        additionalInfo = "";
        violationCount = 0;
    }
    
    void InitializeBan(string pSteamId, string pPlayerName, string pReason, string pBanType, float pDetectedValue)
    {
        steamId = pSteamId;
        playerName = pPlayerName;
        reason = pReason;
        banType = pBanType;
        detectedValue = pDetectedValue;
        isPermanent = true;
        
        banId = GenerateUniqueBanId();
        SetCurrentDateTime();
        
        serverName = "DayZ Server";
        if (GetGame())
        {
            string hostname = GetGame().GetHostName();
            if (hostname != "")
            {
                serverName = hostname;
            }
        }
    }
    
    void SetCurrentDateTime()
    {
        int year, month, day, hour, minute, second;
        GetYearMonthDay(year, month, day);
        GetHourMinuteSecond(hour, minute, second);
        
        banDate = year.ToString() + "-" + month.ToString() + "-" + day.ToString();
        banTime = hour.ToString() + ":" + minute.ToString() + ":" + second.ToString();
    }
    
    string GenerateUniqueBanId()
    {
        int timestamp = GetGame().GetTime();
        int randomNum = Math.RandomInt(1000, 9999);
        return "NT" + timestamp.ToString() + randomNum.ToString() + "AC";
    }
    
    string GetFullBanInfo()
    {
        string info = "=== NT ANTICHEAT BAN INFORMATION ===\n";
        info = info + "Player: " + playerName + "\n";
        info = info + "SteamID: " + steamId + "\n";
        info = info + "Reason: " + reason + "\n";
        info = info + "Ban Type: " + banType + "\n";
        info = info + "Ban Date: " + banDate + " " + banTime + "\n";
        info = info + "Ban ID: " + banId + "\n";
        info = info + "Server: " + serverName + "\n";
        
        if (detectedValue > 0.0)
        {
            info = info + "Detected Value: " + detectedValue.ToString() + "\n";
        }
        
        if (violationCount > 0)
        {
            info = info + "Violation Count: " + violationCount.ToString() + "\n";
        }
        
        info = info + "Status: PERMANENT BAN\n";
        info = info + "\nContact server admin with Ban ID if you believe this is an error.";
        
        return info;
    }
    
    bool IsValidBan()
    {
        if (steamId == "") return false;
        if (playerName == "") return false;
        if (reason == "") return false;
        if (banType == "") return false;
        if (banId == "") return false;
        return true;
    }
};