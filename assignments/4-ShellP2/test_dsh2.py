#!/usr/bin/env python3
"""
Test suite for dsh (Drexel Shell) Part 2 - Fork/Exec
Tests actual command execution, cd command, and quote handling
"""

import subprocess
import pytest
import os
import tempfile
import re

def run_dsh(commands, timeout=5):
    """
    Helper function to run dsh with a list of commands
    
    Args:
        commands: List of command strings to send to dsh
        timeout: Timeout in seconds
    
    Returns:
        (returncode, stdout, stderr)
    """
    # Join commands with newlines and add exit at the end
    input_text = '\n'.join(commands) + '\nexit\n'
    
    try:
        result = subprocess.run(
            ['./dsh'],
            input=input_text,
            capture_output=True,
            text=True,
            timeout=timeout
        )
        return result.returncode, result.stdout, result.stderr
    except subprocess.TimeoutExpired:
        return -1, "", "TIMEOUT"


def clean_output(stdout):
    """Remove prompts and exit messages from output"""
    # Remove all prompt patterns (dsh, dsh2, dsh3, etc.)
    cleaned = re.sub(r'dsh\d*>\s*', '', stdout)
    
    # Remove exit messages
    cleaned = re.sub(r'exiting\.\.\.', '', cleaned)
    cleaned = re.sub(r'cmd loop returned \d+', '', cleaned)
    
    # Clean up extra whitespace and empty lines
    lines = [line.strip() for line in cleaned.strip().split('\n') if line.strip()]
    return '\n'.join(lines)


class TestBasicExecution:
    """Test basic command execution with fork/exec"""
    
    def test_simple_command_uname(self):
        """Test: uname (should print OS name)"""
        returncode, stdout, stderr = run_dsh(['uname'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'Linux' in output or 'Darwin' in output  # Linux or macOS
    
    def test_command_with_args(self):
        """Test: uname -s (should print kernel name)"""
        returncode, stdout, stderr = run_dsh(['uname -s'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'Linux' in output or 'Darwin' in output
    
    def test_echo_simple(self):
        """Test: echo hello"""
        returncode, stdout, stderr = run_dsh(['echo hello'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello' in output
    
    def test_echo_multiple_args(self):
        """Test: echo hello world"""
        returncode, stdout, stderr = run_dsh(['echo hello world'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello world' in output


class TestQuoteHandling:
    """Test quote handling in command parsing"""
    
    def test_double_quotes_with_spaces(self):
        """Test: echo "hello world" (spaces preserved)"""
        returncode, stdout, stderr = run_dsh(['echo "hello world"'])
        
        assert returncode == 0
        output = clean_output(stdout)
        # Should be one argument, so "hello world" together
        assert 'hello world' in output
    
    def test_double_quotes_multiple_spaces(self):
        """Test: echo "hello     world" (multiple spaces preserved)"""
        returncode, stdout, stderr = run_dsh(['echo "hello     world"'])
        
        assert returncode == 0
        output = clean_output(stdout)
        # Multiple spaces should be preserved inside quotes
        assert 'hello     world' in output
    
    def test_single_quotes(self):
        """Test: echo 'hello world'"""
        returncode, stdout, stderr = run_dsh(["echo 'hello world'"])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello world' in output


class TestCdCommand:
    """Test cd built-in command"""
    
    def test_cd_to_tmp(self):
        """Test: cd /tmp, then pwd"""
        returncode, stdout, stderr = run_dsh(['cd /tmp', 'pwd'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert '/tmp' in output
    
    def test_cd_relative(self):
        """Test: cd with relative path"""
        # Create a temp directory for testing
        returncode, stdout, stderr = run_dsh(['pwd', 'cd ..', 'pwd'])
        
        assert returncode == 0
        lines = clean_output(stdout).split('\n')
        # Second pwd should be parent of first pwd
        assert len(lines) >= 2
    
    def test_cd_no_args(self):
        """Test: cd with no arguments (should do nothing)"""
        returncode, stdout, stderr = run_dsh(['pwd', 'cd', 'pwd'])
        
        assert returncode == 0
        lines = clean_output(stdout).split('\n')
        # Both pwd commands should show same directory
        if len(lines) >= 2:
            assert lines[0] == lines[-1] or lines[0] in lines[-1]


class TestPwd:
    """Test pwd command (tests that commands actually execute)"""
    
    def test_pwd_basic(self):
        """Test: pwd (should print current directory)"""
        returncode, stdout, stderr = run_dsh(['pwd'])
        
        assert returncode == 0
        output = clean_output(stdout)
        # Should contain a path with /
        assert '/' in output


class TestLs:
    """Test ls command"""
    
    def test_ls_basic(self):
        """Test: ls (should list files)"""
        returncode, stdout, stderr = run_dsh(['ls'])
        
        assert returncode == 0
        output = clean_output(stdout)
        # Should see some files (dsh, makefile, etc.)
        assert len(output) > 0
    
    def test_ls_with_args(self):
        """Test: ls -la (should list with details)"""
        returncode, stdout, stderr = run_dsh(['ls -la'])
        
        assert returncode == 0
        output = clean_output(stdout)
        # ls -la should show detailed listing
        assert len(output) > 0


class TestCommandNotFound:
    """Test handling of commands that don't exist"""
    
    def test_nonexistent_command(self):
        """Test: notacommand (should handle gracefully)"""
        returncode, stdout, stderr = run_dsh(['notacommand'])
        
        # Shell should continue after error, still exit successfully
        assert returncode == 0
        # Error might be in stdout or stderr
        output = stdout + stderr
        # Should not crash


class TestMultipleCommands:
    """Test multiple commands in sequence"""
    
    def test_two_commands(self):
        """Test: echo hello, then echo world"""
        returncode, stdout, stderr = run_dsh(['echo hello', 'echo world'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello' in output
        assert 'world' in output
    
    def test_command_sequence(self):
        """Test: pwd, cd /tmp, pwd"""
        returncode, stdout, stderr = run_dsh(['pwd', 'cd /tmp', 'pwd'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert '/tmp' in output


class TestExitCommand:
    """Test exit command"""
    
    def test_exit_basic(self):
        """Test: exit (should terminate cleanly)"""
        returncode, stdout, stderr = run_dsh([])
        
        assert returncode == 0
        assert 'exiting...' in stdout
        assert 'cmd loop returned 0' in stdout
    
    def test_commands_before_exit(self):
        """Test: commands run before exit"""
        returncode, stdout, stderr = run_dsh(['echo hello', 'echo world'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello' in output
        assert 'world' in output
        assert 'exiting...' in stdout


class TestWhitespace:
    """Test whitespace handling"""
    
    def test_multiple_spaces_between_args(self):
        """Test: echo    hello    world (multiple spaces)"""
        returncode, stdout, stderr = run_dsh(['echo    hello    world'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello' in output
        assert 'world' in output
    
    def test_leading_whitespace(self):
        """Test:    echo hello (leading spaces)"""
        returncode, stdout, stderr = run_dsh(['   echo hello'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello' in output
    
    def test_trailing_whitespace(self):
        """Test: echo hello    (trailing spaces)"""
        returncode, stdout, stderr = run_dsh(['echo hello   '])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello' in output


class TestComplexCommands:
    """Test more complex command scenarios"""
    
    def test_command_with_many_args(self):
        """Test: echo with many arguments"""
        returncode, stdout, stderr = run_dsh(['echo one two three four five'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'one' in output
        assert 'five' in output
    
    def test_command_with_flags(self):
        """Test: ls -l -a"""
        returncode, stdout, stderr = run_dsh(['ls -l -a'])
        
        assert returncode == 0
        # Just verify it runs without crashing


class TestFileCreation:
    """Test commands that create files"""
    
    def test_touch_command(self):
        """Test: touch testfile (creates file)"""
        # Use temp directory
        with tempfile.TemporaryDirectory() as tmpdir:
            testfile = os.path.join(tmpdir, 'testfile.txt')
            
            returncode, stdout, stderr = run_dsh([
                f'cd {tmpdir}',
                'touch testfile.txt',
                'ls'
            ])
            
            assert returncode == 0
            # File should be created
            # Can't easily check from here, but command should succeed


class TestCat:
    """Test cat command"""
    
    def test_cat_with_file(self):
        """Test: cat on existing file"""
        # Cat the makefile or readme
        returncode, stdout, stderr = run_dsh(['cat makefile'])
        
        # Should succeed (file exists) or fail gracefully (file doesn't exist)
        assert returncode == 0


class TestEmptyCommand:
    """Test empty command handling"""
    
    def test_empty_line(self):
        """Test: empty line should be handled gracefully"""
        returncode, stdout, stderr = run_dsh(['', 'echo hello'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello' in output


class TestStress:
    """Stress tests for shell"""
    
    def test_many_commands(self):
        """Test: many commands in sequence"""
        commands = ['echo test'] * 10
        returncode, stdout, stderr = run_dsh(commands)
        
        assert returncode == 0
        output = clean_output(stdout)
        assert output.count('test') >= 10
    
    def test_long_command_line(self):
        """Test: long command with many arguments"""
        args = ' '.join([f'arg{i}' for i in range(7)])  # Reasonable number
        returncode, stdout, stderr = run_dsh([f'echo {args}'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'arg0' in output
        assert 'arg6' in output

class TestPipeNotSupported:
    """Verify pipes are NOT implemented in Part 2"""
    
    def test_pipe_input_treated_as_single_command(self):
        """Test: ls | grep (should treat as single malformed command or just run ls)"""
        returncode, stdout, stderr = run_dsh(['ls | grep test'])
        
        # Should either fail or run as single command
        # Should NOT execute pipeline
        assert returncode == 0  # Shell doesn't crash

class TestExtraCredit:
    """Test extra credit features (if implemented)"""
    
    def test_rc_command_after_success(self):
        """Test: rc after successful command"""
        returncode, stdout, stderr = run_dsh(['echo hello', 'rc'])
        
        if 'rc not implemented' in stdout:
            pytest.skip("rc command not implemented")
        
        output = clean_output(stdout)
        print(f"Stdout for rc after failure: {stdout}\n")
        print(f"Output for rc after failure: {output}\n")
        assert '0' in output  # Success code
    
    def test_rc_command_after_failure(self):
        """Test: rc after failed command"""
        returncode, stdout, stderr = run_dsh(['notacommand', 'rc'])
        
        if 'rc not implemented' in stdout:
            pytest.skip("rc command not implemented")
        
        output = clean_output(stdout)
        # Should show non-zero error code
        assert '127' in output or '126' in output or output.strip() != '0'

class TestStraceDeliverable:
    """Verify strace analysis file exists"""
    
    def test_strace_analysis_exists(self):
        """Test: strace-fork-exec-analysis.md exists"""
        import os
        assert os.path.exists('strace-fork-exec-analysis.md'), \
            "Missing required strace-fork-exec-analysis.md file"


if __name__ == "__main__":
    # Run pytest when script is executed directly
    pytest.main([__file__, "-v", "--tb=short"])
