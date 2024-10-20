
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

struct user_info {
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
    char user_type; // 'n' - normal user, 'a' - admin user
};

struct account_info {
    long long int account_no;
    double balance;
    char account_type; // 'c' - current, 's' - saving, 'o' - other
    int s_no;
    char feedback[100];
    struct user_info user;
    int status; // 1 for active, 0 for inactive
};

// Function to create a sample account_info structure
struct account_info create_account(long long int account_no, double balance, char account_type, 
                                   int s_no, const char* name_1, const char* name_2, long long int u_id, 
                                   const char* password, const char* city, const char* state, 
                                   const char* country, long long int pin_code, long long int mobile_no, 
                                   const char* email, char user_type, int status) {
    struct account_info account;
    account.account_no = account_no;
    account.balance = balance;
    account.account_type = account_type;
    account.s_no = s_no;
    strcpy(account.user.name_1, name_1);
    strcpy(account.user.name_2, name_2);
    account.user.u_id = u_id;
    strcpy(account.user.password, password);
    strcpy(account.user.city, city);
    strcpy(account.user.state, state);
    strcpy(account.user.country, country);
    account.user.pin_code = pin_code;
    account.user.mobile_no = mobile_no;
    strcpy(account.user.email, email);
    account.user.user_type = user_type;
    account.status = status;
    strcpy(account.feedback, "No feedback");
    return account;
}

// Function to print account details
void print_account_details(struct account_info account) {
    printf("Account No: %lld\n", account.account_no);
    printf("Balance: %.2lf\n", account.balance);
    printf("Account Type: %c\n", account.account_type);
    printf("S No: %d\n", account.s_no);
    printf("User Name: %s %s\n", account.user.name_1, account.user.name_2);
    printf("User ID: %lld\n", account.user.u_id);
    printf("Password: %s\n", account.user.password);
    printf("City: %s, State: %s, Country: %s\n", account.user.city, account.user.state, account.user.country);
    printf("Pin Code: %lld\n", account.user.pin_code);
    printf("Mobile No: %lld\n", account.user.mobile_no);
    printf("Email: %s\n", account.user.email);
    printf("User Type: %c\n", account.user.user_type);
    printf("Status: %d\n", account.status);
    printf("Feedback: %s\n", account.feedback);
    printf("------------------------------------------\n");
}

int main() {
    int fd;
    ssize_t s;
    struct account_info account;

    // Open file for writing
    fd = open("account", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Write 5 different accounts one by one (no need for lseek)

    // Account 1
    account = create_account(1001, 1500.50, 's', 1, "John", "Doe", 1, "pass123", "NYC", "NY", "USA", 12345, 1234567890, "john@example.com", 'a', 1);
    s = write(fd, &account, sizeof(struct account_info));
    if (s == -1) {
        perror("Error writing account 1 to file");
        close(fd);
        return 1;
    }

    // Account 2
    account = create_account(1002, 2000.00, 'c', 2, "Jane", "Smith", 2, "pass234", "LA", "CA", "USA", 54321, 9876543210, "jane@example.com", 'n', 1);
    s = write(fd, &account, sizeof(struct account_info));
    if (s == -1) {
        perror("Error writing account 2 to file");
        close(fd);
        return 1;
    }

    // Account 3
    account = create_account(1003, 5000.75, 's', 3, "Alice", "Brown", 3, "pass345", "Chicago", "IL", "USA", 11111, 1111222233, "alice@example.com", 'n', 1);
    s = write(fd, &account, sizeof(struct account_info));
    if (s == -1) {
        perror("Error writing account 3 to file");
        close(fd);
        return 1;
    }

    // Account 4
    account = create_account(1004, 750.25, 'o', 4, "Bob", "Davis", 4, "pass456", "Houston", "TX", "USA", 22222, 4444555566, "bob@example.com", 'n', 1);
    s = write(fd, &account, sizeof(struct account_info));
    if (s == -1) {
        perror("Error writing account 4 to file");
        close(fd);
        return 1;
    }

    // Account 5
    account = create_account(1005, 100.00, 'c', 5, "Charlie", "Wilson", 5, "pass567", "Miami", "FL", "USA", 33333, 7777888899, "charlie@example.com", 'a', 1);
    s = write(fd, &account, sizeof(struct account_info));
    if (s == -1) {
        perror("Error writing account 5 to file");
        close(fd);
        return 1;
    }

    close(fd);

    // Open file for reading
    fd = open("account", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file for reading");
        return 1;
    }

    // Read and print account details
    printf("Reading account details from file:\n");
    for (int i = 0; i < 5; i++) {
        s = read(fd, &account, sizeof(struct account_info));
        if (s == -1) {
            perror("Error reading from file");
            close(fd);
            return 1;
        }
        print_account_details(account);
    }

    close(fd);
    return 0;
}
