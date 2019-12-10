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
    
    path_fragment =ofFilePath::getAbsolutePath(name_f,true);
    path_vertex = ofFilePath::getAbsolutePath(name_v,true);
    stat(path_vertex.c_str(), &stat_buf_v);
    stat(path_fragment.c_str(),&stat_buf_f);
    cout <<"set shaders"+ofToString(name_v)+ofToString(name_f)<<endl;
    last_file_time_f = stat_buf_f.st_mtime;
    last_file_time_v = stat_buf_v.st_mtime;
    
    shader.load(path_vertex,path_fragment);
    
}

void ofxLiveShader::setup(string name_v,string name_f,string name_g){
    
    shader.setGeometryInputType(inputType);
    shader.setGeometryOutputType(outputType);
    shader.setGeometryOutputCount(outputNum);
    
    
    path_fragment =ofFilePath::getAbsolutePath(name_f,true);
    path_vertex = ofFilePath::getAbsolutePath(name_v,true);
    path_geometry = ofFilePath::getAbsolutePath(name_g,true);
    
    stat(path_vertex.c_str(), &stat_buf_v);
    stat(path_fragment.c_str(),&stat_buf_f);
    stat(path_geometry.c_str(),&stat_buf_g);
    
    cout <<"set shaders"+ofToString(name_v)+" "+ofToString(name_f)+" "+ofToString(name_g)<<endl;
    last_file_time_f = stat_buf_f.st_mtime;
    last_file_time_v = stat_buf_v.st_mtime;
    last_file_time_g = stat_buf_g.st_mtime;
    
    shader.load(path_vertex,path_fragment,path_geometry);
    
    bGeometry = true;
    
}


bool ofxLiveShader::CheckandCompile(){
    if(!bGeometry){
        stat(path_vertex.c_str(),&stat_buf_v);
        stat(path_fragment.c_str(),&stat_buf_f);
        
        if(stat_buf_f.st_mtime != last_file_time_f || stat_buf_v.st_mtime != last_file_time_v){
            cout <<"shader update!"<<endl;
            last_file_time_f = stat_buf_f.st_mtime;
            last_file_time_v = stat_buf_v.st_mtime;
            string backup_v = shader.getShaderSource(GL_VERTEX_SHADER);
            string backup_f = shader.getShaderSource(GL_FRAGMENT_SHADER);
            diff.clear();
            readFromFile(&src_vertex, path_vertex,backup_v,&diff);
            readFromFile(&src_fragment, path_fragment,backup_f,&diff);
            for(int k = 0;k<diff.size();k++){
                cout<<diff[k]<<endl;
            }
            
            compile_f = shader.setupShaderFromSource(GL_FRAGMENT_SHADER,src_fragment);
            compile_v = shader.setupShaderFromSource(GL_VERTEX_SHADER,src_vertex);
            
            if(compile_f == true&&compile_v == true){
                cout <<"success compile shader"<< endl;
                shader.bindDefaults();
                shader.linkProgram();
                src_fragment.clear();
                src_vertex.clear();
                return true;
            }else{
                cout <<"fail to compile shader"<< endl;
                GLsizei bufSize;
                
                shader.setupShaderFromSource(GL_FRAGMENT_SHADER,backup_f);
                shader.setupShaderFromSource(GL_VERTEX_SHADER,backup_v);
                shader.bindDefaults();
                shader.linkProgram();
                src_fragment.clear();
                src_vertex.clear();
                return false;
                
            }
            
            
        }
        
    }else{
        
        stat(path_vertex.c_str(),&stat_buf_v);
        stat(path_fragment.c_str(),&stat_buf_f);
        stat(path_geometry.c_str(),&stat_buf_g);
        
        if(stat_buf_f.st_mtime != last_file_time_f || stat_buf_v.st_mtime != last_file_time_v||stat_buf_g.st_mtime != last_file_time_g){
            cout <<"shader update!"<<endl;
            last_file_time_f = stat_buf_f.st_mtime;
            last_file_time_v = stat_buf_v.st_mtime;
            last_file_time_g = stat_buf_g.st_mtime;
            string backup_v = shader.getShaderSource(GL_VERTEX_SHADER);
            string backup_f = shader.getShaderSource(GL_FRAGMENT_SHADER);
            string backup_g = shader.getShaderSource(GL_GEOMETRY_SHADER);
            diff.clear();
            readFromFile(&src_vertex, path_vertex,backup_v,&diff);
            readFromFile(&src_fragment, path_fragment,backup_f,&diff);
            readFromFile(&src_geometry, path_geometry,backup_g,&diff);
            for(int k = 0;k<diff.size();k++){
                cout<<diff[k]<<endl;
            }
            
            compile_f = shader.setupShaderFromSource(GL_FRAGMENT_SHADER,src_fragment);
            compile_v = shader.setupShaderFromSource(GL_VERTEX_SHADER,src_vertex);
            compile_g = shader.setupShaderFromSource(GL_GEOMETRY_SHADER,src_geometry);
            if(compile_f == true&&compile_v == true&&compile_g == true){
                cout <<"success compile shader!"<< endl;
            }else{
                cout <<"fail to compile shader"<< endl;
                GLsizei bufSize;
                
                shader.setupShaderFromSource(GL_FRAGMENT_SHADER,backup_f);
                shader.setupShaderFromSource(GL_VERTEX_SHADER,backup_v);
                shader.setupShaderFromSource(GL_GEOMETRY_SHADER,backup_g);
                
            }
            shader.bindDefaults();
            shader.linkProgram();
            src_fragment.clear();
            src_vertex.clear();
            src_geometry.clear();
            
        }
        
        
        
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
    shader.begin();
    
}

void ofxLiveShader::end(){
    shader.end();
}

void ofxLiveShader::setUniform1f(string name,float value){
    shader.setUniform1f(name,value);
}
void ofxLiveShader::setUniform2f(string name,ofVec2f value){
    shader.setUniform2f(name,value);
}


void ofxLiveShader::setUniform3f(string name,ofVec3f value){
    shader.setUniform3f(name,value);
}


void ofxLiveShader::setUniform1i(string name,int value){
    shader.setUniform1i(name,value);
}

void ofxLiveShader::setUniformMatrix3f(string name,ofMatrix3x3 value){
    shader.setUniformMatrix3f(name,value);
}


void ofxLiveShader::setUniformMatrix4f(string name,ofMatrix4x4 value){
    shader.setUniformMatrix4f(name,value);
}


void ofxLiveShader::setUniform4f(string name,ofVec4f value){
    shader.setUniform4f(name,value);
}


void ofxLiveShader::setUniform4f(string name,ofFloatColor value){
    shader.setUniform4f(name,value);
}




void ofxLiveShader::setUniformTexture(std::string name, ofTexture tex, int bindPoint){
    shader.setUniformTexture(name,tex,bindPoint);
}


void ofxLiveShader::setGeometryOutputType(GLenum type){
    inputType = type;
}

void ofxLiveShader::setGeometryInputType(GLenum type){
    inputType = type;
}

void ofxLiveShader::setGeometryOutputCount(int num){
    outputNum = num;
}


void ofxLiveShader::setBasicUniforms(){
    shader.setUniform1f("time",ofGetElapsedTimef());
    shader.setUniform2f("resolution",glm::vec2(ofGetWidth(),ofGetHeight()));
}

void ofxLiveShader::setName(string _name){
    name = _name;
}


bool ofxLiveShader::update(){
    CheckandCompile();
}

void ofxLiveShader::setBasicUniforms(ofEasyCam &cam){
    shader.setUniform1f("time",ofGetElapsedTimef());
    shader.setUniform2f("resolution",glm::vec2(ofGetWidth(),ofGetHeight()));
    shader.setUniform3f("eyePos",cam.getPosition());
    shader.setUniformMatrix3f("normalMatrix", glm::inverse(glm::transpose(cam.getModelViewMatrix())));
    
    
}


//--------shaderDirectory
//シンプルに解決するポストエフェクトのみ一括で対応することに
void ofxLiveShaderDirectory::allocate(int width,int height){
    for(int i = 0;i<2;i++){
        fbo[i].allocate(width,height);
    }
}

void ofxLiveShaderDirectory::allocate(vec2 size){
    for(int i = 0;i<2;i++){
        fbo[i].allocate(size.x,size.y);
    }
}
void ofxLiveShaderDirectory::add(ofxLiveShader shader,string name){
   // cout<<"add "<<name<<endl;
    shaders.push_back(shader);
    names.push_back(name);

}

void ofxLiveShaderDirectory::update(){
    for(int i=0;i<shaders.size();i++){
        shaders[i].update();
    }
}

bool ofxLiveShaderDirectory::remove(string _name){

    for(int i = 0;i<shaders.size();i++){
        if(_name == shaders[i].name){
            shaders.erase(shaders.begin()+i);
            names.erase(names.begin()+i);
            return true;
        }
    }
    return false;
}


void ofxLiveShaderDirectory::swap(){
    frame = 1-frame;
}
string ofxLiveShaderDirectory::getEnabledProcess(){
    string tmp = "";
    //cout<<shaders.size()<<endl;
    for(int i = 0;i<shaders.size();i++){
        tmp += names[i];
        tmp+= '\n';
    }
    return tmp;
}

ofFbo ofxLiveShaderDirectory::process(ofTexture  &tex){
    fbo[1-frame].begin();
    ofClear(0,255);
    tex.draw(0.0,0.0,ofGetWidth(),ofGetHeight());
    fbo[1-frame].end();
    swap();
    
    for(int i = 0;i<shaders.size();i++){
        fbo[1-frame].begin();
        ofClear(0,255);
        shaders[i].begin();
        shaders[i].setUniform2f("resolution", vec2(fbo[0].getWidth(),fbo[0].getHeight()));
        shaders[i].setUniform1f("time", ofGetElapsedTimef());
        shaders[i].setUniform1f("div",mod);
  
            shaders[i].setUniformTexture("fbo", fbo[frame].getTexture(), 0);
        
        
        ofDrawRectangle(0.0,0.0,ofGetWidth(),ofGetHeight());
        shaders[i].end();
        fbo[1-frame].end();
        swap();
    }
    return fbo[frame];
}



