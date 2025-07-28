#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#ifdef _WIN32
    #include <conio.h>
#endif

#define MAX_QUESTIONS 1000
#define MAX_USERS 100
#define MAX_USERNAME 25
#define MAX_PASSWORD 20
#define MAX_NAME 50
#define MAX_EMAIL 50
#define MAX_PHONE 15
#define MAX_QUESTION_LENGTH 300
#define MAX_OPTION_LENGTH 150

typedef struct
{
    char question[MAX_QUESTION_LENGTH];
    char options[4][MAX_OPTION_LENGTH];
    int correct_answer;
    int difficulty; // 1=Easy, 2=Medium, 3=Hard
    int category;   // 0-4 for categories
    int points;
    int time_limit;
} Question;

typedef struct
{
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char name[MAX_NAME];
    char email[MAX_EMAIL];
    char phone[MAX_PHONE];
    int General_high_score;
    int rapid_high_score;
    int total_general_games;
    int total_rapid_games;
    int scores[50]; // Last 50 scores
    int score_count;
} User;

typedef struct
{
    Question questions[MAX_QUESTIONS];
    int total_questions;
    int used_questions[MAX_QUESTIONS];
    int used_count;
} QuestionBank;

// Global variables
QuestionBank qbank;
User users[MAX_USERS];
int user_count = 0;
int current_user_index = -1;
char categories[5][30] = {"General Knowledge", "Geography (Nepal)", "Science", "Sports", "Programming"};

// Function prototypes
void init_system();
void main_menu();
void admin_panel();
void user_panel();
int login_user();
void register_user();
void save_users();
void load_users();
void save_questions();
void load_questions();
void load_questions_from_txt();
void add_question();
void view_questions();
void delete_question();
void General_mode();
void rapid_fire_mode();
void display_scores();
void display_game_rules();
int get_random_question(int category, int difficulty);
void display_question(Question *q, int qnum, int total);
int get_answer_with_lifeline(Question *q, int *lifeline_used);
int get_answer_rapid(Question *q);
void apply_lifeline(Question *q);
void shuffle_array(int arr[], int n);
void clear_screen();
void wait_enter();
int check_time_limit();
void get_hidden_password(char *password, int max_length);

void reset_password();
void delete_user();
int is_back_command(char *input);

int main()
{
    srand(time(NULL));
    init_system();
    load_users();
    load_questions();
    main_menu();
    return 0;
}

void init_system()
{
    qbank.total_questions = 0;
    qbank.used_count = 0;
    user_count = 0;
    current_user_index = -1;
    memset(qbank.used_questions, 0, sizeof(qbank.used_questions));
}

void main_menu()
{
    int choice;
    char input[10];
    while (1)
    {
        clear_screen();
        printf("=== QUIZ GAME ===\n");
        printf("1. Admin Panel\n2. User Panel\n3. Exit\n");
        printf("Choice: ");
        scanf("%s", input);

        choice = atoi(input);

        switch (choice)
        {
        case 1:
            admin_panel();
            break;
        case 2:
            user_panel();
            break;
        case 3:
            save_users();
            save_questions();
            printf("Goodbye!\n");
            exit(0);
        default:
            printf("Invalid choice! Please enter 1, 2, or 3.\n");
            wait_enter();
        }
    }
}

void admin_panel()
{
    char admin_pass[20];
    printf("Enter Admin Password (or 'back' to return): ");
    get_hidden_password(admin_pass, sizeof(admin_pass));
    if (is_back_command(admin_pass))
    {
        return;
    }
    if (strcmp(admin_pass, "messigoat") != 0)
    {
        printf("Access Denied!\n");
        wait_enter();
        return;
    }
    int choice;
    char input[10];
    while (1)
    {
        clear_screen();
        printf("=== ADMIN PANEL ===\n");
        printf("1. Add Question\n2. View Questions\n3. Delete Question\n");
        printf("4. Import from questions.txt\n5. View All Users\n6. Check or Reset User Password\n");
        printf("7. Delete User\n8. Back to Main Menu\n"); // Updated menu
        printf("Choice: ");
        scanf("%s", input);

        choice = atoi(input);
        switch (choice)
        {
        case 1:
            add_question();
            break;
        case 2:
            view_questions();
            break;
        case 3:
            delete_question();
            break;
        case 4:
            load_questions_from_txt();
            wait_enter();
            break;
        case 5:
            for (int i = 0; i < user_count; i++)
            {
                printf("%d. %s (%s) - Games: G:%d/R:%d, High: %d/%d\n",
                       i + 1, users[i].name, users[i].username,
                       users[i].total_general_games, users[i].total_rapid_games,
                       users[i].General_high_score, users[i].rapid_high_score);
            }
            wait_enter();
            break;
        case 6:
            reset_password();
            break;
        case 7:
            delete_user(); // New functionality
            break;
        case 8: // Updated case number
            return;
        default:
            printf("Invalid choice! Please enter a number between 1-8.\n"); // Updated range
            wait_enter();
        }
    }
}

// New function to implement - add this function definition
void delete_user()
{
    if (user_count == 0)
    {
        printf("No users found!\n");
        wait_enter();
        return;
    }

    // Display all users
    printf("=== DELETE USER ===\n");
    printf("Current Users:\n");
    for (int i = 0; i < user_count; i++)
    {
        printf("%d. %s (%s) - Games: G:%d/R:%d, High: %d/%d\n",
               i + 1, users[i].name, users[i].username,
               users[i].total_general_games, users[i].total_rapid_games,
               users[i].General_high_score, users[i].rapid_high_score);
    }

    char input[10];
    printf("\nEnter user number to delete (or 'back' to cancel): ");
    scanf("%s", input);

    if (is_back_command(input))
    {
        return;
    }

    int user_index = atoi(input) - 1;

    // Validate input
    if (user_index < 0 || user_index >= user_count)
    {
        printf("Invalid user number!\n");
        wait_enter();
        return;
    }

    // Confirm deletion
    printf("Are you sure you want to delete user '%s' (%s)? (y/n): ",
           users[user_index].name, users[user_index].username);
    char confirm;
    scanf(" %c", &confirm);

    if (confirm != 'y' && confirm != 'Y')
    {
        printf("Deletion cancelled.\n");
        wait_enter();
        return;
    }

    // Delete user by shifting array elements
    for (int i = user_index; i < user_count - 1; i++)
    {
        users[i] = users[i + 1];
    }

    user_count--;

    // Save updated user data to file
    save_users();

    printf("User deleted successfully!\n");
    wait_enter();
}

void user_panel()
{
    if (current_user_index == -1)
    {
        int choice;
        char input[10];
        printf("1. Login\n2. Register\n3. Back\nChoice: ");
        scanf("%s", input);

        choice = atoi(input);

        switch (choice)
        {
        case 1:
            if (!login_user())
                return;
            break;
        case 2:
            register_user();
            return;
        case 3:
            return;
        default:
            printf("Invalid choice! Please enter 1, 2, or 3.\n");
            wait_enter();
            return;
        }
    }

    int choice;
    char input[10];
    while (1)
    {
        clear_screen();
        printf("=== WELCOME %s ===\n", users[current_user_index].name);
        printf("High Scores - General: %d, Rapid: %d\n",
               users[current_user_index].General_high_score,
               users[current_user_index].rapid_high_score);
        printf("Total Games - General: %d, Rapid: %d\n\n",
               users[current_user_index].total_general_games,
               users[current_user_index].total_rapid_games);

        printf("1. General Mode\n2. Rapid Fire Mode\n3. View Scores\n");
        printf("4. Game Rules\n5. Logout\nChoice: ");
        scanf("%s", input);

        if (is_back_command(input))
        {
            current_user_index = -1;
            return;
        }

        choice = atoi(input);

        switch (choice)
        {
        case 1:
            General_mode();
            break;
        case 2:
            rapid_fire_mode();
            break;
        case 3:
            display_scores();
            break;
        case 4:
            display_game_rules();
            break;
        case 5:
            current_user_index = -1;
            return;
        default:
            printf("Invalid choice! Please enter a number between 1-5.\n");
            wait_enter();
        }
    }
}

void register_user()
{
    if (user_count >= MAX_USERS)
    {
        printf("User limit reached!\n");
        wait_enter();
        return;
    }

    User new_user;
    char input[100];
    char confirm_password[MAX_PASSWORD];

    printf("Enter Full Name (or 'back' to return): ");
    getchar();
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    if (is_back_command(input))
    {
        return;
    }

    strcpy(new_user.name, input);

    printf("Enter Email (or 'back' to return): ");
    scanf("%s", input);

    if (is_back_command(input))
    {
        return;
    }

    strcpy(new_user.email, input);

    printf("Enter Phone Number (or 'back' to return): ");
    scanf("%s", input);

    if (is_back_command(input))
    {
        return;
    }

    strcpy(new_user.phone, input);

    // Password input with confirmation
    while (1)
    {
        printf("Enter Password (or 'back' to return): ");
        get_hidden_password(input, sizeof(input));

        if (is_back_command(input))
        {
            return;
        }

        printf("Confirm Password (or 'back' to return): ");
        get_hidden_password(confirm_password, sizeof(confirm_password));

        if (is_back_command(confirm_password))
        {
            return;
        }

        // Check if passwords match
        if (strcmp(input, confirm_password) == 0)
        {
            strcpy(new_user.password, input);
            break; // Passwords match, exit the loop
        }
        else
        {
            printf("Passwords do not match! Please try again.\n\n");
            // Loop will continue, asking for passwords again
        }
    }

    printf("Choose a username (or 'back' to return): ");
    scanf("%s", input);

    if (is_back_command(input))
    {
        return;
    }

    // Check if username already exists
    int username_exists = 0;
    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(users[i].username, input) == 0)
        {
            username_exists = 1;
            break;
        }
    }

    if (username_exists)
    {
        printf("Username already exists! Try another.\n");
        wait_enter();
        return;
    }

    strcpy(new_user.username, input);

    new_user.General_high_score = 0;
    new_user.rapid_high_score = 0;
    new_user.total_general_games = 0;
    new_user.total_rapid_games = 0;
    new_user.score_count = 0;

    users[user_count++] = new_user;
    save_users();

    printf("Registration successful!\n");
    wait_enter();
}

int login_user()
{
    char username[MAX_USERNAME], password[MAX_PASSWORD];
    printf("Username (or 'back' to return): ");
    scanf("%s", username);

    if (is_back_command(username))
    {
        return 0;
    }

    printf("Password (or 'back' to return): ");
    get_hidden_password(password, sizeof(password));

    if (is_back_command(password))
    {
        return 0;
    }

    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0)
        {
            current_user_index = i;
            printf("Login successful!\n");
            wait_enter();
            return 1;
        }
    }

    printf("\nInvalid credentials!\nIf you forgot your password, contact admin at:\n+977-9866348028 via SMS/whatsapp or \ndirectly mail at dhakalsaurab1234@gmail.com and mention your Full Name.\n");
    wait_enter();
    return 0;
}

void generate_username(char *name, char *phone, char *username)
{
    char first_name[MAX_NAME];
    sscanf(name, "%s", first_name);

    // Convert to lowercase
    for (int i = 0; first_name[i]; i++)
    {
        first_name[i] = tolower(first_name[i]);
    }

    int len = strlen(phone);
    char last3[4];
    if (len >= 3)
    {
        strncpy(last3, phone + len - 3, 3);
        last3[3] = '\0';
    }
    else
    {
        strcpy(last3, phone);
    }

    // Create base username
    char base_username[MAX_USERNAME];
    sprintf(base_username, "%s@%s", first_name, last3);

    // Check for duplicates and add number suffix if needed
    int suffix = 0;
    strcpy(username, base_username);

    while (1)
    {
        int exists = 0;
        for (int i = 0; i < user_count; i++)
        {
            if (strcmp(users[i].username, username) == 0)
            {
                exists = 1;
                break;
            }
        }

        if (!exists)
            break;

        suffix++;
        sprintf(username, "%s%d", base_username, suffix);
    }
}

void reset_password()
{
    char username[MAX_USERNAME];
    printf("Enter username to check or reset password of the user(or 'back' to return): ");
    scanf("%s", username);

    if (is_back_command(username))
    {
        return;
    }

    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(users[i].username, username) == 0)
        {
            printf("User found: %s (%s)\n", users[i].name, users[i].email);
            printf("Current password: %s\n", users[i].password);

            char new_password[MAX_PASSWORD];
            printf("Enter new password (or 'back' to cancel): ");
            scanf("%s", new_password);

            if (is_back_command(new_password))
            {
                return;
            }

            strcpy(users[i].password, new_password);
            save_users();
            printf("Password updated successfully!\n");
            wait_enter();
            return;
        }
    }

    printf("Username not found!\n");
    wait_enter();
}

void add_question()
{
    if (qbank.total_questions >= MAX_QUESTIONS)
    {
        printf("Question bank full!\n");
        wait_enter();
        return;
    }

    Question *q = &qbank.questions[qbank.total_questions];
    char input[10];

    printf("Select Category:\n");
    for (int i = 0; i < 5; i++)
    {
        printf("%d. %s\n", i + 1, categories[i]);
    }
    printf("Choice (or 'back' to return): ");
    scanf("%s", input);

    if (is_back_command(input))
    {
        return;
    }

    int category_choice = atoi(input);
    if (category_choice < 1 || category_choice > 5)
    {
        printf("Invalid category choice!\n");
        wait_enter();
        return;
    }
    q->category = category_choice - 1;

    printf("Select Difficulty (1-Easy, 2-Medium, 3-Hard) or 'back': ");
    scanf("%s", input);

    if (is_back_command(input))
    {
        return;
    }

    int difficulty = atoi(input);
    if (difficulty < 1 || difficulty > 3)
    {
        printf("Invalid difficulty!\n");
        wait_enter();
        return;
    }
    q->difficulty = difficulty;

    q->points = q->difficulty;
    q->time_limit = (q->difficulty == 1) ? 20 : (q->difficulty == 2) ? 30
                                                                     : 60;

    printf("Enter Question (or 'back' to return): ");
    getchar();
    fgets(q->question, MAX_QUESTION_LENGTH, stdin);

    if (is_back_command(q->question))
    {
        return;
    }

    q->question[strcspn(q->question, "\n")] = 0;

    for (int i = 0; i < 4; i++)
    {
        printf("Option %d (or 'back' to return): ", i + 1);
        fgets(q->options[i], MAX_OPTION_LENGTH, stdin);

        if (is_back_command(q->options[i]))
        {
            return;
        }

        q->options[i][strcspn(q->options[i], "\n")] = 0;
    }

    printf("Correct Answer (1-4) or 'back': ");
    scanf("%s", input);

    if (is_back_command(input))
    {
        return;
    }

    int correct = atoi(input);
    if (correct < 1 || correct > 4)
    {
        printf("Invalid answer choice!\n");
        wait_enter();
        return;
    }
    q->correct_answer = correct - 1;

    qbank.total_questions++;
    save_questions();
    printf("Question added successfully!\n");
    wait_enter();
}

void view_questions()
{
    if (qbank.total_questions == 0)
    {
        printf("No questions available!\n");
        wait_enter();
        return;
    }

    for (int i = 0; i < qbank.total_questions; i++)
    {
        Question *q = &qbank.questions[i];
        printf("\n%d. [%s] [%s] %s\n", i + 1, categories[q->category],
               (q->difficulty == 1) ? "Easy" : (q->difficulty == 2) ? "Medium"
                                                                    : "Hard",
               q->question);

        for (int j = 0; j < 4; j++)
        {
            printf("   %d. %s %s\n", j + 1, q->options[j],
                   (j == q->correct_answer) ? "[CORRECT]" : "");
        }
    }
    wait_enter();
}

void delete_question()
{
    if (qbank.total_questions == 0)
    {
        printf("No questions to delete!\n");
        wait_enter();
        return;
    }

    char input[10];
    printf("Enter question number to delete (1-%d) or 'back': ", qbank.total_questions);
    scanf("%s", input);

    if (is_back_command(input))
    {
        return;
    }

    int index = atoi(input);
    index--;

    if (index < 0 || index >= qbank.total_questions)
    {
        printf("Invalid question number!\n");
        wait_enter();
        return;
    }

    for (int i = index; i < qbank.total_questions - 1; i++)
    {
        qbank.questions[i] = qbank.questions[i + 1];
    }
    qbank.total_questions--;
    save_questions();
    printf("Question deleted successfully!\n");
    wait_enter();
}

void General_mode()
{
    if (qbank.total_questions == 0)
    {
        printf("No questions available! Contact admin.\n");
        wait_enter();
        return;
    }

    char input[10];
    int num_questions, difficulty, category;

    printf("Enter number of questions (1-50) or 'back': ");
    scanf("%s", input);

    if (is_back_command(input))
    {
        return;
    }

    num_questions = atoi(input);
    if (num_questions <= 0 || num_questions > 50)
    {
        printf("Invalid number! Here is a set of 10 questions instead.\n");
        num_questions = 10;
    }

    printf("Select category:\n");
    for (int i = 0; i < 5; i++)
    {
        printf("%d. %s\n", i + 1, categories[i]);
    }
    printf("Choice (or 'back' to return): ");
    scanf("%s", input);

    if (is_back_command(input))
    {
        return;
    }

    category = atoi(input);
    if (category < 1 || category > 5)
    {
        printf("Invalid category! Using General Knowledge.\n");
        category = 1;
    }
    category--;

    printf("Select difficulty (1-Easy, 2-Medium, 3-Hard, 4-Mix) or 'back': ");
    scanf("%s", input);

    if (is_back_command(input))
    {
        return;
    }

    difficulty = atoi(input);
    if (difficulty < 1 || difficulty > 4)
    {
        printf("Invalid difficulty! Using Easy mode.\n");
        difficulty = 1;
    }

    clear_screen();
    printf("Starting General Mode...\n");
    printf("Category: %s | Difficulty: %s | Questions: %d\n",
           categories[category],
           (difficulty == 1) ? "Easy" : (difficulty == 2) ? "Medium"
                                    : (difficulty == 3)   ? "Hard"
                                                          : "Mix",
           num_questions);
    printf("Time per question: %d seconds\n",
           (difficulty == 1) ? 20 : (difficulty == 2) ? 30
                                                      : 60);
    printf("Press 5 for 50-50 lifeline (once per game)\n");
    printf("Type 'back' anytime to return to main menu\n");
    wait_enter();

    int score = 0, correct = 0, lifeline_used = 0;
    memset(qbank.used_questions, 0, sizeof(qbank.used_questions));
    qbank.used_count = 0;

    time_t start_time, current_time;

    for (int i = 0; i < num_questions; i++)
    {
        int q_index;
        if (difficulty == 4)
        {                                     // Mix mode
            int mix_difficulty = (i % 3) + 1; // Cycle through 1,2,3
            q_index = get_random_question(category, mix_difficulty);
        }
        else
        {
            q_index = get_random_question(category, difficulty);
        }
        if (q_index == -1)
        {
            printf("No more questions available!\n");
            break;
        }

        Question *q = &qbank.questions[q_index];

        clear_screen();
        display_question(q, i + 1, num_questions);

        printf("\nTime limit: %d seconds | Points: %d\n", q->time_limit, q->points);
        if (!lifeline_used)
            printf("Press 5 for 50-50 lifeline\n");
        printf("Type 'back' to return to main menu\n");

        // Start timer
        start_time = time(NULL);

        int answer = get_answer_with_lifeline(q, &lifeline_used);

        if (answer == -2)
        { // Back command
            return;
        }

        // Check time limit
        current_time = time(NULL);
        if ((current_time - start_time) > q->time_limit)
        {
            printf("\nTime's up! Game over.\n");
            printf("Final Score: %d\n", score);
            break;
        }

        if (answer == q->correct_answer)
        {
            printf("\nCorrect! +%d points\n", q->points);
            score += q->points;
            correct++;
        }
        else
        {
            printf("\nWrong! Correct answer: %d. %s\n",
                   q->correct_answer + 1, q->options[q->correct_answer]);
        }

        printf("Score: %d | Correct: %d/%d\n", score, correct, i + 1);
        wait_enter();
    }

    // Update user data
    User *user = &users[current_user_index];
    if (score > user->General_high_score)
    {
        user->General_high_score = score;
        printf("\n*** NEW HIGH SCORE! ***\n");
    }

    if (user->score_count < 50)
    {
        user->scores[user->score_count++] = score;
    }
    else
    {
        for (int i = 0; i < 49; i++)
        {
            user->scores[i] = user->scores[i + 1];
        }
        user->scores[49] = score;
    }

    user->total_general_games++;
    save_users();

    printf("\nGame Complete! Final Score: %d\n", score);
    printf("Accuracy: %.1f%%\n", (float)correct / num_questions * 100);
    wait_enter();
}

void rapid_fire_mode()
{
    if (qbank.total_questions == 0)
    {
        printf("No questions available! Contact admin.\n");
        wait_enter();
        return;
    }

    printf("RAPID FIRE MODE\n");
    printf("Answer as many questions as possible in 60 seconds!\n");
    printf("All medium difficulty questions from all categories.\n");
    printf("Type 'back' anytime to return to main menu\n");
    wait_enter();

    int score = 0, questions_answered = 0;
    int wrong_answers[100], wrong_count = 0;
    memset(qbank.used_questions, 0, sizeof(qbank.used_questions));
    qbank.used_count = 0;

    time_t start_time = time(NULL);
    time_t current_time;

    while (1)
    {
        current_time = time(NULL);
        if ((current_time - start_time) >= 60)
        {
            printf("\nTime's up!\n");
            break;
        }

        int q_index = get_random_question(-1, 2); // Any category, medium difficulty
        if (q_index == -1)
        {
            printf("No more questions!\n");
            break;
        }

        Question *q = &qbank.questions[q_index];

        clear_screen();
        printf("RAPID FIRE - Time: %d seconds left\n", 60 - (int)(current_time - start_time));
        printf("Score: %d | Questions: %d\n\n", score, questions_answered);

        display_question(q, questions_answered + 1, -1);

        int answer = get_answer_rapid(q);

        if (answer == -2)
        { // Back command
            return;
        }

        if (answer == q->correct_answer)
        {
            printf("Correct! +1 point\n");
            score++;
        }
        else
        {
            printf("Wrong! Answer: %d\n", q->correct_answer + 1);
            wrong_answers[wrong_count++] = q_index;
        }

        questions_answered++;
    }

    // Show wrong answers
    if (wrong_count > 0)
    {
        printf("\n=== WRONG ANSWERS ===\n");
        for (int i = 0; i < wrong_count; i++)
        {
            Question *q = &qbank.questions[wrong_answers[i]];
            printf("%d. %s\n", i + 1, q->question);
            printf("   Correct: %s\n\n", q->options[q->correct_answer]);
        }
    }

    // Update user data
    User *user = &users[current_user_index];
    if (score > user->rapid_high_score)
    {
        user->rapid_high_score = score;
        printf("*** NEW HIGH SCORE! ***\n");
    }
    user->total_rapid_games++;
    save_users();

    printf("Final Score: %d | Questions Answered: %d\n", score, questions_answered);
    wait_enter();
}

void display_scores()
{
    User *user = &users[current_user_index];
    printf("=== SCORE HISTORY ===\n");
    printf("Player: %s\n", user->name);
    printf("General Mode High Score: %d\n", user->General_high_score);
    printf("Rapid Fire High Score: %d\n", user->rapid_high_score);
    printf("Total General Games: %d\n", user->total_general_games);
    printf("Total Rapid Fire Games: %d\n\n", user->total_rapid_games);

    if (user->score_count > 0)
    {
        printf("Recent Scores: ");
        for (int i = 0; i < user->score_count && i < 10; i++)
        {
            printf("%d ", user->scores[i]);
        }
        printf("\n");
    }
    wait_enter();
}

void display_game_rules()
{
    clear_screen();
    printf("=== QUIZ GAME RULES ===\n\n");

    printf("=== GENERAL MODE ===\n");
    printf("Choose 1-50 questions from 5 categories.\n");
    printf("Categories: General Knowledge, Geography (Nepal), Science, Sports, Programming\n");
    printf("Difficulty levels:\n");
    printf("  - Easy: 20 seconds, 1 point per question\n");
    printf("  - Medium: 30 seconds, 2 points per question\n");
    printf("  - Hard: 60 seconds, 3 points per question\n");
    printf("  - Mix: Cycles through all difficulty levels\n");
    printf("One 50-50 lifeline per game (press 5 to eliminate 2 wrong answers)\n");
    printf("Game ends if time limit is exceeded.\n\n");

    printf("=== RAPID FIRE MODE ===\n");
    printf("60-second time limit for entire game.\n");
    printf("All medium difficulty questions from mixed categories.\n");
    printf("1 point per correct answer.\n");
    printf("No lifelines available.\n");
    printf("Answer as many questions as possible.\n");
    printf("Review wrong answers at the end.\n\n");

    printf("=== SCORING SYSTEM ===\n");
    printf("High scores saved for both modes.\n");
    printf("Last 50 scores are tracked.\n");
    printf("Total games played are recorded.\n");
    printf("View your score history anytime.\n\n");

    printf("=== TIPS ===\n");
    printf("Type 'back' anytime to return to main menu.\n");
    printf("If you forgot your password, you can contact admin for checking or reseting your password.\n Contact at:\n");
    printf("+977-9866348028 (SMS/WhatsApp)\n");
    printf("dhakalsaurab1234@gmail.com\n");

    wait_enter();
}

int get_random_question(int category, int difficulty)
{
    int available[MAX_QUESTIONS];
    int count = 0;

    for (int i = 0; i < qbank.total_questions; i++)
    {
        if (qbank.used_questions[i])
            continue;

        Question *q = &qbank.questions[i];
        if (category != -1 && q->category != category)
            continue;
        if (q->difficulty != difficulty)
            continue;

        available[count++] = i;
    }

    if (count == 0)
        return -1;

    int selected = available[rand() % count];
    qbank.used_questions[selected] = 1;
    return selected;
}

void display_question(Question *q, int qnum, int total)
{
    if (total > 0)
        printf("Question %d/%d\n", qnum, total);
    else
        printf("Question %d\n", qnum);

    printf("Category: %s\n", categories[q->category]);
    printf("%s\n\n", q->question);

    for (int i = 0; i < 4; i++)
    {
        printf("%d. %s\n", i + 1, q->options[i]);
    }
}

int get_answer_with_lifeline(Question *q, int *lifeline_used)
{
    char input[10];
    int answer;

    while (1)
    {
        printf("Answer (1-4%s): ", !(*lifeline_used) ? " or 5 for lifeline" : "");
        scanf("%s", input);

        if (is_back_command(input))
        {
            return -2;
        }

        answer = atoi(input);

        if (answer == 5 && !(*lifeline_used))
        {
            apply_lifeline(q);
            *lifeline_used = 1;
            display_question(q, 0, 0);
            printf("Answer (1-4): ");
            scanf("%s", input);

            if (is_back_command(input))
            {
                return -2;
            }

            answer = atoi(input);
        }

        if (answer >= 1 && answer <= 4)
        {
            return answer - 1;
        }
        else
        {
            printf("Invalid input! Please enter a number between 1-4.\n");
        }
    }
}

int get_answer_rapid(Question *q)
{
    char input[10];
    int answer;

    while (1)
    {
        printf("Answer (1-4): ");
        scanf("%s", input);

        if (is_back_command(input))
        {
            return -2;
        }

        answer = atoi(input);

        if (answer >= 1 && answer <= 4)
        {
            return answer - 1;
        }
        else
        {
            printf("Invalid input! Please enter a number between 1-4.\n");
        }
    }
}

void apply_lifeline(Question *q)
{
    printf("\n50-50 Lifeline activated!\n");

    int wrong[3], count = 0;
    for (int i = 0; i < 4; i++)
    {
        if (i != q->correct_answer)
            wrong[count++] = i;
    }

    shuffle_array(wrong, count);

    for (int i = 0; i < 2; i++)
    {
        strcpy(q->options[wrong[i]], "[ELIMINATED]");
    }

    printf("Two wrong answers eliminated!\n\n");
}

void shuffle_array(int arr[], int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

int is_back_command(char *input)
{
    if (strcasecmp(input, "back") == 0 || strcasecmp(input, "BACK") == 0)
    {
        return 1;
    }
    return 0;
}

void get_hidden_password(char *password, int max_length)
{
    int i = 0;
    char ch;
    
    #ifdef _WIN32
        
        while (i < max_length - 1)
        {
            ch = _getch();
            
            if (ch == '\r' || ch == '\n') 
            {
                break;
            }
            else if (ch == '\b' && i > 0) 
            {
                printf("\b \b"); 
                i--;
            }
            else if (ch != '\b') 
            {
                password[i] = ch;
                printf("*"); 
                i++;
            }
        }
        password[i] = '\0'; 
        printf("\n");
    #else
        
        
    struct termios oldt, newt;

   
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

  
    newt.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    
    while (i < max_length - 1) {
        ch = getchar();

        if (ch == '\n') {
            break;
        } else if (ch == 127 || ch == '\b') {
            if (i > 0) {
                printf("\b \b");
                i--;
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }

    password[i] = '\0';

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\n");
    #endif
}

void save_users()
{
    FILE *fp = fopen("users.dat", "wb");
    if (fp)
    {
        fwrite(&user_count, sizeof(int), 1, fp);
        fwrite(users, sizeof(User), user_count, fp);
        fclose(fp);
    }
}

void load_users()
{
    FILE *fp = fopen("users.dat", "rb");
    if (fp)
    {
        fread(&user_count, sizeof(int), 1, fp);
        fread(users, sizeof(User), user_count, fp);
        fclose(fp);
    }
}

void save_questions()
{
    FILE *fp = fopen("questions.dat", "wb");
    if (fp)
    {
        fwrite(&qbank.total_questions, sizeof(int), 1, fp);
        fwrite(qbank.questions, sizeof(Question), qbank.total_questions, fp);
        fclose(fp);
    }
}

void load_questions()
{

    FILE *fp = fopen("questions.dat", "rb");
    if (fp)
    {
        fread(&qbank.total_questions, sizeof(int), 1, fp);
        fread(qbank.questions, sizeof(Question), qbank.total_questions, fp);
        fclose(fp);
        printf("Loaded %d questions from database\n", qbank.total_questions);
        return;
    }

    load_questions_from_txt();
}

void load_questions_from_txt()
{
    FILE *file = fopen("questions.txt", "r");
    if (!file)
    {
        printf("No questions.txt found. Use admin panel to add questions.\n");
        return;
    }

    char line[500];
    int question_count = 0;

    while (fgets(line, sizeof(line), file) && question_count < MAX_QUESTIONS)
    {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0)
            continue;

        char *token = strtok(line, "|");
        if (!token)
            continue;

        Question *q = &qbank.questions[question_count];

        // Format: category|difficulty|question|option1|option2|option3|option4|correct_answer
        q->category = atoi(token);

        token = strtok(NULL, "|");
        if (!token)
            continue;
        q->difficulty = atoi(token);

        q->points = q->difficulty;
        q->time_limit = (q->difficulty == 1) ? 20 : (q->difficulty == 2) ? 30
                                                                         : 60;

        token = strtok(NULL, "|");
        if (!token)
            continue;
        strcpy(q->question, token);

        for (int i = 0; i < 4; i++)
        {
            token = strtok(NULL, "|");
            if (!token)
                break;
            strcpy(q->options[i], token);
        }

        token = strtok(NULL, "|");
        if (!token)
            continue;
        q->correct_answer = atoi(token) - 1; 

        question_count++;
    }

    fclose(file);
    qbank.total_questions = question_count;

    if (question_count > 0)
    {
        save_questions(); 
        printf("Imported %d questions from questions.txt\n", question_count);
    }
}

void clear_screen()
{
    system("cls || clear");
}

void wait_enter()
{
    printf("\nPress Enter to continue...");
    while (getchar() != '\n')
        ;
    getchar();
}

int check_time_limit()
{
    
    return 1;
}
