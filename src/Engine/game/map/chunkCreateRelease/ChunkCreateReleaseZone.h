/*
 * ================= ChunkCreateReleaseZone.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_CHUNK_CREATE_ZONE_H
#define TPR_CHUNK_CREATE_ZONE_H

//-------------------- C --------------------//
#include <cmath>

//-------------------- CPP --------------------//
#include <vector>
#include <unordered_set>
#include <unordered_map>

//-------------------- Engine --------------------//
#include "tprCast.h"
#include "config.h"
#include "Chunk.h"
#include "tprAssert.h"
#include "NineBox.h"


// only two instances in esrc:
// -- chunkCreateZone
// -- chunkReleaseZone
class ChunkZone{
public:
    explicit ChunkZone( int sideLen_ ): //- based on chunk
        sideLen(sideLen_),
        offLen(sideLen_/2),
        offLenMPos(offLen*ENTS_PER_CHUNK)
        {
            tprAssert( (sideLen_>0) && (sideLen_%2 != 0) );
        }
    inline int get_offLenMPos() const {
        return this->offLenMPos;
    }
    inline size_t get_chunks() const {
        return cast_2_size_t(this->sideLen * this->sideLen);
    }
    inline int get_offLen() const {
        return this->offLen;
    }
private:
    //===== vals =====//
    int         sideLen     {}; //- alin to chunks
    int         offLen      {}; //- alin to chunks
    int         offLenMPos  {}; //- 半边长, alin to chunk-mpos
};



class ChunkCreateReleaseZone{
public:
    ChunkCreateReleaseZone( int sideLen_create_, 
                            int sideLen_release_ ): //- based on chunk
        createZone( sideLen_create_),
        releaseZone(sideLen_release_),
        LimitOff( std::abs(createZone.get_offLenMPos() - releaseZone.get_offLenMPos()) )
        {
                tprAssert( sideLen_create_ == 3 ); //- tmp
            this->init_createZoneOffMPosesSets();
            this->init_releaseZoneOffMPoses();
        }

    void init( const IntVec2 &playerMPos_ );

    inline const std::vector<IntVec2> &get_createZoneOffMPoses( NineBoxIdx idx_ ) const {
        tprAssert( idx_ != NineBoxIdx::Mid_Mid );
        return this->createZoneOffMPosesSets.at(idx_);
    }
    inline const std::vector<IntVec2> &get_releaseZoneOffMPoses() const {
        return this->releaseZoneOffMPoses;
    }

    void refresh_and_collect_chunks_need_to_be_release( const IntVec2 &playerMPos_ ); // major-func

private:
    void init_createZoneOffMPosesSets();
    void init_releaseZoneOffMPoses();

    //===== vals =====//
    ChunkZone createZone;
    ChunkZone releaseZone;

    int LimitOff;   //- 释放圈 比 新建圈 大出的 半边长，alin to chunk-mpos
                    // 当 两圈center间距离，大于此值，说明新建圈越界，要推动 释放圈了

    IntVec2   releaseZoneCenterChunkMCPos {};
    
    //-- 轮流扮演 old/new 容器，容纳 释放圈覆盖的所有 chunkkeys --
    std::unordered_set<chunkKey_t> releaseChunkKeys_A {};
    std::unordered_set<chunkKey_t> releaseChunkKeys_B {};

    //- 根据 player 的运动方向，来选择不同的 新建chunks次序。
    //  确保优先创建 运动方向前方的 chunks
    //  key 仅为 player 运动方向
    //  ---
    //  目前版本 固定 3*3 激活圈
    std::unordered_map<NineBoxIdx, std::vector<IntVec2>> createZoneOffMPosesSets {};

    //- 释放圈 所有 chunk.offMPos
    //  尺寸基于 releaseZone.sideLen
    std::vector<IntVec2> releaseZoneOffMPoses {};

    //===== flags =====//
    bool is_currentOld_in_A   {};
};


#endif 

