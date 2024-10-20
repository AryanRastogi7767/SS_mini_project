#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

struct user_info{
	char name_1[20];
	char name_2[20];
	long long int u_id;
	char password[10];
	char city[10];
	char state[10];
	char country[10];
	long long int pin_code;
	long long int mobile_no;
	char email[50];
	char user_type;//n-normal user,a-admin user
};


struct account_info{
	long long int account_no;
	double balance;
	char account_type;//c-current,s-saveing,o-other
	int s_no;
	char feedback[100];
  struct user_info user;
	int status;
};

void printAccountDetails(struct account_info account){
	printf("**************** %s ******************\n",account.user.name_1);
	printf("Account_No\t :: %lld\nUser_Id\t\t :: %lld\nPassword\t :: %s\nEmail\t\t :: %s\n",account.account_no,account.user.u_id,account.user.password,account.user.email);
	if(account.account_type=='j'){printf("Name\t\t :: %s\nName\t\t :: %s\n",account.user.name_1,account.user.name_2);}
	else{printf("Name\t\t :: %s\n",account.user.name_1);}
	printf("City \t\t :: %s\nState \t\t :: %s\nCountry \t :: %s\nMobile no \t :: %lld\nPin code \t :: %lld\nSequence No \t :: %d\nBalance \t :: %lf\nStatus \t\t :: %d\n",account.user.city,account.user.state,account.user.country,account.user.mobile_no,account.user.pin_code,account.s_no,account.balance,account.status);
	

}
int main(void)
{
  
int fd=open("account",O_RDONLY);
	struct account_info account;
	while(read(fd,&account,sizeof(account))){
		printAccountDetails(account);
	}
	close(fd);
  return 0;
}
