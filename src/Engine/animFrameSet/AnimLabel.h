/*
 * ========================== AnimLabel.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.12
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * 
 */
#ifndef TPR_ANIM_LABEL_H
#define TPR_ANIM_LABEL_H

//------------------- CPP --------------------//
#include <utility> // pair
#include <string>
#include <vector>

//------------------- Libs --------------------//
#include "tprDataType.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"


//-- 亚种 标签 --
// 一个亚种，最多允许携带 0～2 个标签
enum class AnimLabel : u32_t{
    Default=0, //- 未设置 label 的，自动使用此标签来搜索
    //---
    Sml,
    Mid,
    Big,
    //---
    Tint,
    Dark,
    //---
    Fat,
    Thin,
    HalfDead, //- tmp

    //...
};

inline const std::vector<AnimLabel> emptyAnimLabels {}; //- Must Keep Empty !!!


inline AnimLabel val_2_AnimLabel( u32_t val_ )noexcept{
    switch (val_){
    case 0:  return AnimLabel::Default;

    case 1:  return AnimLabel::Sml;
    case 2:  return AnimLabel::Mid;
    case 3:  return AnimLabel::Big;

    case 4:  return AnimLabel::Tint;
    case 5:  return AnimLabel::Dark;

    case 6:  return AnimLabel::Fat;
    case 7:  return AnimLabel::Thin;
    case 8:  return AnimLabel::HalfDead;
    //.......
    default:
        tprAssert(0);
        return AnimLabel::Default; // never reach
    }
}


inline AnimLabel str_2_AnimLabel( const std::string &str_ )noexcept{
    if(       str_ == "Default" ){  return AnimLabel::Default;
    
    }else if( str_ == "Sml" ){      return AnimLabel::Sml;
    }else if( str_ == "Mid" ){      return AnimLabel::Mid;
    }else if( str_ == "Big" ){      return AnimLabel::Big;

    }else if( str_ == "Tint" ){    return AnimLabel::Tint;
    }else if( str_ == "Dark" ){     return AnimLabel::Dark;

    }else if( str_ == "Fat" ){      return AnimLabel::Fat;
    }else if( str_ == "Thin" ){     return AnimLabel::Thin;
    }else if( str_ == "HalfDead" ){ return AnimLabel::HalfDead;
    }

    else{
        tprAssert(0);
        return AnimLabel::Default; // never reach
    }
}




//-- 将2个 animLabel，合并为一个 key，用于容器 --
using animLabelKey_t = u64_t;


inline animLabelKey_t animLabels_2_key( AnimLabel a_, AnimLabel b_ )noexcept{

    if( (a_==AnimLabel::Default) && (b_==AnimLabel::Default) ){
        return 0;
    }
    //---
    tprAssert( a_ != b_ );
    u32_t ua = static_cast<u32_t>(a_);
    u32_t ub = static_cast<u32_t>(b_);
    //---
    animLabelKey_t key {};
    u32_t *ptr = (u32_t*)(&key);
    if( ua < ub ){
        *ptr = ua;
        ptr++;
        *ptr = ub;
    }else{
        *ptr = ub;
        ptr++;
        *ptr = ua;
    }
    return key;
}


inline std::pair<AnimLabel,AnimLabel> animLabelKey_2_labels( animLabelKey_t key_ )noexcept{

    AnimLabel a {};
    AnimLabel b {};
    u32_t *ptr = (u32_t*)(&key_);
    a = val_2_AnimLabel( *ptr );
    ptr++;
    b = val_2_AnimLabel( *ptr );

    if( !((a==AnimLabel::Default) && (b==AnimLabel::Default)) ){
        tprAssert( a != b );
    }
    return {a,b};
}

#endif 

