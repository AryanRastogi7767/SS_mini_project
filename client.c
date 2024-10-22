#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include<string.h>
#include"client_utill.h"
struct data{
  int action;
  int result;
//  char *result_msg;
  };

struct data_packet execute(struct data_packet packet);
int main(){
	struct data_packet request,response={0};
	int z=0;
	char c;

	printf("\n\n");
	printf("\n\n\t\t\t\tCLIENT LOGIN\n\n");
	printf("ENTER ACCOUNT NO: \t");
	scanf("%lld",&request.account.account_no);
	printf("ENTER PASSWORD: \t");
	scanf(" %10[^\n]",request.account.user.password);
//	printf("%lld,%s\n",request.account.account_no,request.account.user.password);
	request.action=GET_ACC;
	request.result=-1;
	response=execute(request);
//	printf("Response Status :: %d\n",response.result);
  
//  printf("Request acc num: %lld\n", request.account.account_no);

 // printf("Response acc num: %lld\n", response.account.account_no);
  if(response.account.login == 0){
	if(request.account.account_no==response.account.account_no){
	if(strcmp(request.account.user.password,response.account.user.password) == 0){
		response.account.login = 1;  // Mark the user as logged in
    response.action = UPDATE;  // Update the account information
    response.result = -1;
    response = execute(response);  // Save the update
    printf("Login successful.\n");
        //login success
		printf("LOGIN SUCCESS : Account Details Are\n");
		printAccountDetails(response.account);
		if(response.account.user.user_type=='n'){
		//normal user
			printf("Weclome User :: %s\n",response.account.user.name_1);
			int choice,ch;
			do{

			printf("1. Deposit\n");
			printf("2. Withdraw\n");
			printf("3. Balance Enquiry\n");
			printf("4. Password Change\n");
			printf("5. View details\n");
      printf("6. Transfer Funds\n");
      printf("7. Apply for loan\n");
      printf("8. Add Feedback\n");
      printf("9. View Transaction History\n");
			printf("10.Exit\n");
		  printf("Enter Choice: ");
			scanf("%d",&choice);
      printf("\n");
			switch(choice){
			case Deposit:{
				double amount;
				printf("Enter Deposit Amount\n");
				scanf("%lf",&amount);
				if(amount>0){
					struct data_packet a_request,a_response={0};
					a_request.account.account_no=response.account.account_no;
					a_request.action=GET_ACC;
					a_request.result=-1;
					a_response=execute(a_request);
					if(a_response.result>0&&a_response.account.status>0){
						a_response.account.balance=a_response.account.balance+amount;
						a_response.action=UPDATE;
						a_response.result=-1;
						response=execute(a_response);
						printAccountDetails(response.account);
						if(response.result>0){printf("Transaction Success\n");}
					  else{printf("Sorry !! Transaction Fail\n");}
					}else{printf("Sorry !! Unable To Fetch Account Details To Deposit.\n");}
				}else{printf("Sorry !! Invalid Amount\n");}
				break;}

			case Withdraw:{
				double amount;
				printf("Enter Withdraw Amount\n");
				scanf("%lf",&amount);
				struct data_packet a_request,a_response={0};
				a_request.account.account_no=response.account.account_no;
				a_request.action=GET_ACC;
				a_request.result=-1;
				a_response=execute(a_request);
				if(a_response.result>0&&a_response.account.status>0){
					response=a_response;
					if(response.account.balance>=amount){
					response.account.balance=response.account.balance-amount;
					response.action=UPDATE;
					response.result=-1;
					response=execute(response);
					if(response.result>0){printf("Transaction Success\n");}
					else{printf("Sorry !! Transaction Fail\n");}
					}
					else{printf("Sorry !! Not Enough Balance\n");}
					}else{printf("Sorry !! Unable To Fetch Account Details To Withdraw.\n");}
				break;}
			case Balance_Enquiry:{
				response.action=GET_ACC;
				response.result=-1;
				response=execute(response);
				if(response.result>0){printf("Transaction Success\n");
				printf("Available Balance Is INR %lf\n",response.account.balance);
				}
				else{printf("Sorry !! Transaction Fail\n");}
				break;}
			case Password_Change:{
				char password[10];
				printf("Enter New Password\n");
				scanf(" %10[^\n]",response.account.user.password);
				printf("Confirm Password\n");
				scanf(" %10[^\n]",password);
				if(strcmp(password,response.account.user.password) == 0){
				response.action=UPDATE;
				response.result=-1;
				response=execute(response);
				if(response.result>0){printf("Transaction Success !! Password Successfully Changed\n");}
				else{printf("Sorry !! Transaction Fail. Something Going Wrong\n");}
				}else{printf("Sorry !! Password Not Match\n");}
				break;}
			case View_details:{
				response.action=GET_ACC;
				response.result=-1;
				response=execute(response);
				printAccountDetails(response.account);
				break;}
      case Transfer:{
        double amount;
        long long acc_num;

        printf("Enter Account Number of Recipient: \n");
				scanf("%lld", &acc_num);
        printf("Enter Transfer Amount: \n");
        scanf("%lf", &amount);

        if (amount <= 0) {
        printf("Invalid transfer amount.\n");
        }
    
        // Fetch both accounts
        struct data_packet from_request = {0}, to_request = {0};
        from_request.account.account_no = response.account.account_no;
        from_request.action = GET_ACC;
        from_request.result = -1;

        to_request.account.account_no = acc_num;
        to_request.action = GET_ACC;
        to_request.result = -1;

        struct data_packet from_response = execute(from_request);
        struct data_packet to_response = execute(to_request);

        // Check if both accounts exist and have sufficient balance
        if (from_response.result > 0 && to_response.result > 0 &&
            from_response.account.status > 0 && to_response.account.status > 0) {

            if (from_response.account.balance >= amount) {
                // Update balances
                from_response.account.balance -= amount;
                to_response.account.balance += amount;

                // Update both accounts
                from_response.action = UPDATE;
                from_response.result = -1;
                to_response.action = UPDATE;
                to_response.result = -1;

                struct data_packet from_update = execute(from_response);
                struct data_packet to_update = execute(to_response);

                if (from_update.result > 0 && to_update.result > 0) {
                    printf("Transfer successful. Transferred INR %.2f to account %lld\n", amount, to_response.account.account_no);} 
                else {
                    printf("Transfer failed. Could not update account balances.\n");}
            }
            else {
                printf("Insufficient balance in source account.\n");
            }
        } else {
            printf("Error: One or both accounts not found or inactive.\n");}
      break;}

      case Apply_loan:{

            break;}
      case Add_Feedback:{
			    
        // Submit Feedback
        response.action = GET_ACC;  // Assuming UPDATE is used for actions that modify account info
        response.result = -1;
        response=execute(response);

        printf("Enter your feedback (max 100 characters):\n");
        scanf(" %100[^\n]", response.account.feedback);  // Store feedback in the data packet
        

        response.action = UPDATE;
        response.result = -1;
        response = execute(response);
        // Use the execute function to handle communication with the server
        if (response.result > 0) {
            printf("Feedback submitted successfully.\n");
        } else {
            printf("Failed to submit feedback.\n");}
         break;}
      case Transaction_History:{
        response.action=HIST;
				response.result=-1;
				response=execute(response);
				if(response.result>0){printf("Transaction History Fetch Success\n");
          struct hist temp;
          printf("Acc Num\tBalance\tTransaction Type\tAmount\t\n");
          
				  for(int i=0;i<10;i++)
          {
            temp = response.passbook_entries[i];
            printf("%lld\t%lf\t%c\t%lf\t\n",temp.account_no,temp.balance,temp.t_type,temp.amount);
          } 
				}
				else{printf("Sorry !! Transaction History Fetch Failed\n");}

            break;}
			case Exit:{
        response.account.login = 0;  // Mark the user as logged out
        response.action = UPDATE;  // Update the account information
        response.result = -1;
        response = execute(response);  // Save the update
        if(response.result > 0)printf("Logout successful.\n");
				exit(0);
				break;}

			default:{printf("Invalid Choice :: %d\n",choice);}
			}
			printf("Do You Want To Continue (Press 1)\n");
			scanf("%d",&ch);
      if(ch != 1)
      {
      response.account.login = 0;  // Mark the user as logged out
        response.action = UPDATE;  // Update the account information
        response.result = -1;
        response = execute(response);  // Save the update
        if(response.result > 0)printf("Logout successful.\n");

      }
			}while(ch==1);

		}else if(response.account.user.user_type=='a'){
		//admin
			int choice,ch;
			printf("Weclome Admin :: %s\n",response.account.user.name_1);
			do{
					printf("1.Add New Account\n");
			printf("2.Delete An Account\n");
			printf("3.Modify Details or Roles\n");
			printf("4.Search An Account\n");
			printf("5.Exit\n");
    	printf("Enter Choice: ");
			scanf("%d",&choice);
      printf("\n");
			switch(choice){
			case ADD:{
				struct account_info account=getNewAccountDetails(response.account.user.user_type);
				if(account.account_no>0){
				struct data_packet a_request,a_response={0};
				a_request.action=CREATE;
				a_request.result=-1;
				a_request.account=account;
				a_response=execute(a_request);
				if(response.result>0){printf("Transaction Success !! New Account Successfully Created\nDetails Are..\n");
				printAccountDetails(a_response.account);
				}
				else{printf("Sorry !! Transaction Fail. Account Creating Fail\n");}
				}
				else{printf("Sorry !! Account Creating Fail \n");}
				break;}
			case DELETE:{
				struct data_packet a_request,a_response={0};
				printf("Enter Account No To Delete\n");
				scanf("%lld",&a_request.account.account_no);
				a_request.action=GET_ACC;
				a_request.result=-1;
				a_response=execute(a_request);
				if(a_response.result>0&&a_response.account.status>0){
					a_response.account.status=0;
					a_request=a_response;
					a_request.action=UPDATE;
					a_request.result=-1;
					a_response=execute(a_request);
					if(a_response.result>0){
					printf("Transaction Success !! Account Deleted Successfully\n");
					}
					else{printf("Sorry !! Unable To Delete Account For Account No %lld\n",a_request.account.account_no);}
				}
				else{printf("Sorry !! Unable To Fatch Account For Account No %lld\n",a_request.account.account_no);}
				break;}
			case MODIFY:{
				int m_choice,m_ch;
				do{
					printf("Choose What Wants To Update\n");
					printf("1.Update Name\n");
					printf("2.City\n");
					printf("3.State\n");
					printf("4.Country\n");
					printf("5.Pin Code\n");
					printf("6.Mobile No\n");
					printf("7.Email\n");
          printf("8.Roles\n");
          printf("9.Password\n");
        	printf("Enter Choice: ");
					scanf("%d",&m_choice);
          printf("\n");
          struct data_packet a_request,a_response={0};
					printf("Enter Account No To Update Details:\n");
					scanf("%lld",&a_request.account.account_no);
					a_request.action=GET_ACC;
					a_request.result=-1;
					a_response=execute(a_request);

					switch(m_choice){
					case 1:{
							if(a_response.result>0&&a_response.account.status>0){
							if(a_response.account.account_type=='j'){
							printf("Enter User Name\n");
							scanf(" %20[^\n]",a_response.account.user.name_1);
							printf("Enter Second User Name\n");
							scanf(" %20[^\n]",a_response.account.user.name_2);
							}else{
							printf("Enter User Name\n");
							scanf(" %20[^\n]",a_response.account.user.name_1);
							}
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Name Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update Name For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}		
					break;}
					case 2:{
							if(a_response.result>0&&a_response.account.status>0){
							printf("Enter City\n");
							scanf(" %10[^\n]",a_response.account.user.city);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! City Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update City For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}							
					break;}
					case 3:{
						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter State\n");
							scanf(" %10[^\n]",a_response.account.user.state);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! State Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update State For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}		
				
					break;}
					case 4:{
						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter Country\n");
							scanf(" %10[^\n]",a_response.account.user.country);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Country Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update Country For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}		
				
					break;}
					case 5:{
						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter Pin Code\n");
							scanf("%lld",&a_response.account.user.pin_code);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Pin Code Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update Pin Code For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}		

					break;}
					case 6:{
						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter Mobile No\n");
							scanf("%lld",&a_response.account.user.mobile_no);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Mobile No Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update Mobile No For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}		
					
					break;}
					case 7:{
						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter Email Id\n");
							scanf(" %50[^\n]",a_response.account.user.email);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Email Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update Email For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}		
					break;}

          case 8:{
             if (a_response.result > 0 && a_response.account.status > 0) {
               // Display the current user role
             if (a_response.account.user.user_type == 'n') {
              printf("User is a customer.\n");
              } else if (a_response.account.user.user_type == 'm') {
              printf("User is a manager.\n");
              } else if (a_response.account.user.user_type == 'e') {
              printf("User is an employee.\n");
              } else {
              printf("User is an admin.\n");
        }

              // Now, ask for the new role and handle input correctly
              printf("Enter New Role (n for customer, m for manager, e for employee, a for admin): \n");
        
             // Use getchar() to properly handle single-character input
             a_response.account.user.user_type = getchar();
        
             a_request = a_response;
             a_request.action = UPDATE;
             a_request.result = -1;
             a_response = execute(a_request);

            if (a_response.result > 0) {
            printf("Transaction Success! User Role Updated Successfully\n");
           } else {
            printf("Sorry! Unable to Update User Role for Account No %lld\n", a_request.account.account_no);
           }
      } else {
        printf("Sorry! Unable to Fetch Account for Account No %lld\n", a_request.account.account_no);
    }
    break;
}
      case 9:{
          struct data_packet a_request,a_response={0};
						printf("Enter Account No To Update Password\n");
						scanf("%lld",&a_request.account.account_no);
						a_request.action=GET_ACC;
						a_request.result=-1;
						a_response=execute(a_request);
						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter New Password\n");
							scanf(" %50[^\n]",a_response.account.user.password);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Password Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update Password For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}		
					break;
                  }
					default:{printf("Invalid Choice :: %d\n",m_choice);}
					}
					printf("Do You Want To Continue (Press 1 Modify Menu)\n");
					scanf("%d",&m_ch);
					}while(m_ch==1);			
				break;}
			case SEARCH:{	
				struct data_packet a_request,a_response={0};
				printf("Enter Account No To Search\n");
				scanf("%lld",&a_request.account.account_no);
				a_request.action=GET_ACC;
				a_request.result=-1;
				a_response=execute(a_request);
				if(a_response.result>0&&a_response.account.account_no>0&&a_response.account.status>0){
				printf("Transaction Success !! Account Found\n");
				printAccountDetails(a_response.account);
				}
				else{printf("Sorry !! Unable To Fatch Account For Account No %lld\n",a_request.account.account_no);}
				break;}
			case ADMIN_EXIT:{
        response.account.login = 0;  // Mark the user as logged out
        response.action = UPDATE;  // Update the account information
        response.result = -1;
        response = execute(response);  // Save the update
        if(response.result > 0)printf("Logout successful.\n");

				exit(0);
				break;}
			default:{printf("Invalid Choice :: %d\n",choice);}
			}
			printf("Do You Want To Continue (Press 1)\n");
			scanf("%d",&ch);
      if(ch != 1){
        response.account.login = 0;  // Mark the user as logged out
        response.action = UPDATE;  // Update the account information
        response.result = -1;
        response = execute(response);  // Save the update
        if(response.result > 0)printf("Logout successful.\n");
}
			}while(ch==1);

    //manager
		}else if(response.account.user.user_type=='m'){
			int choice,ch;
			printf("Weclome Manager :: %s\n",response.account.user.name_1);
			do{
			printf("1.Activate / Deactivate Customer Accounts\n");
			printf("2.Review Customer Feedback\n");
			printf("3.Change Password\n");
			printf("4.Logout and Exit\n");
			printf("Enter Choice\n");
			scanf("%d",&choice);
      printf("\n");
			switch(choice){
			case ACTDCT:{
          struct data_packet a_request,a_response={0};
						printf("Enter Account No To Activate or Deactivate\n");
						scanf("%lld",&a_request.account.account_no);
						a_request.action=GET_ACC;
						a_request.result=-1;
						a_response=execute(a_request);
						if(a_response.result>0&&a_response.account.user.user_type != 'a'){
							int t;
              if(a_response.account.status == 0)
              {
                  printf("Account is deactivated, Do you want to activate it?(Press 1)\n");
                  scanf("%d",&t);
                  if(t == 1){a_response.account.status = 1;
                    printf("Account activated successfully!!\n");}
                  else{printf("Account not activated.\n");}
              }
              else
              {   printf("Account is active, Do you want to de-activate it?(Press 1)\n");
                  scanf("%d",&t);
                  if(t == 1){a_response.account.status = 0;
                    printf("Account de-activated successfully!!\n");}
                  else{printf("Account not de-activated.\n");}
              }
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Activation / Deactivation Successful\n");
							}
							else{printf("Sorry !! Unable To Activate / Deactivate Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}		
					break;
                  }
      case REVIEW:{
						struct data_packet a_request,a_response={0};
						printf("Enter Account No review feedback: \n");
						scanf("%lld",&a_request.account.account_no);
						a_request.action=GET_ACC;
						a_request.result=-1;
						a_response=execute(a_request);
						if(a_response.result>0&&a_response.account.status>0 && a_response.account.user.user_type == 'n'){
                printf("Customer Feedback: %s\n",a_response.account.feedback);
            }else{
                printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}		
					break;}
      case PASS:{
          struct data_packet a_request,a_response={0};
						printf("Enter Account No To Update Password\n");
						scanf("%lld",&a_request.account.account_no);
						a_request.action=GET_ACC;
						a_request.result=-1;
						a_response=execute(a_request);
						if(a_response.result>0&&a_response.account.status>0&&a_response.account.user.user_type == 'n'){
							printf("Enter New Password\n");
							scanf(" %50[^\n]",a_response.account.user.password);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Password Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update Password For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}		
					break;
                  }
			case MAN_EXIT:{
        response.account.login = 0;  // Mark the user as logged out
        response.action = UPDATE;  // Update the account information
        response.result = -1;
        response = execute(response);  // Save the update
        if(response.result > 0)printf("Logout successful.\n");

				exit(0);
				break;}
			default:{printf("Invalid Choice :: %d\n",choice);}
			}
			printf("Do You Want To Continue (Press 1)\n");
			scanf("%d",&ch);
      if(ch != 1){
        response.account.login = 0;  // Mark the user as logged out
        response.action = UPDATE;  // Update the account information
        response.result = -1;
        response = execute(response);  // Save the update
        if(response.result > 0)printf("Logout successful.\n");
        }
			}while(ch==1);}
      else if(response.account.user.user_type=='e'){
			int choice,ch;
			printf("Weclome Employee :: %s\n",response.account.user.name_1);
			do{
			printf("1.Add New Customer\n");
			printf("2.Modify Customer Details\n");
			printf("3.View Assigned Loan Applications\n");
      printf("4.Process Loan Applications\n");
			printf("5.Logout and Exit\n");
			printf("Enter Choice\n");
			scanf("%d",&choice);
      printf("\n");
			switch(choice){
			case ADD_CUST:{
 			struct account_info account=getNewAccountDetails(response.account.user.user_type);
				if(account.account_no>0){
				struct data_packet a_request,a_response={0};
				a_request.action=CREATE;
				a_request.result=-1;
				a_request.account=account;
				a_response=execute(a_request);
				if(response.result>0){printf("Transaction Success !! New Account Successfully Created\nDetails Are..\n");
				printAccountDetails(a_response.account);
				}
				else{printf("Sorry !! Transaction Fail. Account Creating Fail\n");}
				}
				else{printf("Sorry !! Account Creating Fail \n");}
				break; 
                  }
      case EMP_MOD:{
				int m_choice,m_ch;
				do{
					printf("Choose What Wants To Update\n");
					printf("1.Update Name\n");
					printf("2.City\n");
					printf("3.State\n");
					printf("4.Country\n");
					printf("5.Pin Code\n");
					printf("6.Mobile No\n");
					printf("7.Email\n");
          printf("8.Roles\n");
          printf("9.Password\n");
        	printf("Enter Choice: ");
					scanf("%d",&m_choice);
          printf("\n");
          struct data_packet a_request,a_response={0};
					printf("Enter Account No To Update Details\n");
					scanf("%lld",&a_request.account.account_no);
					a_request.action=GET_ACC;
					a_request.result=-1;
					a_response=execute(a_request);
          if(a_response.account.user.user_type == 'n'){
					switch(m_choice){
					case 1:{

						if(a_response.result>0&&a_response.account.status>0){
							if(a_response.account.account_type=='j'){
							printf("Enter User Name\n");
							scanf(" %20[^\n]",a_response.account.user.name_1);
							printf("Enter Second User Name\n");
							scanf(" %20[^\n]",a_response.account.user.name_2);
							}else{
							printf("Enter User Name\n");
							scanf(" %20[^\n]",a_response.account.user.name_1);
							}
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Name Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update Name For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}		
            //}else{printf("Employee cannot change admin's, manager's and other employees")}
					break;}
					case 2:{

						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter City\n");
							scanf(" %10[^\n]",a_response.account.user.city);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! City Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update City For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}							
					break;}
					case 3:{

						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter State\n");
							scanf(" %10[^\n]",a_response.account.user.state);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! State Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update State For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}		
				
					break;}
					case 4:{

						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter Country\n");
							scanf(" %10[^\n]",a_response.account.user.country);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Country Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update Country For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}		
				
					break;}
					case 5:{
	
						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter Pin Code\n");
							scanf("%lld",&a_response.account.user.pin_code);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Pin Code Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update Pin Code For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}		

					break;}
					case 6:{

						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter Mobile No\n");
							scanf("%lld",&a_response.account.user.mobile_no);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Mobile No Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update Mobile No For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}		
					
					break;}
					case 7:{

						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter Email Id\n");
							scanf(" %50[^\n]",a_response.account.user.email);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Email Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update Email For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}		
					break;}

      case 8:{
          printf("Bank Employees are not allowed to change roles!!\n");
        break;}
      case 9:{

						if(a_response.result>0&&a_response.account.status>0){
							printf("Enter New Password\n");
							scanf(" %50[^\n]",a_response.account.user.password);
							a_request=a_response;
							a_request.action=UPDATE;
							a_request.result=-1;
							a_response=execute(a_request);
							if(a_response.result>0){
							printf("Transaction Success !! Password Updated Successfully\n");
							}
							else{printf("Sorry !! Unable To Update Password For Account No %lld\n",a_request.account.account_no);}
						}
						else{printf("Sorry !! Unable To Fetch Account For Account No %lld\n",a_request.account.account_no);}		
					break;
                  }
					default:{printf("Invalid Choice :: %d\n",m_choice);}
					}}else{printf("Bank Employee can only change csutomer details!!\n");}
					printf("Do You Want To Continue (Press 1 Modify Menu)\n");
					scanf("%d",&m_ch);
					}while(m_ch==1);			
				break;
						}
      case VIEW_LOANS:{
                  break;}
			case PROCESS_LOANS:{
				break;}
      case EMP_EXIT:{
         response.account.login = 0;  // Mark the user as logged out
        response.action = UPDATE;  // Update the account information
        response.result = -1;
        response = execute(response);  // Save the update
        if(response.result > 0)printf("Logout successful.\n");

				exit(0);
				break; }

			default:{printf("Invalid Choice :: %d\n",choice);}
			}
			printf("Do You Want To Continue (Press 1)\n");
			scanf("%d",&ch);
      if(ch != 1){
        response.account.login = 0;  // Mark the user as logged out
        response.action = UPDATE;  // Update the account information
        response.result = -1;
        response = execute(response);  // Save the update
        if(response.result > 0)printf("Logout successful.\n");
        }
			}while(ch==1);}

      else{
			printf("Invalid Password :: %c\n",request.account.user.user_type);
		}

	}else{printf("Invalid Password :: %s\n",request.account.user.password);}
	}else{printf("Invalid Account No :: %lld\n",request.account.account_no);}
}
  else{printf("User logged in already!\n");}
}

struct data_packet execute(struct data_packet request_packet){
	struct data_packet response_packet={0};
	struct sockaddr_in serv,cli;
	int sd;
	char buff[80];
	struct data d;
	char *ip_addr="127.0.0.1";
	sd=socket(AF_INET,SOCK_STREAM,0);
	serv.sin_family=AF_INET;
	serv.sin_addr.s_addr=inet_addr (ip_addr);
	serv.sin_port=htons(7791);
	int result=connect(sd,(void *)(&serv),sizeof(serv));
	if(result<0){printf("connection status :: %d\t\n",result);}
	ssize_t s=write(sd,&request_packet,sizeof(struct data_packet));
	ssize_t r=read(sd,&response_packet,sizeof(struct data_packet));
	close(sd);
	//printf("Write %d Bytes | Read %d Bytes\nClosing Connection...",(int)s,(int)r);
	return response_packet;
}
