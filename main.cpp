#include "mbed.h"
#include <string>

// true rng from mbed
AnalogIn analog (p20);
unsigned int random_generator (void)
{
    unsigned int x = 0;
    unsigned int iRandom = 0;
    for (x = 0; x <= 32; x += 2)
    {
        iRandom += ((analog.read_u16() % 3) << x);
        wait_us (10);
    }
    return iRandom;
}

InterruptIn button1(p21);
InterruptIn button2(p22);
InterruptIn button3(p23);
InterruptIn button4(p24);
InterruptIn button5(p15);

volatile int hitArray[4] = {0,0,0,0};
int difficulty = 0;
int status = 1;
 
void change_status(){
    status += 2;  
    printf("im trying to change status \n\r");  
} 
 
void flip1() {
    hitArray[0] += hitArray[0] + 1;
}
 
void flip2() {
    hitArray[1] += hitArray[1] + 1;
}
 
void flip3() {
    hitArray[2] += hitArray[2] + 1;
}
 
void flip4() {
    hitArray[3] += hitArray[3] + 1;
}

bool get_true(int hit_arr, string res){
    if (hit_arr == 0 && res == "_"){
        return true;
    }
    if( hit_arr !=0 && res != "_"){
        return true;
    }
    else{
        return false;    
    }
}



void start_main(double wait_time, int max_symbols) {
    button1.rise(&flip1);  // attach the address of the flip function to the rising edge
    button2.rise(&flip2);  // attach the address of the flip function to the rising edge
    button3.rise(&flip3);  // attach the address of the flip function to the rising edge
    button4.rise(&flip4);  // attach the address of the flip function to the rising edge
    
    double hits = 0.0;
    int streak = 0;
    int score = 0;
    int max_streak = 0;
    
    // Get the system time.
    srand(random_generator());
    
    // Seed the random number generator.
    // make the play array
    int row_size = 30;
    int col_size = 4;
    string myArray[row_size][col_size];
    for ( size_t i = 0; i < row_size; i++){
        for(size_t j = 0; j < col_size; j++){
            myArray[i][j] = "_";
        }        
        // add in the note
        string color[4] = {"R", "G", "B", "Y"}; 
        for (int k = 0; k < max_symbols; k++){
            int ran = rand() % col_size;
            myArray[i][ran] = color[ran];   
        }
    }
    
        //start padding the top
    int top_pad = 20;
    for (int i = 0; i < top_pad; i++){
        printf("X X X X %d\n\r", i);
    }
    printf("Hit the notes when they reach the top!\n\r");
    
    
    // basically when we let the notes float up the top pads #of rows, we start comparing the hit array to the note array
    // hit array shows how you did on the note that just passed
    // sequence:
    // 1st note is shown, zero out the button hits, do the button hits, gets pushed up
    // 2nd note shown and show the button hits from before, zero out the button hits, gets pushed up
    // start showing the array
    for ( size_t i = 0; i < row_size; i++){
        if(i < top_pad-2){
            for(size_t j = 0; j < col_size; j++){
                printf("%s ", myArray[i][j].c_str());
            }   
            printf("\n\r");
            wait(wait_time);
        }
        else{
            //print next notes
            for(size_t j = 0; j < col_size; j++){
                printf("%s ", myArray[i][j].c_str());
            }   
            
            //show your hit array
            //10 empty spaces
            printf("          ");
            printf("[");
            for (int j = 0; j < 4; j++){
                if(hitArray[j] == 0){
                    printf("%d,", hitArray[j]);
                }
                else{
                    printf("1,");   
                }
            }
            printf("]");
            
            // if hit array is 0 and arraynum is _; good
            // if hit array is 0 and arraynum is not _; bad
            // if hit array is !=0 and arraynum is not_; good
            // if hit array is !=0 and arraynum is _; bad
            if( get_true(hitArray[0],myArray[i-(top_pad-2)][0]) &&
                get_true(hitArray[1],myArray[i-(top_pad-2)][1]) &&
                get_true(hitArray[2],myArray[i-(top_pad-2)][2]) &&
                get_true(hitArray[3],myArray[i-(top_pad-2)][3]) ){
                // add to hits
                hits += 1.0;
                printf("hit");
                // update streak
                streak += 1;
                if (streak > max_streak){
                    max_streak = streak;
                }
                // update score
                if (streak % 30 > 3){
                    score += 4 * 25;   
                }
                else{
                    score += ((streak % 30) + 1) * 25; 
                }
            }else{
                streak = 0;
            }
            
            //reset the hit array to zero
            for (int i = 0; i < 4; i++){
                hitArray[i] = 0;
            }

            //add new line and wait
            printf("\n\r");
            wait(wait_time);
        }
    }
    // push up the rest
    // already printed row_size-(top_pad-2) worth of lines
    // now to printed out the remaining (top_pad-2)
    for (int i = 0; i < top_pad-2; i++){
        printf("X X X X ");
        
        //show your hit array
        //10 empty spaces
        printf("          ");
        printf("[");
        for (int j = 0; j < 4; j++){
            if(hitArray[j] == 0){
                printf("%d,", hitArray[j]);
            }
            else{
                printf("1,");
            }
        }
        printf("]");
        
        // if hit array is 0 and arraynum is _; good
        // if hit array is 0 and arraynum is not _; bad
        // if hit array is !=0 and arraynum is not_; good
        // if hit array is !=0 and arraynum is _; bad
        if( get_true(hitArray[0],myArray[i-(top_pad-2)+row_size][0]) &&
            get_true(hitArray[1],myArray[i-(top_pad-2)+row_size][1]) &&
            get_true(hitArray[2],myArray[i-(top_pad-2)+row_size][2]) &&
            get_true(hitArray[3],myArray[i-(top_pad-2)+row_size][3]) ){
            hits += 1.0;
            printf("hit");
        }
        
        //reset the hit array to zero
        for (int k = 0; k < 4; k++){
            hitArray[k] = 0;
        }
        //add new line and wait
        printf("\n\r");
        wait(wait_time);
    }
    
    for (int i = 0; i < 20; i++){
      printf("X X X X \n\r");
    }    
    printf("score: %d\n\r", score);
    printf("total accuracy: %.2f%%\n\r", hits/row_size*100);
    printf("max streak: %d\n\r", max_streak);
    printf("Thanks for playing \n\r");

        
}
void change_difficulty(){
    difficulty = (difficulty + 1) %4;
    printf("difficulty: ");
    if (difficulty == 0){
        printf("1-Beginner \n\r");
    }
    else if (difficulty == 1){
        printf("2-Normal with 1 note \n\r");    
    } 
    else if (difficulty == 2){
        printf("3-Normal with 2 notes \n\r");   
    }
    else if (difficulty == 3){
        printf("4-Expert\n\r");
    }
}

int main(){
    printf("\n\rWelcome to Rock Band\n\r");
    printf("Current difficulty is Beginner. Press the right most button to change difficulty. \n\r");
    printf("Or Press the left most button to start\n\r");
    button1.rise(&change_status);
    button5.fall(&change_difficulty);  // attach the address of the flip function to the rising edge
    while(status == 1){
        wait(0.1);
    }
    if (difficulty == 0){
        start_main(1.0, 1);
    }
    else if (difficulty == 1){
        start_main(0.5, 1);
    }
    else if (difficulty == 2){
        start_main(0.5, 2);
    }
    else if (difficulty == 3){
        start_main(0.25, 2);
    }
}