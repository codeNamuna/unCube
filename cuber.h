
//Generate a cube of given size and describe its normals

void drawCube(float sizex, float sizey, float sizez, float x, float y, float z, float r, float g, float b){
    sizex=sizex/2;
    sizey=sizey/2;
    sizez=sizez/2;
    GLfloat vertA[3] = {sizex,sizey,sizez};
    GLfloat vertB[3] = {-sizex,sizey,sizez};
    GLfloat vertC[3] = {-sizex,-sizey,sizez};
    GLfloat vertD[3] = {sizex,-sizey,sizez};
    GLfloat vertE[3] = {sizex,sizey,-sizez};
    GLfloat vertF[3] = {-sizex,sizey,-sizez};
    GLfloat vertG[3] = {-sizex,-sizey,-sizez};
    GLfloat vertH[3] = {sizex,-sizey,-sizez};
    glPushMatrix();
    glTranslatef(x,y,z);
    glBegin(GL_QUADS);
        glColor3f(r,g,b);
        glNormal3f(0.0,0.0,1.0);
        glVertex3fv(vertA);
        glVertex3fv(vertB);
        glVertex3fv(vertC);
        glVertex3fv(vertD);
        glNormal3f(-1.0,0.0,0.0);
        glVertex3fv(vertB);
        glVertex3fv(vertF);
        glVertex3fv(vertG);
        glVertex3fv(vertC);
        glNormal3f(0.0,0.0,-1.0);
        glVertex3fv(vertF);
        glVertex3fv(vertE);
        glVertex3fv(vertH);
        glVertex3fv(vertG);
        glNormal3f(1.0,0.0,0.0);
        glVertex3fv(vertE);
        glVertex3fv(vertA);
        glVertex3fv(vertD);
        glVertex3fv(vertH);
        glNormal3f(0.0,1.0,0.0);
        glVertex3fv(vertA);
        glVertex3fv(vertB);
        glVertex3fv(vertF);
        glVertex3fv(vertE);
        glNormal3f(0.0,-1.0,0.0);
        glVertex3fv(vertD);
        glVertex3fv(vertC);
        glVertex3fv(vertG);
        glVertex3fv(vertH);
    glEnd();
    glTranslatef(-x,-y,-z);
    glPopMatrix();
}
