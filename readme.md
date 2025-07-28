# Quiz Game 🎯

A comprehensive console-based quiz application written in C, featuring multiple game modes, user management, and an admin panel for question management.

## Features ✨

### 🎮 Game Modes


- **General Mode**: Choose 1-50 questions with customizable difficulty and category
- **Rapid Fire Mode**: 60-second challenge with medium difficulty questions

### 👤 User Management

- User registration and login system
- Score tracking and history
- High score records for both game modes
- Profile management with personal information

### 🔧 Admin Panel

- Add, view, and delete questions
- Import questions from text files
- User management (view, password reset, delete users)
- Comprehensive question bank management

### 📚 Question Categories

1. **General Knowledge (Nepal)**
2. **Geography (Nepal)**
3. **Science**
4. **Sports**
5. **Programming**

### 🎯 Difficulty Levels

- **Easy**: 20 seconds per question, 1 point
- **Medium**: 30 seconds per question, 2 points
- **Hard**: 60 seconds per question, 3 points
- **Mix**: Cycles through all difficulty levels

## Installation & Setup 🚀

### Prerequisites

- GCC compiler or any C compiler
- Terminal/Command prompt access

### Compilation

```bash
gcc -o quiz_game quiz_game.c
```

### Running the Application

```bash
./quiz_game
```

## File Structure 📁

```
quiz_game/
├── quiz_game.c          # Main source code
├── users.dat            # Binary file storing user data
├── questions.dat        # Binary file storing questions
├── questions.txt        # Text file for importing questions (optional)
└── README.md           # This file
```

## Usage Guide 📖

### First Time Setup

1. Compile and run the application
2. Access Admin Panel with password: `*********`
3. Add questions or import from `questions.txt`
4. Register users or create them via admin panel

### Admin Panel Access

- **Password**: `*********`
- **Features**:
  - Question management (add/view/delete)
  - User management
  - Import questions from text files

### User Registration

Users can register with:

- Full name
- Email address
- Phone number
- Password
- Custom username

### Game Modes

#### General Mode

1. Select number of questions (1-50)
2. Choose category
3. Select difficulty level
4. Use 50-50 lifeline once per game
5. Answer within time limits

#### Rapid Fire Mode

- 60-second time limit
- Medium difficulty questions
- Mixed categories
- No lifelines
- Review wrong answers at the end

## Question Format 📝

### Adding Questions via Admin Panel

Questions include:

- Category selection
- Difficulty level (Easy/Medium/Hard)
- Question text
- Four multiple-choice options
- Correct answer designation

### Importing from questions.txt

Format: `category|difficulty|question|option1|option2|option3|option4|correct_answer`

Example:

```
0|1|What is the capital of Nepal?|Kathmandu|Pokhara|Lalitpur|Bhaktapur|1
1|2|Which is the highest mountain in Nepal?|Everest|Annapurna|Manaslu|Dhaulagiri|1
```

**Category Codes**:

- 0: General Knowledge (Nepal)
- 1: Geography (Nepal)
- 2: Science
- 3: Sports
- 4: Programming

**Difficulty Codes**:

- 1: Easy
- 2: Medium
- 3: Hard

## Scoring System 🏆

### Points

- Easy questions: 1 point
- Medium questions: 2 points
- Hard questions: 3 points

### Records Tracked

- High scores for both game modes
- Total games played
- Last 50 scores
- Accuracy statistics

## Advanced Features 🎨

### Lifelines

- **50-50**: Eliminates two wrong answers (General Mode only)
- Available once per game

### Time Management

- Automatic time tracking
- Game ends if time limit exceeded
- Different time limits per difficulty

### Navigation

- Type `back` at any prompt to return to previous menu
- Consistent navigation throughout the application

## Data Persistence 💾

- **User data**: Stored in `users.dat` (binary format)
- **Questions**: Stored in `questions.dat` (binary format)
- **Automatic saving**: All changes saved immediately
- **Import capability**: Load questions from `questions.txt`

## Contact Information 📞

### Admin Support

If users forget passwords or need assistance:

- **Phone/WhatsApp**: +977-9866348028
- **Email**: dhakalsaurab1234@gmail.com

## Technical Specifications ⚙️

### Limits

- Maximum users: 100
- Maximum questions: 1000
- Username length: 25 characters
- Password length: 20 characters
- Question length: 300 characters
- Option length: 150 characters

### Data Structures

- **User**: Stores user information, scores, and game statistics
- **Question**: Contains question data, options, and metadata
- **QuestionBank**: Manages question collection and usage tracking

## Development Notes 🔧

### Memory Management

- Static array allocation for predictable memory usage
- Efficient binary file storage
- Minimal memory footprint

### Security Features

- Password-protected admin access
- Input validation throughout
- Safe string handling

### Error Handling

- Graceful handling of file operations
- Input validation with user feedback
- Automatic fallbacks for invalid inputs

## Future Enhancements 🚀

Potential improvements could include:

- Online multiplayer support
- Question difficulty balancing
- Enhanced statistics and analytics
- Category-specific leaderboards
- Timed tournament modes

---

**Enjoy the Quiz Game!** Test your knowledge across multiple categories and compete for high scores! 🎉
