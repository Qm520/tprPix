/*
 * ====================== Density.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.03.23
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Density.h"

//------------------- C --------------------//
#include <cmath>
#include <cassert>

//------------------- Engine --------------------//
#include "simplexNoise.h"
#include "esrc_gameSeed.h"
#include "esrc_ecoObj.h"

//#include "debug.h"

//---------- static ----------//
int Density::minLvl {-3};
int Density::maxLvl {3};


/* ===========================================================
 *                         set
 * -----------------------------------------------------------
 * 根据 ecoObj 预设的方案 来配置 density.lvl
 */
void Density::set( const IntVec2 &_fieldMPos, 
                    const float &_ecoObj_densitySeaLvlOff,
                    const std::vector<float> *_ecoObj_densityDivideValsPtr ){
    //-----------//
    //   seaLvl
    //-----------//
    float seaLvlOff = _ecoObj_densitySeaLvlOff; //- [-20.0, 20.0]
    float freqSeaLvl = 0.003125 * 0.5; //- 10*10 个 chunk，构成一个 perlin 晶格
    float pnValSeaLvl = simplex_noise2( _fieldMPos.x * freqSeaLvl, 
                                        _fieldMPos.y * freqSeaLvl ) * 15.0 + seaLvlOff; // [-20.0, 20.0]
    //-----------//
    //   pnVal
    //-----------//
    const glm::vec2 &densitySeed_pposOff = esrc::gameSeed.get_densitySeed_pposOff();
    float freq = 0.03125 * 0.5; //- 1*1 个 chunk，构成一个 perlin 晶格
    float x = static_cast<float>(_fieldMPos.x) + densitySeed_pposOff.x;
    float y = static_cast<float>(_fieldMPos.y) + densitySeed_pposOff.y;
    y *= 1.3; //- 在纵向上适当压缩
    float pnVal = simplex_noise2(x*freq, y*freq) * 100.0 - pnValSeaLvl; // [-120.0, 120.0]

    if( pnVal > 100.0 ){
        pnVal = 100.0;
    }
    if( pnVal < -100.0 ){
        pnVal = 100.0;
    } // [-100.0, 100.0]

    //-----------//
    //   lvl
    //-----------//
    int tmpLvl = Density::minLvl;
    bool  is_find {false};
    for( auto f=_ecoObj_densityDivideValsPtr->cbegin(); 
        f!=_ecoObj_densityDivideValsPtr->cend(); f++ ){
        if( pnVal < *f ){
            is_find = true;
            this->lvl = tmpLvl;
            break;
        }
        tmpLvl++;
    }   
    if( !is_find ){
        this->lvl = Density::maxLvl;
    }
    assert( (this->lvl>=Density::minLvl) && (this->lvl<=Density::maxLvl) );
}


