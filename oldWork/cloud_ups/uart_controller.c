/*
FileName:
Author:
Description:
*/

#include "uart_controller.h"

int fd1,nset,nread,ret;
//char buf[15]={"power off\n"};
//char buf1[100];

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
	 struct termios newtio,oldtio;
	 if  ( tcgetattr( fd,&oldtio)  !=  0) {
	  perror("SetupSerial 1");
	  return -1;
	 }
	 bzero( &newtio, sizeof( newtio ) );
	 newtio.c_cflag  |=  CLOCAL | CREAD; //CLOCAL:忽略modem控制线  CREAD：打开接受者
	 newtio.c_cflag &= ~CSIZE; //字符长度掩码。取值为：CS5，CS6，CS7或CS8

	 switch( nBits )
	 {
	 case 7:
	  newtio.c_cflag |= CS7;
	  break;
	 case 8:
	  newtio.c_cflag |= CS8;
	  break;
	 }

	 switch( nEvent )
	 {
	 case 'O':
	  newtio.c_cflag |= PARENB; //允许输出产生奇偶信息以及输入到奇偶校验
	  newtio.c_cflag |= PARODD;  //输入和输出是奇及校验
	  newtio.c_iflag |= (INPCK | ISTRIP); // INPACK:启用输入奇偶检测；ISTRIP：去掉第八位
	  break;
	 case 'E':
	  newtio.c_iflag |= (INPCK | ISTRIP);
	  newtio.c_cflag |= PARENB;
	  newtio.c_cflag &= ~PARODD;
	  break;
	 case 'N': 
	  newtio.c_cflag &= ~PARENB;
	  break;
	 }

	 switch( nSpeed )
	 {
	 case 2400:
	  cfsetispeed(&newtio, B2400);
	  cfsetospeed(&newtio, B2400);
	  break;
	 case 4800:
	  cfsetispeed(&newtio, B4800);
	  cfsetospeed(&newtio, B4800);
	  break;
	 case 9600:
	  cfsetispeed(&newtio, B9600);
	  cfsetospeed(&newtio, B9600);
	  break;
	 case 115200:
	  cfsetispeed(&newtio, B115200);
	  cfsetospeed(&newtio, B115200);
	  break;
	 case 460800:
	  cfsetispeed(&newtio, B460800);
	  cfsetospeed(&newtio, B460800);
	  break;
	 default:
	  cfsetispeed(&newtio, B9600);
	  cfsetospeed(&newtio, B9600);
	  break;
	 }

	 if( nStop == 1 )
	  newtio.c_cflag &=  ~CSTOPB; //CSTOPB:设置两个停止位，而不是一个
	 else if ( nStop == 2 )
	 newtio.c_cflag |=  CSTOPB;
	 
	 newtio.c_cc[VTIME]  = 0; //VTIME:非cannoical模式读时的延时，以十分之一秒位单位
	 newtio.c_cc[VMIN] = 0; //VMIN:非canonical模式读到最小字符数
	 tcflush(fd,TCIFLUSH); // 改变在所有写入 fd 引用的对象的输出都被传输后生效，所有已接受但未读入的输入都在改变发生前丢弃。
	 if((tcsetattr(fd,TCSANOW,&newtio))!=0) //TCSANOW:改变立即发生
	 {
	  perror("com set error");
	  return -1;
	 }
	 printf("set done!\n\r");
	 return 0;
}


int uart_init(void)
{

	 fd1 = open( "/dev/ttySAC0", O_RDWR|O_SYNC);
	 if (fd1 == -1)
	  exit(1);
	 printf("open  SAC0 success!!\n");

	 nset = set_opt(fd1, 115200, 8, 'N', 1);
	 if (nset == -1)
	  exit(1);
	 printf("SET  SAC0 success!!\n");
	 printf("enter the loop!!\n");

	
 return 1;
}


int do_uart_alarm(char* buf)
{
    	printf("<<<%s\n", __func__);

	 
       //memset(buf1, 0, sizeof(buf1));
	   ret = write(fd1, buf, strlen(buf));
	   if( ret > 0){
	      printf("write success!  wait data receive\n");
	   }else if(ret < 0)
       {
        perror("uart alarm error");
       }

/*	   
	nread = read(fd1, buf1, 100);
	   if(nread > 0){
		printf("redatad: nread = %s\n\n\r", buf1);
	   }
*/
	   sleep(1);
	  //nread = read(fd1, buf1,1);
	  //if(buf1[0] == 'q')
	   //break;
	
 	//close(fd1);

    return ret;

}

void do_uart_alarm_exit(void)
{
    close(fd1);
}
