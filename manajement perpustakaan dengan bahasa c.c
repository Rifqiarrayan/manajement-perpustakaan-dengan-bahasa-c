#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char title[100];
    char author[100];
    char category[100];
    int borrowed; // Added for borrowed status
} Book;

typedef struct Node {
    Book book;
    struct Node* next;
} Node;

typedef struct DNode {
    Book book;
    struct DNode* prev;
    struct DNode* next;
} DNode;

typedef struct BSTNode {
    Book book;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

typedef struct QueueNode {
    Book book;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;

typedef struct StackNode {
    Book book;
    struct StackNode* next;
} StackNode;

typedef struct TreeNode {
    char category[100];
    Book books[100];
    int bookCount;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct GraphNode {
    Book book;
    struct GraphNode* adj[100];
    int adjCount;
} GraphNode;

Book books[100];
int bookCount = 0;
Node* head = NULL;
DNode* dhead = NULL;
BSTNode* root = NULL;
Queue recommendationQueue = { NULL, NULL };
StackNode* top = NULL;
TreeNode* categoryTree = NULL;
GraphNode* graphNodes[100];
int graphNodeCount = 0;

void displayWelcome() {
    printf("=====================================\n");
    printf("         Selamat Datang di           \n");
    printf("    Sistem Manajemen Perpustakaan    \n");
    printf("=====================================\n");
}

void displayMenu() {
    printf("1. Tambah Buku\n");
    printf("2. Cari Buku\n");
    printf("3. Pinjam Buku\n"); // Modified menu option
    printf("4. Riwayat Peminjaman\n");
    printf("5. Rekomendasi Buku\n");
    printf("6. Daftar Buku Berdasarkan Kategori\n");
    printf("7. Keluar\n");
    printf("Pilih opsi: ");
}

void addBook() {
    Book newBook;
    printf("Masukkan ID Buku: ");
    scanf("%d", &newBook.id);
    getchar(); // Clear the newline character
    printf("Masukkan Judul Buku: ");
    fgets(newBook.title, sizeof(newBook.title), stdin);
    newBook.title[strcspn(newBook.title, "\n")] = 0; // Remove newline character
    printf("Masukkan Pengarang Buku: ");
    fgets(newBook.author, sizeof(newBook.author), stdin);
    newBook.author[strcspn(newBook.author, "\n")] = 0; // Remove newline character
    printf("Masukkan Kategori Buku: ");
    fgets(newBook.category, sizeof(newBook.category), stdin);
    newBook.category[strcspn(newBook.category, "\n")] = 0; // Remove newline character
    
    newBook.borrowed = 0; // Initialize borrowed status
    
    books[bookCount++] = newBook;

    // Add to linked list
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->book = newBook;
    newNode->next = NULL;
    if (!head) {
        head = newNode;
    } else {
        Node* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    
    // Add to doubly linked list
    DNode* newDNode = (DNode*)malloc(sizeof(DNode));
    newDNode->book = newBook;
    newDNode->prev = newDNode->next = NULL;
    if (!dhead) {
        dhead = newDNode;
    } else {
        DNode* dtemp = dhead;
        while (dtemp->next) {
            dtemp = dtemp->next;
        }
        dtemp->next = newDNode;
        newDNode->prev = dtemp;
    }
    
    // Add to binary search tree
    BSTNode* newBSTNode = (BSTNode*)malloc(sizeof(BSTNode));
    newBSTNode->book = newBook;
    newBSTNode->left = newBSTNode->right = NULL;
    if (!root) {
        root = newBSTNode;
    } else {
        BSTNode* current = root;
        BSTNode* parent = NULL;
        while (current) {
            parent = current;
            if (strcmp(newBook.title, current->book.title) < 0) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        if (strcmp(newBook.title, parent->book.title) < 0) {
            parent->left = newBSTNode;
        } else {
            parent->right = newBSTNode;
        }
    }
    
    // Add to stack for recommendations
    StackNode* newStackNode = (StackNode*)malloc(sizeof(StackNode));
    newStackNode->book = newBook;
    newStackNode->next = top;
    top = newStackNode;
    
    // Add to category tree
    TreeNode* newTreeNode = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy(newTreeNode->category, newBook.category);
    newTreeNode->books[0] = newBook;
    newTreeNode->bookCount = 1;
    newTreeNode->left = newTreeNode->right = NULL;
    if (!categoryTree) {
        categoryTree = newTreeNode;
    } else {
        TreeNode* current = categoryTree;
        TreeNode* parent = NULL;
        while (current) {
            parent = current;
            if (strcmp(newBook.category, current->category) < 0) {
                current = current->left;
            } else if (strcmp(newBook.category, current->category) > 0) {
                current = current->right;
            } else {
                current->books[current->bookCount++] = newBook;
                free(newTreeNode);
                return;
            }
        }
        if (strcmp(newBook.category, parent->category) < 0) {
            parent->left = newTreeNode;
        } else {
            parent->right = newTreeNode;
        }
    }
    
    // Add to graph
    GraphNode* newGraphNode = (GraphNode*)malloc(sizeof(GraphNode));
    newGraphNode->book = newBook;
    newGraphNode->adjCount = 0;
    graphNodes[graphNodeCount++] = newGraphNode;
    
    printf("Buku berhasil ditambahkan!\n");
}




void displayLoanHistory() {
    DNode* temp = dhead;
    int borrowedFound = 0; // Flag to check if borrowed books are found
    
    printf("Riwayat Peminjaman Buku:\n");
    while (temp) {
        if (temp->book.borrowed) {
            borrowedFound = 1; // Set flag to true
            printf("ID: %d, Judul: %s, Pengarang: %s, Kategori: %s\n", temp->book.id, temp->book.title, temp->book.author, temp->book.category);
        }
        temp = temp->next;
    }
    
    if (!borrowedFound) {
        printf("Maaf, Anda belum pernah meminjam buku, silakan pinjam buku terlebih dahulu.\n");
    }
}

void recommendBooks() {
    StackNode* temp = top;
    printf("Rekomendasi Buku untuk anda:\n");
    while (temp) {
        printf("ID: %d, Judul: %s, Pengarang: %s, Kategori: %s\n", temp->book.id, temp->book.title, temp->book.author, temp->book.category);
        temp = temp->next;
    }
}

void borrowBook() {
    char title[100];
    printf("Masukkan Judul Buku yang Ingin Dipinjam: ");
    getchar(); // Clear the newline character
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0; // Remove newline character
    
    // Search for the book in the binary search tree
    BSTNode* current = root;
    while (current) {
        if (strcmp(title, current->book.title) == 0) {
            // Book found, now check if it has been borrowed before
            // Assuming a simple flag for borrowed status
            if (current->book.borrowed) {
                printf("Buku dengan judul '%s' sudah pernah dipinjam sebelumnya.\n", current->book.title);
            } else {
                // Mark the book as borrowed (assuming we have a borrowed flag in Book struct)
                current->book.borrowed = 1;
                
                // Optionally, add the book to borrow history (linked list)
                // Create a new node for borrow history
                DNode* newBorrowNode = (DNode*)malloc(sizeof(DNode));
                newBorrowNode->book = current->book;
                newBorrowNode->prev = newBorrowNode->next = NULL;
                
                if (!dhead) {
                    dhead = newBorrowNode;
                } else {
                    DNode* dtemp = dhead;
                    while (dtemp->next) {
                        dtemp = dtemp->next;
                    }
                    dtemp->next = newBorrowNode;
                    newBorrowNode->prev = dtemp;
                }
                
                printf("Buku dengan judul '%s' berhasil dipinjam.\n", current->book.title);
            }
            return;
        } else if (strcmp(title, current->book.title) < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    printf("Buku dengan judul '%s' tidak ditemukan dalam sistem.\n", title);
}

void displayBooksByCategoryMenu() {
    printf("Masukkan kategori yang ingin ditampilkan: ");
    char category[100];
    getchar(); // Clear the newline character
    fgets(category, sizeof(category), stdin);
    category[strcspn(category, "\n")] = 0; // Remove newline character

    TreeNode* current = categoryTree;
    int found = 0; // Flag to indicate if category is found

    while (current) {
        int cmp = strcmp(category, current->category);
        if (cmp == 0) {
            found = 1; // Mark category as found
            printf("Daftar Buku dalam Kategori '%s':\n", current->category);
            int i;
            for ( i = 0; i < current->bookCount; ++i) {
                printf("ID: %d, Judul: %s, Pengarang: %s\n", current->books[i].id, current->books[i].title, current->books[i].author);
            }
            break; // Exit loop once category is found
        } else if (cmp < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (!found) {
        printf("Kategori '%s' tidak ditemukan dalam sistem.\n", category);
    }
}



void searchBooksByTitle(BSTNode* node, const char* title) {
    if (node) {
        searchBooksByTitle(node->left, title);
        if (strstr(node->book.title, title) != NULL) {
            printf("ID: %d, Judul: %s, Pengarang: %s, Kategori: %s\n", node->book.id, node->book.title, node->book.author, node->book.category);
        }
        searchBooksByTitle(node->right, title);
    }
}



void freeMemory(Node* head) {
    Node* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void exitProgram() {
    freeMemory(head);
    printf("Terima kasih telah menggunakan Sistem Manajemen Perpustakaan.\n");
    exit(0);
}

int main() {
    int choice;
    do {
        displayWelcome();
        displayMenu();
        scanf("%d", &choice);
        getchar(); // Clear the newline character
        switch (choice) {
            case 1:
                addBook();
                break;
            case 2: {
                char title[100];
                printf("Masukkan Judul Buku yang Dicari: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0; // Remove newline character
                searchBooksByTitle(root, title);
                break;
            }
            case 3:
                borrowBook();
                break;
            case 4:
                displayLoanHistory();
                break;
            case 5:
                recommendBooks();
                break;
            case 6:
                displayBooksByCategoryMenu();
                break;
            case 7:
                exitProgram();
                break;
            default:
                printf("Opsi yang Anda pilih tidak valid. Silakan pilih opsi yang sesuai.\n");
                break;
        }
    } while (choice != 7);

    return 0;
}
