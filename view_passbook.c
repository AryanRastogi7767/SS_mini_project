
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

struct hist {
    long long int account_no;
    double balance;
    char t_type; // 'd': deposit, 'w': withdrawal
    double amount;
};

void viewAllPassbookEntries() {
    int fd = open("passbook", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open passbook file");
        return;
    }

    struct hist entry;
    int entry_count = 0;

    printf("**** Passbook Entries ****\n");
    printf("%-15s %-10s %-10s %-10s\n", "Account No", "Balance", "Type", "Amount");
    printf("-------------------------------------------\n");

    // Read each transaction entry from the passbook and print it
    while (read(fd, &entry, sizeof(struct hist)) > 0) {
        char *type = (entry.t_type == 'd') ? "Deposit" : "Withdraw";

        printf("%-15lld %-10.2f %-10s %-10.2f\n",
               entry.account_no, entry.balance, type, entry.amount);

        entry_count++;
    }

    if (entry_count == 0) {
        printf("No transactions found.\n");
    }

    close(fd);
}

int main() {
    viewAllPassbookEntries();
    return 0;
}
