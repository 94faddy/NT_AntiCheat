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
    
    void InitializeWhitelist(string pSteamId, string pPlayerName, string pReason, string pAddedBy, string pAdminLevel)
    {
        steamId = pSteamId;
        playerName = pPlayerName;
        reason = pReason;
        addedBy = pAddedBy;
        adminLevel = pAdminLevel;
        isActive = true;
        
        SetCurrentDateTime();
    }
    
    void SetCurrentDateTime()
    {
        int year, month, day, hour, minute, second;
        GetYearMonthDay(year, month, day);
        GetHourMinuteSecond(hour, minute, second);
        
        addedDate = year.ToString() + "-" + month.ToString() + "-" + day.ToString();
        addedTime = hour.ToString() + ":" + minute.ToString() + ":" + second.ToString();
    }
    
    bool IsValid()
    {
        if (steamId == "")
            return false;
        if (steamId.Length() != 17)
            return false;
        if (playerName == "")
            return false;
        if (reason == "")
            return false;
        return true;
    }
    
    bool IsValidSteamId(string checkSteamId)
    {
        if (checkSteamId.Length() != 17)
            return false;
        return true;
    }
    
    string GetWhitelistInfo()
    {
        string info = "Admin: " + playerName + " (" + steamId + ")";
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
            notes = notes + " | " + newNotes;
        }
    }
};