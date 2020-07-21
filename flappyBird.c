#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <util/delay.h>
#include "ruota.h"
#include "rios.h"
#include "image.h"

#define BIRD_WIDTH BIG_WIDTH
#define BIRD_HEIGHT BIG_HEIGHT
image bird;
int gameState;

#define TITLE_FRAME 0
#define RESTART_FRAME 1
#define WIN_FRAME 2
#define MAIN_FRAME 3
#define DEMO_FRAME 4

void mainFrame();
void restartFrame();
void setupMainFrame();
void demoFrame();

#define TIME_TO_DEMO 1000/25
volatile timmer = 0;


void showCharacter(image i, int frame){
    if(frame == 0 ){
        draw_image_indexed(i,bird_1); 
    }
    else if(frame == 1){
        draw_image_indexed(i,bird_2); 
    }
    else{
        draw_image_indexed(i,bird_3); 
    }
                    
}

int firstTitle=1;
void titleFrame(){
    if(!(get_switch_state(_BV(SWN)) || get_switch_state(_BV(SWE)) || get_switch_state(_BV(SWW)) || get_switch_state(_BV(SWS))) && firstTitle){
        clear_screen();
        display_color(GOLD,BLACK);
        display_string_xy("FLOPPY BIRD", 85, 75);
        display_string_xy("> Play <", 85, 90);
        firstTitle=0;
        timmer = 0;
    } else if(!firstTitle){     
	    if (get_switch_state(_BV(SWC))) {
                setupMainFrame();
			    gameState=MAIN_FRAME;
			    firstTitle=1;
	    } 
        else if(timmer >= TIME_TO_DEMO){
            setupMainFrame();
            gameState = DEMO_FRAME;
            firstTitle=1;
        }
	}
}

int firstRestart=1;
int select = 0;    // 0 -> restart 1->exit
void restartFrame(){
    if(firstRestart){
       // clear_screen();
        
        rectangle r_b;
        r_b.top = 75;
        r_b.left = 110;
        r_b.bottom = 175;
        r_b.right = 210;
        fill_rectangle(r_b,GOLD);

        rectangle r;
        r.top = 80;
        r.left = 115;
        r.bottom = 170;
        r.right = 205;
        fill_rectangle(r,BLACK);
        display_string_xy("GAME OVER",135,100);
        display_string_xy("SCORE",140,115);
        showScore(175,115);

        display_color (DEEP_SKY_BLUE,BLACK);
        display_string_xy("> RESTART <",130,140);
        display_color (GOLD,BLACK);
        display_string_xy("  EXIT  ",135,155);
        firstRestart=0;
        select = 0;
    }
     if (get_switch_state(_BV(SWN))){
        display_color (DEEP_SKY_BLUE,BLACK);
        display_string_xy("> RESTART <",130,140);
        display_color (GOLD,BLACK);
        display_string_xy("  EXIT  ",135,155);
        select = 0;
     }
     if (get_switch_state(_BV(SWS))){
        display_color (GOLD,BLACK);
        display_string_xy("  RESTART  ",130,140);
        display_color (DEEP_SKY_BLUE,BLACK);
        display_string_xy("> EXIT <",135,155);
        select = 1;
     }
    if (get_switch_state(_BV(SWC))) {
        if(select == 0){                 //restart
            setupMainFrame();
            gameState = MAIN_FRAME;
        }
        else{           
            gameState=TITLE_FRAME;	
            _delay_ms(200);	
        }       
		firstRestart=1;
	}
}

int firstWin=1;
void winFrame(){
    if(firstWin){
       // clear_screen(); 
        draw_hearts(0);
        rectangle r_b;
        r_b.top = 75;
        r_b.left = 110;
        r_b.bottom = 175;
        r_b.right = 210;
        fill_rectangle(r_b,GOLD);

        rectangle r;
        r.top = 80;
        r.left = 115;
        r.bottom = 170;
        r.right = 205;
        fill_rectangle(r,BLACK);
        display_string_xy("YOU WIN!",135,100);
        display_string_xy("SCORE",140,115);
        showScore(175,115);

        display_color (DEEP_SKY_BLUE,BLACK);
        display_string_xy("> RESTART <",130,140);
        display_color (GOLD,BLACK);
        display_string_xy("  EXIT  ",135,155);
        firstWin = 0;
        select = 0;
    }
     if (get_switch_state(_BV(SWN))){
        display_color (DEEP_SKY_BLUE,BLACK);
        display_string_xy("> RESTART <",130,140);
        display_color (GOLD,BLACK);
        display_string_xy("  EXIT  ",135,155);
        select = 0;
     }
     if (get_switch_state(_BV(SWS))){
        display_color (GOLD,BLACK);
        display_string_xy("  RESTART  ",130,140);
        display_color (DEEP_SKY_BLUE,BLACK);
        display_string_xy("> EXIT <",135,155);
        select = 1;
     }
    if (get_switch_state(_BV(SWC))) {
        if(select == 0){                 //restart
            setupMainFrame();
            gameState = MAIN_FRAME;
        }
        else{           
            gameState=TITLE_FRAME;	
            _delay_ms(200);	
        }       
		firstWin=1;
	}
}


void demoFrame(){
    if(get_switch_state(_BV(SWC))){
        gameState = TITLE_FRAME;
        return;
    }
    mainFrame();   //lose or win
    if(gameState!=DEMO_FRAME){
        setupMainFrame();
        gameState = DEMO_FRAME;
    }

    display_color(GOLD,BLACK);
    display_string_xy("FLOPPY BIRD DEMO", 85, 75);
    display_string_xy("> Play <", 85, 90);

}




/************* Main ****************/

int displayFrame(int state);

volatile flag=0;


int displayFrame(int state){   //flash
    timmer++;
    flag=1;
    return state;
}


int main(void) {
    /* 8MHz clock, no prescaling (DS, p. 48) */
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;
      
    init_lcd();
    os_init_scheduler();
    os_init_ruota();


    srand(1);    //as seed of random number generator
    //random_seed = rand_init();          
    gameState = TITLE_FRAME;

    os_add_task(displayFrame,25, 1);    //show frame in period  flag - >1
    sei();



    while(1){
        if(flag){
            cli();
            flag = 0;
            sei();

            switch (gameState)
            {
                case TITLE_FRAME:
                    titleFrame();
                    break;
                case RESTART_FRAME:
                    restartFrame();
                    break;
                case WIN_FRAME:
                    winFrame();
                    break;
                case MAIN_FRAME:
                    mainFrame();
                    break;
                case DEMO_FRAME:
                    demoFrame();
                    break;
                default:
                    break;
            }
        }
        
    }
}

///////////////////////////////////////////////////////////

#define RUN_PLATFORMS 5
#define RUN_PLATFORM_HEIGHT 10

#define RUN_maxVX 15
#define RUN_maxVY 5

#define RUN_OBSTACLES 5
#define RUN_OBSTACLES_PER_PLATFORM 10
#define RUN_OBSTACLE_WIDTH 25

#define HEART 3
#define HEART_HEIGHT 6
#define HEART_WIDTH 7

#define FLOWER 5
#define FLOWER_HEIGHT 20
#define FLOWER_WIDTH 16


#define RUN_SPEED_INITIAL 3.6
#define RUN_SPEED_INCREMENT 0.4

int runVx;
int runVy;

int currentRunPlatform;
image runPlatform;
image topPlatform;



image obstacles[RUN_OBSTACLES];
image obstacles2[RUN_OBSTACLES];
image hearts[HEART];    // three hearts refer to HP
image flower;

int runAtPlatform;
int hitBarrier;

int life;

int frame;
double runSpeed;
const uint16_t runColours[RUN_PLATFORMS]={INDIGO,BLUE,GREEN,ORANGE,RED};

int passedObstacles;
int createdObstacles;
double lastObstacle;
void createRandomObstacle(int i){
    
        obstacles[i].height = 30+rand()%50;
        obstacles[i].top = HEIGHT-RUN_PLATFORM_HEIGHT-obstacles[i].height;
        obstacles[i].left = lastObstacle+RUN_OBSTACLE_WIDTH+120+rand()%100;
        obstacles[i].width = RUN_OBSTACLE_WIDTH;
        obstacles[i].colour = runColours[currentRunPlatform%RUN_PLATFORMS];

        obstacles2[i].height = 30+rand()%50;
        obstacles2[i].top = RUN_PLATFORM_HEIGHT;
        obstacles2[i].left = obstacles[i].left;
        obstacles2[i].width = RUN_OBSTACLE_WIDTH;
        obstacles2[i].colour = runColours[currentRunPlatform%RUN_PLATFORMS];


        lastObstacle=obstacles[i].left;
        createdObstacles++;
        if(createdObstacles>RUN_PLATFORMS*RUN_OBSTACLES_PER_PLATFORM){
            obstacles[i].left = 100000;
            obstacles2[i].left = 100000;
        }
}

int position;
void createFlower(int i){  //after obstacles
    position = i;
    image im = obstacles[i];
    flower.height = FLOWER_HEIGHT;
    flower.top = im.top - FLOWER_HEIGHT;
    flower.left = obstacles[i].left;
    flower.width = FLOWER_WIDTH;
}

void drawFlower(){
    draw_image_indexed(flower,flower_image);
}


void init_hearts(){
    int i=0;
    int left = 200;
    int interval = 9;   ///left
    for(;i<HEART;i++){
        hearts[i].height = HEART_HEIGHT;
        hearts[i].top =  HEIGHT/2;
        hearts[i].left = left;
        hearts[i].width = HEART_WIDTH;
        hearts[i].colour = DEEP_PINK;
        left = left + interval;
    }
        
}

void draw_hearts(int rest_life){
    int i=0;
    if(rest_life == 3){
        for(;i<HEART;i++){
         draw_image_indexed(hearts[i],heart_image);
        }
    }  
    if(rest_life <= 2){   //lost one life
        fill_image(hearts[2],BLACK);
    }
    if(rest_life <= 1){
        fill_image(hearts[1],BLACK);
    }
    if(rest_life <= 0){
        fill_image(hearts[0],BLACK);
    }

}


void draw_obstacles(){
    int i=0;
    for(;i<RUN_OBSTACLES;i++){
        fill_image(obstacles[i],obstacles[i].colour);
        fill_image(obstacles2[i],obstacles2[i].colour);
    }
}

void init_obstacles(){
    int i=0;
    for(;i<RUN_OBSTACLES;i++){
        createRandomObstacle(i);
    }
}

void showScore(uint16_t x, uint16_t y){
    char convert[5];
    itoa(passedObstacles,convert,10);  //decimal base
    display_string_xy(convert,x,y);
}

void bird_die(image bird){
    image temp = bird;
    int bird_y = bird.top;
    for(;bird_y + bird.height < HEIGHT - RUN_PLATFORM_HEIGHT;bird_y++){
        fill_image(temp,BLACK);
        temp.top = bird_y;
        showCharacter(temp,2);
        _delay_ms(5);
    }
    _delay_ms(800);
}

void setupMainFrame(){
   clear_screen();
   passedObstacles=0;
   createdObstacles=0;
   frame=0;
   currentRunPlatform=0;
   runAtPlatform=1;
   hitBarrier = 0;

   life = HEART;

   bird.left=0;
   runVx=0;
   runVy=0;
   lastObstacle=0;
   runSpeed=RUN_SPEED_INITIAL;
   bird.left=0;
   bird.top=HEIGHT-BIRD_HEIGHT-RUN_PLATFORM_HEIGHT;
   bird.width=BIRD_WIDTH;
   bird.height=BIRD_HEIGHT;
   bird.colour=WHITE;

   runPlatform.left=0;
   runPlatform.width=WIDTH;
   runPlatform.top=HEIGHT-RUN_PLATFORM_HEIGHT;
   runPlatform.height=RUN_PLATFORM_HEIGHT;
   runPlatform.colour=runColours[0]; 
   fill_image(runPlatform,runPlatform.colour);

   topPlatform.left=0;
   topPlatform.width=WIDTH;
   topPlatform.top=0;
   topPlatform.height=RUN_PLATFORM_HEIGHT;
   topPlatform.colour=runColours[0];
   fill_image(topPlatform,topPlatform.colour);

   showCharacter(bird,0);
    showScore(WIDTH/2,HEIGHT/2);

   init_obstacles();
   init_hearts();
   draw_obstacles();
   draw_hearts(HEART);


 //  int x = rand()%5;
 //  createFlower(x);
 //  drawFlower();

}

/*
    parts of function mainFrame are referenced from Nikola Bozhinov's 
    LaTowerRun game from 2016
*/

void mainFrame(){
    frame++;
    image temp=bird;
    double offset=runSpeed;   //initial speed value
    lastObstacle-=offset;

    if(gameState == DEMO_FRAME){
        //control automatically
        if(bird.top+runVy >=  LCDWIDTH/2 - BIRD_HEIGHT/2 ){
            runVy=-RUN_maxVY;    //negtive
            runAtPlatform=0;
        }else if(!runAtPlatform && runVy<RUN_maxVY/2)
        {
            runVy++;
        }
    
    }
    else{
         if (get_switch_state(_BV(SWW)) && runVx>-RUN_maxVX) {
	    	runVx--;
        } else if (get_switch_state(_BV(SWE)) && runVx<RUN_maxVX) {
            runVx++;
        } else if(runVx<0){
            runVx++;
        } else if(runVx>0){
            runVx--;
        }
        if (get_switch_state(_BV(SWN))) {      //fly
            runVy=-RUN_maxVY;    //negtive
            runAtPlatform=0;
        } else if(!runAtPlatform && runVy<RUN_maxVY/2){
            runVy++;   //drop
        }
    }

   
	if(runVx<0 && bird.left!=0){
	    temp.left= bird.left+runVx<0 ? 0 : bird.left+runVx;
	    if(temp.left==0){
	        runVx=0;
	    }
	}else if(runVx>0 && bird.left!=WIDTH-BIRD_WIDTH){
	    temp.left= bird.left+runVx+BIRD_WIDTH>=WIDTH? WIDTH-BIRD_WIDTH : bird.left+runVx;
	    if(temp.left==WIDTH-BIRD_WIDTH){
	        runVx=0;
	    }
	}
	if(!runAtPlatform){         //stop dropping
        if(bird.top+runVy>topPlatform.height){        //not reaach the ceil
            temp.top=bird.top+runVy;
            if(runVy>=0){   //drop to the ground
                    if(bird.top+BIRD_HEIGHT<=runPlatform.top && temp.top+BIRD_HEIGHT>runPlatform.top){
                        temp.top=(runPlatform.top-BIRD_HEIGHT);
                        runAtPlatform=1;
                    }
            }
        }
        else{
            temp.top = topPlatform.height;
        }
	}
	temp.left=temp.left-offset<0?0:temp.left-offset;
	int i=0;
	for(;i<RUN_OBSTACLES;i++){       // 5 times
        if(offset<=-RUN_OBSTACLE_WIDTH){              
            fill_image(obstacles[i],BLACK);
            fill_image(obstacles2[i],BLACK);

            obstacles[i].left-=offset;
            obstacles2[i].left-=offset;

            fill_image(obstacles[i],obstacles[i].colour);  //update old blocks
            fill_image(obstacles2[i],obstacles2[i].colour);

            

        } else {
            image im=obstacles[i];
            im.width=offset;
            im.left=obstacles[i].left+obstacles[i].width-offset;
            fill_image(im,BLACK);
            im.left-=obstacles[i].width;
            fill_image(im,im.colour);
            obstacles[i].left-=offset;

            image im2=obstacles2[i];
            im2.width=offset;
            im2.left=obstacles2[i].left+obstacles2[i].width-offset;
            fill_image(im2,BLACK);
            im2.left-=obstacles2[i].width;
            fill_image(im2,im2.colour);
            obstacles2[i].left-=offset;
        }
            // draw flower
       //     if(position == i){
      //          fill_image(flower,BLACK);
      //          flower.left =  obstacles[i].left;
      //          drawFlower();
      //      }

        // lost a life
        if(((temp.left<=obstacles[i].left && temp.left+temp.width>=obstacles[i].left) ||
            (temp.left<=obstacles[i].left + obstacles[i].width && temp.left+temp.width>=obstacles[i].left + obstacles[i].width)) && 
            ((temp.top+temp.height>=obstacles[i].top) ||
            (temp.top<=obstacles[i].top + obstacles[i].height && temp.top+temp.height>=obstacles[i].top + obstacles[i].height))){
                life--;
                draw_hearts(life);
                temp.left = 0;
                temp.top = 2 + HEIGHT/2;
                hitBarrier = 1;
                runAtPlatform = 0;
            }
        if(((temp.left<=obstacles2[i].left && temp.left+temp.width>=obstacles2[i].left) ||
            (temp.left<=obstacles2[i].left + obstacles2[i].width && temp.left+temp.width>=obstacles2[i].left + obstacles2[i].width)) && 
            ((temp.top <= obstacles2[i].height + RUN_PLATFORM_HEIGHT) ||
            (temp.top <= RUN_PLATFORM_HEIGHT && temp.top + temp.height >= RUN_PLATFORM_HEIGHT ))){
                life--;
                draw_hearts(life);
                temp.left = 0;
                temp.top = 2 + HEIGHT/2;
                hitBarrier = 1;
                runAtPlatform = 0;
            }
        //failed
        if(life == 0){        
            fill_image(bird,BLACK);
            bird_die(temp);
            gameState=RESTART_FRAME;
            return;
        }

        
        if(obstacles[i].left+obstacles[i].width<=0){  //move out of the screen
            passedObstacles++;

            showScore(WIDTH/2,HEIGHT/2);

            if(passedObstacles%RUN_OBSTACLES_PER_PLATFORM==0){
                if(passedObstacles==RUN_PLATFORMS*RUN_OBSTACLES_PER_PLATFORM){
                    passedObstacles = passedObstacles + life*10;
                    gameState=WIN_FRAME;
                    return;
                }
                currentRunPlatform+=1;
                runPlatform.colour=runColours[currentRunPlatform%RUN_PLATFORMS];
                topPlatform.colour=runColours[currentRunPlatform%RUN_PLATFORMS];
                runSpeed+=RUN_SPEED_INCREMENT;
                int j=0;
	            for(;j<RUN_OBSTACLES;j++){
	                obstacles[j].colour=runColours[currentRunPlatform%RUN_PLATFORMS];
                    obstacles2[j].colour=runColours[currentRunPlatform%RUN_PLATFORMS];
	            }

                fill_image(topPlatform,topPlatform.colour);
                fill_image(runPlatform,runPlatform.colour);
                
            }
            createRandomObstacle(i);

            //if(position == i){
            //    int x = rand()%5;
            //    createFlower(x);
            //}
            
            
        }
    }
    fill_image(bird,BLACK);
    bird=temp;    
    if(hitBarrier){             //blink
        showCharacter(bird,(frame/5)%3);
        int i = 0;
        for(;i<3;i++){
             _delay_ms(300);
            fill_image(bird,BLACK);
            _delay_ms(300);
            showCharacter(bird,(frame/5)%3);
        }      
        hitBarrier = 0;
    }
    else{
        showCharacter(bird,(frame/5)%3);
    }
}