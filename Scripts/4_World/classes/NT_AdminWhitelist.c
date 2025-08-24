class NT_AdminWhitelist
{
    string steamId;
    string playerName;
    string reason;
    string addedDate;
    string addedTime;
    string addedBy;
    bool isActive;
    string adminLevel;
    string notes;
    
    void NT_AdminWhitelist()
    {
        steamId = "";
        playerName = "";
        reason = "";
        addedDate = "";
        addedTime = "";
        addedBy = "";
        isActive = true;
        adminLevel = "Administrator";
        notes = "";
    }
    
    void InitializeWhitelist(string pSteamId, string pPlayerName, string pReason, string pAddedBy = "System", string pAdminLevel = "Administrator")
    {
        steamId = pSteamId;
        playerName = pPlayerName;
        reason = pReason;
        addedBy = pAddedBy;
        adminLevel = pAdminLevel;
        isActive = true;
        
        // ตั้งค่าวันที่และเวลา
        SetCurrentDateTime();
    }
    
    void SetCurrentDateTime()
    {
        int year, month, day, hour, minute, second;
        GetYearMonthDay(year, month, day);
        GetHourMinuteSecond(hour, minute, second);
        
        addedDate = string.Format("%1-%2-%3", year.ToString(), AddLeadingZero(month), AddLeadingZero(day));
        addedTime = string.Format("%1:%2:%3", AddLeadingZero(hour), AddLeadingZero(minute), AddLeadingZero(second));
    }
    
    string AddLeadingZero(int value)
    {
        if (value < 10)
            return "0" + value.ToString();
        return value.ToString();
    }
    
    bool IsValid()
    {
        return (steamId != "" && steamId.Length() == 17 && playerName != "" && reason != "");
    }
    
    bool IsValidSteamId(string checkSteamId)
    {
        // ตรวจสอบว่า SteamID64 มีความยาว 17 หลักและเป็นตัวเลข
        if (checkSteamId.Length() != 17)
            return false;
            
        // ตรวจสอบว่าเป็นตัวเลขทั้งหมด
        for (int i = 0; i < checkSteamId.Length(); i++)
        {
            string char = checkSteamId.Substring(i, 1);
            if (char != "0" && char != "1" && char != "2" && char != "3" && char != "4" && 
                char != "5" && char != "6" && char != "7" && char != "8" && char != "9")
            {
                return false;
            }
        }
        
        // SteamID64 ต้องขึ้นต้นด้วย 765611980 หรือ 765611981
        string prefix = checkSteamId.Substring(0, 9);
        return (prefix == "765611980" || prefix == "765611981");
    }
    
    string GetWhitelistInfo()
    {
        string info = "=== ADMIN WHITELIST INFO ===\n";
        info += "Player: " + playerName + "\n";
        info += "SteamID: " + steamId + "\n";
        info += "Admin Level: " + adminLevel + "\n";
        info += "Reason: " + reason + "\n";
        info += "Added Date: " + addedDate + " " + addedTime + "\n";
        info += "Added By: " + addedBy + "\n";
        info += "Status: " + (isActive ? "ACTIVE" : "INACTIVE") + "\n";
        
        if (notes != "")
        {
            info += "Notes: " + notes + "\n";
        }
        
        return info;
    }
    
    void SetActive(bool active)
    {
        isActive = active;
    }
    
    void AddNotes(string newNotes)
    {
        if (notes == "")
        {
            notes = newNotes;
        }
        else
        {
            notes += " | " + newNotes;
        }
    }
};