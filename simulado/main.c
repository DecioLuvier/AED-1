#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//
bool lemonadeChange(int* bills, int billsSize) {
    // Each index correspond a bill type
    // 0 = 5
    // 1 = 10
    // 2 = 20
    // Starting with 0 at each
    int billsReceived[3] = {};

    // For every bill received do a action
    for(int i = 0; i < billsSize; i++) {
        switch (bills[i]) {
        case 5:     // Never needs a change
            billsReceived[0]++; // Receive 5 bill
            break;
        case 10:    // Always needs a 5 bill type change
            if(billsReceived[0] == 0)
                return false;   // cannot provide change
            
            billsReceived[0]--; // Provide 5 change
            billsReceived[1]++; // Receive 10 bill
            break;
        case 20:    // Need a 10 or a 5 bill type change with this priority list
            int _temp = 0;
            if(billsReceived[1] > 0){   // if has a 10 bill
                _temp += 10;
                billsReceived[1]--;
            }

            while(billsReceived[0] > 0) {   // Provide 5 bill until over 
               billsReceived[0]--;
                _temp += 5;
                if(_temp == 15) 
                    break;
            }

            if(_temp != 15)                 // If don't provide bills
                return false;

            billsReceived[2]++; // Receive 20 bill
            break;
        default:
            return false;   // Unsuported bill type
        }
    }


    return true;
}

int main(){
    // Input
    int input[] = {5, 5, 10, 10, 20};
    // Static get size
    int len = sizeof(input)/(sizeof(int));
    // printf("%d", len); // printf -> 5, working
    bool expectedResult = true;

    bool responseResult = lemonadeChange(input, len);

    if(expectedResult == responseResult)
        printf("Correct answer!\nExpected: %d Received: %d", expectedResult, responseResult);
    else 
         printf("Incorrect answer.\nExpected: %d Received: %d", expectedResult, responseResult);
            
    return 0;
}