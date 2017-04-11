
//Render various kinds of fonts

void RenderFont(float x, float y, const char * str){
    const char * c;
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -1.0f);
    glColor3f(1.0f,1.0f,1.0f);
    glRasterPos2f(x,y);
    for(c=str; *c!='\0'; c++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glEnable(GL_FOG);
    glEnable(GL_LIGHTING);
}

void RenderFontSmall(float x, float y, const char * str){
    const char * c;
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -1.0f);
    glColor3f(1.0f,1.0f,1.0f);
    glRasterPos2f(x,y);
    for(c=str; *c!='\0'; c++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
    }
    glEnable(GL_FOG);
    glEnable(GL_LIGHTING);
}

void RenderGameOverFont(float x, float y, const char * str){
    const char * c;
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -1.0f);
    glColor3f(0.8f,0.0f,0.0f);
    glRasterPos2f(x,y);
    for(c=str; *c!='\0'; c++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    glEnable(GL_FOG);
    glEnable(GL_LIGHTING);
}
