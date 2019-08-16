//
//  ofxLiveShader.cpp
//  filecheck_test
//
//  Created by yutaokuyama on 2019/04/01.
//


////使い方
/*
setupにシェーダの名前ぶっこむ
 updateでcompile〜回してあげるといける
 vectorでdiff渡してあげると差分が入って帰ってくる
 
 
 
 */

#include "ofxLiveShader.h"

void ofxLiveShader::setup(string name_v,string name_f){

    shader.path_fragment =ofFilePath::getAbsolutePath(name_f,true);
    shader.path_vertex = ofFilePath::getAbsolutePath(name_v,true);
    stat(shader.path_vertex.c_str(), &shader.stat_buf_v);
    stat(shader.path_fragment.c_str(),&shader.stat_buf_f);
    cout <<"set shaders"+ofToString(name_v)+ofToString(name_f)<<endl;
    shader.last_file_time_f = shader.stat_buf_f.st_mtime;
       shader.last_file_time_v = shader.stat_buf_f.st_mtime;
    
    shader.shader.load(shader.path_vertex,shader.path_fragment);
    
}

void ofxLiveShader::CheckandCompile(){
    stat(shader.path_vertex.c_str(),&shader.stat_buf_v);
     stat(shader.path_fragment.c_str(),&shader.stat_buf_f);
    if(shader.stat_buf_f.st_mtime != shader.last_file_time_f || shader.stat_buf_v.st_mtime != shader.last_file_time_v){
        cout <<"shader modify detect"<<endl;
        shader.last_file_time_f = shader.stat_buf_f.st_mtime;
        shader.last_file_time_v = shader.stat_buf_v.st_mtime;
        string backup_v = shader.shader.getShaderSource(GL_VERTEX_SHADER);
        string backup_f = shader.shader.getShaderSource(GL_FRAGMENT_SHADER);
        shader.diff.clear();
        readFromFile(&shader.src_vertex, shader.path_vertex,backup_v,&shader.diff);
        readFromFile(&shader.src_fragment, shader.path_fragment,backup_f,&shader.diff);
        for(int k = 0;k<shader.diff.size();k++){
            cout<<shader.diff[k]<<endl;
        }
        
        shader.compile_f = shader.shader.setupShaderFromSource(GL_FRAGMENT_SHADER,shader.src_fragment);
        shader.compile_v = shader.shader.setupShaderFromSource(GL_VERTEX_SHADER,shader.src_vertex);
        
        if(shader.compile_f == true&&shader.compile_v == true){
            cout <<"success compile shader"<< endl;
        }else{
            cout <<"fail to compile shader"<< endl;
            shader.shader.setupShaderFromSource(GL_FRAGMENT_SHADER,backup_f);
            shader.shader.setupShaderFromSource(GL_VERTEX_SHADER,backup_v);
        }
        shader.shader.bindDefaults();
        shader.shader.linkProgram();
        shader.src_fragment.clear();
        shader.src_vertex.clear();
        
    }
    
}


void ofxLiveShader::readFromFile(string *str,string path,string backup,vector<string> *diff){

    //------split backup to row
    stringstream ss(backup);
    string item;
    vector<string> splited_backup;
    while(getline(ss, item)){
        splited_backup.push_back(item);
         // ofLog()<<item<<endl;
    }

    ofBuffer buffer  = ofBufferFromFile(path);
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;
        bool se = true;
        for(int i = 0;i<splited_backup.size();i++){
            if(line.c_str() == splited_backup[i]){
                se = false;
            }
        }
        if(se){
            if(line != ""){
            diff->push_back(line);
            }
        }
        if(line.empty() == false) {
            *str += line;
            *str += "\n";
        }
    }
}

void ofxLiveShader::change_path(string v_name,string f_name){
    setup(v_name, f_name);
    CheckandCompile();
}

void ofxLiveShader::begin(){
    shader.shader.begin();
   
}

void ofxLiveShader::end(){
    shader.shader.end();
}

void ofxLiveShader::setUniform1f(string name,float value){
    shader.shader.setUniform1f(name,value);
}

void ofxLiveShader::setUniform3f(string name,ofVec3f value){
    shader.shader.setUniform3f(name,value);
}

void ofxLiveShader::setUniform1i(string name,int value){
    shader.shader.setUniform1f(name,value);
}

void ofxLiveShader::setUniformMatrix3f(string name,ofMatrix3x3 value){
    shader.shader.setUniformMatrix3f(name,value);
}


void ofxLiveShader::setUniformMatrix4f(string name,ofMatrix4x4 value){
    shader.shader.setUniformMatrix4f(name,value);
}










