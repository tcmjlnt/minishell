#!/bin/bash

# ============================================================================
# 🧪 TESTEUR COMPLET DES CODES DE SORTIE - MINISHELL
# ============================================================================

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
RESET='\033[0m'

# Counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Minishell executable
MINISHELL="./minishell"

# Log file
LOG_FILE="exit_status_test_results.log"

# ============================================================================
# 🛠️ UTILITY FUNCTIONS
# ============================================================================

print_header() {
    echo -e "\n${CYAN}============================================================================${RESET}"
    echo -e "${CYAN}🧪 $1${RESET}"
    echo -e "${CYAN}============================================================================${RESET}"
}

print_test() {
    echo -e "\n${BLUE}📋 Test:${RESET} $1"
    echo -e "${BLUE}📋 Expected exit code:${RESET} $2"
}

run_test() {
    local test_name="$1"
    local command="$2"
    local expected_code="$3"
    local timeout_duration="${4:-5}"

    TOTAL_TESTS=$((TOTAL_TESTS + 1))

    print_test "$test_name" "$expected_code"

    # Run command with timeout
    timeout "$timeout_duration" bash -c "echo '$command' | $MINISHELL"
    local actual_code=$?

    # Check if timeout occurred
    if [ $actual_code -eq 124 ]; then
        echo -e "${RED}❌ TIMEOUT${RESET} (>${timeout_duration}s)"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        echo "TIMEOUT: $test_name | Command: $command | Expected: $expected_code" >> "$LOG_FILE"
        return 1
    fi

    # Compare exit codes
    if [ $actual_code -eq $expected_code ]; then
        echo -e "${GREEN}✅ PASS${RESET} (exit code: $actual_code)"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        echo "PASS: $test_name | Command: $command | Expected: $expected_code | Actual: $actual_code" >> "$LOG_FILE"
    else
        echo -e "${RED}❌ FAIL${RESET} (expected: $expected_code, got: $actual_code)"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        echo "FAIL: $test_name | Command: $command | Expected: $expected_code | Actual: $actual_code" >> "$LOG_FILE"
    fi
}

run_signal_test() {
    local test_name="$1"
    local command="$2"
    local signal="$3"
    local expected_code="$4"

    TOTAL_TESTS=$((TOTAL_TESTS + 1))

    print_test "$test_name" "$expected_code"

    # Start minishell in background
    echo "$command" | $MINISHELL &
    local pid=$!

    # Wait a moment then send signal
    sleep 0.5
    kill -$signal $pid 2>/dev/null

    # Wait for process and get exit code
    wait $pid 2>/dev/null
    local actual_code=$?

    # Compare exit codes
    if [ $actual_code -eq $expected_code ]; then
        echo -e "${GREEN}✅ PASS${RESET} (exit code: $actual_code)"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        echo "PASS: $test_name | Signal: $signal | Expected: $expected_code | Actual: $actual_code" >> "$LOG_FILE"
    else
        echo -e "${RED}❌ FAIL${RESET} (expected: $expected_code, got: $actual_code)"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        echo "FAIL: $test_name | Signal: $signal | Expected: $expected_code | Actual: $actual_code" >> "$LOG_FILE"
    fi
}

# ============================================================================
# 🎯 TEST SUITES
# ============================================================================

test_success_cases() {
    print_header "SUCCESS CASES (Exit Code 0)"

    run_test "Empty command" "exit" 0
    run_test "Simple echo" "echo hello" 0
    run_test "Exit with 0" "exit 0" 0
    run_test "True command" "/bin/true" 0
    run_test "Valid builtin pwd" "pwd" 0
    run_test "Valid pipe" "echo hello | cat" 0
}

test_general_errors() {
    print_header "GENERAL ERRORS (Exit Code 1)"

    run_test "Invalid redirection" "echo hello > /dev/null/impossible" 1
    run_test "Exit too many args" "exit 1 2 3" 1
    run_test "CD to invalid dir" "cd /nonexistent/directory" 1
    run_test "Export invalid identifier" "export 123invalid=value" 1
}

test_builtin_misuse() {
    print_header "BUILTIN MISUSE (Exit Code 2)"

    run_test "Exit non-numeric" "exit hello" 2
    run_test "Exit with text" "exit abc123" 2
    run_test "Exit with special chars" "exit @#$" 2
}

test_cannot_execute() {
    print_header "CANNOT EXECUTE (Exit Code 126)"

    # Create test directory
    mkdir -p test_dir_temp 2>/dev/null

    run_test "Execute directory" "./test_dir_temp" 126
    run_test "Directory in PATH" "test_dir_temp" 126

    # Cleanup
    rmdir test_dir_temp 2>/dev/null
}

test_command_not_found() {
    print_header "COMMAND NOT FOUND (Exit Code 127)"

    run_test "Unknown command" "unknown_command_12345" 127
    run_test "Typo in command" "ehco hello" 127
    run_test "Empty string command" '""' 127
    run_test "Command with path" "/bin/nonexistent" 127
}

test_signal_exits() {
    print_header "SIGNAL EXITS (Exit Codes 130+)"

    # Note: These tests may be flaky depending on signal handling
    echo -e "${YELLOW}⚠️  Signal tests may require manual verification${RESET}"

    # SIGINT (Ctrl+C) = 130
    run_signal_test "SIGINT during sleep" "sleep 10" "INT" 130

    # SIGQUIT (Ctrl+\) = 131
    run_signal_test "SIGQUIT during sleep" "sleep 10" "QUIT" 131

    # SIGTERM = 143 (128 + 15)
    run_signal_test "SIGTERM during sleep" "sleep 10" "TERM" 143
}

test_edge_cases() {
    print_header "EDGE CASES"

    run_test "Exit with large number" "exit 999" 231  # 999 % 256 = 231
    run_test "Exit with negative" "exit -1" 255      # -1 % 256 = 255
    run_test "Exit with 256" "exit 256" 0            # 256 % 256 = 0
    run_test "False command" "/bin/false" 1
}

test_pipe_exit_codes() {
    print_header "PIPE EXIT CODES"

    run_test "Pipe with success" "echo hello | cat" 0
    run_test "Pipe with failure" "unknown_cmd | cat" 127
    run_test "Pipe last cmd fails" "echo hello | unknown_cmd" 127
    run_test "Pipe first cmd fails" "unknown_cmd | cat" 127
}

# ============================================================================
# 🚀 MAIN EXECUTION
# ============================================================================

main() {
    # Clear log file
    > "$LOG_FILE"

    echo -e "${MAGENTA}🚀 Starting Minishell Exit Status Tests${RESET}"
    echo -e "${MAGENTA}📝 Results will be logged to: $LOG_FILE${RESET}"

    # Check if minishell exists
    if [ ! -f "$MINISHELL" ]; then
        echo -e "${RED}❌ Error: $MINISHELL not found. Please compile first.${RESET}"
        echo -e "${YELLOW}💡 Run: make${RESET}"
        exit 1
    fi

    # Run all test suites
    test_success_cases
    test_general_errors
    test_builtin_misuse
    test_cannot_execute
    test_command_not_found
    test_signal_exits
    test_edge_cases
    test_pipe_exit_codes

    # Print summary
    print_header "TEST SUMMARY"
    echo -e "${BLUE}📊 Total tests:${RESET} $TOTAL_TESTS"
    echo -e "${GREEN}✅ Passed:${RESET} $PASSED_TESTS"
    echo -e "${RED}❌ Failed:${RESET} $FAILED_TESTS"

    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "\n${GREEN}🎉 ALL TESTS PASSED! 🎉${RESET}"
        exit 0
    else
        echo -e "\n${RED}😞 Some tests failed. Check $LOG_FILE for details.${RESET}"
        exit 1
    fi
}

# Run main if script is executed directly
if [ "${BASH_SOURCE[0]}" == "${0}" ]; then
    main "$@"
fi
