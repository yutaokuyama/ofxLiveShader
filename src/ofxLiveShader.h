#pragma once
#include "ofMain.h"
#include <sys/stat.h>
#include <fstream>



struct check_shader{
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
    struct stat stat_buf_f;
    struct stat stat_buf_v;
     struct stat stat_buf_g;
    time_t last_file_time_f;
    time_t last_file_time_v;
    time_t last_file_time_g;
    
    ofShader shader;
    
    
};

class ofxLiveShader{
public:
    
    void setup(string name_v,string name_f);
    void setup(string name_v,string name_f,string name_g);
    
    void CheckandCompile();
    void change_path(string v_name,string f_name);
    void readFromFile(string *str,string path,string backup,vector<string> *diff);
    
    void begin();
    void end();
    void setUniform3f(string name,ofVec3f value);
    void setUniform1f(string name,float value);
    void setUniform1i(string name,int value);
    void setUniform2f(string name,ofVec2f value);
    
    void setUniformMatrix4f(string name,ofMatrix4x4 value);
    void setUniformMatrix3f(string name,ofMatrix3x3 value);
    void setUniformTexture(string name,ofTexture tex,int bindPoint);
    
    
   // void drawMessage();
    
    //check_shader shader;
    
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
    
    void setGeometryInputType(GLenum  type);
    void setGeometryOutputType(GLenum type);
    void setGeometryOutputCount(int num);
    
    GLenum inputType  = GL_TRIANGLES;
    GLenum outputType = GL_TRIANGLE_STRIP;
    int outputNum = 0;
    
    
    
    
    
};
