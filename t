echo	bonjour

    Reads input: from file, string, or terminal.
    Tokenizes input: breaks into words/operators, applies alias expansion.
    Parses tokens: organizes tokens into commands.
    Performs expansions: variables, filenames, etc.
    Handles redirections: sets up file descriptors, removes redirection tokens.
    Executes command.
    Waits for completion and collects exit status (if needed).