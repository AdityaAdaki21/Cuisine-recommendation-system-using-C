#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cuisine {
    char name[50];
    char description[100];
    char region[50];
    struct cuisine* prev;
    struct cuisine* next;
} Cuisine;

Cuisine* head = NULL;

void *insertEnd(Cuisine** head, const char* name, const char* description, const char* region) {
    Cuisine* newCuisine = (Cuisine*)malloc(sizeof(Cuisine));
    strcpy(newCuisine->name, name);
    strcpy(newCuisine->description, description);
    strcpy(newCuisine->region, region);
    newCuisine->prev = NULL;
    newCuisine->next = NULL;

    if (*head == NULL) {
        *head = newCuisine;
        (*head)->prev = *head;
        (*head)->next = *head;
    } else {
        Cuisine* last = (*head)->prev;
        last->next = newCuisine;
        newCuisine->prev = last;
        newCuisine->next = *head;
        (*head)->prev = newCuisine;
    }
}

void displayCuisines(Cuisine* head) {
    if (head == NULL) {
        printf("No cuisines available.\n");
        return;
    }

    Cuisine* temp = head;
    do {
        printf("Name: %s\n", temp->name);
        printf("Description: %s\n", temp->description);
        printf("Region: %s\n\n", temp->region);
        temp = temp->next;
    } while (temp != head);
}

void recommendCuisines(const char* filename, const char* preference, const char* region, int filter) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    int found = 0;
    char name[50];
    char description[100];
    char cuisineRegion[50];
    while (fgets(name, sizeof(name), file) != NULL) {
        name[strcspn(name, "\n")] = '\0';
        if (fgets(description, sizeof(description), file) != NULL && 
            fgets(cuisineRegion, sizeof(cuisineRegion), file) != NULL) {
            description[strcspn(description, "\n")] = '\0';
            cuisineRegion[strcspn(cuisineRegion, "\n")] = '\0';
            
            if (filter == 1 && strcmp(cuisineRegion, region) == 0) {
                printf("Name: %s\n", name);
                printf("Description: %s\n", description);
                printf("Region: %s\n\n", cuisineRegion);
                found = 1;
            } else if (filter == 2 && strstr(description, preference) != NULL) {
                printf("Name: %s\n", name);
                printf("Description: %s\n", description);
                printf("Region: %s\n\n", cuisineRegion);
                found = 1;
            } else if (filter == 3 && strcmp(cuisineRegion, region) == 0 && strstr(description, preference) != NULL) {
                printf("Name: %s\n", name);
                printf("Description: %s\n", description);
                printf("Region: %s\n\n", cuisineRegion);
                found = 1;
            }
        }
    }

    if (found == 0) {
        printf("No cuisines found matching your filter criteria.\n");
    }

    fclose(file);
}


void deallocateCuisines(Cuisine** head) {
    if (*head == NULL) {
        return;
    }

    Cuisine* current = *head;
    Cuisine* next = NULL;
    do {
        next = current->next;
        free(current);
        current = next;
    } while (current != *head);

    *head = NULL;
}

void manager() {
    char username[50];
    char password[50];
    
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    if (strcmp(username, "admin") != 0 || strcmp(password, "admin123") != 0) {
        printf("Invalid username or password. Login failed.\n");
        return;
    }

    int numCuisines;
    printf("\nEnter the number of cuisines: ");
    scanf("%d", &numCuisines);
    getchar();

    FILE* file = fopen("cuisines_region.txt", "a");
    if (file == NULL) {
        printf("Error creating file.\n");
        return;
    }

    for (int i = 0; i < numCuisines; i++) {
        char name[50];
        char description[100];
        char region[50];

        printf("Enter cuisine name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';
        fprintf(file, "%s\n", name);

        printf("Enter cuisine description: ");
        fgets(description, sizeof(description), stdin);
        description[strcspn(description, "\n")] = '\0';
        fprintf(file, "%s\n", description);

        printf("Enter cuisine region: ");
        fgets(region, sizeof(region), stdin);
        region[strcspn(region, "\n")] = '\0';
        fprintf(file, "%s\n", region);

        insertEnd(&head, name, description, region);
    }

    fclose(file);

    printf("\nAll Cuisines:\n");
    displayCuisines(head);
    getchar();
    printf("Press Enter to continue...");
    getchar();
    system("cls");
}


void customer() {
    int filter;
    char preference[100];
    char region[50];

    printf("\nFilter Options:\n");
    printf("1. Regional Filter\n");
    printf("2. Preference Filter\n");
    printf("3. Regional and Preference Filter\n");
    printf("Enter your filter option: ");
    scanf("%d", &filter);
    getchar();

    switch (filter) {
        case 1:
            printf("Enter the region: ");
            fgets(region, sizeof(region), stdin);
            region[strcspn(region, "\n")] = '\0';
            recommendCuisines("cuisines_region.txt", NULL, region, 1);
            break;

        case 2:
            printf("Enter your preference: ");
            fgets(preference, sizeof(preference), stdin);
            preference[strcspn(preference, "\n")] = '\0';
            recommendCuisines("cuisines_region.txt", preference, NULL, 2);
            break;

        case 3:
            printf("Enter your preference: ");
            fgets(preference, sizeof(preference), stdin);
            preference[strcspn(preference, "\n")] = '\0';
            printf("Enter the region: ");
            fgets(region, sizeof(region), stdin);
            region[strcspn(region, "\n")] = '\0';
            recommendCuisines("cuisines_region.txt", preference, region, 3);
            break;

        default:
            printf("Invalid filter option.\n");
            break;
    }

    getchar();
    printf("Press Enter to continue...");
    getchar();
}

int main() {
    int role;
    do {
        system("cls");
        printf("Enter your role (1 for Manager, 2 for Customer, 0 to exit): ");
        scanf("%d", &role);
        getchar();

        switch (role) {
            case 1:
                manager();
                break;

            case 2:
                customer();
                break;

            default:
                break;
        }
    } while (role != 0);

    deallocateCuisines(&head);    

    return 0;
}