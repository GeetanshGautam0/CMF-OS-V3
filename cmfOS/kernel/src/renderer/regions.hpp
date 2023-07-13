#pragma once
#include "../std/random32.h"
#include "../std/codes.h"
#include "../std/ctypedef.h"

#define DefRegions_KeyLen 32
#define NRegions 1000
#define __NO_RECURSSIVE_OVERLAY            // sub-regions cannot define sub-regions of their own

struct Region {
    unsigned int x0; 
    unsigned int y0;
    unsigned int xf;
    unsigned int yf;
};

struct RegionDefinition {
    Region region;
    bool prtc;                              // Prevents region from being overwritten by other regions
    bool _occ = true;
    const char* key;                        // Key + null terminator (always need the key)

    // Sub-region management
    bool is_sub_region = false;
    int n_sub_regions = 0;
    RegionDefinition* master_region_ptr = (RegionDefinition*)NULL;
};

class DefinedRegions {
    public: 
        void setup();
        ReturnCode define_new_region(Region region, bool protect, const char key[DefRegions_KeyLen + 1]); 
        ReturnCode check_region(Region region, const char* key);
        ReturnCode remove_region_definition(Region region, const char _key[DefRegions_KeyLen + 1]);
        const char* gen_key(unsigned int seed);
        bool check_region_o(Region region, const char _key[DefRegions_KeyLen + 1]);

        ReturnCode define_overlay_region(Region region, const char _key[DefRegions_KeyLen + 1], const char _mrK[DefRegions_KeyLen + 1]);
        
        // Make sure you can't get your own region definition (then you would be able to define a new overlay, 
        // get your regiondef and the pointer to the master's regiondef, and subsequently its UID, in turn being able
        // to modify it (most serious effect on basic renderer)).

        // Accessing a redacted version of the private regions list (keys removed, master_region_ptr removed). 
        RegionDefinition Regions[NRegions];
        void rfRgn();

    private:
        const RegionDefinition empty_defn = {{0, 0, 0, 0}, false, false, ""};
        RegionDefinition unused_defn = {{0, 0, 0, 0}, false, false, ""};
        int find_avail_index();
};


bool operator ==(Region region1, Region region2);
bool operator ==(RegionDefinition rDef1, RegionDefinition rDef2);

