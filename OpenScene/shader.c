#include "shader.h"
#ifdef WIN32
#include "glew.h"
#else
#include <GL/gl.h>
#endif

ObjectProgram shader_compile(char* source, GLenum type)
{
    FILE * pFile;
    //unsigned long fileLen;
    pFile = fopen(source,"r+");
    ObjectProgram obj = 0;
    int lines = 0;
    if(pFile)
    {
        obj = glCreateShader(type);
        char buffer[600];
        while(! feof(pFile))
        {
            fgets(buffer,600,pFile);
            //printf("%s",buffer);
            lines++;
        }

        fseek(pFile, 0, SEEK_SET);
        lines--;
        char ** src = (char**) malloc(sizeof(char*)*lines);
        int* src_lenghts = (int*) malloc(sizeof(int)*lines);
        int i,lenght;

        for(i = 0; i < lines; i++)
        {
            fgets(buffer,600,pFile);
            lenght = strlen(buffer);

            src[i] = (char*) malloc(lenght);
            memcpy(src[i],buffer,lenght);

            src_lenghts[i] = lenght;
        }
        glShaderSource(obj,lines,(const GLchar **)src,src_lenghts);

        glCompileShader(obj);

        int compiled = 0;
        glGetShaderiv(obj,GL_COMPILE_STATUS,&compiled);

        if(! compiled)
        {
            char log[5000];
            int log_size = 0;
            glGetShaderInfoLog(obj,5000,&log_size,log);

            printf("ERROR COMPILING SHADER: %s:\n%s\n",source,log);
        }
        for(i = 0; i < lines; i++)
        {
            free(src[i]);
        }
        free(src);
        free(src_lenghts);
    }
    else
    {
        printf("ERROR: opening shader file %s.\n",source);
    }

    return obj;
}

Shader shader_new(char* vert_file, char* frag_file)
{
    Shader shader = 0;
    ObjectProgram vert_p = shader_compile(vert_file, GL_VERTEX_SHADER);
    ObjectProgram frag_p = shader_compile(frag_file, GL_FRAGMENT_SHADER);

    if(vert_p && frag_p)
    {
        shader = glCreateProgram();
        glAttachShader(shader,vert_p);
        glAttachShader(shader,frag_p);
        glLinkProgram(shader);

        int linked = 0;

        glGetProgramiv(shader,GL_LINK_STATUS,&linked);

        if( !linked)
        {
            char log[5000];
            int log_size = 0;
            glGetProgramInfoLog(shader,5000,&log_size,log);

            printf("ERROR LINKING SHADER: %s, %s:\n%s\n",vert_file,frag_file,log);
        }
    }
    return shader;
}

void shader_free(Shader shader)
{
    glDeleteProgram(shader);
}

void shader_use(Shader shader)
{
    glUseProgram(shader);
}

void shader_stop(Shader shader)
{
    glUseProgram(0);
}

GLint shader_get_att_loc(Shader shader, char* att)
{
    return glGetAttribLocation(shader, att);
}

GLint shader_get_unif_loc(Shader shader, char* unif)
{
    return glGetUniformLocation(shader,unif);
}
