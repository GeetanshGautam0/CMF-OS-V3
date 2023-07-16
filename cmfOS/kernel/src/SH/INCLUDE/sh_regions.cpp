#include "../sh_regions.hpp"


static struct RegionDefinition regions[SH_RGN_N_RGNS];
static bool _is_setup = false;


bool
DefinedRegions::IsSetup()
{

    return _is_setup;

}


void 
DefinedRegions::setup() 
{
    if (_is_setup) return;

    for (short int i = 0; i < SH_RGN_N_RGNS; i++) {
        regions[i] = empty_defn;
    }
    _is_setup = true; 

}


ReturnCode 
DefinedRegions::DefineNewRegion(
    Region r, 
    bool p, 
    const char id[SH_RGN_KEYLEN + 1]
)
{
    #ifndef _SH_REGIONS_SCRIPT
        return ReturnCode::ERROR;
    #endif 
    if (!_is_setup) return ReturnCode::ERROR;

    ReturnCode rc = CheckRegion(r, id);
    if (rc != ReturnCode::SUCCESS) return rc;
    RegionDefinition def = {r, p, true, id};

    signed int rgn_index = find_avail_index();
    if (rgn_index < 0) return ReturnCode::ERROR;            // No more indexes are available

    regions[rgn_index] = def;
    return ReturnCode::SUCCESS;
}


ReturnCode
DefinedRegions::CheckRegion(
    Region r, 
    const char* k
) 
{
    #ifndef _SH_REGIONS_SCRIPT
        return ReturnCode::ERROR;
    #endif
    if (!_is_setup) return ReturnCode::ERROR;

    for 
    (
        unsigned int i = 0;
        i < SH_RGN_N_RGNS;
        i++
    )
    {
        if (!regions[i].prtc && regions[i].region_id != k) continue;

        if (
            // If the regions overlap (ERROR if prtc)
            (r.x0 >= regions[i].region.x0 && r.x0 <= regions[i].region.xf) ||
            (r.xf >= regions[i].region.x0 && r.xf <= regions[i].region.xf) ||
            (r.y0 >= regions[i].region.y0 && r.y0 <= regions[i].region.yf) ||
            (r.yf >= regions[i].region.y0 && r.yf <= regions[i].region.yf) ||

            // Or, if the key is the same (WARN)
            (regions[i].region_id == k)
        ) {
            if (regions[i].region_id == k) return ReturnCode::WARNING;
            
            return ReturnCode::ERROR; 
        }
    }

    return ReturnCode::SUCCESS;

}


namespace SH_RGN_rmv_rgn_def {

    int tmp_ind_storage[SH_RGN_N_RGNS - 1];
    uint32_t n1 = 0; 
    bool return_warning = false;
    RegionDefinition def;

    void 
    clear_tmp_ind_storage()
    {
        n1 = 0;
        for
        (
            unsigned int i = 0;
            i < (SH_RGN_N_RGNS - 1);
            i ++
        )
            tmp_ind_storage[i] = -1;
    }

}


ReturnCode
DefinedRegions::RemoveRegion(
    Region r, 
    const char k[SH_RGN_KEYLEN + 1]
)
{
    if (!_is_setup) return ReturnCode::ERROR;
    #ifndef _SH_REGIONS_SCRIPT
        return ReturnCode::ERROR;
    #endif

    #ifndef __NO_RECURSIVE_OVERLAY
        uint32_t n2 = 0;
        int tmp_buffer[SH_RGN_N_RGNS - 1];
    #endif

    SH_RGN_rmv_rgn_def::return_warning = false;
    for 
    (
        unsigned int i = 0; 
        i < SH_RGN_N_RGNS; 
        i++
    ) 
    {
        SH_RGN_rmv_rgn_def::def = regions[i];
        if (SH_RGN_rmv_rgn_def::def.region == r && SH_RGN_rmv_rgn_def::def.region_id == k) {

            if (SH_RGN_rmv_rgn_def::def.n_sub_regions > 0) {
                SH_RGN_rmv_rgn_def::clear_tmp_ind_storage();

                for 
                (
                    unsigned int j = 0; 
                    j < SH_RGN_N_RGNS; 
                    j++
                ) 
                {
                    
                    if (j == i) continue;
                    if (!regions[j].is_sub_region) continue;

                    if (*(regions[j].master_region_ptr) == SH_RGN_rmv_rgn_def::def) {
                        // This is a sub-region of this master region
                        SH_RGN_rmv_rgn_def::tmp_ind_storage[SH_RGN_rmv_rgn_def::n1] = j;
                        SH_RGN_rmv_rgn_def::n1 ++;
                    }

                }

                for (uint32_t j = 0; j < SH_RGN_rmv_rgn_def::n1; j++) {
                    #ifdef __NO_RECURSIVE_OVERLAY
                        regions[SH_RGN_rmv_rgn_def::tmp_ind_storage[j]] = empty_defn;
                    #else
                        n2 = SH_RGN_rmv_rgn_def::n1;
                        for (unsigned int k = 0; k < SH_RGN_rmv_rgn_def::n1; k++)
                            tmp_buffer[k] = SH_RGN_rmv_rgn_def::tmp_ind_storage[k];
                        
                        RemoveRegion(
                            regions[SH_RGN_rmv_rgn_def::tmp_ind_storage[j]].region, 
                            regions[SH_RGN_rmv_rgn_def::tmp_ind_storage[j]].key
                        );

                        for (unsigned int k = 0; k < SH_RGN_rmv_rgn_def::n1; k++)
                            SH_RGN_rmv_rgn_def::tmp_ind_storage[k] = tmp_buffer[k];
                        
                        SH_RGN_rmv_rgn_def::n1 = n2;

                    #endif

                    SH_RGN_rmv_rgn_def::def.n_sub_regions--;
                }

                if (SH_RGN_rmv_rgn_def::def.n_sub_regions != 0) 
                    SH_RGN_rmv_rgn_def::return_warning = true;

                if (SH_RGN_rmv_rgn_def::def.is_sub_region)
                    SH_RGN_rmv_rgn_def::def.master_region_ptr->n_sub_regions -= 1;

            } else 
            {
                // Does not have any sub regions
                if (SH_RGN_rmv_rgn_def::def.is_sub_region) SH_RGN_rmv_rgn_def::def.master_region_ptr->n_sub_regions -= 1;
                regions[i] = empty_defn;
            }

            return (SH_RGN_rmv_rgn_def::return_warning ? ReturnCode::WARNING : ReturnCode::SUCCESS);

        }
    }

    return ReturnCode::ERROR; // Could not find region. 
}


ReturnCode
DefinedRegions::DefineOverlay(
    Region r,
    const char k[SH_RGN_KEYLEN + 1],
    const char mk[SH_RGN_KEYLEN + 1]
)
{

    if (!_is_setup) return ReturnCode::ERROR;
    #ifndef _SH_REGIONS_SCRIPT
        return ReturnCode::ERROR;
    #endif

    RegionDefinition* master;
    bool found = false;

    for 
    (
        uint32_t i = 0; 
        i < SH_RGN_N_RGNS;
        i ++
    ) 
    {
        if (regions[i].region_id == mk) {
            master = (RegionDefinition*)(regions[i].master_region_ptr);
            found = true;
            break;
        }
    }

    if (!found) return ReturnCode::ERROR;  // master not found by ID. 

    if (! (
    (master->region.x0 <= r.x0)
    && (master->region.xf >= r.x0)
    && (r.x0 < r.xf)
    && (master->region.x0 <= r.xf)
    && (master->region.xf >= r.xf)
    && (master->region.y0 <= r.y0)
    && (master->region.yf >= r.y0)
    && (r.y0 < r.yf)
    && (master->region.y0 <= r.yf)
    && (master->region.yf >= r.yf)
    )) return ReturnCode::ERROR;

    #ifdef __NO_RECURSIVE_OVERLAY

        if (master->is_sub_region) 
            return ReturnCode::ERROR;
    
    #endif

    signed int n_index = find_avail_index();
    if (n_index < 0) return ReturnCode::ERROR;

    RegionDefinition n_def;

    n_def.is_sub_region = true;
    n_def.region_id = k;
    n_def.master_region_ptr = master;
    n_def.n_sub_regions = 0;
    n_def.prtc = true;
    n_def._occ = true;
    n_def.region = r;

    regions[n_index] = n_def;
    master->n_sub_regions ++;

    return ReturnCode::SUCCESS;

}


bool 
DefinedRegions::CheckRegionO(
    Region r,
    const char k[SH_RGN_KEYLEN + 1]
)
{

    if (!_is_setup) return false;
    #ifndef _SH_REGIONS_SCRIPT
        return false;
    #endif

    for 
    (
        uint32_t i = 0;
        i < SH_RGN_N_RGNS;
        i++
    )
    {

        if (regions[i].region == r && regions[i].region_id == k)
            return true;

    }

    return false;

}


const static char a[] = "klmnopabcdetuvwxyzfghijqrs";
const static char n[] = "1230456789";
const static char s[] = "!@#$%^&*()_-={}{}|:;'<>,.?/";

int _di = rand();
char __GenKeyOutput[SH_RGN_KEYLEN + 1];

const char*
DefinedRegions::GenerateID(
    unsigned int seed
)
{

    _di %= 256;

    for
    (
        short i = 0;
        i <= SH_RGN_KEYLEN;
        i++
    )
    {
        _di += srand((uint32_t)seed + rand());
        if (_di%3 == 0) __GenKeyOutput[i] = a[_di%26];
        if (_di%3 == 1) __GenKeyOutput[i] = n[_di%10];
        if (_di%3 == 2) __GenKeyOutput[i] = s[_di%27];
    }

    __GenKeyOutput[SH_RGN_KEYLEN] = '\0';
    return __GenKeyOutput;

}


signed int 
DefinedRegions::find_avail_index() 
{

    #if !defined(_SH_REGIONS_SCRIPT)
        return -2;
    #endif

    if (!_is_setup) return -3;

    for
    (
        uint32_t i = 0;
        i < SH_RGN_N_RGNS;
        i++
    )
    {
        if (!regions[i]._occ && !regions[i].prtc) return i;
    }

    return -1;

}


bool 
operator ==(
    Region r1, 
    Region r2
)
{
    
    return (
            r1.x0 == r2.x0
        &&  r1.xf == r2.xf
        &&  r1.y0 == r2.y0
        &&  r1.yf == r2.yf
    );

}


bool 
operator ==(
    RegionDefinition rd1,
    RegionDefinition rd2
)
{

    return (
            rd1.region_id == rd2.region_id 
        &&  rd1.region == rd2.region
        &&  rd1.prtc == rd2.prtc
        &&  rd1._occ == rd2._occ
        &&  rd1.master_region_ptr == rd2.master_region_ptr
        &&  rd1.is_sub_region == rd2.is_sub_region
        &&  rd1.n_sub_regions == rd2.n_sub_regions
    );

}
