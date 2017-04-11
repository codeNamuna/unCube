#include<GL/glut.h>
#include<iostream>
#include<math.h>
#include<string>
#include<sstream>
#include "cuber.h"
#include "map_extractor.h"
#include "movement_extractor.h"
#include "RenderFont.h"
#include<windows.h>
#include<mmsystem.h>

#define PI 3.14
#define POP 3
#define ROWS 15
#define COLUMNS 30

using namespace std;

int STEPS = 45;                                                                     //Steps Remaining
int FREEZE_GAME = 0;                                                                //State of the Game
int winid;                                                                          //Window id

int FLASHES = 9;                                                                    //Remaining flashes

static GLint fogMode = GL_EXP;                                                      //Exponential fog

float camerax = 0.0f;                                                               //Camera Positions
float cameray = 0.0f;
float cameraz = 0.0f;

float lightx=2.0f;                                                                  //Cube positions
float lighty=1.0f;
float lightz=0.0f;

int ambient=-50;                                                                    //Light parameters
int diffuse=-100;
int specular=0;

bool spin =0;                                                                       //Cube spin parameters
bool spinValid=1;
float angle = 0.0f;

unsigned char direction='w';                                                        //Initial key press

GLfloat no_mat[]={0.0,0.0,0.0,1.0};                                                 //Material properties
GLfloat mat_diffuse[]={0.3,0.3,0.3,1.0};
GLfloat no_shininess[]={0.0};
GLfloat final_pos[]={0.0f,0.0f};

int beginGame = 1;                                                                  //Initialize game

int currentx = 0;                                                                   //Current block of cube
int currentz = 0;
int finalx = 0;                                                                     //final block
int finaly = 0;

int contour[ROWS*COLUMNS];                                                          //Map generation parameters
int movement[ROWS*COLUMNS*4];
int collisions[ROWS][COLUMNS][4];
int powerState[ROWS][COLUMNS];

GLfloat powerUps[POP][4];                                                           //Power-ups
bool poweredUp=0;
bool POWERUP_CHANGE_IMPENDING=0;

GLfloat fog_density = 0.08f;

void pandora(unsigned char key, int x, int y);
void lightsaber(int key, int x, int y);
void alignCollisionArray();
bool checkCollision(char key);

void drawLight(void){                                                               //Light and fog setup
    GLfloat amb[]={0.01*ambient, 0.01*ambient, 0.01*ambient, 0.2};
    GLfloat diff[]={0.01*diffuse, 0.01*diffuse, 0.01*diffuse, 0.2};
    GLfloat spec[]={0.01*specular, 0.01*specular, 0.01*specular, 0.2};
    GLfloat pos0[]={lightx-2.0, lighty+3.0f, lightz, 1.0};
    GLfloat pos1[]={lightx+2.0, lighty+3.0f, lightz, 1.0};
    GLfloat pos2[]={lightx, lighty+3.0f, lightz-2.0, 1.0};
    GLfloat pos3[]={lightx, lighty+3.0f, lightz+2.0, 1.0};

    glColor3f(1.0f,1.0f,1.0f);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
    glLightfv(GL_LIGHT0, GL_POSITION, pos0);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec);
    glLightfv(GL_LIGHT1, GL_POSITION, pos1);
    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT2, GL_SPECULAR, spec);
    glLightfv(GL_LIGHT2, GL_POSITION, pos2);
    glEnable(GL_LIGHT3);
    glLightfv(GL_LIGHT3, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT3, GL_SPECULAR, spec);
    glLightfv(GL_LIGHT3, GL_POSITION, pos3);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90.0f);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0f);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 90.0f);
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 90.0f);


   glEnable(GL_FOG);
   {
      GLfloat fogColor[4] = {0.2, 0.2, 0.2, 1.0};

      fogMode = GL_EXP;
      glFogi (GL_FOG_MODE, fogMode);
      glFogfv (GL_FOG_COLOR, fogColor);
      glFogf (GL_FOG_DENSITY, fog_density);//0.08);
      glHint (GL_FOG_HINT, GL_DONT_CARE);
      glFogf (GL_FOG_START, 50.0);
      glFogf (GL_FOG_END, 100.0);
   }
   glClearColor(0.2, 0.2, 0.2, 1.0);  /* fog color */
}

void timer(int val){                                                                //Animating function
    if( spin && angle <= 90.0f){                                                    //Spinning the cube
        angle+=3.0f;

        switch(direction){
        case 'w':
            lightx-=(2.0/30.0);
            camerax-=(2.0/30.0);
            break;
        case 's':
            lightx+=(2.0/30.0);
            camerax+=(2.0/30.0);
            break;
        case 'a':
            lightz+=(2.0/30.0);
            cameraz+=(2.0/30.0);
            break;
        case 'd':
            lightz-=(2.0/30.0);
            cameraz-=(2.0/30.0);
            break;
        }

        cameray=sin(angle*PI/90.0);                                                     //Jump camera on y axis
    }

    else if(angle >= 90.0f){
        angle=0.0f;
        spin=0;
        spinValid=1;
        lightx=(float)round(lightx);
        lightz=(float)round(lightz);
        camerax=(float)round(camerax);
        cameraz=(float)round(cameraz);
        cameray=0.0f;
        STEPS--;
        if(POWERUP_CHANGE_IMPENDING){                                                       //Power-up Encountered
            powerUps[powerState[currentx][currentz]-1][0]=0.0f;
            powerState[currentx][currentz]=0;
            POWERUP_CHANGE_IMPENDING=0;
            STEPS+=20;
            diffuse=125;
        }

        if(FREEZE_GAME == 2){                                                               //Final (Target) reached
            diffuse=300;
        }

        if(STEPS==0 && FREEZE_GAME==0)                                                      //Steps run out
            FREEZE_GAME = 1;
    }

    if(diffuse>30){                                                                         //Fading Light Effect (Flash)
        diffuse--;
    }

    if(FREEZE_GAME==1){                                                                     //Fog out the map on game over
        fog_density+=0.001f;
    }

    glutTimerFunc(16,timer,0);                                                              //Recall in 16ms
    glutPostRedisplay();                                                                    //Refresh screen
}

void changeBro(int w, int h) {                                                      //Screen Resize handler
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

void generateMapBro(){                                                                //Map generation Algorithm

    int cube_mode=0;
    int pop_count=0;

    float localx = -ROWS*1.0f + 1.0f;
    float localz = COLUMNS*1.0f - 1.0f;
    for(int i=0 ;i<ROWS; i++){
        for(int j=0; j<COLUMNS; j++){
            cube_mode=contour[i*COLUMNS + j];
            switch(cube_mode){
            case 0:                                                                     //Empty block
                break;
            case 1:                                                                     //Wall
                glEnable(GL_COLOR_MATERIAL);
                glPushMatrix();
                    glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
                    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
                    glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
                    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
                    drawCube(2.0f,2.0f,2.0f,localx + 2.0*i,1.0f,localz - 2.0*j,0.2f,0.2f,0.2f);
                glPopMatrix();
                powerState[i][j]=0;
                break;
            case 2:                                                                      //Beginning Position
                if(beginGame==1){
                    lightx=localx + 2.0*i;
                    lightz=localz - 2.0*j;
                    camerax=lightx;
                    cameraz=lightz;
                    currentx=i;
                    currentz=j;
                }
                powerState[i][j]=0;
                break;
            case 3:                                                                     //Final Position (Target)
                final_pos[0]=localx + 2.0*i;
                final_pos[1]=localz - 2.0*j;
                finalx=i;
                finaly=j;
                powerState[i][j]=0;
                break;
            case 4:                                                                     //Power-up
                if(beginGame==1){
                    powerUps[pop_count][0]=1.0f;
                    powerState[i][j]=pop_count+1;
                }

                if(powerUps[pop_count][0]==1.0f)
                {
                    powerUps[pop_count][1]=localx + 2.0*i;
                    powerUps[pop_count][2]=1.0f;
                    powerUps[pop_count][3]=localz - 2.0*j;
                }
                pop_count++;
                break;
            }
        }
    }
}

void renderBro(void){                                                                   //Main Rendering function
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(6.0f+camerax,10.0f+ 0.3*cameray,4.0f+cameraz,
              camerax, 0.3*cameray,cameraz,
              0.0f,1.0f,0.0f);
    drawLight();
    glEnable(GL_COLOR_MATERIAL);
    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
        glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
        drawCube(ROWS * 2.0f,0.2f, COLUMNS * 2.0f,0.0f,-0.2f,0.0f,0.1f,0.1f,0.1f);
    glPopMatrix();

    generateMapBro();
    beginGame=0;

    //draw the major cube

    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glTranslatef(lightx,lighty,lightz);
    switch(direction){
    case 'w':
        glRotatef(angle,0.0f,0.0f,1.0f);
        break;
    case 's':
        glRotatef(angle,0.0f,0.0f,-1.0f);
        break;
    case 'a':
        glRotatef(angle,1.0f,0.0f,0.0f);
        break;
    case 'd':
        glRotatef(angle,-1.0f,0.0f,0.0f);
        break;
    }
    drawCube(2.0f,2.0f,2.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f);
    switch(direction){
    case 'w':
        glRotatef(angle,0.0f,0.0f,-1.0f);
        break;
    case 's':
        glRotatef(angle,0.0f,0.0f,1.0f);
        break;
    case 'a':
        glRotatef(angle,-1.0f,0.0f,0.0f);
        break;
    case 'd':
        glRotatef(angle,1.0f,0.0f,0.0f);
        break;
    }
    glTranslatef(-lightx,-lighty,-lightz);

    //draw the power ups

    for(int i=0; i<POP; i++){
        if(powerUps[i][0]==1.0f){
            drawCube(2.0f,0.2f,2.0f,powerUps[i][1],powerUps[i][2]-0.8f,powerUps[i][3],1.0f,0.85f,0.22f);
        }
    }

    drawCube(2.0f,0.2f,2.0f,final_pos[0],0.2f,final_pos[1],0.0f,0.67f,0.93f);

    std::ostringstream strm;                                                                //Display Text data
    strm << STEPS;
    std::string numStr = strm.str();
    char const * ca = numStr.c_str();
    char zero[] = {'0','\0'};
    if(strlen(ca)==2)
    RenderFont(-0.07f, -0.35f, ca);
    else
    {
        RenderFont(-0.07f, -0.35f, zero);
        RenderFont(-0.058f, -0.35f, ca);
    }
    RenderFontSmall(-0.077f, -0.37, "STEPS");
    RenderFontSmall(-0.094f, -0.385, "REMAINING");

    std::ostringstream ntrm;
    ntrm << FLASHES;
    numStr = ntrm.str();
    ca = numStr.c_str();
    RenderFont(0.052f, -0.35f, ca);
    RenderFontSmall(0.033f, -0.37, "FLASHES");
    RenderFontSmall(0.026f, -0.385, "REMAINING");

    RenderFontSmall(-0.2f, -0.385, "ESC to quit");
    RenderFontSmall(0.13f, -0.385, "F to flash");

    if(FREEZE_GAME == 1){                                                               //Steps run out
        RenderGameOverFont(-0.07f,0.07f, "GAME OVER");
    }

    if(FREEZE_GAME == 2){                                                               //Final position reached
        RenderGameOverFont(-0.07f,0.08f, "YOU RULE");
    }
    glutSwapBuffers();
}

void gameLogic(int argc, char **argv) {                                                     //Controller
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1000,500);
	winid = glutCreateWindow("Rolling in the Deep");
    glutFullScreen();
    glutSetCursor(GLUT_CURSOR_NONE);

    map_extractor(ROWS, COLUMNS, contour);
    movement_extractor(ROWS,COLUMNS, movement);
    alignCollisionArray();

	glutDisplayFunc(renderBro);
	glutReshapeFunc(changeBro);
	glutIdleFunc(renderBro);

    glutKeyboardFunc(pandora);
    glutTimerFunc(0,timer,0);

    glutIgnoreKeyRepeat(1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    PlaySound(TEXT("C:\\Users\\codeNamuna\\Desktop\\glut\\bruh\\sounds\\deso.wav"), NULL, SND_LOOP | SND_ASYNC);

	glutMainLoop();
}

void pandora(unsigned char key, int x, int y){                                              //Key Handler

    if(spinValid && FREEZE_GAME==0){
        switch(key){
        case 'w':
            if(checkCollision(key)){
                spin=spin ? 0:1;
                currentx-=1;
                direction=key;
                spinValid=0;
                if(powerState[currentx][currentz] > 0){                                     //Encountered Power-up
                    POWERUP_CHANGE_IMPENDING=1;
                }
                if((currentx == finalx) && (currentz == finaly)){                           //Reached Final Position
                    FREEZE_GAME = 2;
                }
            }
            break;
        case 's':
            if(checkCollision(key)){
                spin=spin ? 0:1;
                currentx+=1;
                direction=key;
                spinValid=0;
                if(powerState[currentx][currentz] > 0){
                    POWERUP_CHANGE_IMPENDING=1;
                }
                if((currentx == finalx) && (currentz == finaly)){
                    FREEZE_GAME = 2;
                }
            }
            break;
        case 'a':
            if(checkCollision(key)){
                spin=spin ? 0:1;
                currentz-=1;
                direction=key;
                spinValid=0;
                if(powerState[currentx][currentz] > 0){
                    POWERUP_CHANGE_IMPENDING=1;
                }
                if((currentx == finalx) && (currentz == finaly)){
                    FREEZE_GAME = 2;
                }
            }
            break;
        case 'd':
            if(checkCollision(key)){
                spin=spin ? 0:1;
                currentz+=1;
                direction=key;
                spinValid=0;
                if(powerState[currentx][currentz] > 0){
                    POWERUP_CHANGE_IMPENDING=1;
                }
                if((currentx == finalx) && (currentz == finaly)){
                    FREEZE_GAME = 2;
                }
            }
            break;
        }
    }

    if(key == 27){
        glutDestroyWindow(winid);
        exit(0);
    }

    if(key=='f' && FLASHES>0){
        FLASHES--;
        diffuse=125;
    }


}

void alignCollisionArray(){                                                             //Generate 3D array out of
    int c=0;                                                                            //1D input of collision data
    for(int i=0; i<ROWS; i++){
        for(int j=0; j<COLUMNS; j++){
            for(int k=0; k<4; k++){
                collisions[i][j][k]=movement[c];
                c++;
            }
        }
    }
}

bool checkCollision(char key){                                                          //Collision handler
    bool valid=0;
    switch(key){
    case 'w':
        if(collisions[currentx][currentz][0]==1)
            valid=1;
        break;
    case 's':
        if(collisions[currentx][currentz][2]==1)
            valid=1;
        break;
    case 'a':
        if(collisions[currentx][currentz][3]==1)
            valid=1;
        break;
    case 'd':
        if(collisions[currentx][currentz][1]==1)
            valid=1;
        break;
    }
    return valid;
}
