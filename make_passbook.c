
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

struct hist {
    long long int account_no;
    double balance;
    char t_type; // 'd' for deposit, 'w' for withdrawal
    double amount;
};

int main() {
    struct hist transactions[5] = {
        {1001, 5000.0, 'd', 1000.0},
        {1001, 4000.0, 'w', 1000.0},
        {1002, 3000.0, 'd', 1500.0},
        {1001, 3000.0, 'd', 1000.0},
        {1003, 7000.0, 'd', 2000.0}
    };

    int fd = open("passbook", O_CREAT | O_WRONLY, 0666); // Create the passbook file

    if (fd < 0) {
        perror("Failed to open passbook file");
        return 1;
    }

    // Write the transactions to the file
    for (int i = 0; i < 5; i++) {
        write(fd, &transactions[i], sizeof(struct hist));
    }

    close(fd);
    printf("Passbook file created and written successfully.\n");
    return 0;
}
