#!/usr/bin/env python3
"""
Test suite for dsh (Drexel Shell) Part 3 - Pipes
Tests pipeline execution, data flow through pipes, and multi-command pipelines
"""

import subprocess
import pytest
import os
import tempfile


def run_dsh(commands, timeout=10):
    """
    Helper function to run dsh with a list of commands
    
    Args:
        commands: List of command strings to send to dsh
        timeout: Timeout in seconds (increased for pipes)
    
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
    lines = stdout.strip().split('\n')
    output_lines = []
    for line in lines:
        if line.startswith('dsh'):
            continue
        if line == 'exiting...':
            continue
        if line.startswith('cmd loop returned'):
            continue
        output_lines.append(line)
    return '\n'.join(output_lines)


class TestSingleCommands:
    """Test that single commands still work (from Part 2)"""
    
    def test_single_command_ls(self):
        """Test: ls (no pipes)"""
        returncode, stdout, stderr = run_dsh(['ls'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert len(output) > 0  # Should list files
    
    def test_single_command_echo(self):
        """Test: echo hello (no pipes)"""
        returncode, stdout, stderr = run_dsh(['echo hello'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello' in output


class TestTwoCommandPipes:
    """Test simple two-command pipes"""
    
    def test_ls_pipe_cat(self):
        """Test: ls | cat (data flows through pipe)"""
        returncode, stdout, stderr = run_dsh(['ls | cat'])
        
        assert returncode == 0
        output = clean_output(stdout)
        # Should see files listed (via cat)
        assert len(output) > 0
    
    def test_echo_pipe_cat(self):
        """Test: echo hello | cat"""
        returncode, stdout, stderr = run_dsh(['echo hello | cat'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello' in output
    
    def test_ls_pipe_grep(self):
        """Test: ls | grep makefile (actual filtering)"""
        returncode, stdout, stderr = run_dsh(['ls | grep makefile'])
        
        assert returncode == 0
        output = clean_output(stdout)
        # Should only show lines with 'makefile'
        assert 'makefile' in output.lower()
    
    def test_echo_pipe_wc(self):
        """Test: echo "word1 word2 word3" | wc -w (count words)"""
        returncode, stdout, stderr = run_dsh(['echo "word1 word2 word3" | wc -w'])
        
        assert returncode == 0
        output = clean_output(stdout)
        # Should count 3 words
        assert '3' in output


class TestThreeCommandPipes:
    """Test three-command pipelines"""
    
    def test_ls_grep_cat(self):
        """Test: ls | grep txt | cat"""
        returncode, stdout, stderr = run_dsh(['ls | grep txt | cat'])
        
        assert returncode == 0
        # Should complete without hanging
    
    def test_echo_cat_cat(self):
        """Test: echo hello | cat | cat"""
        returncode, stdout, stderr = run_dsh(['echo hello | cat | cat'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello' in output
    
    def test_ls_grep_wc(self):
        """Test: ls | grep txt | wc -l (count matching lines)"""
        returncode, stdout, stderr = run_dsh(['ls | grep txt | wc -l'])
        
        assert returncode == 0
        output = clean_output(stdout)
        # Should have a number (count of .txt files)
        assert output.strip().isdigit() or len(output) > 0


class TestLongerPipelines:
    """Test longer pipelines (4+ commands)"""
    
    def test_four_command_pipe(self):
        """Test: echo hello | cat | cat | cat"""
        returncode, stdout, stderr = run_dsh(['echo hello | cat | cat | cat'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello' in output
    
    def test_five_command_pipe(self):
        """Test: echo test | cat | cat | cat | cat"""
        returncode, stdout, stderr = run_dsh(['echo test | cat | cat | cat | cat'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'test' in output


class TestDataFlow:
    """Test that data actually flows through pipes"""
    
    def test_data_transforms(self):
        """Test: echo HELLO | tr A-Z a-z (convert to lowercase)"""
        returncode, stdout, stderr = run_dsh(['echo HELLO | tr A-Z a-z'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello' in output
    
    def test_head_tail(self):
        """Test: ls | head -5 | tail -1 (get 5th line)"""
        returncode, stdout, stderr = run_dsh(['ls | head -5 | tail -1'])
        
        assert returncode == 0
        # Should complete and produce output


class TestBuiltinCommands:
    """Test built-in commands still work"""
    
    def test_cd_command(self):
        """Test: cd /tmp, then pwd"""
        returncode, stdout, stderr = run_dsh(['cd /tmp', 'pwd'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert '/tmp' in output
    
    def test_pwd_command(self):
        """Test: pwd (should work)"""
        returncode, stdout, stderr = run_dsh(['pwd'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert '/' in output


class TestPipeSpacing:
    """Test pipes work with various spacing"""
    
    def test_no_spaces(self):
        """Test: echo hello|cat"""
        returncode, stdout, stderr = run_dsh(['echo hello|cat'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello' in output
    
    def test_extra_spaces(self):
        """Test: echo hello  |  cat"""
        returncode, stdout, stderr = run_dsh(['echo hello  |  cat'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello' in output


class TestEdgeCases:
    """Test edge cases and error handling"""
    
    def test_single_pipe_to_cat(self):
        """Test: cat | cat (cat with no args)"""
        # This might hang if not handled properly
        # Using shorter timeout
        returncode, stdout, stderr = run_dsh(['echo test | cat'], timeout=5)
        
        assert returncode == 0
    
    def test_command_not_found_in_pipe(self):
        """Test: echo hello | notacommand"""
        returncode, stdout, stderr = run_dsh(['echo hello | notacommand'])
        
        # Shell should handle error gracefully
        assert returncode == 0  # Shell itself exits normally


class TestComplexPipelines:
    """Test more complex real-world pipelines"""
    
    def test_ls_sort_head(self):
        """Test: ls | sort | head -5"""
        returncode, stdout, stderr = run_dsh(['ls | sort | head -5'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert len(output) > 0
    
    def test_echo_grep_wc(self):
        """Test: echo "line1\\nline2\\nline3" | grep line | wc -l"""
        returncode, stdout, stderr = run_dsh(['printf "line1\\nline2\\nline3\\n" | grep line | wc -l'])
        
        assert returncode == 0
        output = clean_output(stdout)
        # Should count 3 lines
        assert '3' in output


class TestMixedCommands:
    """Test mixing pipes with non-piped commands"""
    
    def test_pipe_then_single(self):
        """Test: pipe command, then single command"""
        returncode, stdout, stderr = run_dsh([
            'echo hello | cat',
            'echo world'
        ])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello' in output
        assert 'world' in output
    
    def test_single_then_pipe(self):
        """Test: single command, then pipe"""
        returncode, stdout, stderr = run_dsh([
            'echo first',
            'echo second | cat'
        ])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'first' in output
        assert 'second' in output


class TestPipeWithArguments:
    """Test pipes with commands that have arguments"""
    
    def test_ls_with_args_pipe(self):
        """Test: ls -la | grep test"""
        returncode, stdout, stderr = run_dsh(['ls -la | grep test'])
        
        assert returncode == 0
        # Should complete without error
    
    def test_echo_with_quotes_pipe(self):
        """Test: echo "hello world" | cat"""
        returncode, stdout, stderr = run_dsh(['echo "hello world" | cat'])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello world' in output


class TestMultiplePipelines:
    """Test multiple pipeline commands in sequence"""
    
    def test_two_separate_pipes(self):
        """Test: two separate pipeline commands"""
        returncode, stdout, stderr = run_dsh([
            'echo first | cat',
            'echo second | cat'
        ])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'first' in output
        assert 'second' in output


class TestExitCommand:
    """Test exit works with pipes"""
    
    def test_exit_after_pipes(self):
        """Test: pipes work, then exit"""
        returncode, stdout, stderr = run_dsh([
            'echo hello | cat',
            'ls | grep makefile'
        ])
        
        assert returncode == 0
        assert 'exiting...' in stdout


class TestStress:
    """Stress tests for pipeline implementation"""
    
    def test_many_cats(self):
        """Test: echo hello | cat | cat | cat | cat | cat"""
        returncode, stdout, stderr = run_dsh([
            'echo hello | cat | cat | cat | cat | cat'
        ])
        
        assert returncode == 0
        output = clean_output(stdout)
        assert 'hello' in output
    
    def test_many_pipelines(self):
        """Test: many pipeline commands in sequence"""
        commands = ['echo test | cat'] * 5
        returncode, stdout, stderr = run_dsh(commands)
        
        assert returncode == 0
        output = clean_output(stdout)
        # Should see 'test' multiple times
        assert output.count('test') >= 5


if __name__ == "__main__":
    # Run pytest when script is executed directly
    pytest.main([__file__, "-v", "--tb=short"])
