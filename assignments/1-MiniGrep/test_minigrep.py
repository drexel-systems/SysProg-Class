"""
Test suite for minigrep assignment

Usage:
    pytest test_minigrep.py -v                    # Run all tests with verbose output
    pytest test_minigrep.py -v -k "basic"         # Run only tests with "basic" in name
    pytest test_minigrep.py --tb=short            # Shorter traceback format
    pytest test_minigrep.py --json-report         # Generate JSON report for autograding
    pytest test_minigrep.py --html=report.html    # Generate HTML report

Point values are assigned via marks and can be totaled for grading.
"""

import subprocess
import pytest
import os
from pathlib import Path

# Test fixtures
@pytest.fixture(scope="session")
def executable():
    """Ensure the minigrep executable exists and is compiled"""
    exe_path = Path("./minigrep")
    if not exe_path.exists():
        pytest.fail("minigrep executable not found. Run 'make' first.")
    return "./minigrep"

@pytest.fixture(scope="session")
def test_files(tmp_path_factory):
    """Create test files for the test suite"""
    test_dir = tmp_path_factory.mktemp("test_data")
    
    # Test file 1: Basic error messages
    test1 = test_dir / "test1.txt"
    test1.write_text(
        "This is line one\n"
        "This is line two with ERROR\n"
        "Line three is here\n"
        "Another ERROR on line four\n"
        "Line five has a warning\n"
    )
    
    # Test file 2: Case variations
    test2 = test_dir / "test2.txt"
    test2.write_text(
        "Hello World\n"
        "hello world\n"
        "HELLO WORLD\n"
        "HeLLo WoRLd\n"
        "goodbye world\n"
    )
    
    # Test file 3: TODO markers
    test3 = test_dir / "test3.txt"
    test3.write_text(
        "TODO: implement feature\n"
        "FIXME: bug here\n"
        "TODO: add tests\n"
        "Nothing to see here\n"
        "TODO: refactor code\n"
    )
    
    # Empty file
    empty = test_dir / "empty.txt"
    empty.write_text("")
    
    # Long line file (for buffer testing)
    long_line = test_dir / "long_line.txt"
    long_line.write_text("a" * 300 + "\n" + "short line\n")
    
    return {
        "test1": str(test1),
        "test2": str(test2),
        "test3": str(test3),
        "empty": str(empty),
        "long_line": str(long_line),
        "dir": str(test_dir)
    }

def run_minigrep(executable, args):
    """Helper function to run minigrep and capture output"""
    result = subprocess.run(
        [executable] + args,
        capture_output=True,
        text=True
    )
    return result

# ============================================================================
# BASIC FUNCTIONALITY TESTS (5 points each)
# ============================================================================

@pytest.mark.points(2)
def test_no_arguments_shows_usage(executable):
    """Test that running with no arguments shows usage and returns error code"""
    result = run_minigrep(executable, [])
    assert result.returncode == 2, "Should exit with code 2 for missing arguments"
    assert "usage:" in result.stdout.lower(), "Should display usage message"

@pytest.mark.points(1)
def test_help_flag_shows_usage(executable):
    """Test that -h flag shows usage and exits successfully"""
    result = run_minigrep(executable, ["-h"])
    assert result.returncode == 0, "Help flag should exit with code 0"
    assert "usage:" in result.stdout.lower(), "Should display usage message"

@pytest.mark.points(1)
def test_missing_filename_shows_error(executable):
    """Test that missing filename shows error"""
    result = run_minigrep(executable, ["test"])
    assert result.returncode == 2, "Should exit with code 2 for missing filename"

@pytest.mark.points(2)
def test_nonexistent_file_shows_error(executable):
    """Test that nonexistent file returns error code 3"""
    result = run_minigrep(executable, ["test", "nonexistent_file_xyz.txt"])
    assert result.returncode == 3, "Should exit with code 3 for file errors"
    assert "error" in result.stdout.lower() or "cannot" in result.stdout.lower(), \
           "Should display error message"

@pytest.mark.points(5)
def test_basic_search_finds_matches(executable, test_files):
    """Test basic pattern matching returns correct lines"""
    result = run_minigrep(executable, ["ERROR", test_files["test1"]])
    assert result.returncode == 0, "Should exit with code 0 when pattern found"
    lines = result.stdout.strip().split('\n')
    # Should find exactly 2 lines with ERROR
    error_lines = [line for line in lines if "ERROR" in line]
    assert len(error_lines) == 2, f"Should find 2 lines with ERROR, found {len(error_lines)}"

@pytest.mark.points(2)
def test_basic_search_no_matches(executable, test_files):
    """Test that search with no matches returns exit code 1"""
    result = run_minigrep(executable, ["NOTFOUND", test_files["test1"]])
    assert result.returncode == 1, "Should exit with code 1 when pattern not found"
    assert result.stdout.strip() == "", "Should not print anything when no matches"

@pytest.mark.points(3)
def test_pattern_at_start_of_line(executable, test_files):
    """Test matching pattern at beginning of line"""
    result = run_minigrep(executable, ["This", test_files["test1"]])
    assert result.returncode == 0, "Should find pattern at start of lines"
    lines = result.stdout.strip().split('\n')
    assert len(lines) == 2, "Should find 2 lines starting with 'This'"

@pytest.mark.points(3)
def test_pattern_in_middle_of_line(executable, test_files):
    """Test matching pattern in middle of line (critical for correctness)"""
    result = run_minigrep(executable, ["line", test_files["test1"]])
    assert result.returncode == 0, "Should find pattern in middle of lines"
    lines = result.stdout.strip().split('\n')
    # 3 lines contain lowercase "line"
    assert len(lines) == 3, f"Should find 3 lines with 'line', found {len(lines)}"

@pytest.mark.points(2)
def test_pattern_at_end_of_line(executable, test_files):
    """Test matching pattern at end of line"""
    result = run_minigrep(executable, ["here", test_files["test1"]])
    assert result.returncode == 0, "Should find pattern at end of line"
    lines = result.stdout.strip().split('\n')
    assert len(lines) == 1, "Should find 1 line ending with 'here'"

@pytest.mark.points(1)
def test_empty_file_returns_not_found(executable, test_files):
    """Test that empty file returns code 1"""
    result = run_minigrep(executable, ["test", test_files["empty"]])
    assert result.returncode == 1, "Empty file should return 1 (not found)"

# ============================================================================
# LINE NUMBERS OPTION (-n) TESTS (5 points total)
# ============================================================================

@pytest.mark.points(5)
def test_line_numbers_option(executable, test_files):
    """Test -n flag shows line numbers"""
    result = run_minigrep(executable, ["-n", "ERROR", test_files["test1"]])
    assert result.returncode == 0, "Should find matches"
    lines = result.stdout.strip().split('\n')
    
    # Check that line numbers appear
    assert any("2:" in line for line in lines), "Should show line 2"
    assert any("4:" in line for line in lines), "Should show line 4"
    
    # Verify format is "number: content"
    for line in lines:
        assert ":" in line, "Each line should contain ':' separator"
        parts = line.split(":", 1)
        assert parts[0].strip().isdigit(), f"First part should be a number, got: {parts[0]}"

# ============================================================================
# CASE-INSENSITIVE OPTION (-i) TESTS (5 points total)
# ============================================================================

@pytest.mark.points(5)
def test_case_insensitive_search(executable, test_files):
    """Test -i flag performs case-insensitive matching"""
    result = run_minigrep(executable, ["-i", "hello", test_files["test2"]])
    assert result.returncode == 0, "Should find matches"
    lines = result.stdout.strip().split('\n')
    
    # Should match all 4 variations of "hello"
    assert len(lines) == 4, f"Should find 4 case variations of 'hello', found {len(lines)}"
    
    # Verify we got the expected variations
    output = result.stdout.lower()
    assert "hello world" in output, "Should match 'Hello World'"

@pytest.mark.points(3)
def test_case_sensitive_search_default(executable, test_files):
    """Test that default search is case-sensitive"""
    result = run_minigrep(executable, ["hello", test_files["test2"]])
    assert result.returncode == 0, "Should find exact match"
    lines = result.stdout.strip().split('\n')
    
    # Should only match exact case "hello world"
    assert len(lines) == 1, f"Should find only 1 exact match, found {len(lines)}"
    assert "hello world" in lines[0].lower(), "Should match lowercase 'hello world'"

# ============================================================================
# COUNT OPTION (-c) TESTS (5 points total)
# ============================================================================

@pytest.mark.points(3)
def test_count_option_with_matches(executable, test_files):
    """Test -c flag counts matches"""
    result = run_minigrep(executable, ["-c", "TODO", test_files["test3"]])
    assert result.returncode == 0, "Should find matches"
    
    # Should print count, not the lines themselves
    assert "Matches found: 3" in result.stdout or "3" in result.stdout, \
           "Should display count of 3 matches"
    
    # Should NOT print the actual lines
    assert "TODO:" not in result.stdout or result.stdout.count("TODO:") <= 1, \
           "Should not print actual matching lines with -c"

@pytest.mark.points(2)
def test_count_option_no_matches(executable, test_files):
    """Test -c flag with no matches"""
    result = run_minigrep(executable, ["-c", "NOTFOUND", test_files["test1"]])
    assert result.returncode == 1, "Should return 1 when no matches"
    assert "No matches found" in result.stdout or "0" in result.stdout, \
           "Should indicate no matches found"

# ============================================================================
# COMBINED FLAGS TESTS (3 points total)
# ============================================================================

@pytest.mark.points(2)
def test_combined_flags_in(executable, test_files):
    """Test -in flag (case-insensitive + line numbers)"""
    result = run_minigrep(executable, ["-in", "error", test_files["test1"]])
    assert result.returncode == 0, "Should find matches"
    lines = result.stdout.strip().split('\n')
    
    # Should find both ERROR lines case-insensitively
    assert len(lines) == 2, "Should find 2 lines"
    
    # Should have line numbers
    assert any("2:" in line for line in lines), "Should show line numbers"

@pytest.mark.points(1)
def test_combined_flags_ic(executable, test_files):
    """Test -ic flag (case-insensitive + count)"""
    result = run_minigrep(executable, ["-ic", "hello", test_files["test2"]])
    assert result.returncode == 0, "Should find matches"
    assert "4" in result.stdout, "Should count 4 case-insensitive matches"

# ============================================================================
# EXTRA CREDIT: INVERT MATCH (-v) TESTS (10 points total)
# ============================================================================

@pytest.mark.points(5)
@pytest.mark.extra_credit
def test_invert_match_basic(executable, test_files):
    """Test -v flag prints non-matching lines (EXTRA CREDIT)"""
    result = run_minigrep(executable, ["-v", "ERROR", test_files["test1"]])
    
    # If not implemented, might show usage or error
    if result.returncode != 0 and "not implemented" in result.stdout.lower():
        pytest.skip("Invert match not implemented (extra credit)")
    
    assert result.returncode == 0, "Should succeed"
    lines = result.stdout.strip().split('\n')
    
    # Should show 3 lines without ERROR
    assert len(lines) == 3, f"Should show 3 non-ERROR lines, got {len(lines)}"
    
    # Verify none of the output lines contain ERROR
    for line in lines:
        assert "ERROR" not in line, "Should not show lines with ERROR"

@pytest.mark.points(5)
@pytest.mark.extra_credit
def test_invert_match_with_count(executable, test_files):
    """Test -vc flag counts non-matching lines (EXTRA CREDIT)"""
    result = run_minigrep(executable, ["-vc", "ERROR", test_files["test1"]])
    
    if result.returncode != 0 and "not implemented" in result.stdout.lower():
        pytest.skip("Invert match not implemented (extra credit)")
    
    assert result.returncode == 0, "Should succeed"
    assert "3" in result.stdout, "Should count 3 non-ERROR lines"

# ============================================================================
# EDGE CASES AND ERROR HANDLING (2 points total)
# ============================================================================

@pytest.mark.points(1)
def test_single_character_pattern(executable, test_files):
    """Test searching for single character"""
    result = run_minigrep(executable, ["e", test_files["test1"]])
    assert result.returncode == 0, "Should find single character"
    lines = result.stdout.strip().split('\n')
    assert len(lines) >= 3, "Should find multiple lines containing 'e'"

@pytest.mark.points(1)
def test_wrong_option_flag(executable):
    """Test that invalid flag shows error"""
    result = run_minigrep(executable, ["-z", "pattern", "file.txt"])
    assert result.returncode == 2, "Invalid flag should return error code 2"

# ============================================================================
# UTILITY FUNCTIONS FOR GRADING
# ============================================================================

def pytest_configure(config):
    """Configure pytest with custom markers"""
    config.addinivalue_line("markers", "points(n): assign point value to test")
    config.addinivalue_line("markers", "extra_credit: mark test as extra credit")

def pytest_collection_modifyitems(config, items):
    """Add point values to test report"""
    for item in items:
        # Get points marker
        points_marker = item.get_closest_marker("points")
        if points_marker:
            points = points_marker.args[0]
            item.user_properties.append(("points", points))
        
        # Mark extra credit
        if item.get_closest_marker("extra_credit"):
            item.user_properties.append(("extra_credit", True))

@pytest.hookimpl(tryfirst=True, hookwrapper=True)
def pytest_runtest_makereport(item, call):
    """Add points info to test reports"""
    outcome = yield
    report = outcome.get_result()
    
    if report.when == "call":
        # Get point value
        points = dict(item.user_properties).get("points", 0)
        extra_credit = dict(item.user_properties).get("extra_credit", False)
        
        if report.passed:
            report.points_earned = points
        else:
            report.points_earned = 0
        
        report.points_possible = points
        report.is_extra_credit = extra_credit

def pytest_terminal_summary(terminalreporter, exitstatus, config):
    """Print point summary at end of test run"""
    passed = terminalreporter.stats.get('passed', [])
    failed = terminalreporter.stats.get('failed', [])
    
    total_points = 0
    earned_points = 0
    extra_credit_earned = 0
    extra_credit_possible = 0
    
    for report in passed + failed:
        if hasattr(report, 'points_possible'):
            if report.is_extra_credit:
                extra_credit_possible += report.points_possible
                extra_credit_earned += report.points_earned
            else:
                total_points += report.points_possible
                earned_points += report.points_earned
    
    terminalreporter.write_sep("=", "GRADING SUMMARY")
    terminalreporter.write_line(f"Base Points: {earned_points}/{total_points}")
    if extra_credit_possible > 0:
        terminalreporter.write_line(f"Extra Credit: {extra_credit_earned}/{extra_credit_possible}")
        terminalreporter.write_line(f"Total: {earned_points + extra_credit_earned}/{total_points + extra_credit_possible}")
    else:
        terminalreporter.write_line(f"Extra Credit: 0/15 (not attempted)")
    
    percentage = (earned_points / total_points * 100) if total_points > 0 else 0
    terminalreporter.write_line(f"Percentage: {percentage:.1f}%")
