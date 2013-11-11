#include "danish_time.h"
#include "string.h"
#include "pebble_os.h"
#include "pebble_app.h"
static const char* const MONTHS[] = {
  "januar",
  "februar",
  "marts",
  "april",
  "maj",
  "juni",
  "juli",
  "august",
  "september",
  "oktober",
  "november",
  "december"
};
static const char* const DAYS[] = {
  "søndag",
  "mandag",
  "tirsdag",
  "onsdag",
  "torsdag",
  "fredag",
  "lørdag"
};
static const char* const ONETEENS[] = {
  "nul",
  "et",
  "to",
  "tre",
  "fire",
  "fem",
  "seks",
  "syv",
  "otte",
  "ni",
  "ti",
  "elleve",
  "tolv",
  "tretten",
  "fjorten",
  "femten",
  "seksten",
  "sytten",
  "atten",
  "nitten"
};

static const char* const TWENS[] = {
  "tyve",
  "tredive",
  "fyrre",
  "halvtreds",
};

static const char* STR_NOON = "tolv";
static const char* STR_MIDNIGHT = "midnat";
static const char* STR_QUARTER = "kvart";
static const char* STR_TO = "i";
static const char* STR_PAST = "over";
static const char* STR_HALF = "halv";
static const char* STR_ITS = "den er";
static const char* STR_ALMOST = "næsten";
static const char* STR_JUST = "lidt";

#define LINE_BUFFER_SIZE 50;
char *itoa(int num)
{
	static char buff[20] = {};
	int i = 0, temp_num = num, length = 0;
	char *string = buff;
	
	if(num >= 0) {
		// count how many characters in the number
		while(temp_num) {
			temp_num /= 10;
			length++;
		}
		
		// assign the number to the buffer starting at the end of the 
		// number and going to the begining since we are doing the
		// integer to character conversion on the last number in the
		// sequence
		for(i = 0; i < length; i++) {
		 	buff[(length-1)-i] = '0' + (num % 10);
			num /= 10;
		}
		buff[i] = '\0'; // can't forget the null byte to properly end our string
	}
	else
		return "Unsupported Number";
	
	return string;
}
	
void status_bar(PblTm* t, char* str_topbar, char* str_bottombar) {
	strcpy(str_topbar,"");
	strcat(str_topbar,DAYS[t->tm_wday]);
	strcat(str_topbar," | ");
	strcat(str_topbar,itoa(t->tm_mday));
	strcat(str_topbar,". ");	
	strcat(str_topbar,MONTHS[t->tm_mon]);
  	string_format_time(str_bottombar, 50, " %H:%M | uge %V", t);
}

void fuzzy_time(int hours, int minutes, char* line1, char* line2, char* line3) {

  strcpy(line1, "");
  strcpy(line2, "");
  strcpy(line3, "");

  if (minutes > 0 && minutes < 3) {
    strcat(line1,STR_JUST);
  }
  else if ((minutes >= 3 && minutes < 8) || (minutes >= 54 && minutes < 58)) {
    strcat(line1,ONETEENS[5]); //Five
  }
  else if ((minutes >= 8 && minutes < 13) || (minutes >= 48 && minutes < 54)) {
    strcat(line1,ONETEENS[10]); //Ten
  }
  else if ((minutes >= 13 && minutes < 18) || (minutes >= 43 && minutes < 48)) {
    strcat(line1,STR_QUARTER); //Quarter
  }
  else if ((minutes >= 18 && minutes < 23) || (minutes >= 37 && minutes < 43)) {
    strcat(line1,TWENS[0]); //Twenty
  }
  else if ((minutes >= 23 && minutes < 28) || (minutes >= 33 && minutes < 37)) {
    strcat(line1,ONETEENS[5]); //Five
  }
  else if (minutes >= 28 && minutes < 33) {
    strcat(line2,STR_HALF); //Half
  }
  else if (minutes >=58 && minutes < 60) {
    strcat(line2,STR_ALMOST);
  }

  if(minutes >= 23) {
		hours+=1;
  }
  if (hours == 24) hours = 0;
  if (hours > 12) hours -= 12;

  if(minutes == 0){ //whole hours
    strcat(line2,STR_ITS);
    if(hours == 0) {
      strcat(line3,STR_MIDNIGHT);
    }
    else if (hours == 12) {
      strcat(line3, STR_NOON);
    }
    else {
      strcat(line3, ONETEENS[hours]);
    }
  } 
  else { 
    if(minutes >=1 && minutes < 23) { //before half
      strcat(line2,STR_PAST);
    }
	else if(minutes >= 23 && minutes<28) {
		strcat(line2, STR_TO);
		strcat(line2, " ");
		strcat(line2, STR_HALF);
	}
	else if(minutes >= 33 && minutes<37) {
		strcat(line2, STR_PAST);
		strcat(line2, " ");
		strcat(line2, STR_HALF);
	}
    else if (minutes >35 && minutes<58) { //after half     
	 	strcat(line2,STR_TO); 
	}
  }
	  
    if(hours == 0 && minutes>0) {
      strcat(line3,STR_MIDNIGHT);
    }
    else if (hours == 12 && minutes>0) {
      strcat(line3, STR_NOON);
    }
    else if (minutes>0) {
      strcat(line3, ONETEENS[hours]);
    }

  }
  
