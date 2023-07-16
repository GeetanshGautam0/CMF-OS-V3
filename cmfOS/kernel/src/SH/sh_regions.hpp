#pragma once
#include "../HEADERS/str.hpp"
#include "../HEADERS/std.hpp"
#include "../HEADERS/math.hpp"
#include "../HEADERS/code.hpp"
#include "../HEADERS/random.hpp"


#ifndef _SH_REGIONS
    #define _SH_REGIONS
    #define _SH_REGIONS_SCRIPT
    #define _SH_REGIONS_CONSTS

    #define SH_RGN_KEYLEN 32
    #define SH_RGN_N_RGNS 1000
    #define __NO_RECURSIVE_OVERLAY          // Policy: sub regions cannot make their own sub regions

#endif


struct Region {
    unsigned int x0;
    unsigned int y0;
    unsigned int xf;
    unsigned int yf;
};


struct RegionDefinition {
    Region region;
    bool prtc;                              // protected. Used for sub-region management. 
    bool _occ;                              // occupied. If set to FALSE, the system can override this region's definition. 
    const char* region_id;

    bool is_sub_region = false;
    int n_sub_regions = 0;
    RegionDefinition* master_region_ptr = (RegionDefinition*)NULL;
};


class DefinedRegions {
    public:

        void setup();
        ReturnCode DefineNewRegion(Region region, bool protect, const char id[SH_RGN_KEYLEN + 1]);
        ReturnCode CheckRegion(Region region, const char* id);
        ReturnCode RemoveRegion(Region region, const char id[SH_RGN_KEYLEN + 1]);

        const char* GenerateID(unsigned int seed);
        bool CheckRegionO(Region region, const char id[SH_RGN_KEYLEN + 1]);

        bool IsSetup();

        ReturnCode DefineOverlay(Region region, const char id[SH_RGN_KEYLEN + 1], const char master_id[SH_RGN_KEYLEN + 1]);

    private:
        const RegionDefinition empty_defn = {{0, 0, 0, 0}, false, false, "sh_regions.empty_rgn_defn      ."};
        RegionDefinition unused_defn =      {{0, 0, 0, 0}, false, false, "sh_regions.unused_rgn_defn     ."};
        signed int find_avail_index();

};


bool operator ==(Region r1, Region r2);
bool operator ==(RegionDefinition rd1, RegionDefinition rd2);
