
#include "bar_progress/bar_progress.h"

#define bg_red "\033[0;41m"
#define bg_green "\033[0;42m"
#define bg_yellow "\033[0;43m"
#define bg_blue "\033[0;44m"
#define bg_magenta "\033[0;45m"
#define bg_cyan "\033[0;46m"
#define bg_white "\033[30;47m"
#define bg_gray "\033[37;40m"

#define BG_GREEN "\033[42;30m"
#define BG_RED "\033[41m"

static struct winsize w;
static int initialized = 0;


static void set_window_height_for_bar_progress(int height)
{
	fprintf(stdout, "\n\0337"		// save cursor
			"\033[0;%dr"		// set scroll region (this will place the cursor in the top left)
			"\0338\033[1A\033[J"	// restore cursor but ensure its inside the scrolling area
			, height);
	fflush(stdout);
}


static void init_progress_bar()
{
	//ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	ioctl(1, TIOCGWINSZ, &w);
	set_window_height_for_bar_progress(w.ws_row - 1);
}


static void abort_progress_bar()
{
	bar_progress_stop();
	exit(0);
}


void bar_progress_start()
{
	signal(SIGWINCH, init_progress_bar);
	signal(SIGINT, abort_progress_bar);
	//signal(SIGSEGV, abort_progress_bar);
	init_progress_bar();
	initialized = 1;
	bar_progress_step(0);
}


void bar_progress_step_msg(int step_progress, int all_progress, char *msg, char fill_bar, char fill_dot, int colored /*bool */)
{
  static int cur_round=0;
  char prgss[]="\\|/-";
  int len = strlen(prgss);

	if (initialized == 0) {
		fprintf(stderr, "error: bar_progress_step() called before bar_progress_start().\n");
		exit(1);
	}

	if (step_progress < 0) {
		step_progress = 0;
	} 
  else if (step_progress >= all_progress) {
		step_progress = all_progress -1;
	}
  int size_char_log= strlen(msg) + strlen("(-) Progress: [100a] [aaaa/bbbb] a");
	int width = (w.ws_col - size_char_log);
  //int j;
  /*
  for(int j=0; j< width ; ++j)
    if (j<(int)(width * (step_progress / 100))) bar[j]='=';
    else bar[j]='.';
	bar[width] = 0;
  */
  int status_percent =  (step_progress+1) * 100 / all_progress;
  int status_progress = (step_progress+1) * width / all_progress;

	char *bar = malloc(status_progress + 1);  //w.ws_col);
	char *dot = malloc(width - status_progress +1 ); //w.ws_col);

  /*for(j=0; j< status_progress ; ++j)
    bar[j]=fill_bar;//' ';//'#';//'=';
 */ 
  memset(bar,fill_bar,status_progress); 

  bar[status_progress]='\0';

  /*for(j=0;j<width - status_progress; ++j)
     dot[j]=fill_dot;//' ';//'-'; //'.';
  */
  memset(dot,fill_dot, width-status_progress);
  dot[width - status_progress]='\0'; 
   

	if(colored)printf("\e[s\e[%d;0H(%c) "bg_white"Progress: [%3d%%]\e[0m ["bg_green"%s"bg_red"%s\e[0m] %s [%3d/%3d]\e[u", w.ws_row + 1, prgss[cur_round], status_percent, bar, dot, msg, step_progress + 1, all_progress);
  else printf("\e[s\e[%d;0H(%c) "bg_gray"Progress: [%3d%%]\e[0m ["bg_white"%s"bg_gray"%s\e[0m] %s [%3d/%3d]\e[u", w.ws_row + 1, prgss[cur_round], status_percent, bar, dot, msg, step_progress + 1, all_progress);
	//fprintf(stdout, "\e[s\e[%d;0H(%c) "bg_green"Progress: [%3d%%]\e[0m ["bg_green"%s"bg_red"%s\e[0m] %s [%3d/%3d]\e[u", w.ws_row + 1,prgss[cur_round], status_percent, bar,dot,msg,step_progress,all_progress);
	//fprintf(stdout, "\e[s\e[%d;0H(%c) "bg_green"Progress: [%3d%%]\e[0m ["BG_GREEN"%s"BG_RED"%s\e[0m] %s [%3d/%3d]\e[u", w.ws_row + 1,prgss[cur_round], status_percent, bar,dot,msg,step_progress,all_progress);
	//fprintf(stdout, "\e[s\e[%d;0H(%c) \e[42;30mProgress: [%3d%%]\e[0m [%s%s%s%s\033[0m] %s [%3d/%3d]\e[u", w.ws_row + 1,prgss[cur_round], status_percent,BG_GREEN, bar, BG_RED,dot,msg,step_progress,all_progress);
	//fprintf(stdout, "\e[s\e[%d;0H\e[42;30mProgress: [%3d%%]\e[0m [\033[42;30m%s\033[41m%s\033[0m]\e[u", w.ws_row + 1, (int) step_progress, bar,dot);
	//fprintf(stdout, "\e[s\e[%d;0H\e[42;30mProgress: [%3d%%]\e[0m [%s]\e[u", w.ws_row + 1, (int) step_progress, bar);
	//fprintf(stdout, "\e[s\e[%d;0H\e[42;30mProgress: [%3d%%]\e[0m [%s%s]\e[u", w.ws_row + 1, (int) step_progress, bar,dot);
	
  fflush(stdout);

  if(cur_round<len-1) ++cur_round;
  else cur_round = 0;

	free(bar);
	free(dot);
}

void bar_progress_step(float step_progress)
{
	if (initialized == 0) {
		fprintf(stderr, "error: bar_progress_step() called before bar_progress_start().\n");
		exit(1);
	}
	char *bar = malloc(w.ws_col);
	char *dot = malloc(w.ws_col);

	if (step_progress < 0) {
		step_progress = 0;
	} else if (step_progress > 100) {
		step_progress = 100;
	}
	int width = (w.ws_col - 20);
  int j;
  /*
  for(int j=0; j< width ; ++j)
    if (j<(int)(width * (step_progress / 100))) bar[j]='=';
    else bar[j]='.';
	bar[width] = 0;
  */
  int status = (int)(width * (step_progress / 100));
  for(j=0; j< status ; ++j)
    bar[j]='=';
    
  bar[j]='\0';
  for(j=0;j<width-status; ++j)
     dot[j]='.';
  dot[j]='\0'; 
   

	fprintf(stdout, "\e[s\e[%d;0H\e[42;30mProgress: [%3d%%]\e[0m [%s%s%s%s\033[0m]\e[u", w.ws_row + 1, (int) step_progress,BG_GREEN, bar, BG_RED,dot);
	//fprintf(stdout, "\e[s\e[%d;0H\e[42;30mProgress: [%3d%%]\e[0m [\033[42;30m%s\033[41m%s\033[0m]\e[u", w.ws_row + 1, (int) step_progress, bar,dot);
	//fprintf(stdout, "\e[s\e[%d;0H\e[42;30mProgress: [%3d%%]\e[0m [%s]\e[u", w.ws_row + 1, (int) step_progress, bar);
	//fprintf(stdout, "\e[s\e[%d;0H\e[42;30mProgress: [%3d%%]\e[0m [%s%s]\e[u", w.ws_row + 1, (int) step_progress, bar,dot);
	
  fflush(stdout);

	free(bar);
	free(dot);
}


void bar_progress_stop()
{
	set_window_height_for_bar_progress(w.ws_row);
}
