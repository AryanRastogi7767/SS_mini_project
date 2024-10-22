
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

// Define the hist structure
struct hist {
    long long int account_no;
    double balance;
    char t_type; // d: deposit, w: withdrawal
    double amount;
};

void print_passbook_entries(const char *filename) {
    int fd = open(filename, O_RDONLY); // Open file in read-only mode
    if (fd < 0) {
        perror("Error opening file");
        return;
    }

    struct hist entry;
    size_t count = 0;

    // Read entries from the file until EOF
    while (read(fd, &entry, sizeof(struct hist)) == sizeof(struct hist)) {
        count++;
        // Print the data for each entry
        printf("Entry %zu:\n", count);
        printf("Account No: %lld\n", entry.account_no);
        printf("Balance: %.2f\n", entry.balance);
        printf("Transaction Type: %c\n", entry.t_type);
        printf("Amount: %.2f\n\n", entry.amount);
    }

    // Check for errors during read
    if (count == 0 && entry.account_no == 0) {
        perror("Error reading file or no entries found");
    }

    close(fd); // Close the file
}

int main() {
    const char *filename = "passbook"; // Name of the passbook file
    print_passbook_entries(filename); // Call function to print entries
    return 0;
}
