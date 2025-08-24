class NT_AntiCheatConfig
{
    float checkInterval;        // วินาทีระหว่างการตรวจสอบ
    float maxSpeed;            // ความเร็วสูงสุด (m/s)
    float maxHeight;           // ความสูงสูงสุดจากพื้น (m)
    float maxTeleportDistance; // ระยะทางเทเลพอร์ตสูงสุด (m)
    bool enableSpeedCheck;     // เปิดตรวจสอบ Speed Hack
    bool enableFlyCheck;       // เปิดตรวจสอบ Fly Hack
    bool enableTeleportCheck;  // เปิดตรวจสอบ Teleport Hack
    bool enableGodModeCheck;   // เปิดตรวจสอบ God Mode
    bool enableNoClipCheck;    // เปิดตรวจสอบ NoClip
    bool autoKickEnabled;      // เปิดใช้ auto kick
    bool autoBanEnabled;       // เปิดใช้ auto ban
    int logLevel;              // ระดับ log (0=ปิด, 1=console, 2=file)
    int maxViolationsBeforeBan; // จำนวนการละเมิดก่อนแบน
    float saveInterval;        // ช่วงเวลาการบันทึกข้อมูล (วินาที)
    
    void NT_AntiCheatConfig()
    {
        checkInterval = 2.5;
        maxSpeed = 10.0;
        maxHeight = 80.0;
        maxTeleportDistance = 300.0;
        enableSpeedCheck = true;
        enableFlyCheck = true;
        enableTeleportCheck = true;
        enableGodModeCheck = false;
        enableNoClipCheck = false;
        autoKickEnabled = true;
        autoBanEnabled = true;
        logLevel = 2;
        maxViolationsBeforeBan = 3;
        saveInterval = 30.0;
    }
    
    // ฟังก์ชันสำหรับตรวจสอบค่าที่ถูกต้อง
    void ValidateConfig()
    {
        if (checkInterval < 1.0) checkInterval = 1.0;
        if (checkInterval > 10.0) checkInterval = 10.0;
        
        if (maxSpeed < 5.0) maxSpeed = 5.0;
        if (maxSpeed > 50.0) maxSpeed = 50.0;
        
        if (maxHeight < 10.0) maxHeight = 10.0;
        if (maxHeight > 500.0) maxHeight = 500.0;
        
        if (maxTeleportDistance < 50.0) maxTeleportDistance = 50.0;
        if (maxTeleportDistance > 1000.0) maxTeleportDistance = 1000.0;
        
        if (logLevel < 0) logLevel = 0;
        if (logLevel > 2) logLevel = 2;
        
        if (maxViolationsBeforeBan < 1) maxViolationsBeforeBan = 1;
        if (maxViolationsBeforeBan > 10) maxViolationsBeforeBan = 10;
        
        if (saveInterval < 10.0) saveInterval = 10.0;
        if (saveInterval > 300.0) saveInterval = 300.0;
    }
};