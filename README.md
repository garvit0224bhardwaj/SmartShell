# SmartShell

SmartShell is a custom shell implementation written in C, featuring basic shell commands, directory navigation, and AI integration using the Llama model. It also includes a web interface built with Flask for interactive command execution and a visually appealing terminal experience.

## Features

- Built-in commands: `cd`, `ls`, `pwd`, `help`, `exit`
- AI prompt support: `ai: <prompt>` (runs Llama model)
- Directory navigation and management
- Web-based terminal UI (Flask + HTML/CSS/JS)
- Customizable and extendable

## How to Run

### Native Shell (C)

1. Open the shell implementation folder.
2. Build the shell using the Makefile:
    ```sh
    make
    ```
3. Run the shell:
    ```sh
    ./mysh
    ```

### Web Interface (Python/Flask)

1. Open the `app/static` folder.
2. Start the Flask server:
    ```sh
    python app.py
    ```
3. Open the provided link in your browser (usually `http://127.0.0.1:5000/`).
