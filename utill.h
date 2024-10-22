#include <stdio.h> 
#include <stdlib.h> 
#include<time.h>
#define CREATE 1
#define UPDATE 2
#define GET_ACC 3
#define HIST 4
#define Deposit 1
#define Withdraw 2
#define Balance_Enquiry 3
#define Password_Change 4
#define View_details 5
#define Exit 6
#define ADD 1
#define DELETE 2
#define MODIFY 3
#define SEARCH 4
#define ADMIN_EXIT 5
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
	char user_type;//n-normal user,a-admin user
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
//	char *result_msg;
};
int addAccount(struct account_info account);
int openAccount(struct account_info account){
	int result1=addAccount(account);
	return (result1);
}
void printAccountDetails(struct account_info account){
	printf("**************** %s ******************\n",account.user.name_1);
	printf("Account_No\t :: %lld\nUser_Id\t\t :: %lld\nPassword\t :: %s\nEmail\t\t :: %s\n",account.account_no,account.user.u_id,account.user.password,account.user.email);
	if(account.account_type=='j'){printf("Name\t\t :: %s\nName\t\t :: %s\n",account.user.name_1,account.user.name_2);}
	else{printf("Name\t\t :: %s\n",account.user.name_1);}
	printf("City \t\t :: %s\nState \t\t :: %s\nCountry \t :: %s\nMobile no \t :: %lld\nPin code \t :: %lld\nBalance \t :: %lf\nStatus \t\t :: %d\n",account.user.city,account.user.state,account.user.country,account.user.mobile_no,account.user.pin_code,account.balance,account.status);
	

}
int addAccount(struct account_info account){
	int fd;
	fd=open("account",O_WRONLY|O_APPEND);
	//printf("opening db status %d \n",fd);
	ssize_t s=write(fd,&account,sizeof(account));
	close(fd);
	if(s>0){printf("Write Success\n");return 1;}
	else return -1;
	}


// long long int generateAccountNo(){
// 	srand(time(0));
//  	long long int random=rand()%10000000;
// 	return random;
// }
void print_all(){
	int fd=fd=open("account",O_RDONLY);
	struct account_info account;
	while(read(fd,&account,sizeof(account))){
		printAccountDetails(account);
	}
	close(fd);
}
struct account_info searchByAccountNo(long long int account_no){	
	int fd=fd=open("account",O_RDONLY);
	struct account_info account={0},acc={0};
	while(read(fd,&account,sizeof(account))){
		if(account.account_no==account_no&&account.status>0){
		acc=account;
		break;}
	}
	close(fd);
	return acc;
}
int getRecordNoByAccountNo(long long int account_no){	
	int fd=open("account",O_RDONLY);
	struct account_info account={0},acc={0};
	int record_no=-1,i=-1;
	while(read(fd,&account,sizeof(account))){
		i++;
    printf("%lld,    %lld\n",account.account_no, account_no);
		if(account.account_no==account_no){
		//acc=account;
		record_no=i;
		break;}
	}
	close(fd);
  printf("Record No: %d", record_no);
	return record_no;
}
int updateRecord(struct account_info account,int record_no){
	int fd;
	fd=open("account",O_RDWR);
	struct flock lock;
	size_t s=0;
	off_t off=lseek(fd,(record_no*sizeof(account)),SEEK_SET);

	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_CUR;
	lock.l_start=0;
	lock.l_len=sizeof(account);
	lock.l_pid=getpid();
	printf("File Discripter value :: %d, Process id %ld\n",fd,(long)getpid());
	printf("Before Entering To Critical Section\n");
	int i=fcntl(fd,F_SETLKW,&lock);
  lseek(fd, (record_no * sizeof(account)), SEEK_SET);
  struct account_info existing_account;
  ssize_t read_bytes = read(fd, &existing_account, sizeof(existing_account));
  lseek(fd, (record_no * sizeof(account)), SEEK_SET);

  if(i!=-1){
		printf("Locking Status :: %d\n",i);
		printf("Entering Critical Section\n");

    
		s=write(fd,&account,sizeof(struct account_info));
    lseek(fd,(record_no*sizeof(account)),SEEK_SET);

		lock.l_type=F_UNLCK;
		int u=fcntl(fd,F_SETLK,&lock);
		printf("Exiting CS... Unlocking Status :: %d\n",u);
    int fd2 = open("passbook", O_CREAT|O_RDWR|O_APPEND,0666);
    printf("*********fd2************: %d\n", fd2);
    double init = existing_account.balance;
    double amt = account.balance;
    printf("amount %lf, init: %lf\n", amt, init);
    struct hist h;
    h.account_no = account.account_no;

    h.balance = amt;
    if(amt !=  init)
    {
      if(amt > init){
      h.t_type = 'd';
      h.amount = amt - init;
      size_t i = write(fd2, &h, sizeof(struct hist));
      if(i < 0){perror("Write not Done to passbook!!");}
    }
    else
    {
      h.t_type = 'w';
      h.amount = init - amt;
      int i = write(fd2, &h, sizeof(struct hist));
      if(i < 0){perror("Write not Done to passbook!!");}
    }}
    close(fd2);

		/*CS End*/
		}else{
		printf("Lock Not Accquired");
		}
		close(fd);
		return s;
}
// struct account_info getDetails(){
// 		struct account_info account;
// 		printf("****************** Enter User Details ****************");
// 		account.account_no=generateAccountNo();
// 		account.status=1;//active
// 		struct user_info user;
// 		 while(1){
//                  printf("Enter Account Type\nc - Current Account\ns - Saving Account\nj - Joint Account\n");
//                  scanf("%c",&account.account_type);
//                  if(account.account_type=='c'||account.account_type=='s'||account.account_type=='j')break;
//                  }
// 		if(account.account_type=='j'){
// 		printf("Enter User Name\n");
// 		scanf(" %20[^\n]",user.name_1);
// 		printf("Enter Second User Name\n");
// 		scanf(" %20[^\n]",user.name_2);
// 		}else{
// 		printf("Enter User Name\n");
// 		scanf(" %20[^\n]",user.name_1);
// 		}
// 		user.u_id=account.account_no;
// 		printf("Enter Password\n");
// 		scanf(" %10[^\n]",user.password);
// 		printf("Enter City\n");
// 		scanf(" %10[^\n]",user.city);
// 		printf("Enter State\n");
// 		scanf(" %10[^\n]",user.state);
// 		printf("Enter Country\n");
// 		scanf(" %10[^\n]",user.country);
// 		printf("Enter Pin Code\n");
// 		scanf("%lld",&user.pin_code);
// 		printf("Enter Mobile No\n");
// 		scanf("%lld",&user.mobile_no);
// 		printf("Enter Email Id\n");
// 		scanf(" %50[^\n]",user.email);
// 		while(1){
// 		printf("Enter User Type\na - Admin\nn - Normal User\n");
// 		scanf("%c",&user.user_type);
// 		if(user.user_type=='a'||user.user_type=='n')break;
// 		}
// 		account.user=user;
// 		return account;
// }
struct account_info getRecord(int record_no){
	int fd;
	struct account_info account;
	fd=open("account",O_RDONLY);
	struct flock lock;
	size_t s=0;
	off_t off=lseek(fd,(record_no*sizeof(account)),SEEK_SET);
	lock.l_type=F_RDLCK;
	lock.l_whence=SEEK_CUR;
	lock.l_start=0;
	lock.l_len=sizeof(account);
	lock.l_pid=getpid();
	printf("File Discripter value :: %d, Process id %ld\n",fd,(long)getpid());
	printf("Before Entering To Critical Section\n");
	int i=fcntl(fd,F_SETLKW,&lock);
	if(i!=-1){
		printf("Locking Status :: %d\n",i);
		printf("Entering Critical Section\n");
	
    s=read(fd,&account,sizeof(struct account_info));
    lock.l_type=F_UNLCK;
		int u=fcntl(fd,F_SETLK,&lock);
		printf("Exiting CS... Unlocking Status :: %d\n",u);
		/*CS End*/
		}else{
		printf("Lock Not Accquired");
		}
		close(fd);
		return account;
}

int fetchTransactionHistory(struct data_packet response,struct data_packet request)
{
  int fd = open("passbook", O_RDONLY);
  struct hist temp;
  int i = 0;
  while(read(fd, &temp,sizeof(struct hist)))
  {
       if(i<10)
    {
        printf("%lld\t%lld\n", temp.account_no, request.account.account_no);

      if((temp.account_no - request.account.account_no) == 0)
      {response.passbook_entries[i] = temp;
        printf("ACC: %lld",temp.account_no);
        printf("Balance: %lf", temp.balance);
      // {response.passbook_entries[i].account_no = temp.account_no;
      // response.passbook_entries[i].balance = temp.balance;
      // response.passbook_entries[i].t_type = temp.t_type;
      // response.passbook_entries[i].amount = temp.amount;}
      i++;
    }
    }
    else
    {
      break;
    }
  }
  return 1;
}

// int createNewAccount(){
//
// 		struct account_info account=getDetails();
// 		int status=openAccount(account);
// 		if(status>0){
// 			printf("Account Created ... Account details are\n");
// 			printAccountDetails(account);
// 		}else{
// 			printf("Sorry :: Account Creation Fail..\n");
// 		}
//   return 1;
// }
//
// struct account_info getNewAccountDetails(){
//
// 		struct account_info account={0};
// 		account=getDetails();
// 		return account;
// }
// int createAdmin(){
// 		struct account_info account=getDetails();
// 		int status=updateRecord(account,0);
// 		if(status>0){
// 			printf("Account Created ... Account details are\n");
// 			printAccountDetails(account);
// 		}else{
// 			printf("Sorry :: Account Creation Fail..\n");
// 		}			
// }
