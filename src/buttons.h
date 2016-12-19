struct Mouse 
{
	int x;		/*	the x coordinate of the mouse cursor	*/
	int y;		/*	the y coordinate of the mouse cursor	*/
	int lmb;	/*	is the left button pressed?		*/

	int xpress; /*	stores the x-coord of when the first button press occurred	*/
	int ypress; /*	stores the y-coord of when the first button press occurred	*/
};
typedef struct Mouse Mouse;
typedef void (*ButtonCallback)();
struct Button 
{
	int   x;							/* top left x coord of the button */
	int   y;							/* top left y coord of the button */
	int   w;							/* the width of the button */
	int   h;							/* the height of the button */
	int	  state;						/* the state, 1 if pressed, 0 otherwise */
	int	  highlighted;					/* is the mouse cursor over the control? */
	const char* label;					/* the text label of the button */
	ButtonCallback callbackFunction;	/* A pointer to a function to call if the button is pressed */
};

typedef struct Button Button;

extern void ButtonDraw(Button *b);
extern void ButtonDraw1(Button *b);
extern void ButtonPassive(Button *b,int x,int y);
extern void ButtonPress(Button *b,int x,int y);
extern void ButtonRelease(Button *b,int x,int y);
extern int ButtonClickTest(Button* b,int x,int y) ;
extern void Font(void *font,const char *text,int x,int y);

extern void MouseButton(int button,int state,int x, int y);
extern void MousePassiveMotion(int x, int y);
extern void ThenewgameCallback();
extern void ThejoingameCallback();
extern void TheexitgameCallback();
extern void ThechangeuserCallback();
extern void ThedifficultyCallback();
extern void ThestartgameCallback();
extern void ThecancelgamenewCallback();
extern bool send_difficulty;	/* Called when the starting player presses "start game" button */
