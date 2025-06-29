# Quiz Game System Design

## Overview
This is a comprehensive C-based quiz game system with user management, question banking, and multiple game modes. The system provides both administrative and user interfaces with persistent data storage.

## System Architecture Diagram

```mermaid
graph TB
    subgraph "User Interface Layer"
        MM[Main Menu]
        UP[User Panel]
        AP[Admin Panel]
    end
    
    subgraph "Authentication System"
        LOGIN[Login System]
        REG[Registration System]
        PASS[Password Reset]
    end
    
    subgraph "Game Engine"
        GM[General Mode]
        RF[Rapid Fire Mode]
        QE[Question Engine]
        SE[Scoring Engine]
        TE[Timer Engine]
    end
    
    subgraph "Data Management"
        UM[User Management]
        QM[Question Management]
        SM[Score Management]
    end
    
    subgraph "File System"
        UF[(users.dat)]
        QF[(questions.dat)]
        TF[questions.txt]
    end
    
    MM --> UP
    MM --> AP
    UP --> LOGIN
    UP --> REG
    LOGIN --> GM
    LOGIN --> RF
    AP --> PASS
    AP --> QM
    GM --> QE
    RF --> QE
    QE --> SE
    GM --> TE
    RF --> TE
    UM --> UF
    QM --> QF
    QM --> TF
    SM --> UF
```

## Data Structure Design

```mermaid
erDiagram
    USER {
        string username
        string password
        string name
        string email
        string phone
        int general_high_score
        int rapid_high_score
        int total_general_games
        int total_rapid_games
        int scores_array
        int score_count
    }
    
    QUESTION {
        string question_text
        string options_array
        int correct_answer
        int difficulty
        int category
        int points
        int time_limit
    }
    
    QUESTION_BANK {
        question questions_array
        int total_questions
        int used_questions_array
        int used_count
    }
    
    USER ||--o{ GAME_SESSION : plays
    QUESTION_BANK ||--o{ QUESTION : contains
    GAME_SESSION }o--|| QUESTION : uses
```

## User Flow Diagram

```mermaid
flowchart TD
    START([Start Application]) --> MAIN{Main Menu}
    
    MAIN -->|1| ADMIN[Admin Panel]
    MAIN -->|2| USER[User Panel]
    MAIN -->|3| EXIT([Exit])
    
    ADMIN --> AUTH{Admin Authentication}
    AUTH -->|Success| ADMIN_MENU[Admin Functions]
    AUTH -->|Fail| MAIN
    
    ADMIN_MENU --> ADD_Q[Add Question]
    ADMIN_MENU --> VIEW_Q[View Questions]
    ADMIN_MENU --> DEL_Q[Delete Question]
    ADMIN_MENU --> IMPORT[Import Questions]
    ADMIN_MENU --> VIEW_U[View Users]
    ADMIN_MENU --> RESET_P[Reset Password]
    ADMIN_MENU --> DEL_U[Delete User]
    
    USER --> USER_AUTH{User Login/Register}
    USER_AUTH -->|Login| LOGIN[Login Process]
    USER_AUTH -->|Register| REGISTER[Registration Process]
    USER_AUTH -->|Back| MAIN
    
    LOGIN -->|Success| USER_MENU[User Dashboard]
    LOGIN -->|Fail| USER
    REGISTER --> USER
    
    USER_MENU --> GENERAL[General Mode]
    USER_MENU --> RAPID[Rapid Fire Mode]
    USER_MENU --> SCORES[View Scores]
    USER_MENU --> RULES[Game Rules]
    USER_MENU --> LOGOUT[Logout]
    
    GENERAL --> GAME_CONFIG[Configure Game]
    GAME_CONFIG --> PLAY_GENERAL[Play General Mode]
    PLAY_GENERAL --> RESULTS[Show Results]
    
    RAPID --> PLAY_RAPID[Play Rapid Fire]
    PLAY_RAPID --> RAPID_RESULTS[Show Results]
    
    RESULTS --> USER_MENU
    RAPID_RESULTS --> USER_MENU
    SCORES --> USER_MENU
    RULES --> USER_MENU
    LOGOUT --> MAIN
```

## Game Mode Flow

```mermaid
sequenceDiagram
    participant U as User
    participant S as System
    participant Q as Question Bank
    participant T as Timer
    participant SC as Score Calculator
    
    Note over U,SC: General Mode Flow
    U->>S: Select General Mode
    S->>U: Request Configuration
    U->>S: Provide (questions, category, difficulty)
    S->>Q: Get Random Questions
    Q->>S: Return Question Set
    
    loop For Each Question
        S->>U: Display Question
        S->>T: Start Timer
        U->>S: Provide Answer (or lifeline)
        T->>S: Check Time Limit
        alt Time Not Exceeded
            S->>SC: Calculate Score
            SC->>S: Return Points
            S->>U: Show Result
        else Time Exceeded
            S->>U: Game Over (Time's Up)
        end
    end
    
    S->>U: Display Final Results
    S->>S: Update User Stats
```

## File System Architecture

```mermaid
graph LR
    subgraph "Application Files"
        MAIN[quiz_game.c]
        EXE[quiz_game.exe]
    end
    
    subgraph "Data Files"
        USERS[users.dat<br/>Binary User Data]
        QUESTIONS[questions.dat<br/>Binary Question Data]
        IMPORT[questions.txt<br/>Text Import File]
    end
    
    subgraph "File Operations"
        SAVE[Save Operations]
        LOAD[Load Operations]
        IMPORT_OP[Import Operations]
    end
    
    EXE --> SAVE
    EXE --> LOAD
    EXE --> IMPORT_OP
    
    SAVE --> USERS
    SAVE --> QUESTIONS
    LOAD --> USERS
    LOAD --> QUESTIONS
    IMPORT_OP --> IMPORT
    IMPORT_OP --> QUESTIONS
```

## Security & Access Control

```mermaid
graph TD
    subgraph "Access Levels"
        ADMIN[Admin Level<br/>Password: messigoat]
        USER[User Level<br/>Username/Password]
        GUEST[Guest Level<br/>Registration Only]
    end
    
    subgraph "Admin Permissions"
        A1[Add/Delete Questions]
        A2[View All Users]
        A3[Reset User Passwords]
        A4[Delete Users]
        A5[Import Questions]
    end
    
    subgraph "User Permissions"
        U1[Play Games]
        U2[View Own Scores]
        U3[Update Profile]
        U4[View Game Rules]
    end
    
    ADMIN --> A1
    ADMIN --> A2
    ADMIN --> A3
    ADMIN --> A4
    ADMIN --> A5
    
    USER --> U1
    USER --> U2
    USER --> U3
    USER --> U4
    
    GUEST --> REG[Registration Only]
```

## Key Features

### User Management
- User registration with validation
- Secure login system
- Password reset functionality (admin only)
- User profile management
- Score tracking and history

### Question Management
- Multiple categories (General Knowledge, Geography-Nepal, Science, Sports, Programming)
- Three difficulty levels (Easy, Medium, Hard)
- Dynamic question import from text files
- Question CRUD operations (admin only)

### Game Modes
1. **General Mode**
   - Configurable number of questions (1-50)
   - Category and difficulty selection
   - Time limits based on difficulty
   - 50-50 lifeline feature
   - Points system based on difficulty

2. **Rapid Fire Mode**
   - 60-second time limit
   - Medium difficulty questions
   - Mixed categories
   - Rapid scoring system
   - Wrong answer review

### Data Persistence
- Binary file storage for performance
- Automatic backup and recovery
- Import/export capabilities
- Score history tracking

## Technical Specifications

- **Language**: C
- **Data Storage**: Binary files (.dat) and text files (.txt)
- **Memory Management**: Static arrays with defined limits
- **Timer System**: System time-based timing
- **Random Generation**: Seeded random number generation
- **Cross-platform**: Compatible with Windows and Unix systems

## Contact Information
- **SMS/WhatsApp**: +977-9866348028
- **Email**: dhakalsaurab1234@gmail.com