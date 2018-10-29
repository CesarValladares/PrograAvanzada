#include <ncurses.h>
#include <unistd.h>
#include <pthread.h>

#define DELAY 30000

int up_down = 0;
int program = 1;

// Thread Functions 
void * moveloop(void * arg);
void * getKey(void * arg);
void startThreads();

int main(int argc, char *argv[]) {

    initscr();
    noecho();
    curs_set(FALSE);
    cbreak();
    keypad(stdscr, TRUE);
    
    startThreads();

    endwin();
}

void * moveloop(void * arg){
    int x = 0, y = 0;
    int max_y = 0, max_x = 0;
    int next_x = 0;
    int next_y = 0;
    int direction = 1;
    
    // Global var `stdscr` is created by the call to `initscr()`
    getmaxyx(stdscr, max_y, max_x);

    while(1) {
        clear(); // Clear the screen of all
        // previously-printed characters
        mvprintw(y, x, "o"); // Print our "ball" at the current xy position
        refresh();

        usleep(DELAY); // Shorter delay between movements
        
        next_x = x + direction;
        next_y = y + up_down;

        if (next_y <= (max_y-1) && next_y >= 0){

            y+=up_down;

        }else{ up_down *= -1;}  

        if (next_x >= max_x || next_x < 0){

            direction *= -1;

        }
        else{

            x+=direction;

        }  
    }
}

void * getKey(void * arg){

    int ch; 

    switch(ch){
        case KEY_UP:

            up_down = -1;
            break;
        case KEY_DOWN:
            up_down = 1;                
            break;
    }

}

// Prepare the two threads and initalize the data
void startThreads()
{
    pthread_t tid[2];
    int status;

    // Start the two threads
    status = pthread_create(&tid[0], NULL, &moveloop, NULL);
    if (status)
    {
        fprintf(stderr, "ERROR: pthread_create %d\n", status);
    }
    status = pthread_create(&tid[1], NULL, &getKey, NULL);
    if (status)
    {
        fprintf(stderr, "ERROR: pthread_create %d\n", status);
    }

    pthread_exit(NULL);
}