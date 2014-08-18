Skaar
=====

House Mod for Skyrim

#### Development
##### Initial Setup
1. Install Skyrim on Steam
1. Install all the DLCs
  - Steam -> Library -> Right click on Skyrim  -> Properties -> DLCs
1. Install [Nexus Mod Manager](http://www.nexusmods.com/skyrim/mods/modmanager/?)
1. Install patches
  - [Unofficial Skryim Patch](http://www.nexusmods.com/skyrim/mods/19/?)
  - [Unofficial Dawnguard Patch](http://www.nexusmods.com/skyrim/mods/23491/?)
  - [Unofficial Dragonborn Patch](http://www.nexusmods.com/skyrim/mods/31083/?)
  - [Unofficial Hearthfire Patch](http://www.nexusmods.com/skyrim/mods/25127/?)
1. Install [SKSE](http://skse.silverlock.org/)
1. Install [BOSS](http://boss-developers.github.io/)
1. Run BOSS
1. Install [GitHub for Windows](https://windows.github.com/)
1. Install [Sublime Text](http://www.sublimetext.com/) with 
  - Clone [SublimePapyrus](https://github.com/Kapiainen/SublimePapyrus)
  - Put the Papyrus folder into %AppData%\Sublime Text 2\Packages
  - Copy the contents of the SKSE folder into the Papyrus folder in the Sublime Text folder in the above step
1. Install Creation Kit
  - Steam -> Library -> Tools -> Creation Kit
1. Set Bethesda DLCs to load in Creation Kit
  - In [General] on SkyrmiEditor.ini: 
    - `sLanguage=ENGLISH`
    - `bAllowMultipleMasterLoads=1`
  - In [Archive]:
    - Append onto `SResoruceArchiveList2`: `Dawnguard.bsa, HearthFires.bsa, Dragonborn.bsa`
1. Set Creation Kit Settings in SkyrimEditor.ini
  - `bBlockMessageBoxes=1` (Prevent the annoying error messsgaes on Creation Kit file load)
1. Enable Papyrus Debug Logging.  In Skyrim.ini:
  - bEnableTrace=1
  - bEnableLogging=1
  - bLoadDebugInformation=1

##### Everyday Setup
1. Start Creation Kit
1. Double click these files to load and then hit Ok.  Load time will be long, but Creation Kit should not crash.
  - Skyrim.esm
  - Update.esm
  - Dragonborn.esm
  - HearthFires.esm
  - Dawnguard.esm
  - Unofficial Skyrim Patch.esp
  - Unofficial Hearthfire Patch.esp
  - Unofficial Dragonborn Patch.esp
  - Unofficial Dawnguard Patch.esp
  - skaar.esp (Also select it and Set Active)

#### Gameplay
- Optimal gameplay will require using all of these: Magic + Thu'um + Potions + Enchantments + Personal Spell Effects + Food + Daily Racial Abilities
