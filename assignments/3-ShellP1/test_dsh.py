#!/usr/bin/env python3
"""
Test suite for dsh (Drexel Shell) Part 1 - Command Line Parser
Converted to pytest for better error reporting and test organization
"""

import subprocess
import pytest


def run_dsh(commands):
    """
    Helper function to run dsh with a list of commands
    
    Args:
        commands: List of command strings to send to dsh
    
    Returns:
        (returncode, stdout, stderr)
    """
    # Join commands with newlines and add exit at the end
    input_text = '\n'.join(commands) + '\nexit\n'
    
    result = subprocess.run(
        ['./dsh'],
        input=input_text,
        capture_output=True,
        text=True
    )
    
    return result.returncode, result.stdout, result.stderr


def extract_output_lines(stdout, num_commands):
    """
    Extract the actual output lines (skip prompts and exit messages)
    
    Args:
        stdout: Full stdout from dsh
        num_commands: Number of commands sent
    
    Returns:
        List of output lines between prompts
    """
    lines = stdout.strip().split('\n')
    
    # Filter out prompt lines and exit messages
    output_lines = []
    for line in lines:
        if line.startswith('dsh>'):
            continue
        if line == 'exiting...':
            continue
        if line.startswith('cmd loop returned'):
            continue
        output_lines.append(line)
    
    return output_lines


class TestSingleCommands:
    """Test parsing of single commands without pipes"""
    
    def test_single_command_no_args(self):
        """Test: cmd"""
        returncode, stdout, stderr = run_dsh(['cmd'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 1' in stdout
        assert '<1> cmd' in stdout
        # Should NOT have brackets for commands with no args
        assert '[' not in stdout.split('\n')[1]  # Second line should be command
    
    def test_single_command_with_args(self):
        """Test: cmd_args a1 a2 -a3 --a4"""
        returncode, stdout, stderr = run_dsh(['cmd_args a1 a2 -a3 --a4'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 1' in stdout
        assert '<1> cmd_args [a1 a2 -a3 --a4]' in stdout
    
    def test_command_with_single_arg(self):
        """Test: ls -l"""
        returncode, stdout, stderr = run_dsh(['ls -l'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 1' in stdout
        assert '<1> ls [-l]' in stdout
    
    def test_command_with_path_arg(self):
        """Test: cd /tmp"""
        returncode, stdout, stderr = run_dsh(['cd /tmp'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 1' in stdout
        assert '<1> cd [/tmp]' in stdout


class TestPipedCommands:
    """Test parsing of piped commands"""
    
    def test_two_commands_pipe(self):
        """Test: cmd1 | cmd2"""
        returncode, stdout, stderr = run_dsh(['cmd1 | cmd2'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 2' in stdout
        assert '<1> cmd1' in stdout
        assert '<2> cmd2' in stdout
    
    def test_three_commands_pipe(self):
        """Test: cmd1 | cmd2 | cmd3"""
        returncode, stdout, stderr = run_dsh(['cmd1 | cmd2 | cmd3'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 3' in stdout
        assert '<1> cmd1' in stdout
        assert '<2> cmd2' in stdout
        assert '<3> cmd3' in stdout
    
    def test_pipe_with_args(self):
        """Test: cmda1 a1 a2 | cmda2 a3 a4 | cmd3"""
        returncode, stdout, stderr = run_dsh(['cmda1 a1 a2 | cmda2 a3 a4 | cmd3'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 3' in stdout
        assert '<1> cmda1 [a1 a2]' in stdout
        assert '<2> cmda2 [a3 a4]' in stdout
        assert '<3> cmd3' in stdout
    
    def test_max_pipes(self):
        """Test: c1 | c2 | c3 | c4 | c5 | c6 | c7 | c8 (8 commands = max)"""
        returncode, stdout, stderr = run_dsh(['c1 | c2 | c3 | c4 | c5 | c6 | c7 | c8'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 8' in stdout
        # Check all 8 commands are listed
        for i in range(1, 9):
            assert f'<{i}> c{i}' in stdout


class TestPipeSpacing:
    """Test that pipes work with various spacing patterns"""
    
    def test_pipe_no_spaces(self):
        """Test: pipe1|pipe2|pipe3|pipe4"""
        returncode, stdout, stderr = run_dsh(['pipe1|pipe2|pipe3|pipe4'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 4' in stdout
        assert '<1> pipe1' in stdout
        assert '<2> pipe2' in stdout
        assert '<3> pipe3' in stdout
        assert '<4> pipe4' in stdout
    
    def test_pipe_mixed_spacing(self):
        """Test: pipe1|pipe2 |pipe3 pipe4| pipe5"""
        returncode, stdout, stderr = run_dsh(['pipe1|pipe2 |pipe3 pipe4| pipe5'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 4' in stdout
        assert '<1> pipe1' in stdout
        assert '<2> pipe2' in stdout
        # pipe3 and pipe4 should be parsed as cmd with arg
        assert '<3> pipe3 [pipe4]' in stdout
        assert '<4> pipe5' in stdout
    
    def test_pipe_extra_spaces(self):
        """Test: cmd1  |  cmd2   |   cmd3"""
        returncode, stdout, stderr = run_dsh(['cmd1  |  cmd2   |   cmd3'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 3' in stdout
        assert '<1> cmd1' in stdout
        assert '<2> cmd2' in stdout
        assert '<3> cmd3' in stdout


class TestErrorCases:
    """Test error handling"""
    
    def test_empty_command(self):
        """Test: (empty line)"""
        returncode, stdout, stderr = run_dsh([''])
        
        assert returncode == 0
        assert 'warning: no commands provided' in stdout
    
    def test_whitespace_only(self):
        """Test: (spaces only)"""
        returncode, stdout, stderr = run_dsh(['   '])
        
        assert returncode == 0
        assert 'warning: no commands provided' in stdout
    
    def test_too_many_pipes(self):
        """Test: c1|c2|c3|c4|c5|c6|c7|c8|c9 (9 commands > max 8)"""
        returncode, stdout, stderr = run_dsh(['c1|c2|c3|c4|c5|c6|c7|c8|c9'])
        
        assert returncode == 0
        assert 'error: piping limited to 8 commands' in stdout


class TestExitCommand:
    """Test exit command functionality"""
    
    def test_exit_command(self):
        """Test that exit command terminates shell"""
        returncode, stdout, stderr = run_dsh([])  # Just sends 'exit'
        
        assert returncode == 0
        assert 'exiting...' in stdout
        assert 'cmd loop returned 0' in stdout
    
    def test_commands_before_exit(self):
        """Test commands work before exit"""
        returncode, stdout, stderr = run_dsh(['cmd1', 'cmd2'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 1' in stdout
        assert '<1> cmd1' in stdout
        assert '<1> cmd2' in stdout  # Second command
        assert 'exiting...' in stdout


class TestRealCommands:
    """Test parsing of real Linux commands"""
    
    def test_ls_command(self):
        """Test: ls"""
        returncode, stdout, stderr = run_dsh(['ls'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 1' in stdout
        assert '<1> ls' in stdout
    
    def test_grep_pipe(self):
        """Test: ls | grep txt"""
        returncode, stdout, stderr = run_dsh(['ls | grep txt'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 2' in stdout
        assert '<1> ls' in stdout
        assert '<2> grep [txt]' in stdout
    
    def test_complex_pipe(self):
        """Test: ls -la | grep txt | wc -l"""
        returncode, stdout, stderr = run_dsh(['ls -la | grep txt | wc -l'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 3' in stdout
        assert '<1> ls [-la]' in stdout
        assert '<2> grep [txt]' in stdout
        assert '<3> wc [-l]' in stdout


class TestEdgeCases:
    """Test edge cases and special scenarios"""
    
    def test_multiple_spaces_in_args(self):
        """Test that multiple spaces between args are handled"""
        returncode, stdout, stderr = run_dsh(['cmd    arg1    arg2'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 1' in stdout
        # Should collapse multiple spaces
        assert '<1> cmd [arg1 arg2]' in stdout
    
    def test_leading_spaces(self):
        """Test:    cmd (leading spaces)"""
        returncode, stdout, stderr = run_dsh(['   cmd'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 1' in stdout
        assert '<1> cmd' in stdout
    
    def test_trailing_spaces(self):
        """Test: cmd    (trailing spaces)"""
        returncode, stdout, stderr = run_dsh(['cmd   '])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 1' in stdout
        assert '<1> cmd' in stdout
    
    def test_mixed_args_and_flags(self):
        """Test: cmd -a --flag arg1 -b arg2"""
        returncode, stdout, stderr = run_dsh(['cmd -a --flag arg1 -b arg2'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 1' in stdout
        assert '<1> cmd [-a --flag arg1 -b arg2]' in stdout


class TestMultipleCommands:
    """Test sending multiple commands in sequence"""
    
    def test_two_separate_commands(self):
        """Test sending two commands in sequence (not piped)"""
        returncode, stdout, stderr = run_dsh(['cmd1', 'cmd2'])
        
        assert returncode == 0
        # Should see two separate parsed outputs
        assert stdout.count('PARSED COMMAND LINE - TOTAL COMMANDS 1') == 2
        assert '<1> cmd1' in stdout
        assert '<1> cmd2' in stdout
    
    def test_command_then_empty_then_command(self):
        """Test: cmd1, (empty), cmd2"""
        returncode, stdout, stderr = run_dsh(['cmd1', '', 'cmd2'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 1' in stdout
        assert 'warning: no commands provided' in stdout
        assert '<1> cmd1' in stdout
        assert '<1> cmd2' in stdout


class TestQuoteHandling:
    """Test quote handling (if implemented)"""
    
    def test_double_quoted_string(self):
        """Test: echo "hello world" """
        returncode, stdout, stderr = run_dsh(['echo "hello world"'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 1' in stdout
        # Quotes should be removed, space preserved as single arg
        assert '<1> echo [hello world]' in stdout
    
    def test_single_quoted_string(self):
        """Test: echo 'hello world' """
        returncode, stdout, stderr = run_dsh(["echo 'hello world'"])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 1' in stdout
        assert '<1> echo [hello world]' in stdout
    
    def test_quoted_empty_string(self):
        """Test: cmd "" arg2"""
        returncode, stdout, stderr = run_dsh(['cmd "" arg2'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 1' in stdout
        # Empty quoted string should still be an argument
        # Implementation may vary - this tests if they handle it
    
    def test_multiple_quoted_args(self):
        """Test: cmd "arg 1" "arg 2" """
        returncode, stdout, stderr = run_dsh(['cmd "arg 1" "arg 2"'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 1' in stdout
        # Both quoted strings should be preserved with spaces


class TestLongCommandLines:
    """Test commands with many arguments"""
    
    def test_many_arguments(self):
        """Test command with 7 arguments (max allowed)"""
        args = ' '.join([f'arg{i}' for i in range(7)])
        returncode, stdout, stderr = run_dsh([f'cmd {args}'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 1' in stdout
        assert '<1> cmd' in stdout
        # Should have all 7 arguments
        for i in range(7):
            assert f'arg{i}' in stdout
    
    def test_very_long_single_argument(self):
        """Test command with one very long argument"""
        # Create a single long argument
        long_arg = 'a' * 100
        returncode, stdout, stderr = run_dsh([f'cmd {long_arg}'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 1' in stdout
        assert '<1> cmd' in stdout
        assert long_arg in stdout
    
    def test_command_with_long_path(self):
        """Test command with very long path argument"""
        long_path = '/very/long/path/to/some/directory/structure/file.txt'
        returncode, stdout, stderr = run_dsh([f'cat {long_path}'])
        
        assert returncode == 0
        assert 'PARSED COMMAND LINE - TOTAL COMMANDS 1' in stdout
        assert f'<1> cat [{long_path}]' in stdout


class TestExtraCredit:
    """Test extra credit dragon command - informational only"""
    
    def test_dragon_extra_credit_check(self):
        """Check if dragon extra credit is implemented (does not affect pass/fail)"""
        returncode, stdout, stderr = run_dsh(['dragon'])
        
        # Look for dragon art indicators (@ or % characters common in ASCII art)
        # or check if it prints something other than parsed command
        has_dragon_art = ('@' in stdout or '%' in stdout) and 'PARSED COMMAND LINE' not in stdout
        
        if has_dragon_art:
            pytest.skip("✓ Extra credit implemented: Dragon command works! (+5 points)")
        else:
            pytest.skip("○ Extra credit not implemented: Dragon command missing (0 extra points)")


if __name__ == "__main__":
    # Run pytest when script is executed directly
    pytest.main([__file__, "-v", "--tb=short"])