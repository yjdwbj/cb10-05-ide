
extern void testfun1() ;
extern int test1(int a,int b) ;
int pow() __asm("ex8_start");
/**/ 
void main()
{
	int a;
	int b;
	int c ;
	a = 0x1000 ;
	b = 0x200 ;
	c = test1(a,b);
pow() ;
/*
  printf("%x",c) ;
	*/
 	
	
}
int test1(int a,int b)
{
	a=a*10 ;
	a=a-0x100 ;
	/*	
	 testfun1() ;
*/
return a*b ;	
}