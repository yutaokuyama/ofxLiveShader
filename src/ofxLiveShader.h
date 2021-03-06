#pragma once
#include "ofMain.h"
#include <sys/stat.h>
#include <fstream>

using namespace glm;

class ofxLiveShader{
public:
    
    void setup(string name_v,string name_f);
    void setup(string name);
    void setup(string name_v,string name_f,string name_g);
    void setName(string _name);
    
    bool CheckandCompile();
    bool update();
    void change_path(string v_name,string f_name);
    void readFromFile(string *str,string path,string backup,vector<string> *diff);
    
    void begin();
    void end();
    void setUniform3f(string name,ofVec3f value);
    void setUniform1f(string name,float value);
    void setUniform1i(string name,int value);
    void setUniform2f(string name,ofVec2f value);
    
    void setUniform4f(string name,ofVec4f value);
    void setUniform4f(string name, ofFloatColor value);
    
    
    void setUniformMatrix4f(string name,ofMatrix4x4 value);
    void setUniformMatrix3f(string name,ofMatrix3x3 value);
    void setUniformTexture(string name,ofTexture tex,int bindPoint);
     void setUniformTexture(string name,ofTexture tex);
    
    
    void setBasicUniforms();
    void setBasicUniforms(ofEasyCam &cam);
     void setBasicUniforms(ofEasyCam &cam,vec3 lightPos);
    
    void setCamAndLightUniforms(ofEasyCam &cam,vec3 lightPos);
    
    void drawFillRectangle();
    
    //モデル変換行列ガンガンに使ってる中で座標とか送るとたぶんすごいずれるから送るタイミングズラせるようにしよう！
    
    
    // void drawMessage();

    
    string path_fragment;
    string path_vertex;
    string path_geometry;
    string src_fragment;
    string src_vertex;
    string src_geometry;
    vector<string> diff;
    bool compile_f;
    bool compile_v;
    bool compile_g;
    bool bGeometry = false;
    struct stat stat_buf_f;
    struct stat stat_buf_v;
    struct stat stat_buf_g;
    time_t last_file_time_f;
    time_t last_file_time_v;
    time_t last_file_time_g;
    
    ofShader shader;
    string name;
    
    void setGeometryInputType(GLenum  type);
    void setGeometryOutputType(GLenum type);
    void setGeometryOutputCount(int num);
    
    
    GLenum inputType  = GL_TRIANGLES;
    GLenum outputType = GL_TRIANGLE_STRIP;
    int outputNum = 0;
    
    int textureCount =0;
    bool isSetuped = false;
    
};


class ofxLiveShaderDirectory{
public:
    //ofxLiveShaderDirectory();
    
    void allocate(int width,int height);
    void allocate(vec2 size);
    
    void add(ofxLiveShader shader,string name);
    void update();
    bool remove(string _name);
    
    
    vector<ofxLiveShader> shaders;
    vector<string> names;
    //----for postProcess
    ofFbo process(ofTexture &tex);
    ofFbo fbo[2];
    int frame = 0;
    void swap();
    string getEnabledProcess();

    int mod = 1;
    
    
};
