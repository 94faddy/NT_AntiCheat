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
    
    void InitializeBan(string pSteamId, string pPlayerName, string pReason, string pBanType, float pDetectedValue = 0.0)
    {
        steamId = pSteamId;
        playerName = pPlayerName;
        reason = pReason;
        banType = pBanType;
        detectedValue = pDetectedValue;
        isPermanent = true;
        
        // สร้าง Ban ID
        banId = GenerateUniqueBanId();
        
        // ตั้งค่าวันที่และเวลา
        SetCurrentDateTime();
        
        // ตั้งชื่อเซิร์ฟเวอร์
        if (GetGame() && GetGame().GetHostName() != "")
        {
            serverName = GetGame().GetHostName();
        }
        else
        {
            serverName = "DayZ Server";
        }
    }
    
    void SetCurrentDateTime()
    {
        int year, month, day, hour, minute, second;
        GetYearMonthDay(year, month, day);
        GetHourMinuteSecond(hour, minute, second);
        
        banDate = string.Format("%1-%2-%3", year.ToString(), AddLeadingZero(month), AddLeadingZero(day));
        banTime = string.Format("%1:%2:%3", AddLeadingZero(hour), AddLeadingZero(minute), AddLeadingZero(second));
    }
    
    string AddLeadingZero(int value)
    {
        if (value < 10)
            return "0" + value.ToString();
        return value.ToString();
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
        info += "Player: " + playerName + "\n";
        info += "SteamID: " + steamId + "\n";
        info += "Reason: " + reason + "\n";
        info += "Ban Type: " + banType + "\n";
        info += "Ban Date: " + banDate + " " + banTime + "\n";
        info += "Ban ID: " + banId + "\n";
        info += "Server: " + serverName + "\n";
        
        if (detectedValue > 0)
        {
            info += "Detected Value: " + detectedValue.ToString() + "\n";
        }
        
        if (violationCount > 0)
        {
            info += "Violation Count: " + violationCount.ToString() + "\n";
        }
        
        info += "Status: " + (isPermanent ? "PERMANENT BAN" : "TEMPORARY BAN") + "\n";
        info += "\nContact server admin with Ban ID if you believe this is an error.";
        
        return info;
    }
    
    bool IsValidBan()
    {
        return (steamId != "" && playerName != "" && reason != "" && banType != "" && banId != "");
    }
};