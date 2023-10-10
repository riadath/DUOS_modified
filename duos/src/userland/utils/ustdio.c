#include <ustdio.h>
#include <unistd.h>
#include <ustring.h>
#include <stdint.h>
void firstTest(char* args)
{
    ok(args);
}

void uprintf(char *format,...)
{
    //Copied from Kprintf implementation given in the starter code
    char *tr;
	uint32_t i;
	uint8_t *str;
	va_list list;
	double dval;

    char ans[256];
    int ptr = 0;
	//uint32_t *intval;
	va_start(list,format);
	for(tr = format;*tr != '\0';tr++)
	{
		while(*tr != '%' && *tr!='\0')
		{
            ans[ptr] = *tr;
            ptr++;
		    tr++;
		}
		if(*tr == '\0') break;
		tr++;
		switch (*tr)
		{
            case 'c':
                //Cast character into int
                i = va_arg(list,int);
                //Assign it to our built string
                ans[ptr] = i;
                ptr++;
                break;
            case 'd': i = va_arg(list,int);
                if(i<0)
                {
                    //negative value
                    ans[ptr] = '-';
                    ptr++;
                    i=-i;				
                }
                //Convert to base 10
                //cast it into character array
                uint8_t* cur =  convert(i,10);
                int temp_ptr = 0;
                while(cur[temp_ptr])
                {
                    ans[ptr] = cur[temp_ptr];
                    ptr++;
                    temp_ptr++;
                }
                break;
            case 'o': 
                i = va_arg(list,int);
                //Same code copy-paste
                if(i<0)
                {
                    //negative value
                    ans[ptr] = '-';
                    ptr++;
                    i=-i;				
                }
                //Convert to base 8
                //cast it into character array
                cur =  convert(i,8);
                temp_ptr = 0;
                while(cur[temp_ptr])
                {
                    ans[ptr] = cur[temp_ptr];
                    ptr++;
                    temp_ptr++;
                }
                break;
            case 'x': 
                i = va_arg(list,int);
                if(i<0)
                {
                    //negative value
                    ans[ptr] = '-';
                    ptr++;
                    i=-i;				
                }
                //Convert to base 16
                //cast it into character array
                cur =  convert(i,16);
                temp_ptr = 0;
                while(cur[temp_ptr])
                {
                    ans[ptr] = cur[temp_ptr];
                    ptr++;
                    temp_ptr++;
                }
                break;
            case 's': 
                //Direct assigning for string
                str = va_arg(list,uint8_t*);
                cur = str;
                temp_ptr = 0;
                while(cur[temp_ptr])
                {
                    ans[ptr] = cur[temp_ptr];
                    ptr++;
                    temp_ptr++;
                }
                break;
            case 'f': 
                dval = va_arg(list,double);
                cur = (uint8_t*)float2str(dval);
                temp_ptr = 0;
                while(cur[temp_ptr] != 0)
                {
                    ans[ptr] = cur[temp_ptr];
                    ptr++;
                    temp_ptr++;
                }
                break;	
            default:
                break;
		}
	}
    ans[ptr]=0;
    

	va_end(list);
    char *return_param = ans;
    write(return_param);
}

void uscanf(char *format, ...)
{
    // Code copied from kScanf from starter code
    char *toRead;
    va_list list;
	char *ptr;
	uint8_t buff[50];
	ptr=format;
	va_start(list,format);
	while (*ptr)
	{
		/* code */
		if(*ptr == '%') //looking for format of an input
		{
            //ensures toRead will have our string
            read(&toRead);
			ptr++;
			switch (*ptr)
			{
                case 'c': //charater
                    //First character of the read string is our character to be assigned
                    *(uint8_t*)va_arg(list,uint8_t*)=toRead[0];
                    break;
                case 'd': //integer number 
                    //Converting the string to decimal
                    *(uint32_t*)va_arg(list,uint32_t*)=__str_to_num(toRead,10);	
                    break;
                case 's': //need to update -- string
                    
                    *(uint32_t*)va_arg(list,uint32_t*)= toRead;
                    break;
                case 'x': //hexadecimal number
                    *(uint32_t*)va_arg(list,uint32_t*)=__str_to_num(toRead,16);		
                    break;	
                case 'o': //octal number
                    *(uint32_t*)va_arg(list,uint32_t*)=__str_to_num(toRead,8);		
                    break;	
                case 'f': //floating point number
                    *(uint32_t*)va_arg(list,float*)=str2float(toRead);
                    break;	
                default: //rest not recognized
                    break;
			}
		}
		ptr++;
	}
	va_end(list);
}