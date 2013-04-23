
#include <stdlib.h>
#include <stdio.h>
#include<math.h>
#include<time.h>
#include <GL/glut.h>

#define PI 3.14
#define v 2100
# define N 7 // cube drawing


//Declaration of global variables for snakes and ladders 

int count=1,exit_flag=0;;
void scanfill(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4);
void rotate(float,int,int);
void elip(int,int,int,int,int);
void movepawn(int xin[2],int yin[2],int rnd,int xcen[2],int ycen[2]);
void init();
void display();
void dice(int );
int randm();
int temp;
int dicenum=3,val1=0,val2=0;
int xcen[2]={300,300},ycen[2]={100,300},xin[2]={0,0},yin[2]={0,0},flag1=0;
int static p1,p2;
static int val=0,win;
void createMenu(void);
void menu(int value);
static int mainmenu;
void setcolor(void);

void createMenu(void)
{

	//menu

	//Creating a submenu
	p1=glutCreateMenu(menu);

	// Adding sub menu entry
	glutAddMenuEntry("Blue",1);
	glutAddMenuEntry("Green",2);
	glutAddMenuEntry("Purple",3);

	p2=glutCreateMenu(menu);

	glutAddMenuEntry("Black",4);
	glutAddMenuEntry("White",5);
	glutAddMenuEntry("Red",6);

	//Creating the current menu
	mainmenu= glutCreateMenu(menu);

	//Creating entries
	glutAddSubMenu("Player 1",p1);
	glutAddSubMenu("Player 2",p2);
	glutAddMenuEntry("Exit",0);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu(int value)
{
	if(value==0)
	{
	glutDestroyWindow(win);
	exit(0);
	}

	else
	{
		if(value==1||value==2||value==3)
			val1=value;
		else
			val2=value;
	}
	glutPostRedisplay();
}

/*----------------------------------------------------------------------------------------
 *	Global Variables
 */

/*
 *	A structure to represent the mouse information
 */
struct Mouse
{
	int x;		/*	the x coordinate of the mouse cursor	*/
	int y;		/*	the y coordinate of the mouse cursor	*/
	int lmb;	/*	is the left button pressed?		*/
	int mmb;	/*	is the middle button pressed?	*/
	int rmb;	/*	is the right button pressed?	*/

	int xpress; /*	stores the x-coord of when the first button press occurred	*/
	int ypress; /*	stores the y-coord of when the first button press occurred	*/
};

typedef struct Mouse Mouse;
Mouse TheMouse = {0,0,0,0,0};

void drawstring(float x,float y,float z,char *string) /* to display text */
{
	char *c;
	glRasterPos3f(x,y,z);
 
	for(c=string;*c!='\0';c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*c);
	}
}

//To generate a random number
int randm()
{
	return rand()%6+1;
}


//Variables to hold the current size of the window.
int winw = 700;
int winh = 700;


/*----------------------------------------------------------------------------------------
 *	BUTTON
 */

/*
 *	We will define a function pointer type.  that
 *	looks a bit like this :
 *
 *	void func() {
 *	}
 */
typedef void (*ButtonCallback)(); //ButtonCallback is a pointer to a function

/*
 *	This is a simple structure that holds a button.
 */
struct Button
{
	int   x;							/* top left x coord of the button */
	int   y;							/* top left y coord of the button */
	int   w;							/* the width of the button */
	int   h;							/* the height of the button */
	int	  state;						/* the state, 1 if pressed, 0 otherwise */
	int	  highlighted;					/* is the mouse cursor over the control? */
	char* label;						/* the text label of the button */
	ButtonCallback callbackFunction;	/* A pointer to a function to call if the button is pressed */
};

typedef struct Button Button;



/*----------------------------------------------------------------------------------------
 *	This is an example callback function. Notice that it's type is the same
 *	as the ButtonCallback type. We can assign a pointer to this function which
 *	we can store and later call.
 */
void TheButtonCallback()
{
	//printf("AAA\n");
	dicenum=randm();
	dice(dicenum);
	
    movepawn(xin,yin,dicenum,xcen,ycen);
    //usleep(10000);

}

/*----------------------------------------------------------------------------------------
 *	This is the button visible in the viewport. This is a shorthand way of
 *	initialising the structure's data members. Notice that the last data
 *	member is a pointer to the above function.
 */Button MyButton = {625,420, 50,25, 0,0, "Roll Dice", TheButtonCallback };

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws a text string to the screen using glut bitmap s.
 *	\param	font	-	the font to use. it can be one of the following :
 *
 *					GLUT_BITMAP_9_BY_15
 *					GLUT_BITMAP_8_BY_13
 *					GLUT_BITMAP_TIMES_ROMAN_10
 *					GLUT_BITMAP_TIMES_ROMAN_24
 *					GLUT_BITMAP_HELVETICA_10
 *					GLUT_BITMAP_HELVETICA_12
 *					GLUT_BITMAP_HELVETICA_18
 *
 *	\param	text	-	the text string to output
 *	\param	x		-	the x co-ordinate
 *	\param	y		-	the y co-ordinate
 */
void Font(void *font,char *text,int x,int y)
{
	glRasterPos2i(x, y);

	while( *text != '\0' )
	{
		glutBitmapCharacter( font, *text );
		++text;
	}
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function is used to see if a mouse click or event is within a button
 *			client area.
 *	\param	b	-	a pointer to the button to test
 *	\param	x	-	the x coord to test
 *	\param	y	-	the y-coord to test
 */
int ButtonClickTest(Button* b,int x,int y)
{
	if( b)
	{
		/*
		 *	If clicked within button area, then return true
		 */
	    if( x > b->x      &&
			x < b->x+b->w &&
			y > b->y      &&
			y < b->y+b->h ) {
				return 1;
		}
	}

	/*
	 *	otherwise false.
	 */
	return 0;
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to check.
 *	\param	x	-	the x location of the mouse cursor.
 *	\param	y	-	the y location of the mouse cursor.
 */
void ButtonRelease(Button *b,int x,int y)
{
	if(b)
	{
		/*
		 *	If the mouse button was pressed within the button area
		 *	as well as being released on the button.....
		 */
		if( ButtonClickTest(b,TheMouse.xpress,TheMouse.ypress) &&
			ButtonClickTest(b,x,y) )
		{
			/*
			 *	Then if a callback function has been set, call it.
			 */
			if (b->callbackFunction) {
				b->callbackFunction();
			}
		}

		/*
		 *	Set state back to zero.
		 */
		b->state = 0;
	}
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to check.
 *	\param	x	-	the x location of the mouse cursor.
 *	\param	y	-	the y location of the mouse cursor.
 */
void ButtonPress(Button *b,int x,int y)
{
	if(b)
	{
		/*
		 *	if the mouse click was within the buttons client area,
		 *	set the state to true.
		 */
		if( ButtonClickTest(b,x,y) )
		{
			b->state = 1;
		}
	}
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to check.
 *	\param	x	-	the x location of the mouse cursor.
 *	\param	y	-	the y location of the mouse cursor.

 */
void ButtonPassive(Button *b,int x,int y)
{
	if(b)
	{
		/*
		 *	if the mouse moved over the control
		 */
		if( ButtonClickTest(b,x,y) )
		{
			/*
			 *	If the cursor has just arrived over the control, set the highlighted flag
			 *	and force a redraw. The screen will not be redrawn again until the mouse
			 *	is no longer over this control
			 */
			if( b->highlighted == 0 ) {
				b->highlighted = 1;
				glutPostRedisplay();
			}
		}
		else

		/*
		 *	If the cursor is no longer over the control, then if the control
		 *	is highlighted (ie, the mouse has JUST moved off the control) then
		 *	we set the highlighting back to false, and force a redraw.
		 */
		if( b->highlighted == 1 )
		{
			b->highlighted = 0;
			glutPostRedisplay();
		}
	}
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function draws the specified button.
 *	\param	b	-	a pointer to the button to draw.
 */
void ButtonDraw(Button *b)
{
	int fontx;
	int fonty;

	if(b)
	{
		/*
		 *	We will indicate that the mouse cursor is over the button by changing its
		 *	colour.
		 */
		if (b->highlighted)
			glColor3f(0.7f,0.7f,0.8f);
		else
			glColor3f(0.6f,0.6f,0.6f);

		/*
		 *	draw background for the button.
		 */
		glBegin(GL_QUADS);
			glVertex2i( b->x     , b->y      );
			glVertex2i( b->x     , b->y+b->h );
			glVertex2i( b->x+b->w, b->y+b->h );
			glVertex2i( b->x+b->w, b->y      );
		glEnd();

		/*
		 *	Draw an outline around the button with width 3
		 */
		glLineWidth(3);

		/*
		 *	The colours for the outline are reversed when the button.
		 */
		if (b->state)
			glColor3f(0.4f,0.4f,0.4f);
		else
			glColor3f(0.8f,0.8f,0.8f);

		glBegin(GL_LINE_STRIP);
			glVertex2i( b->x+b->w, b->y      );
			glVertex2i( b->x     , b->y      );
			glVertex2i( b->x     , b->y+b->h );
		glEnd();

		if (b->state)
			glColor3f(0.8f,0.8f,0.8f);
		else
			glColor3f(0.4f,0.4f,0.4f);

		glBegin(GL_LINE_STRIP);
			glVertex2i( b->x     , b->y+b->h );
			glVertex2i( b->x+b->w, b->y+b->h );
			glVertex2i( b->x+b->w, b->y      );
		glEnd();

		glLineWidth(1);


		/*
		 *	Calculate the x and y coords for the text string in order to center it.
		 */
		fontx = b->x + (b->w - glutBitmapLength(GLUT_BITMAP_HELVETICA_10,b->label)) / 2 ;
		fonty = b->y + (b->h+10)/2;

		/*
		 *	if the button is pressed, make it look as though the string has been pushed
		 *	down. It's just a visual thing to help with the overall look....
		 */
		if (b->state) {
			fontx+=2;
			fonty+=2;
		}

		/*
		 *	If the cursor is currently over the button we offset the text string and draw a shadow
		 */
		if(b->highlighted)
		{
			glColor3f(0,0,0);
			Font(GLUT_BITMAP_HELVETICA_10,b->label,fontx,fonty);
			fontx--;
			fonty--;
		}

		glColor3f(1,1,1);
		Font(GLUT_BITMAP_HELVETICA_10,b->label,fontx,fonty);
	}
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function is called to initialise opengl.
 */

void Init()
{
	glEnable(GL_LIGHT0);
}

/*----------------------------------------------------------------------------------------
 *	This function will be used to draw the 3D scene
 */
void Draw3D()
{
	gluLookAt(0,1,5,0,0,0,0,1,0);
	glutSolidTeapot(1);
}

/*----------------------------------------------------------------------------------------
 *	This function will be used to draw an overlay over the 3D scene.
 *	This will be used to draw our fonts, buttons etc......
 */
void Draw2D()
{
	ButtonDraw(&MyButton);
}

/*----------------------------------------------------------------------------------------
 *	This is the main display callback function. It sets up the drawing for
 *	The 3D scene first then calls the Draw3D() function. After that it switches to
 *	an orthographic projection and calls Draw2D().
 */

void Draw()
{
	/*
	 *	Clear the background
	 */
	glClear( GL_COLOR_BUFFER_BIT |
			 GL_DEPTH_BUFFER_BIT );

	/*
	 *	Enable lighting and the z-buffer
	 */
/*glutSwapBuffers();
  usleep(100000);*/


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	/*
	 *	Set perspective viewing transformation
	 */

	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45,(winh==0)?(1):((float)winw/winh),1,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	/*
	 *	Draw the 3D elements in the scene

	Draw3D();*/

	/*
	 *	Disable depth test and lighting for 2D elements
	 */
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	/*
	 *	Set the orthographic viewing transformation
	 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,winw,winh,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

glColor3f(0.8,0.5,0.2);
drawstring(1000,1200,0,"YAY! YOU DID IT!!");
drawstring(1000,1000,0,"YOU HAVE UNLOCKED THE GAME!!");
sleep(2);
	/*
	 *	Draw the 2D overlay
	 */
 Draw2D();

	/*
	 *	Bring the back buffer to the front and vice-versa.
	 */
	init();
	 display();
	glutSwapBuffers();
}

/*----------------------------------------------------------------------------------------
 *	This function is called when the window is resized. All this does is simply
 *	store the new width and height of the window which are then referenced by
 *	the draw function to set the correct viewing transforms
 */
void Resize(int w, int h)
{
	winw = w;
	winh = h;

	/*
	 *	Allow drawing in full region of the screen
	 */
	glViewport(0,0,w,h);
}


/*----------------------------------------------------------------------------------------
 *	\brief	This function is called whenever a mouse button is pressed or released
 *	\param	button	-	GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, or GLUT_MIDDLE_BUTTON
 *	\param	state	-	GLUT_UP or GLUT_DOWN depending on whether the mouse was released
 *						or pressed respectivly.
 *	\param	x		-	the x-coord of the mouse cursor.
 *	\param	y		-	the y-coord of the mouse cursor.
 */
void MouseButton(int button,int state,int x, int y)
{
	/*
	 *	update the mouse position
	 */
	TheMouse.x = x;
	TheMouse.y = y;

	/*
	 *	has the button been pressed or released?
	 */
	if (state == GLUT_DOWN)
	{
		/*
		 *	This holds the location of the first mouse click
		 */
		if ( !(TheMouse.lmb || TheMouse.mmb || TheMouse.rmb) ) {
			TheMouse.xpress = x;
			TheMouse.ypress = y;
		}

		/*
		 *	Which button was pressed?
		 */
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			TheMouse.lmb = 1;
			ButtonPress(&MyButton,x,y);
		case GLUT_MIDDLE_BUTTON:
			TheMouse.mmb = 1;
			break;
		case GLUT_RIGHT_BUTTON:
			TheMouse.rmb = 1;
			break;
		}
	}
	else
	{
		/*
		 *	Which button was released?
		 */
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			TheMouse.lmb = 0;
			ButtonRelease(&MyButton,x,y);
			break;
		case GLUT_MIDDLE_BUTTON:
			TheMouse.mmb = 0;
			break;
		case GLUT_RIGHT_BUTTON:
			TheMouse.rmb = 0;
			break;
		}
	}

	/*
	 *	Force a redraw of the screen. If we later want interactions with the mouse
	 *	and the 3D scene, we will need to redraw the changes.
	 */
	glutPostRedisplay();
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function is called whenever the mouse cursor is moved AND A BUTTON IS HELD.
 *	\param	x	-	the new x-coord of the mouse cursor.
 *	\param	y	-	the new y-coord of the mouse cursor.
 */
void MouseMotion(int x, int y)
{
	/*
	 *	Calculate how much the mouse actually moved
	 */
	int dx = x - TheMouse.x;
	int dy = y - TheMouse.y;

	/*
	 *	update the mouse position
	 */
	TheMouse.x = x;
	TheMouse.y = y;


	/*
	 *	Check MyButton to see if we should highlight it cos the mouse is over it
	 */
	ButtonPassive(&MyButton,x,y);

	/*
	 *	Force a redraw of the screen
	 */
	glutPostRedisplay();
}

/*----------------------------------------------------------------------------------------
 *	\brief	This function is called whenever the mouse cursor is moved AND NO BUTTONS ARE HELD.
 *	\param	x	-	the new x-coord of the mouse cursor.
 *	\param	y	-	the new y-coord of the mouse cursor.
 */
void MousePassiveMotion(int x, int y)
{
	/*
	 *	Calculate how much the mouse actually moved
	 */
	int dx = x - TheMouse.x;
	int dy = y - TheMouse.y;

	/*
	 *	update the mouse position
	 */
	TheMouse.x = x;
	TheMouse.y = y;

	/*
	 *	Check MyButton to see if we should highlight it cos the mouse is over it
	 */
	ButtonPassive(&MyButton,x,y);

	/*
	 *	Note that I'm not using a glutPostRedisplay() call here. The passive motion function
	 *	is called at a very high frequency. We really don't want much processing to occur here.
	 *	Redrawing the screen every time the mouse moves is a bit excessive. Later on we
	 *	will look at a way to solve this problem and force a redraw only when needed.
	 */
}

//----------------------------------------------------------------------------------------

//To set pixel position
void setPixel(int x,int y)
{
	glBegin(GL_POINTS);
	glVertex2f(x,y);
	glEnd();
	glFlush();
}

//For setting the projection for every task except task2
void init()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,4700,0,4700);
}

//Draw line using Bresenham's line drawing algorithm
void linebres(int x1,int y1,int x2,int y2)
{
	int dx,dy,steps,i,p;
	float x,y;
	x=x1;
	y=y1;
	dx=x2-x1;
	dy=y2-y1;
	if(abs(dx)>abs(dy))
	{
	p=(2*abs(dy))-abs(dx);
	setPixel(x,y);
	for(i=0;i<abs(dx)-1;i++)
	{
	if(p<0)
	{
	x+=(dx/abs(dx));
	p+=(2*abs(dy));
	}
	else
	{
	x+=(dx/abs(dx));
	y+=(dy/abs(dy));
	p+=((2*abs(dy))-(2*abs(dx)));
	}
	setPixel(x,y);
	}
	}
	else
	{
	p=(2*abs(dx))-abs(dy);
	setPixel(x,y);
	for(i=0;i<abs(dy)-1;i++)
	{
	if(p<0)
	{
	y+=(dy/abs(dy));
	p+=(2*abs(dx));
	}
	else
	{
	x+=(dx/abs(dx));
	y+=(dy/abs(dy));
	p+=((2*abs(dx))-(2*abs(dy)));
	}
	setPixel(x,y);
	}
	}
}

//To print the number one
void one(int x,int y,int offset)
{
	int x1,y1,x2,y2;
	x1=x+offset;
	y1=y+25;
	x2=x+offset;
	y2=y1+50;
	linebres(x1,y1,x2,y2);
}

//To print the number two
void two(int x,int y,int offset)
{
	int x1,y1,x2,y2;
	x1=x+offset;
	x2=x+offset+50;
	y1=y+25;
	y2=y+25;
	linebres(x1,y1,x2,y2);
	x1=x2=x+offset;
	y1=y+25;
	y2=y1+25;
	linebres(x1,y1,x2,y2);
	x1=x+offset;
	x2=x+offset+50;
	y1=y+50;
	y2=y+50;
	linebres(x1,y1,x2,y2);
	x1=x+offset;
	x2=x+offset+50;
	y1=y+75;
	y2=y+75;
	linebres(x1,y1,x2,y2);
	x1=x2=x+offset+50;
	y1=y+50;
	y2=y1+25;
	linebres(x1,y1,x2,y2);
}

//To print the number three
void three(int x,int y,int offset)
{
	int x1,x2,y1,y2;
	x1=x+offset;
	x2=x1+50;
	y1=y2=y+25;
	linebres(x1,y1,x2,y2);
	x1=x+offset;
	x2=x1+50;
	y1=y2=y+50;
	linebres(x1,y1,x2,y2);
	x1=x+offset;
	x2=x1+50;
	y1=y2=y+75;
	linebres(x1,y1,x2,y2);
	x1=x2=x+offset+50;
	y1=y+25;
	y2=y1+50;
	linebres(x1,y1,x2,y2);
}

//To print the number four
void four(int x,int y,int offset)
{
	int x1,x2,y1,y2;
	x1=x+offset+50;
	x2=x+offset+50;
	y1=y+25;
	y2=y1+50;
	linebres(x1,y1,x2,y2);
	x1=x+offset;
	y1=y+50;
	x2=x1;
	y2=y1+25;
	linebres(x1,y1,x2,y2);
	x1=offset+x;
	y1=y+50;
	x2=x1+50;
	y2=y1;
	linebres(x1,y1,x2,y2);
}

//To print the number five
void five(int x,int y,int offset)
{
	int x1,x2,y1,y2;
	x1=x+offset;
	y1=y2=y+25;
	x2=x1+50;
	linebres(x1,y1,x2,y2);
	x1=x+offset;
	y1=y2=y+50;
	x2=x1+50;
	linebres(x1,y1,x2,y2);
	x1=x+offset;
	y1=y2=y+75;
	x2=x1+50;
	linebres(x1,y1,x2,y2);
	x1=x2=x+offset+50;
	y1=y+25;
	y2=y+50;
	linebres(x1,y1,x2,y2);
	x1=x2=x+offset;
	y1=y+50;
	y2=y+75;
	linebres(x1,y1,x2,y2);
}

//To print the number six
void six(int x,int y,int offset)
{
	int x1,x2,y1,y2;
	x1=x+offset;
	y1=y2=y+25;
	x2=x1+50;
	linebres(x1,y1,x2,y2);
	x1=x+offset;
	y1=y2=y+50;
	x2=x1+50;
	linebres(x1,y1,x2,y2);
	x1=x+offset;
	y1=y2=y+75;
	x2=x1+50;
	linebres(x1,y1,x2,y2);
	x1=x2=x+offset+50;
	y1=y+25;
	y2=y+50;
	linebres(x1,y1,x2,y2);
	x1=x2=x+offset;
	y1=y+25;
	y2=y+75;
	linebres(x1,y1,x2,y2);
}

//To print the number seven
void seven(int x,int y,int offset)
{
	int x1,y1,x2,y2;
	x1=x+offset+50;
	y1=y+25;
	x2=x+offset+50;
	y2=y1+50;
	linebres(x1,y1,x2,y2);
	x1=x+offset;
	y1=y+75;
	x2=x+offset;
	y2=y+60;
	linebres(x1,y1,x2,y2);
	x1=x+offset;
	y1=y+75;
	x2=x1+50;
	y2=y1;
	linebres(x1,y1,x2,y2);
}

//To print the number eight
void eight(int x,int y,int offset)
{
	int x1,x2,y1,y2;
	x1=x+offset;
	y1=y2=y+25;
	x2=x1+50;
	linebres(x1,y1,x2,y2);
	x1=x+offset;
	y1=y2=y+50;
	x2=x1+50;
	linebres(x1,y1,x2,y2);
	x1=x+offset;
	y1=y2=y+75;
	x2=x1+50;
	linebres(x1,y1,x2,y2);
	x1=x2=x+offset+50;
	y1=y+25;
	y2=y+75;
	linebres(x1,y1,x2,y2);
	x1=x2=x+offset;
	y1=y+25;
	y2=y+75;
	linebres(x1,y1,x2,y2);
}

//To print the number nine
void nine(int x,int y,int offset)
{
	int x1,x2,y1,y2;
	x1=x+offset+50;
	x2=x+offset+50;
	y1=y+25;
	y2=y1+50;
	linebres(x1,y1,x2,y2);
	x1=x+offset;
	y1=y+50;
	x2=x1;
	y2=y1+25;
	linebres(x1,y1,x2,y2);
	x1=offset+x;
	y1=y+50;
	x2=x1+50;
	y2=y1;
	linebres(x1,y1,x2,y2);
	x1=offset+x;
	y1=y+25;
	x2=x1+50;
	y2=y1;
	linebres(x1,y1,x2,y2);
	x1=offset+x;
	y1=y+75;
	x2=x1+50;
	y2=y1;
	linebres(x1,y1,x2,y2);
}

//To print the number zero
void zero(int x,int y,int offset)
{
	int x1,x2,y1,y2;
	x1=x+offset;
	y1=y2=y+25;
	x2=x1+50;
	linebres(x1,y1,x2,y2);
	x1=x+offset;
	y1=y2=y+75;
	x2=x1+50;
	linebres(x1,y1,x2,y2);
	x1=x2=x+offset+50;
	y1=y+25;
	y2=y+75;
	linebres(x1,y1,x2,y2);
	x1=x2=x+offset;
	y1=y+25;
	y2=y+75;
	linebres(x1,y1,x2,y2);
}

//To print the digits in unit place depending on the direction
void num(int x,int y,int flag)
{
	//For printing left to right
	if(flag==0)
	{
		one(x,y,125);
		two(x+2*200,y,100);
		three(x+2*400,y,100);
		four(x+2*600,y,100);
		five(x+2*800,y,100);
		six(x+2*1000,y,100);
		seven(x+2*1200,y,100);
		eight(x+2*1400,y,100);
		nine(x+2*1600,y,100);
		zero(x+2*1800,y,100);
	}
	//For printing right to left
	else
	{
		x=x+2*1800;
		one(x,y,125);
		two(x-2*200,y,100);
		three(x-2*400,y,100);
		four(x-2*600,y,100);
		five(x-2*800,y,100);
		six(x-2*1000,y,100);
		seven(x-2*1200,y,100);
		eight(x-2*1400,y,100);
		nine(x-2*1600,y,100);
		zero(x-2*1800,y,100);
	}
}

//To print the last row
void hundred(int x,int y)
{
	one(x,y,125);
	two(x-2*200,y,100);
	three(x-2*400,y,100);
	four(x-2*600,y,100);
	five(x-2*800,y,100);
	six(x-2*1000,y,100);
	seven(x-2*1200,y,100);
	eight(x-2*1400,y,100);
	nine(x-2*1600,y,100);
	x=0;
	one(x,y,25);
	zero(x,y,50);
	zero(x,y,100);
}

//To draw circle using midpoint circle drawing algorithm
/*
flag=0 =>complete circle
flag=-1=>left half 
flag=1 =>right half
flag=-2=>3rd quadrant
flag=2 =>4th quadrant 
flag=3 =>left half without rotate
*/
void midpoint(int r,float xc,float yc,int flag)
{      
	float x0=0,y0=r;
	setPixel(round(x0),round(y0));
	float x=x0,y=y0,p0,dx;
	p0=5/4-r;
	int i;
	while(x<y)
	{

		if(p0<0)
		{
			x+=1;
			p0+=2*x+1;
		}
		else
		{
			x+=1;
			y-=1;
			p0+=2*x+1-2*y;
		}
		if(flag==0)
		{
			setPixel(round(x+xc),round(y+yc));
			setPixel(round(x+xc),round(-y+yc));
			setPixel(round(-x+xc),round(y+yc));
			setPixel(round(-x+xc),round(-y+yc));
			setPixel(round(y+xc),round(x+yc));
			setPixel(round(y+xc),round(-x+yc));
			setPixel(round(-y+xc),round(x+yc));
			setPixel(round(-y+xc),round(-x+yc));
		}
  		else if(flag==1)
		{
			rotate(15.0,(int)(y+xc),(int)(x+yc));
	 		rotate(15.0,(int)(y+xc),(int)(-x+yc));
  	 	}
		else if(flag==-1)
 		{
			rotate(15.0,(int)(-y+xc),(int)(x+yc));
			rotate(15.0,(int)(-y+xc),(int)(-x+yc));
		}
		else if(flag==3)
		{
			setPixel((int)(-y+xc),(int)(x+yc));
			setPixel((int)(-x+xc),(int)(y+yc));
			setPixel((int)(-x+xc),(int)(-y+yc));
			setPixel((int)(-y+xc),(int)(-x+yc));
			
		}
		else if(flag==2)
		{
			setPixel(round(y+xc),round(-x+yc));
			setPixel(round(x+xc),round(-y+yc));
		}
		else 
		{
			setPixel(round(-y+xc),round(-x+yc));
			setPixel(round(-x+xc),round(-y+yc));
		}
	}
}

//To draw an elipse using midpoint ellipse drawing algorithm
//Algorithm rotates ellipse by angle theta
void elip(int xcen,int ycen,int rx,int ry,int theta)
{
	float x,y,rx2,ry2;
	float p1,p2;
	p1=(ry*ry)-(rx*rx*ry)+(rx*rx*0.25);
	x=0;
	y=ry;
	rotate(theta,x+xcen,y+ycen);
	rotate(theta,-x+xcen,y+ycen);
	rotate(theta,x+xcen,-y+ycen);
	rotate(theta,-x+xcen,-y+ycen);
	ry2=ry*ry;
	rx2=rx*rx;

	while((2*ry2*x)<(2*rx2*y))
	{
		if(p1<0)
		{
			x++;
			p1=p1+(2*ry2*x)+(ry2);
		}
		else
		{
			y--;
			x++;
			p1=p1+(2*ry2*x)-(2*rx2*y)+(ry2);
		}
		rotate(theta,x+xcen,y+ycen);
		rotate(theta,-x+xcen,y+ycen);
		rotate(theta,x+xcen,-y+ycen);
		rotate(theta,-x+xcen,-y+ycen);
	}

	p2=(ry2*(x+0.5)*(x+0.5))+(rx2*(y-1)*(y-1))-(rx2*ry2);

	while(y>0)
	{
		if(p2>0)
		{
			y--;
			p2+=(-(2*rx2*y)+(rx2));
		}
		else
		{
			y--;
			x++;
			p2+=((2*ry2*x)-(2*rx2*y)+(rx2));
		}
		rotate(theta,x+xcen,y+ycen);
		rotate(theta,-x+xcen,y+ycen);
		rotate(theta,x+xcen,-y+ycen);
		rotate(theta,-x+xcen,-y+ycen);
	}
}

//To draw the larger snake
void snake1()
{
	int i;
	glColor3f(0.309,0.7,0.184);
	for(i=0;i<=100;i+=1)
	{
		midpoint(1000,800+i,2640,1);
		midpoint(1000,2225+i,1240,-1);
	}
	for(i=0;i<=25;i++)
		linebres(1330+4*i,905+i,1530,860);
	
	//To draw head
	for(i=0;i<75;i++)
		elip(2400,2900,75-i,150,30);

	glColor3f(0,0,0);
	//To draw eyes
	midpoint(20,600,3700,0);
	midpoint(20,650,3750,0);
}

//To draw the smaller snake
void snake2()
{
	int i;
	glColor3f(0.309,0.7,0.184);
	
	for(i=0;i<75;i++)
		midpoint(1000,2925+i,1000,1);
	
	linebres(3445,1220,3505,1260);
	//To draw head
	for(i=0;i<60;i++)
		elip(4050,-350,60-i,125,45);
	
	glColor3f(0,0,0);
	//To draw eyes
	midpoint(15,3100,2670,0);
	midpoint(15,3075,2620,0);
}

//To draw the dice
//n signifies the number that appears on the dice
void dice(int n)
{

	glColor3f(0.7f,0.5f,0.7f);

	//To draw background for the dice.
	glBegin(GL_QUADS);
	glVertex2i(4200,2000);
	glVertex2i(4200,2300);
	glVertex2i(4500,2300);
	glVertex2i(4500,2000);
	glEnd();

	//To draw an outline around the dice with width 3
	glLineWidth(5);
	glColor3f(0.6f,0.4f,0.8f);

	//To give 3D effect
	glBegin(GL_LINE_STRIP);
	glVertex2i(4500,2000);
	glVertex2i(4200,2000);
	glVertex2i(4200,2300);
	glEnd();

	glColor3f(0.8f,0.3f,0.5f);

	glBegin(GL_LINE_STRIP);
	glVertex2i(4200,2300);
	glVertex2i(4500,2300);
	glVertex2i(4500,2000);
	glEnd();

	glColor3f(1.0,1.0,1.0);

	//To draw the number on dice
	switch(n)
	{
		case 1: midpoint(10,4200+150,2000+150,0);
			break;

		case 2: midpoint(10,4275,2150,0);
			midpoint(10,4425,2150,0);
			break;

		case 3: midpoint(10,4200+150,2000+150,0);
			midpoint(10,4250,2215,0);
			midpoint(10,4450,2085,0);
			break;

		case 4: midpoint(10,4250,2215,0);
			midpoint(10,4450,2085,0);
			midpoint(10,4250,2085,0);
			midpoint(10,4450,2215,0);
			break;

		case 5: midpoint(10,4250,2215,0);
			midpoint(10,4450,2085,0);
			midpoint(10,4250,2085,0);
			midpoint(10,4450,2215,0);
			midpoint(10,4200+150,2000+150,0);
			break;

		case 6: midpoint(10,4250,2215,0);
			midpoint(10,4450,2085,0);
			midpoint(10,4250,2085,0);
			midpoint(10,4450,2215,0);
			midpoint(10,4350,2085,0);
			midpoint(10,4350,2215,0);
			break;
	}
}

//To draw ladder
/*
xb=>bottom x coordinate
yb=>bottom y coordinate
xu=>upper x coordinate
yu=>upper y coordinate
*/
void ladder(int xb1,int yb1,int xb2,int yb2,int xu1,int yu1,int xu2,int yu2)
{
	int i,j;
	float dy=(yu1-yb1);
	float dx=(xu1-xb1);
	float slope=dy/dx;

	glColor3f(0,0,0);
	linebres(xb1,yb1,xu1,yu1);
	linebres(xb2,yb2,xu2,yu2);
	dx=100/slope;

	//To draw steps in ladder
	for(i=xb1,j=yb1;i<xu1-200,j<yu1-200;)
	{
		i+=dx;
		j+=100;
		xb2+=dx;
		yb2+=100;
		linebres(i,j,xb2,yb2);
	}
}

//To multiply two matrices a and b and store the result in c
void matmul(float a[3][3],float b[3][1],float c[3][1])
{
	int i,j,k;
	for(i=0;i<3;i++)
	{
		for(j=0;j<1;j++)
		{
			c[i][j]=0;
			for(k=0;k<3;k++)
			{
				c[i][j]+=(a[i][k]*b[k][j]);
			}
		}
	}
}

//To rotate one point by an angle theta
void rotate(float theta,int x1,int y1)
{
	float res1[3][1];
	float rot[3][3]={{cos(theta*PI/180.0),-sin(theta*PI/180.0),0},
			 {sin(theta*PI/180.0),cos(theta*PI/180.0),0},
			 {0,0,1}};
	float orig1[3][1]={(float)x1,(float)y1,1.0};

	matmul(rot,orig1,res1);
	setPixel((int)res1[0][0],(int)res1[1][0]);
}

//To create the board with grid,numbers and dice
void numbers(int xcen[2],int ycen[2])
{

	int i,j,count=0;
	//Drawing the grid
	for(i=0;i<10;i++)
	{
		count++;
		for(j=0;j<10;j++)
		{
			//alternating color based on count
			if((count++)%2)
			{
				glColor3f(1.0,1.0,0.0);
			}
			else
			{
				glColor3f(1.0,0.25,0.0);
			}
			scanfill(i*400,j*400,(i+1)*400,j*400,(i+1)*400,(j+1)*400,i*400,(j+1)*400);

		}
	}

	glColor3f(0,0,0);
	// Printing the digit in ten's place
	for(i=1;i<=9;i++)
		one(i*2*200,2*200,75);
	for(i=0;i<9;i++)
		two(i*2*200,2*400,25);
	for(i=1;i<=9;i++)
		three(i*2*200,2*600,25);
	for(i=0;i<9;i++)
		four(i*2*200,2*800,25);
	for(i=1;i<=9;i++)
		five(i*2*200,2*1000,25);
	for(i=0;i<9;i++)
		six(i*2*200,2*1200,25);
	for(i=1;i<=9;i++)
		seven(i*2*200,2*1400,25);
	for(i=0;i<9;i++)
		eight(i*2*200,2*1600,25);
	for(i=1;i<=9;i++)
		nine(i*2*200,2*1800,25);
	
	// printing the number in one's place
	for(i=0;i<9;i++)
		num(0,i*2*200,i%2);


	hundred(2*1800,2*1800);

	// printing the 1st row
	one(2*1800,0,75);
	two(0,2*200,25);
	three(2*1800,2*400,25);
	four(0,2*600,25);
	five(2*1800,2*800,25);
	six(0,2*1000,25);
	seven(2*1800,2*1200,25);
	eight(0,2*1400,25);
	nine(2*1800,2*1600,25);

	// drawing the ladders
	for(i=0;i<10;i++)
	{
		ladder(2800+175+i,300,2800+275+i,225,3600+275+i,1200+200,3600+375+i,1200+125);
		ladder(2000+75+i,400+275,2000+175+i,400+350,1200+275+i,3600+75,1200+375+i,3600+150);
		ladder(2000+275+i,2400+300,2000+375+i,2400+225,2400+275+i,3200+200,2400+375+i,3200+125);
	}

	// drawing the snakes
	snake1();
	snake2();

	// draw the pawns with different colors
	temp=flag1;
	flag1=0;
	setcolor();
	for(i=0;i<=60;i++)
	midpoint(i,xcen[0],ycen[0],0);
	flag1=1;
	setcolor();
	for(i=0;i<=60;i++)
	midpoint(i,xcen[1],ycen[1],0);
	flag1=temp;
	glColor3f(1,1,1);

	// to draw the dice
	dice(dicenum);

	// to print player 1 / player 2
	if(flag1==0)
	{       setcolor();
		glColor3f(0,0,0);
		drawstring(4050.0,2500.0,0.0,"PLAYER 2");
		glColor3f(0.5,0.5,0.5);
		drawstring(4050.0,2500.0,0.0,"PLAYER 1");
		setcolor();
	}
	else
	{       setcolor();
		glColor3f(0,0,0);
		drawstring(4050.0,2500.0,0.0,"PLAYER 1");
		glColor3f(0.5,0.5,0.5);
		drawstring(4050.0,2500.0,0.0,"PLAYER 2");
		setcolor();
	
	}
	glColor3f(1,0,0);

}

void translate(int dx,int dy,int x1[2],int y1[2],int flag,int rnd)
{

	int i,j,k,genmat[3][3]={{1,0,dx},{0,1,dy},{0,0,1}},pointmat[3][1]={x1[flag],y1[flag],1},endmat[3][1],finx,finy;

	// matrix multiplication
	for(i=0;i<3;i++)
		for(j=0;j<1;j++)
		{
			endmat[i][j]=0;
			for(k=0;k<3;k++)
			endmat[i][j]+=genmat[i][k]*pointmat[k][j];
		}

	x1[flag]=endmat[0][0];
	y1[flag]=endmat[1][0];

	// draw pawn
	for(i=0;i<=60;i++)
	midpoint(i,x1[flag],y1[flag],0);

	// check if player has reached hundred. Display appropriate message
	if(x1[flag]==300&&y1[flag]==3700)
	{
		for(i=0;i<=60;i++)
			midpoint(i,x1[flag],y1[flag],0);
		usleep(1000000);
		glutSwapBuffers();
		glClear(GL_COLOR_BUFFER_BIT);

		glColor3f(1,0,0);
		if(flag==0)
		{
			drawstring(1250.0,2000.0,0.0,"CONGRATULATIONS PLAYER 1!");
			glutSwapBuffers();
		}
		else
		{
			drawstring(1250.0,2000.0,0.0,"CONGRATULATIONS PLAYER 2!");
			glutSwapBuffers();
		}
		usleep(5000000);
		glutDestroyWindow(glutGetWindow());
		exit(0);
	}
	else
	{
		usleep(1000000);
		glutSwapBuffers();
	}
}

// calling the numbers function
void display()
{
	int i;
	glColor3f(1,1,1);
	numbers(xcen,ycen);
	glFlush();
}

// setting color for pawn according to colors chosen in the menu
void setcolor()
{
	if(flag1==0)
	{
		switch(val1)
		{
		case 1:glColor3f(0,0,1);
		break;
		case 2:glColor3f(0,1,0);
		break;
		case 3:glColor3f(0.53,0.12,0.47);
		break;
		}
	}
	else
	{
		switch(val2)
		{
		case 4:glColor3f(0,0,0);
		break;
		case 5:glColor3f(1,1,1);
		break;
		case 6:glColor3f(1,0,0);
		break;
		}
	}
}

// moves the pawn according to the ranom number generated by the dice
void movepawn(int xin[2],int yin[2],int rnd,int xcen[2],int ycen[2])
{
	int direc,i,random;
	int start[5][2]={{7*400,0},{5*400,400},{5*400,6*400},{400,400*9},{7*400,6*400}};
	int end[5][2]={{9*400,3*400},{3*400,9*400},{6*400,8*400},{3*400,400*2},{8*400,3*400}};
	random=rnd;
	if(yin[flag1]%800==0)
		direc=1;
	else
		direc=0;
	while(rnd--)
	{
		if(direc==1)
		{
			if(xin[flag1]%800==0)
			glColor3f(1,1,0);
			else
			glColor3f(1,0.25,0);
			if(xin[flag1]==3600)
			{
				direc=0;
				for(i=0;i<=60;i++)
				midpoint(i,xcen[flag1],ycen[flag1],0);
				setcolor();
				translate(0,400,xcen,ycen,flag1,random);
				yin[flag1]+=400;
			}
			else
			{
				for(i=0;i<=60;i++)
				midpoint(i,xcen[flag1],ycen[flag1],0);
				setcolor();		
				translate(400,0,xcen,ycen,flag1,random);
				xin[flag1]+=400;
			}
		}
		else
		{
			if(xin[flag1]%800==0)
			glColor3f(1,0.25,0);
			else
			glColor3f(1,1,0);
			if(xin[flag1]==0)
			{
				direc=1;
				for(i=0;i<=60;i++)
				midpoint(i,xcen[flag1],ycen[flag1],0);
				setcolor();
				translate(0,400,xcen,ycen,flag1,random);
				yin[flag1]+=400;
			}
			else
			{
				if(yin[flag1]==3600)
				{
					if(xin[flag1]-(random*400)>=0)
					{
						rnd=random;
						while(rnd--)
						{
							if(xin[flag1]%800==0)
								glColor3f(1,0.25,0);
							else
								glColor3f(1,1,0);
							midpoint(60,xcen[flag1],ycen[flag1],0);
							setcolor();
							translate(-400,0,xcen,ycen,flag1,random);
							xin[flag1]-=400;
						}
					 	
						break;
					}				
				}
				else
				{
					for(i=0;i<=60;i++)
					midpoint(i,xcen[flag1],ycen[flag1],0);
					setcolor();		
					translate(-400,0,xcen,ycen,flag1,random);
					xin[flag1]-=400;
				}
			}
		}
	}
		for(i=0;i<5;i++)
		if(xin[flag1]==start[i][0]&&yin[flag1]==start[i][1])
		{
			if(direc==0)
			{
				if(xin[flag1]%800==0)
				glColor3f(1,0.25,0);
				else
				glColor3f(1,1,0);
			}
			else
			{
				if(xin[flag1]%800==0)
				glColor3f(1,1,0);
				else
				glColor3f(1,0.25,0);
			}
			midpoint(60,xcen[flag1],ycen[flag1],0);
			setcolor();
		 	xin[flag1]=end[i][0];
			yin[flag1]=end[i][1];
	  		translate(end[i][0]-start[i][0],end[i][1]-start[i][1],xcen,ycen,flag1,random);
		}

		if(flag1==0)
		{
			setcolor();
			glColor3f(0,0,0);
			drawstring(4050.0,2500.0,0.0,"PLAYER 1");
			glColor3f(0.5,0.5,0.5);
			drawstring(4050.0,2500.0,0.0,"PLAYER 2");
			setcolor();		
			flag1=1;
		}
		else
		{
			setcolor();
			glColor3f(0,0,0);
			drawstring(4050.0,2500.0,0.0,"PLAYER 2");
			glColor3f(0.5,0.5,0.5);
			drawstring(4050.0,2500.0,0.0,"PLAYER 1");
			setcolor();
			flag1=0;
		}
}

// For the scanfill
void edgedetect(float x1,float y1,float x2,float y2,int *le,int *re)
{
    float temp,x,mx;
    int i;

    if(y1>y2)
    {
        temp=x1,x1=x2,x2=temp;
        temp=y1,y1=y2,y2=temp;
    }

    if(y1==y2)
        mx=x2-x1;
    else
        mx=(x2-x1)/(y2-y1);

    x=x1;
    for(i=((int)y1);i<=((int)y2);i++)
    {
        if(x<(float)le[i] )
           le[i]=((int)x);
        if( x>(float)re[i])
           re[i]=((int)x);
        x+=mx;
    }
}

// To fill objects objects using scanfill
void scanfill(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4)
{
    int le[4700],re[4700],i,j;

    for(i=0;i<4700;i++)
        le[i]=4700,re[i]=0;

    edgedetect(x1,y1,x2,y2,le,re);
    edgedetect(x2,y2,x3,y3,le,re);
    edgedetect(x3,y3,x4,y4,le,re);
    edgedetect(x4,y4,x1,y1,le,re);

    for(j=0;j<4700;j++)
    {
          if(le[j]<=re[j])
            for(i=le[j];i<re[j];i++)
                setPixel(i,j);
    }
}
//---------------------------------------------------------------------------------------------------------
//CODE FOR SCREEN3 - FIND THE RIGHT CIRCLE


int numLines,flg1=0,flg2=0,xt1,yt1,ymax,ymin,xmax,xmin;

typedef enum state
{
	waitingforclick,
	clickedonce,
}state;

typedef struct point
{
	int x;
	int y;
}point;

point lines[256][4] ;

int gState=waitingforclick;
int lineisvalid=0;
int gHeight;
float gColor[3]={0,1,0};

//Function for clipping a line
void clipping(int x1,int y1,int x2,int y2)
{
	int code1[5]={0},code2[5]={0};
	int cd1,cd2,var;
	int new;
	if(y1>ymax)
	code1[0]=1;
	if(y1<ymin)
	code1[1]=1;
	if(x1>xmax)
	code1[2]=1;
	if(x1<xmin)
	code1[3]=1;

	cd1=1*code1[3]+2*code1[2]+4*code1[1]+8*code1[0];

	if(y2>ymax)
	code2[0]=1;
	if(y2<ymin)
	code2[1]=1;
	if(x2>xmax)
	code2[2]=1;
	if(x2<xmin)
	code2[3]=1;

	cd2=1*code2[3]+2*code2[2]+4*code2[1]+8*code2[0];

	//Line is accepted	
	if(!(cd1|cd2))
	{
	glColor3f(1,1,1);
	linebres(x1,y1,x2,y2);
	}
	//else if(cd1&cd2!=0) - line rejected

	else if(!(cd1&cd2))
	{
		if(cd1!=0)
		{
			if(code1[0]==1)
			{ 
				new=((ymax-y1)*(x2-x1))/(y2-y1)+x1;
				clipping(new,ymax,x2,y2);	
			}
			else if(code1[1]==1)
			{
				new=((ymin-y1)*(x2-x1))/(y2-y1)+x1;
				clipping(new,ymin,x2,y2);
			}
			else if(code1[2]==1)
			{
				var=(y2-y1)/(x2-x1);
				new=y2-((x2-xmax)*var);
				clipping(xmax,new,x2,y2);
			}
			else if(code1[3]==1)
			{
				new=y1-(((y2-y1)*(x1-xmin))/(x2-x1));
				clipping(xmin,new,x2,y2);
			}
		}
		else
		{
			if(code2[0]==1)
			{ new=((ymax-y1)*(x2-x1))/(y2-y1)+x1;
			clipping(x1,y1,new,ymax);
			}
			else if(code2[1]==1)
			{
			new=((ymin-y1)*(x2-x1))/(y2-y1)+x1;
			clipping(x1,y1,new,ymin);
			}
			else if(code2[2]==1)
			{
			new=y1-(((y2-y1)*(x1-xmax))/(x2-x1));
			clipping(x1,y1,xmax,new);
			}
			else if(code2[3]==1)
			{
			new=y1-(((y2-y1)*(x1-xmin))/(x2-x1));
			clipping(x1,y1,xmin,new);
			}
		}
	}

}

//To draw the boundary to enclose the circle - set clipping window
/*lines[i][0] => start point
  lines[i][3] => end point
*/
void drawlines()
{
	int i;
	glColor3fv(gColor);
	glBegin(GL_LINES);
	for( i=0; i<=numLines; i++)
	{
		glVertex2i(lines[i][0].x, gHeight-lines[i][0].y);
		glVertex2i(lines[i][2].x, gHeight-lines[i][2].y);

		glVertex2i(lines[i][2].x, gHeight-lines[i][2].y);
		glVertex2i(lines[i][3].x, gHeight-lines[i][3].y);

		glVertex2i(lines[i][3].x, gHeight-lines[i][3].y);
		glVertex2i(lines[i][1].x, gHeight-lines[i][1].y);

		glVertex2i(lines[i][1].x, gHeight-lines[i][1].y);
		glVertex2i(lines[i][0].x, gHeight-lines[i][0].y);
	}
	glEnd();
}

//To draw the background grid
void back()
{
	int i;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,1,1);
	glBegin(GL_LINES);
	for(i=0;i<=700;i+=50)
	{
		glVertex2i(0+i,0);
		glVertex2i(700,700-i);
	}
	for(i=0;i<=700;i+=50)
	{
		glVertex2i(0,0+i);
		glVertex2i(700-i,700);
	}
	for(i=0;i<=700;i+=50)
	{
		glVertex2i(0+i,0);
		glVertex2i(0,0+i);
	}
	for(i=0;i<=700;i+=50)
	{
		glVertex2i(0+i,700);
		glVertex2i(700,0+i);
	}
	glEnd();
	glColor3f(1,0,0);
	for(i=0;i<8;i++)
	{
		midpoint(i,500,500,0);
		glColor3f(0,1,0);
		midpoint(i,100,300,0);
		glColor3f(0,0,1);
		midpoint(i,600,500,0);
		glColor3f(1,0.5,0.5);
		midpoint(i,200,600,0);
		glColor3f(0.5,0.5,0.5);
		midpoint(i,600,200,0);
		glColor3f(0.3,0.7,0.5);
		midpoint(i,350,350,0);
		glColor3f(1,0.5,1);
		midpoint(i,200,200,0);
	}
	glFlush();
}

//Display function for screen3
void display_clip()
{

	back();
	drawlines();
	
	glutSwapBuffers();
}

void init_clip()
{
	glClearColor(0,0,0,1);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0,700,0,700);
	numLines=-1;
}

//Sending each background line for clipping
void clip()
{
	glClear(GL_COLOR_BUFFER_BIT);
	int i;
	for(i=0;i<=700;i+=50)
		clipping(i,0,700,700-i);

	for(i=0;i<=700;i+=50)
		clipping(0,i,700-i,700);

	for(i=0;i<=700;i+=50)
		clipping(i,0,0,i);
	
	for(i=0;i<=700;i+=50)
		clipping(i,700,700,i);
	
	glutSwapBuffers();
	sleep(2);
	
	//To display the next screen - the main game
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowSize(winw,winh);
	glutInitWindowPosition(200,100);
	glutCreateWindow("SNAKES AND LADDERS");

	createMenu();
	glutDisplayFunc(Draw);

	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);

	Init();
}

//To set coordinates for window and check if circle is enclosed
void mouseclick(int button, int state,int x, int y)
{
	int i;
	if(button==GLUT_LEFT_BUTTON && state==GLUT_UP)
	{
	switch(gState)
	{
		case waitingforclick:
			++numLines;
			lines[numLines][0].x=x;
			lines[numLines][0].y=y;
			lines[numLines][1].x=x;
			lines[numLines][1].y=y;
			lines[numLines][2].x=x;
			lines[numLines][2].y=y;
			lines[numLines][3].x=x;
			lines[numLines][3].y=y;
			xt1=x;
			yt1=y;
			gState++;
			flg2=0;
			break;
		case clickedonce:
			lines[numLines][0].x=xt1;
			lines[numLines][0].y=yt1;
			lines[numLines][1].x=x;
			lines[numLines][1].y=yt1;
			lines[numLines][2].x=xt1;
			lines[numLines][2].y=y;
			lines[numLines][3].x=x;
			lines[numLines][3].y=y;
			back();
			drawlines();

			//To detect if circle (centre) lies in the boundary chosen
			if((lines[numLines][0].x<600) && (lines[numLines][3].x>600)&& (lines[numLines][0].y<200) && (lines[numLines][3].y>200))
			{        
				flg2=1;
				xmax=lines[numLines][3].x;
				xmin=lines[numLines][0].x;
				ymax=700-lines[numLines][0].y;
				ymin=700-lines[numLines][3].y;
				clip();

			}
			gState=waitingforclick;
			break;
	}
	}
	glutPostRedisplay();
}

//To update each point as mouse is dragged
void mousedrag(int x, int y)
{
	if(gState==clickedonce)
	{
		lines[numLines][0].x=xt1;
		lines[numLines][0].y=yt1;
		lines[numLines][1].x=x;
		lines[numLines][1].y=yt1;
		lines[numLines][2].x=xt1;
		lines[numLines][2].y=y;
		lines[numLines][3].x=x;
		lines[numLines][3].y=y;
	}
	glutPostRedisplay();
}
void reshape(int width, int height)
{
	gHeight=height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,700,0,700);
	glMatrixMode(GL_MODELVIEW);
}


//-----------------------------------------------------------------------------------------------------------
//CODE FOR SCREEN2 - GUIDE MARY TO THE TROPHY


struct disp
{
	int opcode;
	int x;
	int y;
	int flag;
};

typedef struct
{
	int segno;
	int start;
	int size;
	int vis;
	int tx,ty;
	int sx,sy;
	int theta;
}seg;

seg table[2];

// points for current position of mary
int mptx[12]={150+v,150+v,130+v,50+v,50+v,100+v,100+v,200+v,200+v,250+v,250+v,170+v};
int mpty[12]={470+v,400+v,330+v,225+v,100+v,100+v,0+v,0+v,100+v,100+v,225+v,330+v};

// points for previous mary position
int tmpx[12]={150+v,150+v,130+v,50+v,50+v,100+v,100+v,200+v,200+v,250+v,250+v,170+v};
int tmpy[12]={470+v,400+v,330+v,225+v,100+v,100+v,0+v,0+v,100+v,100+v,225+v,330+v};

int imaryx,imaryy;

void man();
void star();
void trophy();
void draw_trophy();

// To read from segment file
void segment()
{
	int i=0;
	char line[20],c;
	FILE *s;
	s=fopen("seg.txt","r");
	while(fgets(line,sizeof(line),s))
	{
	sscanf(line,"%d %d %d %d %d %d %d %d %d %c",&table[i].segno,&table[i].start,&table[i].size,&table[i].vis,&table[i].tx,&table[i].ty,&table[i].sx,&table[i].sy,&table[i].theta,&c);
	i++;
	}
	fclose(s);
}

void file_update()
{
	FILE *s,*s1;
	int count=0;
	struct disp d1;
	s=fopen("seg.txt","w+");

	table[1].tx=mptx[0]-imaryx;
	table[1].ty=mpty[0]-imaryy;
	
	fprintf(s,"%d %d %d %d %d %d %d %d %d\n",table[0].segno,table[0].start,table[0].size,table[0].vis,table[0].tx,table[0].ty,table[0].sx,table[0].sy,table[0].theta);
	fprintf(s,"%d %d %d %d %d %d %d %d %d\n",table[1].segno,table[1].start,table[1].size,table[1].vis,table[1].tx,table[1].ty,table[1].sx,table[1].sy,table[1].theta);

	fclose(s);
}


// To draw ellipse in the trophy
//flag=1=>top, flag=-1=>bottom, flag=0=>full

void elip_trop(int xcen,int ycen,int rx,int ry,int flag)
{
	float x,y,rx2,ry2;
	float p1,p2;
	p1=(ry*ry)-(rx*rx*ry)+(rx*rx*0.25);
	x=0;
	y=ry;
	ry2=ry*ry;
	rx2=rx*rx;

	while((2*ry2*x)<(2*rx2*y))
	{
		if(p1<0)
		{
			x++;
			p1=p1+(2*ry2*x)+(ry2);
		}
		else
		{
			y--;
			x++;
			p1=p1+(2*ry2*x)-(2*rx2*y)+(ry2);
		}
		if(flag==0)
		{  
			setPixel(x+xcen,y+ycen);
			setPixel(-x+xcen,y+ycen);
			setPixel(x+xcen,-y+ycen);
			setPixel(-x+xcen,-y+ycen);
		}
		else if(flag==-1)
		{
			setPixel(x+xcen,-y+ycen);
			setPixel(-x+xcen,-y+ycen);
		}
		else
		{
			setPixel(x+xcen,y+ycen);
			setPixel(-x+xcen,y+ycen);
		}
	}

	p2=(ry2*(x+0.5)*(x+0.5))+(rx2*(y-1)*(y-1))-(rx2*ry2);

	while(y>0)
	{
		if(p2>0)
		{
			y--;
			p2+=(-(2*rx2*y)+(rx2));
		}
		else
		{
			y--;
			x++;
			p2+=((2*ry2*x)-(2*rx2*y)+(rx2));
		}
		if(flag==0)
		{  
			setPixel(x+xcen,y+ycen);
			setPixel(-x+xcen,y+ycen);
			setPixel(x+xcen,-y+ycen);
			setPixel(-x+xcen,-y+ycen);
		}
		else if(flag==-1)
		{
			setPixel(x+xcen,-y+ycen);
			setPixel(-x+xcen,-y+ycen);
		}
		else
		{
			setPixel(x+xcen,y+ycen);
			setPixel(-x+xcen,y+ycen);
		}
	}

}


int flg=0,check=1;

// To translate mary
void tran(int dx,int dy,int x,int y,int pt)
{
	if(flg==0) //To draw bankground
	{
		glBegin(GL_POLYGON); 
		glColor3f(1.0,0.5,0); 
		glVertex2f(0,0); 
		glColor3f(0.5,0.5,0); 
		glVertex2f(0,4700); 
		glColor3f(0.5,0,0.5); 
		glVertex2f(4700,4700); 
		glColor3f(0,0.8,0.8); 
		glVertex2f(4700,0); 
		glEnd(); 
		glFlush();
		flg=1;
		draw_trophy();
	}
	int i;

	//To check if mary has reached trophy
	if(mptx[0]+100>3850&&mpty[0]+100>3750&&check)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glutSwapBuffers();
		file_update(); 
		check=0;

		for(i=4200;i>1750;i-=501)
		{       
			if(i==4200)
			{
				glColor3f(0,0,1);
				star(i,0);
			}
			else
			{ 
				glColor3f(0,0,0);
				usleep(100000);
				glutSwapBuffers();

				if(i%2==0)
				  glColor3f(0,0,1);
				else
				  glColor3f(0,1,0);

				star(i,i%2);
			}	
		}
		glColor3f(0.8,0.5,0.2);
		drawstring(1000,1400,0,"YAY! YOU DID IT!!");
		drawstring(1000,1200,0,"YOU MAY PROCEED TO NEXT TASK!!");
		glColor3f(0.2,0.5,0.2);
		drawstring(1000,800,0,"TASK2: ");
		drawstring(1000,600,0,"CLICK-DRAG-CLICK TO FIND THE RIGHT CIRCLE");
		glutSwapBuffers();	
		sleep(2);

		glutDestroyWindow(glutGetWindow());
		sleep(2);
		glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
		glutInitWindowPosition(200,100);
		glutInitWindowSize(700,700);
		glutCreateWindow("SCREEN 3");
		glutReshapeFunc(reshape);
		glutDisplayFunc(display_clip);
		glutMouseFunc(mouseclick);
		glutPassiveMotionFunc(mousedrag);
		glutPostRedisplay(); //added function for speed and clarity
		init_clip();
	}
	else
	{
		glColor3f(0,0,0);
		man(tmpx,tmpy);	

		int i,j,k,genmat[3][3]={{1,0,dx},{0,1,dy},{0,0,1}},pointmat[3][1]={x,y,1},endmat[3][1];

		for(i=0;i<3;i++)
			for(j=0;j<1;j++)
			{
				endmat[i][j]=0;
				for(k=0;k<3;k++)
					endmat[i][j]+=genmat[i][k]*pointmat[k][j];
			}
		for(i=0;i<12;i++)
		{ 
			tmpx[i]=mptx[i];
			tmpy[i]=mpty[i];
		}
		mptx[pt]=endmat[0][0];
		mpty[pt]=endmat[1][0];

	}
}

// Move using arrow keys
void keySpecial (int key, int x, int y) 
{
	int i,j,t=50;
	if(check)
	{
		 if(key==GLUT_KEY_RIGHT)
		{
			for(j=0;j<12;j++)
				tran(t,0,mptx[j],mpty[j],j);
		}
		else if(key==GLUT_KEY_LEFT)
		{
			for(j=0;j<12;j++)
				tran(-t,0,mptx[j],mpty[j],j);
		}
		else if(key==GLUT_KEY_UP)
		{
			for(j=0;j<12;j++)
				tran(0,t,mptx[j],mpty[j],j);
	
		}
		else if(key==GLUT_KEY_DOWN)
		{
			for(j=0;j<12;j++)
				tran(0,-t,mptx[j],mpty[j],j);

		}
		if(check)
		{
		glColor3f(0.53,0.12,0.47);
		man(mptx,mpty);
		}
	}
}



// To draw the trophy and mary by reading from display file
/*
opcode=0 => Move
opcode=1 => Line
opcode=2 => Circle
opcode=3 => Ellipse
*/
void trophy()
{
int rx,ry,count=0;
char ch,line[20];
int xpos,ypos,i=0;
FILE *f;
struct disp d1;
//reading from file
f=fopen("trop.txt","r");
	while(fgets(line,sizeof(line),f))
	{
		if(count<=(table[0].start+table[0].size-1))
	 	{
			sscanf(line,"%d %d %d %d %c",&d1.opcode,&d1.x,&d1.y,&d1.flag,&ch);
			switch(d1.opcode)
			{
			case 0: xpos=d1.x;
				ypos=d1.y;
				break;
			case 1: linebres(xpos,ypos,d1.x,d1.y);
				xpos=d1.x;
				ypos=d1.y;
				break;
			case 2: midpoint(d1.x,(float)xpos,(float)ypos,d1.flag);
				break;
			case 3: elip_trop(xpos,ypos,d1.x,d1.y,d1.flag);	
				break;
			default: break;
			}
		}
		else
			break;
		count++;
	}
fclose(f);
}
// To draw trophy
void draw_trophy()
{
	glColor3f(1,1,1);
	elip_trop(4200,4200,150,300,-1);
	midpoint(130,4340,4180,2);
	midpoint(130,4060,4180,-2);
	linebres(4350,4195,4470,4180);
	linebres(4050,4195,3930,4180);
	linebres(4165,3900,4165,3800);
	linebres(4235,3900,4235,3800);
	elip_trop(4200,4200,150,50,0);
	elip_trop(4200,3760,100,40,0);
}

void draw_mary()
{
int count=1;

int rx,ry;
char ch,line[20];
int xpos,ypos,i=0;
FILE *f;
struct disp d1;
//reading from file
f=fopen("trop.txt","r");
	while(fgets(line,sizeof(line),f))
	{
		sscanf(line,"%d %d %d %d %c",&d1.opcode,&d1.x,&d1.y,&d1.flag,&ch);
		if(count==table[1].start)
		{	imaryx=d1.x;
			imaryy=d1.y;
		}
		if(count>=table[1].start)
		{
			switch(d1.opcode)
			{
			case 0: xpos=d1.x+table[1].tx;
				ypos=d1.y+table[1].ty;
				break;
			case 1: linebres(xpos,ypos,d1.x+table[1].tx,d1.y+table[1].ty);
				xpos=d1.x+table[1].tx;
				ypos=d1.y+table[1].ty;
				break;
			case 2: midpoint(d1.x,(float)xpos,(float)ypos,d1.flag);
				break;
			case 3: elip_trop(xpos,ypos,d1.x,d1.y,d1.flag);	
				break;
			default: break;
			}
		}
		count++;
	}
fclose(f);
//sleep(4);
count=1;
f=fopen("trop.txt","r");
	while(fgets(line,sizeof(line),f))
	{
		sscanf(line,"%d %d %d %d %c",&d1.opcode,&d1.x,&d1.y,&d1.flag,&ch);

		if(count>=table[1].start)
		{
			switch(d1.opcode)
			{
			case 0: xpos=d1.x;
				ypos=d1.y;
				break;
			case 1: linebres(xpos,ypos,d1.x,d1.y);
				xpos=d1.x;
				ypos=d1.y;
				break;
			case 2: midpoint(d1.x,(float)xpos,(float)ypos,d1.flag);				
				break;
			case 3: elip_trop(xpos,ypos,d1.x,d1.y,d1.flag);	
				break;
			default: break;
			}
		}
		count++;
	}
fclose(f);
}

// to draw the star
void star(int x,int flag)
{
	int i;
	if(flag==0)
		for(i=0;i<4600;i+=500)
		{
			glBegin(GL_TRIANGLES);
			glVertex2f(0+i,100+x);
			glVertex2f(125+i,100+x);
			glVertex2f(62.5+i,0+x);
			glVertex2f(62.5+i,125+x);
			glVertex2f(125+i,25+x);
			glVertex2f(0+i,25+x);
		}	
	else
		for(i=250;i<4600;i+=500)
		{
			glBegin(GL_TRIANGLES);
			glVertex2f(0+i,100+x);
			glVertex2f(125+i,100+x);
			glVertex2f(62.5+i,0+x);
			glVertex2f(62.5+i,125+x);
			glVertex2f(125+i,25+x);
			glVertex2f(0+i,25+x);
		}
	usleep(100000);
	glutSwapBuffers();
	glEnd();
	glFlush();

}

// The display function for task1
void display_mary()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutSpecialFunc(keySpecial);
	glColor3f(0.5,0.5,0);
	drawstring(100,4000,0,"COMPLETE TWO TASKS TO UNLOCK THE GAME");
	glColor3f(0.8,0.5,0.2);
	drawstring(100,3200,0,"TASK1: ");
	drawstring(100,3000,0,"GUIDE MARY TO THE TROPHY USING ARROW KEYS!");
	trophy();
	draw_mary();
}

// To draw the girl
void man(int *tx,int *ty)
{

	midpoint(70,tx[0],ty[0],0);
	glBegin(GL_TRIANGLES);
	glVertex2f(tx[1],ty[1]);
	glVertex2f(tx[4],ty[4]);
	glVertex2f(tx[9],ty[9]);
	glEnd();

	linebres(tx[2],ty[2],tx[3],ty[3]);
	linebres(tx[11],ty[11],tx[10],ty[10]);
	linebres(tx[5],ty[5],tx[6],ty[6]);
	linebres(tx[7],ty[7],tx[8],ty[8]);
	glFlush();
}

//------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//CODE FOR SCREEN1 - THE CUBE

typedef float Mat[3][3];
Mat inimat;

float ptsIni[20][2];
float ptsFin[20][2];
float refpt[2]={2350,2350};
float ScaleX=1.01,ScaleY=1.01;
void TransformPoints(void);
int PlotCt;
void copy();

// Initialises the matrix as Unit Matrix
void matrixSetIdentity(Mat m)           
{
	int i, j;
	for (i=0; i<3; i++)
	for (j=0; j<3; j++)
	m[i][j] = (i == j);
}

// Multiplies matrix a times b, putting result in b                                                                      
void matrixPreMultiply(Mat a , Mat b)          
{
	int i,j;
	Mat tmp;
	for (i = 0; i < 3; i++)
	for (j = 0; j < 3; j++)
	tmp[i][j] = a[i][0]*b[0][j] + a[i][1]*b[1][j] + a[i][2]*b[2][j];
	for (i = 0; i < 3; i++)
	for (j = 0; j < 3; j++)
	inimat[i][j] = tmp[i][j];
}
void welc(int x,int y)
{
	
	//Draw W
	linebres(1000+x,2500+y,1000+x,2400+y);
	linebres(1000+x,2400+y,1050+x,2450+y);
	linebres(1050+x,2450+y,1100+x,2400+y);
	linebres(1100+x,2400+y,1100+x,2500+y);

	//Draw E	
	linebres(1150+x,2500+y,1150+x,2400+y);
	linebres(1150+x,2500+y,1250+x,2500+y);
	linebres(1150+x,2450+y,1225+x,2450+y);
	linebres(1150+x,2400+y,1250+x,2400+y);

	//Draw L
	linebres(1300+x,2500+y,1300+x,2400+y);
	linebres(1300+x,2400+y,1400+x,2400+y);

	midpoint(60,1500+x,2450+y,3);
	midpoint(60,1600+x,2450+y,0);

	linebres(1700+x,2500+y,1700+x,2400+y);
	linebres(1700+x,2500+y,1750+x,2450+y);
	linebres(1800+x,2500+y,1750+x,2450+y);
	linebres(1800+x,2500+y,1800+x,2400+y);

	linebres(1850+x,2500+y,1850+x,2400+y);
	linebres(1850+x,2500+y,1950+x,2500+y);
	linebres(1850+x,2450+y,1925+x,2450+y);
	linebres(1850+x,2400+y,1950+x,2400+y);
}

	
	

// To scale the cube in screen1
void Scale(float sx , float sy)
{
	Mat m;
	glClear (GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON); 
	glColor3f(1.0,0,0); 
	glVertex2f(0,0); 
	glColor3f(0.0,1.0,0); 
	glVertex2f(0,4700); 
	glColor3f(0.0,0,1.0); 
	glVertex2f(4700,4700); 
	glColor3f(1.0,1.0,0); 
	glVertex2f(4700,0); 
	glEnd(); 
	glFlush();

	glColor3f(0.5,1.0,0.2);
	welc(700,0);
	glColor3f(0,1,1);
	matrixSetIdentity(m);
	m[0][0] = sx;
	m[0][2] = (1-sx)*refpt[0];
	m[1][1] = sy;
	m[1][2] = (1-sy)*refpt[1];
	matrixPreMultiply(m,inimat);
	TransformPoints();   

	glColor3f (0, 0, 0);
	glBegin(GL_LINE_LOOP);          
	   // Plot the Initial Polygon
	for(PlotCt=0 ; PlotCt<6 ; PlotCt++)
		glVertex2f(ptsFin[PlotCt][0],ptsFin[PlotCt][1]);

	glEnd();
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2f(ptsFin[0][0],ptsFin[0][1]);
	glVertex2f(ptsFin[6][0],ptsFin[6][1]);
	glVertex2f(ptsFin[6][0],ptsFin[6][1]);
	glVertex2f(ptsFin[2][0],ptsFin[2][1]);
	glVertex2f(ptsFin[6][0],ptsFin[6][1]);
	glVertex2f(ptsFin[4][0],ptsFin[4][1]);
	glEnd();

	glFlush();
}


void TransformPoints()
{
	int k;
	float tmp ;
	for (k = 0 ; k<N && k<20 ; k++)
	{
		ptsFin[k][0] = inimat[0][0]*ptsIni[k][0] + inimat[0][1]*ptsIni[k][1] + inimat[0][2];
		ptsFin[k][1] = inimat[1][0]*ptsIni[k][0] + inimat[1][1]*ptsIni[k][1] + inimat[1][2];
	}
	copy();
}

// copies final points are the initial points
void copy()
{
	int i;
	for(i=0;i<N;i++)
	{
		ptsIni[i][0]=ptsFin[i][0];
		ptsIni[i][1]=ptsFin[i][1];
	}
}

// Display function for screen1
void display_cube(void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON); 
	glColor3f(1.0,0,0); 
	glVertex2f(0,0); 
	glColor3f(0.0,1.0,0); 
	glVertex2f(0,4700); 
	glColor3f(0.0,0,1.0); 
	glVertex2f(4700,4700); 
	glColor3f(1.0,1.0,0); 
	glVertex2f(4700,0); 
	glEnd(); 
	glFlush();

	ptsIni[0][0]=2000;ptsIni[0][1]=2500;
	ptsIni[1][0]=2000;ptsIni[1][1]=2000;
	ptsIni[2][0]=2500;ptsIni[2][1]=2000;
	ptsIni[3][0]=2750;ptsIni[3][1]=2250;
	ptsIni[4][0]=2750;ptsIni[4][1]=2750;   
	ptsIni[5][0]=2250;ptsIni[5][1]=2750;
	ptsIni[6][0]=2500;ptsIni[6][1]=2500;

	glColor3f (0.5, 1.0, 0.2);
	int i;                 // Set the color to RED
	glColor3f (0, 0, 0);
	glBegin(GL_LINE_LOOP);          
	   // Plot the Initial Polygon
	for(PlotCt=0 ; PlotCt<6 ; PlotCt++)
		glVertex2f(ptsIni[PlotCt][0],ptsIni[PlotCt][1]);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(ptsIni[0][0],ptsIni[0][1]);
	glVertex2f(ptsIni[6][0],ptsIni[6][1]);
	glVertex2f(ptsIni[6][0],ptsIni[6][1]);
	glVertex2f(ptsIni[2][0],ptsIni[2][1]);
	glVertex2f(ptsIni[6][0],ptsIni[6][1]);
	glVertex2f(ptsIni[4][0],ptsIni[4][1]);
	glEnd();
	matrixSetIdentity(inimat);
	usleep(500000);
	for(i=0;i<17;i++)
	{
		glClear (GL_COLOR_BUFFER_BIT);
		Scale(ScaleX,ScaleY);
		usleep(50000);
	}
	glFlush();


	//CREATING SCREEN2 WINDOW
	sleep(2);
	glutDestroyWindow(glutGetWindow());
	glutInitWindowSize(winw,winh);	
	glutInitWindowPosition(200,100);
	glutCreateWindow("SCREEN 2");
	init();
	glutDisplayFunc(display_mary);
	glutSpecialFunc(keySpecial);
}

//------------------------------------------------------------------------------------------------------------------

void writefile()
{
	seg s1={1,1,0,1,0,0,1,1,0};
	FILE *f,*s;
	struct disp d;
	d.opcode=0;
	d.x=4200;
	d.y=4200;
	d.flag=0;
	f=fopen("trop.txt","w+");
	s=fopen("seg.txt","w+");
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	d.opcode=3;
	d.x=150;
	d.y=300;
	d.flag=-1;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	d.opcode=0;
	d.x=4340;
	d.y=4180;
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	d.opcode=2;
	d.x=130;
	d.y=0;
	d.flag=2;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	d.opcode=0;
	d.x=4060;
	d.y=4180;
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	d.opcode=2;
	d.x=130;
	d.y=0;
	d.flag=-2;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	d.opcode=0;
	d.x=4350;
	d.y=4195;
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	d.opcode=1;
	d.x=4470;
	d.y=4180;
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	d.opcode=0;
	d.x=4050;
	d.y=4195;
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	d.opcode=1;
	d.x=3930;
	d.y=4180;
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	d.opcode=0;
	d.x=4200;
	d.y=4200;
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	d.opcode=3;
	d.x=150;
	d.y=50;
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	d.opcode=0;
	d.x=4165;
	d.y=3900;
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	d.opcode=1;
	d.x=4165;
	d.y=3800;
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	d.opcode=0;
	d.x=4235;
	d.y=3900;
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	d.opcode=1;
	d.x=4235;
	d.y=3800;
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	d.opcode=0;
	d.x=4200;
	d.y=3760;
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);	
	s1.size++;
	d.opcode=3;
	d.x=100;
	d.y=40;
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	fprintf(s,"%d %d %d %d %d %d %d %d %d\n",s1.segno,s1.start,s1.size,s1.vis,s1.tx,s1.ty,s1.sx,s1.sy,s1.theta);
    
//END of trophy and beginning of bloody mary
	
	s1.segno=2;
	s1.start=s1.size+1;
	s1.size=0;

	d.opcode=0;
	d.x=150+v;
	d.y=470+v;
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
       	d.opcode=2;
	d.x=70;
	d.y=0;
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
        d.opcode=0;
	d.x=150+v;
	d.y=400+v;
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
        d.opcode=1;
	d.x=mptx[4];
	d.y=mpty[4];
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
        d.opcode=1;
	d.x=mptx[9];
	d.y=mpty[9];
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
          d.opcode=1;
	d.x=mptx[1];
	d.y=mpty[1];
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
        d.opcode=0;
	d.x=mptx[2];
	d.y=mpty[2];
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	 d.opcode=1;
	d.x=mptx[3];
	d.y=mpty[3];
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	  d.opcode=0;
	d.x=mptx[11];
	d.y=mpty[11];
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	 d.opcode=1;
	d.x=mptx[10];
	d.y=mpty[10];
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
          d.opcode=0;
	d.x=mptx[5];
	d.y=mpty[5];
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	 d.opcode=1;
	d.x=mptx[6];
	d.y=mpty[6];
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
          d.opcode=0;
	d.x=mptx[7];
	d.y=mpty[7];
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	 d.opcode=1;
	d.x=mptx[8];
	d.y=mpty[8];
	d.flag=0;
	fprintf(f,"%d %d %d %d\n",d.opcode,d.x,d.y,d.flag);
	s1.size++;
	fprintf(s,"%d %d %d %d %d %d %d %d %d\n",s1.segno,s1.start,s1.size,s1.vis,s1.tx,s1.ty,s1.sx,s1.sy,s1.theta);
	fclose(s);
        fclose(f);
}

int main(int argc,char **argv)
{
	srand ( time(NULL) );
	//writefile();	
	segment();
	
	glutInit(&argc,argv);
	glutInitWindowSize(winw,winh);

	//FOR CUBE
	glutInitWindowPosition(200,100);
	glutCreateWindow("cube");
	init();
	glutDisplayFunc(display_cube);
	glutMainLoop();
}















