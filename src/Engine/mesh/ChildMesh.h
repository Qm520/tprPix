/*
 * ========================= ChildMesh.h ==========================
 *                          -- tpr --
 *                                        创建 -- 2019.01.22
 *                                        修改 -- 2019.01.22
 * ----------------------------------------------------------
 *  
 * ----------------------------
 */
#ifndef _TPR_CHILD_MESH_H_
#define _TPR_CHILD_MESH_H_

//--- glm - 0.9.8 ---
#include <glm/glm.hpp>
            //-- glm::vec2
            //-- glm::vec3
            //-- glm::vec4
            //-- glm::mat4
#include <glm/gtc/matrix_transform.hpp>
            //-- glm::translate
            //-- glm::rotate
            //-- glm::scale
            //-- glm::perspective
#include <glm/gtc/type_ptr.hpp> 
            //-- glm::value_ptr


//-------------------- Engine --------------------//
#include "ShaderProgram.h" //-- each GameObjMesh instance,will bind a shader

//--- need ---//
class GameObjMesh;
class GameObj;


//-- pic/shadow 共用一套结构 --//
class ChildMesh{
public:
    explicit ChildMesh( bool _isPic ):
        isPic(_isPic)
        {}

    inline void init( GameObj *_goPtr, GameObjMesh *_goMeshPtr ){
        goPtr = _goPtr;
        goMeshPtr = _goMeshPtr;
    }

    void draw();

    inline void set_shader_program( ShaderProgram *_sp ) { shaderPtr=_sp; }
    inline void set_rotate_z( float _z )                 { rotate_z=_z; }
    inline void set_scale( const glm::vec3 &_v )         { scale_val=_v; }

    //-- 此函数 只能在 RenderUpdate 阶段被调用 --
    //-- 其余代码 不应随意调用 此函数!!! --
    void refresh_translate();

    //- 大部分 具象go实例 的 goMesh图元 长宽值 与 action数据 强关联 --
    //  所以可以直接从 action 中获取数据
    //  这个函数很常用
    //  但如果 action实例 并不更换，也没必要 每1视觉帧 都执行此函数
    void refresh_scale_auto();


    //- 通过 translate_val.z 值 来给 待渲染的 goMeshs 排序 --
    inline float get_render_z() const {
        return translate_val.z;
    }

    //-- 外部 debug 用
    inline const glm::vec3 &get_translate_val() const {
        return translate_val;
    }

    
private:
    GameObj      *goPtr       {nullptr};
    GameObjMesh  *goMeshPtr {nullptr};

    bool     isPic {true}; //-- pic / shadow

    ShaderProgram  *shaderPtr  {nullptr}; 
    //+++++++++ 与 图元 矩阵计算 有关的 变量 ++++++++++++
    glm::mat4 mat4_model {}; //-- 每个 物体obj 都私有一个 模型矩阵
                          //-- 自动初始化为 标准矩阵

    //-- 位移／旋转／缩放 变化向量。
    glm::vec3 translate_val {};    
    float     rotate_z    {0.0f};  //- 只有 z轴旋转角度
    glm::vec3 scale_val  {glm::vec3(1.0f, 1.0f, 1.0f)}; //- 缩放比例（用半径来缩放）

    //--------- funcs ----------
    void update_mat4_model(); //-- 重新计算 model矩阵
};


#endif 
