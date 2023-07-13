#include "regions.hpp"
#include "../scheduling/pit/pit.h"
#include <stddef.h>

static struct RegionDefinition regions[NRegions];

    RegionDefinition tmp;

    void DefinedRegions::rfRgn() {
        for (unsigned int i = 0; i < NRegions; i++) {
            tmp = regions[i];           
            tmp.key = "[redacted]";     // Do not expose keys.
            tmp.master_region_ptr = &unused_defn;

            Regions[i] = tmp;
        }
    }

/* END OF TODO */


void DefinedRegions::setup() {
     #ifndef __RST_RgnCounter

        for (short int i = 0; i < NRegions; i++) {
            regions[i] = empty_defn;
        }

        #define __RST_RgnCounter
    
    #else 
        return; 

    #endif
}

ReturnCode DefinedRegions::define_new_region(Region r, bool _p, const char key[DefRegions_KeyLen + 1]) {    
    if (check_region(r, key) == ReturnCode::ERROR) return ReturnCode::ERROR;
    RegionDefinition def = {r, _p, true, key};
    
    #ifndef __RST_RgnCounter
        return ReturnCode::ERROR;
    #endif 

    int RgnIndx = find_avail_index();
    if (RgnIndx < 0) return ReturnCode::ERROR; // No available indexes

    regions[RgnIndx] = def;

    return ReturnCode::SUCCESS;
}


ReturnCode DefinedRegions::check_region(Region region, const char* key) {

    #ifndef __RST_RgnCounter
        return ReturnCode::ERROR;
    #endif 

    for (unsigned int i = 0; i < NRegions; i++) {
        if (!regions[i].prtc) continue;

        if (
            // If the regions are overlapping
            (regions[i].region.x0 > region.x0 && regions[i].region.x0 < region.xf) ||
            (regions[i].region.y0 > region.y0 && regions[i].region.y0 < region.yf) ||

            // OR, if the provided key already exists in the list
            (regions[i].key == key)

        ) return ReturnCode::ERROR; // Then, do not allow for the region definition to be made
    }

    return ReturnCode::SUCCESS; // Otherwise, the definition can be made.

}

namespace rmv_rgn_def {

    int tmp_ind_storage[NRegions - 1];
    uint32_t n1 = 0;
    bool return_warning = false;
    RegionDefinition def;

    void clear_tmp_ind_storage() {
        n1 = 0;

        for (unsigned int i = 0; i < (NRegions - 1); i++) {
            tmp_ind_storage[i] = -1;
        }
    }

}

ReturnCode DefinedRegions::remove_region_definition(Region region, const char _K[DefRegions_KeyLen + 1]) {
    #ifndef __RST_RgnCounter
        return ReturnCode::ERROR;  
    #endif 

    #ifndef __NO_RECURSSIVE_OVERLAY
        int tmp_buffer[NRegions - 1];
    #endif 

    rmv_rgn_def::return_warning = false;

    for (unsigned int i = 0; i < NRegions; i++) {
        rmv_rgn_def::def = regions[i];
        if (rmv_rgn_def::def.region == region && rmv_rgn_def::def.key == _K) {
            if (rmv_rgn_def::def.n_sub_regions > 0) {
                rmv_rgn_def::clear_tmp_ind_storage();

                for (unsigned int j = 0; j < NRegions; j++) {
                    if (j == i) continue;
                    else if (!regions[j].is_sub_region) continue;
                    else if (*(regions[j].master_region_ptr) == rmv_rgn_def::def) { 
                        // This is a sub-region of this master region
                        rmv_rgn_def::tmp_ind_storage[rmv_rgn_def::n1] = j;
                        rmv_rgn_def::n1 ++; 
                    }
                }                

                for (uint32_t j = 0; j < rmv_rgn_def::n1; j++) {
                    #ifdef __NO_RECURSSIVE_OVERLAY
                        regions[rmv_rgn_def::tmp_ind_storage[j]] = empty_defn;

                    #else
                        for (unsigned int k = 0; k < rmv_rgn_def::n1; k++) tmp_buffer[k] = rmv_rgn_def::tmp_ind_storage[k];

                        remove_region_definition(regions[rmv_rgn_def::tmp_ind_storage[j]].region, regions[rmv_rgn_def::tmp_ind_storage[j]].key);
                        
                        for (unsigned int k = 0; k < rmv_rgn_def::n1; k++) rmv_rgn_def::tmp_ind_storage[k] = tmp_buffer[k];

                    #endif
                    
                    rmv_rgn_def::def.n_sub_regions--;
                }

                // Ensure that the number of sub-regions found is the same as the number of sub-regions expected
                if (rmv_rgn_def::def.n_sub_regions == 0)
                    rmv_rgn_def::return_warning = true;

                #ifndef __NO_RECURSSIVE_OVERLAY

                    if (rmv_rgn_def::def.is_sub_region) rmv_rgn_def::def.master_region_ptr->n_sub_regions--;
                
                #endif

            } else {
                if (rmv_rgn_def::def.is_sub_region) rmv_rgn_def::def.master_region_ptr->n_sub_regions--;
                regions[i] = empty_defn;

            }

            return (rmv_rgn_def::return_warning ? ReturnCode::WARNING : ReturnCode::SUCCESS);
        }
    }

    return ReturnCode::ERROR;
}


ReturnCode DefinedRegions::define_overlay_region(Region region, const char _key[DefRegions_KeyLen + 1], const char _mrK[DefRegions_KeyLen + 1]) {
     // Find regiondef by key
    RegionDefinition* masterDef;
    bool found = false;

    for (uint32_t i = 0; i < NRegions; i++) {
        if (regions[i].key == _mrK) {
            masterDef = &regions[i];
            found = true;
            break;
        }
    }

    if (!found) return ReturnCode::ERROR;

    // Check to make sure the overlay is in the region of the master regiondef
    if (!(

            (masterDef->region.x0 <= region.x0)
        &&  (masterDef->region.xf >= region.x0)
        
        &&  (masterDef->region.x0 <= region.xf)
        &&  (masterDef->region.xf >= region.xf)

        &&  (masterDef->region.y0 <= region.y0) 
        &&  (masterDef->region.yf >= region.y0)

        &&  (masterDef->region.y0 <= region.yf)
        &&  (masterDef->region.yf >= region.yf)

    )) return ReturnCode::ERROR;

    #ifdef __NO_RECURSSIVE_OVERLAY
        // Recursive overlay definitions not allowed
        if (masterDef->is_sub_region) return ReturnCode::ERROR;

    #endif

    // Check if another region can be defined
    int nIndex = find_avail_index();
    if (nIndex < 0) return ReturnCode::ERROR; // No more regions can be defined 

    // Master regiondef is found, it meets the policy requirements, and another region can be defined
    // Create new region definition
    RegionDefinition nDef; 

    nDef.is_sub_region = true;              // This IS a sub region
    nDef.key = _key;                        // Store its UID key
    nDef.master_region_ptr = masterDef;     // Sure the master rgndef ptr
    nDef.n_sub_regions = 0;                 // No sub regions yet
    nDef.prtc = true;                       // Protected = true
    nDef._occ = true;                       // Occupied = true
    nDef.region = region;                   // Store region info

    regions[nIndex] = nDef; // store the region 
    masterDef->n_sub_regions ++; // increment the master rgn's sub region counter 

    return ReturnCode::SUCCESS;
}


bool DefinedRegions::check_region_o(Region region, const char _K[DefRegions_KeyLen + 1]) {
    #ifndef __RST_RgnCounter
        return false;
    #endif

    for (unsigned int i = 0; i < NRegions; i++) {
        if (regions[i].region == region && regions[i].key == _K) return true; 
    }

    return false;
}

const static char a[] = "abcdefghijklmnopqrstuvwxyz";
const static char n[] = "1234567890";
const static char s[] = "!@#$%^&*()_-={}{}|:;'<>,.?/";

int _di = rand();
char __GenKeyOutput[DefRegions_KeyLen + 1];
const char* DefinedRegions::gen_key(unsigned int seed) {
    _di %= 256; 

    for (int i = 0; i <=DefRegions_KeyLen; i++) {
        _di += srand((uint32_t)(seed + rand()));

        if (_di%3 == 0) __GenKeyOutput[i] = a[_di%26];
        if (_di%3 == 1) __GenKeyOutput[i] = n[_di%10];
        if (_di%3 == 2) __GenKeyOutput[i] = s[_di%27];

    }

    __GenKeyOutput[DefRegions_KeyLen] = '\0';
    return __GenKeyOutput;
}


int DefinedRegions::find_avail_index() {
    for (int i = 0; i < NRegions; i++) {
        if (!regions[i]._occ && !regions[i].prtc) return i;
    }
    return -1;
}


bool operator ==(Region region1, Region region2) {
    return (
            region1.x0 == region2.x0
        &&  region1.xf == region2.xf
        &&  region1.y0 == region2.y0
        &&  region1.yf == region2.yf
    );
}


bool operator ==(RegionDefinition rDef1, RegionDefinition rDef2) {
    return (
            rDef1.key == rDef2.key 
        &&  rDef1.region == rDef2.region
        &&  rDef1.prtc == rDef2.prtc
        &&  rDef1._occ == rDef2._occ
        &&  rDef1.master_region_ptr == rDef2.master_region_ptr
        &&  rDef1.is_sub_region == rDef2.is_sub_region
        &&  rDef1.n_sub_regions == rDef2.n_sub_regions
    );
}
