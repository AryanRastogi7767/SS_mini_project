#include <stdio.h> 
#include <stdlib.h> 
#include<time.h>

#define Deposit 1
#define Withdraw 2
#define Balance_Enquiry 3
#define Password_Change 4
#define View_details 5
#define Transfer 6
#define Apply_loan 7
#define Add_Feedback 8
#define Transaction_History 9
#define Exit 10
#define ADD 1
#define DELETE 2
#define MODIFY 3
#define SEARCH 4
#define ADMIN_EXIT 5
#define CREATE 1
#define UPDATE 2
#define GET_ACC 3
#define HIST 4
#define ACTDCT 1
#define REVIEW 2
#define PASS 3
#define MAN_EXIT 4
#define ADD_CUST 1
#define EMP_MOD 2
#define VIEW_LOANS 3
#define PROCESS_LOANS 4
#define EMP_EXIT 5
long long int generateAccountNo();
struct hist
{
  long long int account_no;
  double balance;
  char t_type; //d: deposit w: withdrawl
  double amount;
};
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
	char user_type;//n-normal user,a-admin user, e-employee, m-manager
};
struct account_info{
	long long int account_no;
	double balance;
	char account_type;//c-current,s-saveing,o-other
	//int s_no;
  char feedback[100];
	struct user_info user;
	int status;
  int login;
};
struct data_packet{
	int action;
	struct account_info account;
  struct hist passbook_entries[10];
	int result;
};
void printAccountDetails(struct account_info account){
	printf("**************** %s ******************\n",account.user.name_1);
	printf("Account_No\t :: %lld\nUser_Id\t\t :: %lld\nPassword\t :: %s\nEmail\t\t :: %s\n",account.account_no,account.user.u_id,account.user.password,account.user.email);
	if(account.account_type=='j'){printf("Name\t\t :: %s\nName\t\t :: %s\n",account.user.name_1,account.user.name_2);}
	else{printf("Name\t\t :: %s\n",account.user.name_1);}
	printf("City \t\t :: %s\nState \t\t :: %s\nCountry \t :: %s\nMobile no \t :: %lld\nPin code \t :: %lld\nBalance \t :: %lf\nStatus \t\t :: %d\n",account.user.city,account.user.state,account.user.country,account.user.mobile_no,account.user.pin_code,account.balance,account.status);
	

}
struct account_info getDetails(char user_type){
		struct account_info account;
		printf("****************** Enter User Details ****************");
		account.account_no=generateAccountNo();
		account.status=1;//active
		struct user_info user;
		 while(1){
                 printf("Enter Account Type\nc - Current Account\ns - Saving Account\nj - Joint Account\n");
                 scanf("%c",&account.account_type);
                 if(account.account_type=='c'||account.account_type=='s'||account.account_type=='j')break;
                 }
		if(account.account_type=='j'){
		printf("Enter User Name\n");
		scanf(" %20[^\n]",user.name_1);
		printf("Enter Second User Name\n");
		scanf(" %20[^\n]",user.name_2);
		}else{
		printf("Enter User Name\n");
		scanf(" %20[^\n]",user.name_1);
		}
		user.u_id=account.account_no;
		printf("Enter Password\n");
		scanf(" %10[^\n]",user.password);
		printf("Enter City\n");
		scanf(" %10[^\n]",user.city);
		printf("Enter State\n");
		scanf(" %10[^\n]",user.state);
		printf("Enter Country\n");
		scanf(" %10[^\n]",user.country);
		printf("Enter Pin Code\n");
		scanf("%lld",&user.pin_code);
		printf("Enter Mobile No\n");
		scanf("%lld",&user.mobile_no);
		printf("Enter Email Id\n");
		scanf(" %50[^\n]",user.email);
		while(1){
    if(user_type == 'e'){user.user_type = 'c'; break;}
    else{
		printf("Enter User Type\na - Admin\nn - Normal User\nm - manager\ne - employee\n");
		scanf("%c",&user.user_type);
		if(user.user_type=='a'||user.user_type=='n')break;}
		}
		account.user=user;
		return account;
}
long long int generateAccountNo(){
	srand(time(0));
 	long long int random=rand()%10000000;
	return random;
}
struct account_info getNewAccountDetails(char user_type){

		struct account_info account={0};
		account=getDetails(user_type);
		return account;
}
