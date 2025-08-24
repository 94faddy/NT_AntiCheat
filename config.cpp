class CfgPatches
{
    class NT_AntiCheat
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 1.0;
        requiredAddons[] = {"DZ_Data", "DZ_Scripts"};
        version = "2.0";
        versionStr = "2.0";
        versionAr[] = {2,0};
        author = "NT Development";
        authorID = "";
        name = "NT AntiCheat System";
    };
};

class CfgMods
{
    class NT_AntiCheat
    {
        dir = "NT_AntiCheat";
        picture = "";
        action = "";
        hideName = 1;
        hidePicture = 1;
        name = "NT AntiCheat System";
        credits = "Advanced AntiCheat with Admin Whitelist";
        author = "NT Development";
        authorID = "";
        version = "2.0";
        extra = 0;
        type = "mod";
        
        dependencies[] = {"Game", "World", "Mission"};
        
        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] = {"NT_AntiCheat/Scripts/3_Game"};
            };
            
            class worldScriptModule
            {
                value = "";
                files[] = {"NT_AntiCheat/Scripts/4_World"};
            };
            
            class missionScriptModule
            {
                value = "";
                files[] = {"NT_AntiCheat/Scripts/5_Mission"};
            };
        };
    };
};